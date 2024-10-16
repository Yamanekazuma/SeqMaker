#include <OperandInfo.hpp>

#include <cassert>

using namespace seq;

static std::uint32_t calcAddress(const Registers& regs, const ZydisDecodedInstruction& inst, const ZydisDecodedOperand& op) noexcept;
static std::uint32_t convertRegisterToValue(const Registers& regs, const ZydisDecodedOperandReg& op) noexcept;
static std::uint32_t convertImmediateToValue(const Registers& regs, const ZydisDecodedInstruction& inst, const ZydisDecodedOperand& op) noexcept;
static bool isFakeMemoryAccessingMnemonic(ZydisMnemonic mnemonic) noexcept;

MemoryProtectionInfo::MemoryProtectionInfo() noexcept : protection_{} {}
MemoryProtectionInfo::MemoryProtectionInfo(protect::Protections protection) noexcept : protection_{protection} {}

MemoryProtectionInfo& MemoryProtectionInfo::operator=(MemoryProtectionInfo&& other) noexcept {
  protection_ = std::move(other.protection_);
  other.protection_ = std::nullopt;
  return *this;
}

MemoryProtectionInfo::MemoryProtectionInfo(MemoryProtectionInfo&& other) noexcept : protection_{other.protection_} {
  other.protection_ = std::nullopt;
}

const std::string MemoryProtectionInfo::string() const noexcept {
  if (!protection_) {
    return "None";
  }

  switch (protection_.value()) {
    case protect::Protections::None:
      return "NonMemoryAccess";
    case protect::Protections::R:
      return "MemoryAccess{R}";
    case protect::Protections::W:
      return "MemoryAccess{W}";
    case protect::Protections::X:
      return "MemoryAccess{X}";
    case protect::Protections::RW:
      return "MemoryAccess{RW}";
    case protect::Protections::RX:
      return "MemoryAccess{RX}";
    case protect::Protections::WX:
      return "MemoryAccess{WX}";
    case protect::Protections::RWX:
      return "MemoryAccess{RWX}";
    default:
      assert(0);
      return "";
  }
}

bool MemoryProtectionInfo::has_value() const noexcept {
  return protection_.has_value();
}

bool MemoryProtectionInfo::isMemoryAccessing() const noexcept {
  return protection_.has_value() && *protection_ != protect::Protections::None;
}

template <OperandType T>
OperandInfo<T>::OperandInfo() noexcept : address_{}, protection_{} {}

template <OperandType T>
OperandInfo<T>::OperandInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedInstruction& inst, const ZydisDecodedOperand& op)
    : address_{calcAddress(regs, inst, op)}, value_{address_}, protection_{protect::Protections::None}, isAccessViolation_{false} {
  if (op.type != ZYDIS_OPERAND_TYPE_MEMORY) {
    return;
  }

  protect::Protections protection;
  if (op.mem.segment == ZYDIS_REGISTER_FS) {
    protection = protect::Protections::RW;
  } else {
    try {
      protection = protect::ProtectionMaster::queryMemoryProtection(hProcess, address_);
    } catch (...) {
      protection = protect::Protections::None;
    }
  }

  if (ReadProcessMemory(hProcess, reinterpret_cast<void*>(address_), &value_, sizeof(value_), nullptr) == 0) {
    value_ = 0;
  }

  if constexpr (T == OperandType::Destination) {
    if (!protect::ProtectionMaster::isWritable(protection)) {
      isAccessViolation_ = true;
    }
  } else if constexpr (T == OperandType::Source) {
    if (!protect::ProtectionMaster::isReadable(protection)) {
      isAccessViolation_ = true;
    }
  }

  protection_ = protection;
}

template <OperandType T>
OperandInfo<T>& OperandInfo<T>::operator=(OperandInfo<T>&& other) noexcept {
  value_ = std::move(other.value_);
  address_ = std::move(other.address_);
  protection_ = std::move(other.protection_);

  other.address_ = 0;
  other.value_ = 0;

  return *this;
}

