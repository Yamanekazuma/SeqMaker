#include <OperandInfo.hpp>

#include <cassert>

using namespace seq;

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
OperandInfo<T>::OperandInfo() noexcept : protection_{}, address_{} {}

template <OperandType T>
OperandInfo<T>::OperandInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedOperand& op) : protection_{}, address_{} {
  if (op.type != ZYDIS_OPERAND_TYPE_MEMORY) {
    return;
  }

  protect::Protections protection{};
  if (op.mem.segment == ZYDIS_REGISTER_FS) {
    protection = protect::Protections::RW;
  } else {
    try {
      address_ = MemoryMaster::calcMemoryAddress(regs, op.mem);
    } catch (...) {
      // 例外は発生しないはず
    }

    try {
      protection = protect::ProtectionMaster::queryMemoryProtection(hProcess, address_);
    } catch (...) {
      // 参照不可領域
      protection = protect::Protections::None;
    }
  }

  if constexpr (T == OperandType::Destination) {
    if (!protect::ProtectionMaster::isWritable(protection)) {
      throw std::runtime_error("書き込み不可領域への書き込みを検出しました．");
    }
  } else if constexpr (T == OperandType::Source) {
    if (!protect::ProtectionMaster::isReadable(protection)) {
      throw std::runtime_error("読み出し不可領域からの読み出しを検出しました．");
    }
  }

  protection_ = protection;
}

template <OperandType T>
OperandInfo<T>& OperandInfo<T>::operator=(OperandInfo<T>&& other) noexcept {
  protection_ = std::move(other.protection_);
  address_ = std::move(other.address_);
  return *this;
}

template <OperandType T>
OperandInfo<T>::OperandInfo(OperandInfo<T>&& other) noexcept : protection_{std::move(other.protection_)}, address_{std::move(other.address_)} {}

template <OperandType T>
bool OperandInfo<T>::isNone() const noexcept {
  return protection_.has_value();
}

template <OperandType T>
bool OperandInfo<T>::isMemoryAccessing() const noexcept {
  return protection_.isMemoryAccessing();
}

template class OperandInfo<OperandType::Destination>;
template class OperandInfo<OperandType::Source>;
