#pragma once

#include "NGram.hpp"
#include "Registers.hpp"
#include "SeqUnit.hpp"

#include "SeqUnitOriginal.hpp"

#include <handleapi.h>
#include <memoryapi.h>
#include <processthreadsapi.h>
#include <winnt.h>

#include <bit>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace seq {

class ISeqMaker {
 public:
  virtual ~ISeqMaker() = 0;
  virtual const SeqUnit& addInstruction(const Registers& regs) = 0;
  virtual char* createNGramString(std::size_t n) const = 0;
};

ISeqMaker::~ISeqMaker() {}

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
class SeqMaker : public ISeqMaker {
 public:
  SeqMaker(std::uint32_t pid);
  ~SeqMaker() noexcept;

  SeqMaker(SeqMaker&&) = delete;
  SeqMaker(const SeqMaker&) = delete;
  SeqMaker operator=(SeqMaker&&) = delete;
  SeqMaker operator=(const SeqMaker&) = delete;

  const SeqUnit& addInstruction(const Registers& regs) override;
  char* createNGramString(std::size_t n) const override;

 private:
  inline constexpr static std::size_t IA32_MAX_INST_LENGTH_ = 15;
  HANDLE hProcess_;
  std::vector<U> seq_;
};

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
SeqMaker<U>::SeqMaker(std::uint32_t pid) : hProcess_{}, seq_{} {
  hProcess_ = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
  if (hProcess_ == nullptr) {
    throw std::runtime_error("プロセスを開けませんでした．");
  }
}

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
SeqMaker<U>::~SeqMaker() noexcept {
  CloseHandle(hProcess_);
  hProcess_ = nullptr;
}

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
const SeqUnit& SeqMaker<U>::addInstruction(const Registers& regs) {
  std::uint8_t buffer[IA32_MAX_INST_LENGTH_]{};

  if (ReadProcessMemory(hProcess_, std::bit_cast<LPCVOID>(static_cast<uintptr_t>(regs.EIP)), buffer, sizeof(buffer), nullptr) == 0) {
    throw std::runtime_error("実行バイト列の取得に失敗しました．");
  }

  ZydisDisassembledInstruction inst;
  if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_COMPAT_32, static_cast<ZyanU64>(regs.EIP), buffer, sizeof(buffer), &inst))) {
    throw std::runtime_error("ディスアセンブルに失敗しました．");
  }

  seq_.push_back(std::move(SeqUnitFactory<U>::create(hProcess_, regs, inst)));
  return static_cast<const SeqUnit&>(seq_.back());
}

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
char* SeqMaker<U>::createNGramString(std::size_t n) const {
  return _strdup(NGram<U>(seq_, n).string().c_str());
}

}  // namespace seq