template <OperandType T>
OperandInfo<T>::OperandInfo(OperandInfo<T>&& other) noexcept
    : address_{std::move(other.address_)}, value_{std::move(other.value_)}, protection_{std::move(other.protection_)} {
  other.address_ = 0;
  other.value_ = 0;
}

template <OperandType T>
bool OperandInfo<T>::isNone() const noexcept {
  return protection_.has_value();
}

template <OperandType T>
bool OperandInfo<T>::isMemoryAccessing() const noexcept {
  return protection_.isMemoryAccessing();
}

template <OperandType T>
bool OperandInfo<T>::isAccessViolation() const noexcept {
  return isAccessViolation_;
}

template <OperandType T>
OperandType OperandInfo<T>::getOperandType() const noexcept {
  return T;
}

template class seq::OperandInfo<OperandType::Destination>;
template class seq::OperandInfo<OperandType::Source>;

OperandSet::OperandSet(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst)
    : destOps_{}, srcOps_{}, isReadAccessViolation_{false}, isWriteAccessViolation_{false} {
  // メモリ参照の形をとるものの，実際にはメモリ参照を行わない命令については
  // 例外的にメモリ参照先オペランドをNone（存在しないもの）として扱う．
  // ただし，現時点ではいずれも他のオペランドも不要な情報であるため，
  // 簡単のためにすべてのオペランドをNoneとして扱うものとする．
  if (isFakeMemoryAccessingMnemonic(inst.info.mnemonic)) {
    return;
  }

  try {
    for (const auto& op : inst.operands) {
      if (op.visibility != ZYDIS_OPERAND_VISIBILITY_EXPLICIT) {
        continue;
      }

      if (op.actions & ZYDIS_OPERAND_ACTION_WRITE) {
        if ((destOps_.emplace_back(hProcess, regs, inst.info, op)).isAccessViolation()) {
          isWriteAccessViolation_ = true;
        }
      } else if (op.actions & ZYDIS_OPERAND_ACTION_READ) {
        if ((srcOps_.emplace_back(hProcess, regs, inst.info, op)).isAccessViolation()) {
          isReadAccessViolation_ = true;
        }
      }
    }
  } catch (...) {
    throw;
  }

  destOps_.shrink_to_fit();
  srcOps_.shrink_to_fit();
}

const DestInfo OperandSet::destAt(std::size_t no) const {
  return destOps_.at(no);
}

const SrcInfo OperandSet::srcAt(std::size_t no) const {
  return srcOps_.at(no);
}

static std::uint32_t calcAddress(const Registers& regs, const ZydisDecodedInstruction& inst, const ZydisDecodedOperand& op) noexcept {
  switch (op.type) {
    case ZYDIS_OPERAND_TYPE_REGISTER:
      return convertRegisterToValue(regs, op.reg);
    case ZYDIS_OPERAND_TYPE_MEMORY:
      try {
        return MemoryMaster::calcMemoryAddress(regs, op.mem);
      } catch (...) {
        return 0;
      }
    case ZYDIS_OPERAND_TYPE_POINTER:
      return static_cast<std::uint32_t>(op.ptr.offset);
    case ZYDIS_OPERAND_TYPE_IMMEDIATE:
      return convertImmediateToValue(regs, inst, op);
    default:
      // その他の形式は非対応
      return 0;
  }
}

