#include <SeqUnitMnemonic.hpp>

using namespace Seq;

SeqUnitMnemonic::SeqUnitMnemonic(HANDLE, const Registers&, ZydisDisassembledInstruction&& inst) : mnemonic_{} {
  const char* mnemonic = ZydisMnemonicGetString(inst.info.mnemonic);
  if (mnemonic == nullptr) {
    throw std::runtime_error("不明な命令を検出しました．");
  }
  mnemonic_ = mnemonic;
}
