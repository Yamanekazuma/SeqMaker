#include <SeqUnitOriginal.hpp>

#include <memoryapi.h>
#include <processthreadsapi.h>
#include <windef.h>

using namespace seq;
using namespace seq::protect;

static const char* instructionToMnemonic(const ZydisDisassembledInstruction& inst);

static bool isReadOperand(const ZydisDecodedOperand& op) noexcept;
static bool isWrittenOperand(const ZydisDecodedOperand& op) noexcept;

SeqUnitOriginal::SeqUnitOriginal(HANDLE hProcess, const Registers& regs, ZydisDisassembledInstruction&& inst)
    : mnemonic_{instructionToMnemonic(inst)}, dest_{}, src1_{}, src2_{} {
  for (std::size_t i = 0; i < inst.info.operand_count; ++i) {
    const auto& op = inst.operands[i];
    if (op.visibility != ZYDIS_OPERAND_VISIBILITY_EXPLICIT) {
      continue;
    }

    if (isReadOperand(op)) {
      if (src1_.isNone()) {
        src1_ = SrcInfo{hProcess, regs, op};
      } else if (src2_.isNone()) {
        src2_ = SrcInfo{hProcess, regs, op};
      }
    } else if (isWrittenOperand(op)) {
      if (dest_.isNone()) {
        dest_ = DestInfo{hProcess, regs, op};
      }
    }
  }
}

std::string SeqUnitOriginal::makeString() noexcept {
  return std::format("{{mnemonic: {}, dest: {}, src1: {}, src2: {}}}", mnemonic_, dest_.protection().string(), src1_.protection().string(),
                     src2_.protection().string());
}

static const char* instructionToMnemonic(const ZydisDisassembledInstruction& inst) {
  const char* mnemonic = ZydisMnemonicGetString(inst.info.mnemonic);
  if (mnemonic == nullptr) {
    throw std::runtime_error("不明な命令を検出しました．");
  }
  return mnemonic;
}

static bool isReadOperand(const ZydisDecodedOperand& op) noexcept {
  return (op.actions & ZYDIS_OPERAND_ACTION_READ);
}

static bool isWrittenOperand(const ZydisDecodedOperand& op) noexcept {
  return (op.actions & ZYDIS_OPERAND_ACTION_WRITE);
}
