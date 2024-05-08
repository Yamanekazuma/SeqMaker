#include <SeqUnitOriginal.hpp>

#include <memoryapi.h>
#include <processthreadsapi.h>
#include <windef.h>

using namespace seq;
using namespace seq::protect;

static const char* instructionToMnemonic(const ZydisDisassembledInstruction& inst);

SeqUnitOriginal::SeqUnitOriginal(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst)
    : SeqUnit{hProcess, regs, inst}, mnemonic_{instructionToMnemonic(inst)}, dest_{}, src1_{}, src2_{} {
  if (allDestOps_.size() > 0) {
    dest_ = std::move(allDestOps_[0]);
  }

  if (allSrcOps_.size() > 0) {
    src1_ = std::move(allSrcOps_[0]);
    if (allSrcOps_.size() > 1) {
      src2_ = std::move(allSrcOps_[1]);
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
