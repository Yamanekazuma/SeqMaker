#pragma once

#include "NGram.hpp"
#include "Registers.hpp"
#include "SeqUnit.hpp"

#include <handleapi.h>
#include <memoryapi.h>
#include <processthreadsapi.h>
#include <winnt.h>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace Seq {

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
class SeqMaker {
public:
  inline SeqMaker(std::uint32_t pid) : hProcess_{}, seq_{} {
    hProcess_ = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    // MSDN曰く、OpenProcess失敗時には「NULL」が返るとのことなので、
    // nullptrにはしないこと
    if (hProcess_ == NULL) {
      throw std::runtime_error("プロセスを開けませんでした．");
    }
  }

  inline ~SeqMaker() noexcept {
    CloseHandle(hProcess_);
    hProcess_ = nullptr;
  }

  SeqMaker(SeqMaker&&) = delete;
  SeqMaker(const SeqMaker&) = delete;
  SeqMaker operator=(SeqMaker&&) = delete;
  SeqMaker operator=(const SeqMaker&) = delete;

  inline void addInstruction(const Registers& regs) {
    std::uint8_t buffer[IA32_MAX_INST_LENGTH_]{};

    if (ReadProcessMemory(hProcess_, reinterpret_cast<LPCVOID>(regs.EIP), buffer, sizeof(buffer), NULL) == 0) {
      throw std::runtime_error("実行バイト列の取得に失敗しました．");
    }

    ZydisDisassembledInstruction inst;
    if (!ZYAN_SUCCESS(
            ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_COMPAT_32, static_cast<ZyanU64>(regs.EIP), buffer, sizeof(buffer), &inst))) {
      throw std::runtime_error("ディスアセンブルに失敗しました．");
    }

    seq_.emplace_back(hProcess_, regs, std::move(inst));
  }

  template <std::size_t N>
  inline const NGram::NGram<U, N> createNGram() const {
    return NGram::NGram<U, N>{seq_};
  }

private:
  inline constexpr static std::size_t IA32_MAX_INST_LENGTH_ = 15;
  HANDLE hProcess_;
  std::vector<U> seq_;
};

}  // namespace Seq