static std::uint32_t convertRegisterToValue(const Registers& regs, const ZydisDecodedOperandReg& op) noexcept {
  switch (op.value) {
    case ZYDIS_REGISTER_EAX:
      return regs.EAX;
    case ZYDIS_REGISTER_EBX:
      return regs.EBX;
    case ZYDIS_REGISTER_ECX:
      return regs.ECX;
    case ZYDIS_REGISTER_EDX:
      return regs.EDX;
    case ZYDIS_REGISTER_EDI:
      return regs.EDI;
    case ZYDIS_REGISTER_ESI:
      return regs.ESI;
    case ZYDIS_REGISTER_EBP:
      return regs.EBP;
    case ZYDIS_REGISTER_ESP:
      return regs.ESP;
    case ZYDIS_REGISTER_EIP:
      return regs.EIP;
    case ZYDIS_REGISTER_AX:
      return (regs.EAX & 0x0000FFFF);
    case ZYDIS_REGISTER_BX:
      return (regs.EBX & 0x0000FFFF);
    case ZYDIS_REGISTER_CX:
      return (regs.ECX & 0x0000FFFF);
    case ZYDIS_REGISTER_DX:
      return (regs.EDX & 0x0000FFFF);
    case ZYDIS_REGISTER_DI:
      return (regs.EDI & 0x0000FFFF);
    case ZYDIS_REGISTER_SI:
      return (regs.ESI & 0x0000FFFF);
    case ZYDIS_REGISTER_BP:
      return (regs.EBP & 0x0000FFFF);
    case ZYDIS_REGISTER_SP:
      return (regs.ESP & 0x0000FFFF);
    case ZYDIS_REGISTER_IP:
      return (regs.EIP & 0x0000FFFF);
    case ZYDIS_REGISTER_AL:
      return (regs.EAX & 0x000000FF);
    case ZYDIS_REGISTER_BL:
      return (regs.EBX & 0x000000FF);
    case ZYDIS_REGISTER_CL:
      return (regs.ECX & 0x000000FF);
    case ZYDIS_REGISTER_DL:
      return (regs.EDX & 0x000000FF);
    case ZYDIS_REGISTER_DIL:
      return (regs.EDI & 0x000000FF);
    case ZYDIS_REGISTER_SIL:
      return (regs.ESI & 0x000000FF);
    case ZYDIS_REGISTER_BPL:
      return (regs.EBP & 0x000000FF);
    case ZYDIS_REGISTER_SPL:
      return (regs.ESP & 0x000000FF);
    case ZYDIS_REGISTER_AH:
      return (regs.EAX & 0x0000FF00);
    case ZYDIS_REGISTER_BH:
      return (regs.EBX & 0x0000FF00);
    case ZYDIS_REGISTER_CH:
      return (regs.ECX & 0x0000FF00);
    case ZYDIS_REGISTER_DH:
      return (regs.EDX & 0x0000FF00);
    case ZYDIS_REGISTER_CS:
      return static_cast<std::uint32_t>(regs.CS);
    case ZYDIS_REGISTER_DS:
      return static_cast<std::uint32_t>(regs.DS);
    case ZYDIS_REGISTER_ES:
      return static_cast<std::uint32_t>(regs.ES);
    case ZYDIS_REGISTER_FS:
      return static_cast<std::uint32_t>(regs.FS);
    case ZYDIS_REGISTER_GS:
      return static_cast<std::uint32_t>(regs.GS);
    case ZYDIS_REGISTER_SS:
      return static_cast<std::uint32_t>(regs.SS);
    default:
      // その他のレジスタは現状非対応
      return 0;
  }
}

static std::uint32_t convertImmediateToValue(const Registers& regs, const ZydisDecodedInstruction& inst, const ZydisDecodedOperand& op) noexcept {
  if (op.imm.is_relative) {
    ZyanU64 address;
    if (ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(&inst, &op, regs.EIP, &address))) {
      return static_cast<std::uint32_t>(address & 0xFFFFFFFF);
    } else {
      return 0;
    }
  } else {
    return static_cast<std::uint32_t>(op.imm.value.u & 0xFFFFFFFF);
  }
}

static bool isFakeMemoryAccessingMnemonic(ZydisMnemonic mnemonic) noexcept {
  switch (mnemonic) {
    case ZYDIS_MNEMONIC_NOP:
    case ZYDIS_MNEMONIC_LEA:
    case ZYDIS_MNEMONIC_UD1:
    case ZYDIS_MNEMONIC_UD0:
      return true;
    default:
      return false;
  }
}
