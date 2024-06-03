#include <SeqUnitOriginal.hpp>

#include <memoryapi.h>
#include <processthreadsapi.h>
#include <windef.h>

using namespace seq;
using namespace seq::protect;

SeqUnitOriginal::SeqUnitOriginal(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst)
    : SeqUnit{hProcess, regs, inst}, mnemonic_{[&inst]() -> std::string {
        const char* mnemonic = ZydisMnemonicGetString(inst.info.mnemonic);
        if (mnemonic == nullptr) {
          throw std::runtime_error("不明な命令を検出しました．");
        }
        return mnemonic;
      }()} {}

std::string SeqUnitOriginal::makeString() noexcept {
  auto&& dests = operands().dest();
  auto&& dest = dests.size() > 0 ? dests[0] : DestInfo{};
  auto&& srcs = operands().src();
  auto&& src1 = srcs.size() > 0 ? srcs[0] : SrcInfo{};
  auto&& src2 = srcs.size() > 1 ? srcs[1] : SrcInfo{};
  return std::format("{{mnemonic: {}, dest: {}, src1: {}, src2: {}, note: {}}}", mnemonic_, dest.protection().string(), src1.protection().string(),
                     src2.protection().string(), note());
}
