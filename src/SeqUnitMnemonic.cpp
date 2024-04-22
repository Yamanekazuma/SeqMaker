#include <SeqUnitMnemonic.hpp>

#include <format>
#include <stdexcept>

using namespace seq;

static const char* instructionToMnemonic(const ZydisDisassembledInstruction& inst);

SeqUnitMnemonic::SeqUnitMnemonic(HANDLE, const Registers&, ZydisDisassembledInstruction&& inst) : mnemonic_{instructionToMnemonic(inst)} {}

std::string SeqUnitMnemonic::makeString() noexcept {
  return std::format("{{mnemonic: {}}}", mnemonic_);
}

static const char* instructionToMnemonic(const ZydisDisassembledInstruction& inst) {
  const char* mnemonic = ZydisMnemonicGetString(inst.info.mnemonic);
  if (mnemonic == nullptr) {
    throw std::runtime_error("不明な命令を検出しました．");
  }
  return mnemonic;
}