#include <SeqUnit.hpp>

#include <memoryapi.h>
#include <processthreadsapi.h>
#include <windef.h>

using namespace Protection;
using namespace Seq;

OperandInfo::~OperandInfo() {}

std::uint32_t OperandInfo::calcMemoryAddress(const Registers& regs, const ZydisDecodedOperandMem& mem) {
  // base取得
  std::uint32_t base = 0;
  if (mem.base != ZYDIS_REGISTER_NONE) {
    base = RegisterMaster::getRegisterValueFromZydis(regs, mem.base);
  }

  // disp取得
  std::uint32_t disp = 0;
  if (mem.disp.has_displacement) {
    disp = static_cast<std::uint32_t>(mem.disp.value);
  }

  // index取得
  std::uint32_t index = 0;
  if (mem.index != ZYDIS_REGISTER_NONE) {
    index = RegisterMaster::getRegisterValueFromZydis(regs, mem.index);
  }

  // scale取得
  std::uint32_t scale = mem.scale;

  if (mem.segment != ZYDIS_REGISTER_FS) {
    return disp + base + index * scale;
  } else {
    std::uint32_t fs = regs.FS;
    return fs * 16 + disp + base + index * scale;
  }
}

Protections OperandInfo::getMemoryProtection(HANDLE hProcess, std::uint32_t address) {
  MEMORY_BASIC_INFORMATION mbi{};
  if (VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi)) == 0) {
    throw std::runtime_error("メモリ保護属性の取得に失敗しました．");
  }
  return ProtectionMaster::convertFromWinConsts(mbi.Protect);
}

DestInfo::DestInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedOperand& op) {
  if (op.type != ZYDIS_OPERAND_TYPE_MEMORY) {
    val_ = Values::NonMemoryAccess;
    return;
  }

  Protections protect = getMemoryProtection(hProcess, calcMemoryAddress(regs, op.mem));
  if (!ProtectionMaster::isWritable(protect)) {
    throw std::runtime_error("書き込み不可領域への書き込みを検出しました．");
  }
  switch (protect) {
    case Protections::W:
      val_ = Values::MemoryAccessW;
      break;
    case Protections::RW:
      val_ = Values::MemoryAccessRW;
      break;
    case Protections::WX:
      val_ = Values::MemoryAccessWX;
      break;
    case Protections::RWX:
      val_ = Values::MemoryAccessRWX;
      break;
    default:
      throw std::runtime_error("不明なメモリ保護属性が指定されました．");
  }
}

SrcInfo::SrcInfo(HANDLE hProcess, const Registers& regs, const ZydisDecodedOperand& op) {
  if (op.type != ZYDIS_OPERAND_TYPE_MEMORY) {
    val_ = Values::NonMemoryAccess;
    return;
  }

  Protections protect = getMemoryProtection(hProcess, calcMemoryAddress(regs, op.mem));
  if (!ProtectionMaster::isReadable(protect)) {
    throw std::runtime_error("読み出し不可領域からの読み出しを検出しました．");
  }
  switch (protect) {
    case Protections::R:
      val_ = Values::MemoryAccessR;
      break;
    case Protections::RW:
      val_ = Values::MemoryAccessRW;
      break;
    case Protections::RX:
      val_ = Values::MemoryAccessRX;
      break;
    case Protections::RWX:
      val_ = Values::MemoryAccessRWX;
      break;
    default:
      throw std::runtime_error("不明なメモリ保護属性が指定されました．");
  }
}

SeqUnit::SeqUnit(HANDLE hProcess, const Registers& regs, ZydisDisassembledInstruction&& inst) : mnemonic_{}, dest_{}, src1_{}, src2_{} {
  const char* mnemonic = ZydisMnemonicGetString(inst.info.mnemonic);
  if (mnemonic == NULL) {
    throw std::runtime_error("不明な命令を検出しました．");
  }
  mnemonic_ = mnemonic;

  for (std::size_t i = 0; i < inst.info.operand_count; ++i) {
    const auto& op = inst.operands[i];
    if (op.visibility != ZYDIS_OPERAND_VISIBILITY_EXPLICIT) {
      continue;
    }
    if (isDestOperand(op)) {
      dest_.emplace(hProcess, regs, op);
    } else if (isSrc1Operand(op)) {
      src1_.emplace(hProcess, regs, op);
    } else if (isSrc2Operand(op)) {
      src2_.emplace(hProcess, regs, op);
    }
  }
}
