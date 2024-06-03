#include <SeqUnitMnemonic.hpp>

#include <format>
#include <stdexcept>

using namespace seq;

SeqUnitMnemonic::SeqUnitMnemonic(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst)
    : SeqUnit{hProcess, regs, inst}, mnemonic_{[&inst]() -> std::string {
        const char* mnemonic = ZydisMnemonicGetString(inst.info.mnemonic);
        if (mnemonic == nullptr) {
          throw std::runtime_error("不明な命令を検出しました．");
        }
        return mnemonic;
      }()} {}

std::string SeqUnitMnemonic::makeString() noexcept {
  return std::format("{{mnemonic: {}, note: {}}}", mnemonic_, note());
}
