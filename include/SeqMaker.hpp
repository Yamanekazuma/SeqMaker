#pragma once

#include "NGram.hpp"
#include "Registers.hpp"
#include "SeqUnit.hpp"

#include <winnt.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace Seq {

class SeqMaker {
public:
  SeqMaker(std::uint32_t pid);
  ~SeqMaker() noexcept;

  SeqMaker(SeqMaker&&) = delete;
  SeqMaker(const SeqMaker&) = delete;
  SeqMaker operator=(SeqMaker&&) = delete;
  SeqMaker operator=(const SeqMaker&) = delete;

  void addInstruction(const Registers& regs);

  template <std::size_t N>
  inline const NGram::NGram<SeqUnit, N> createNGram() const {
    return NGram::NGram<SeqUnit, N>{seq_};
  }

private:
  inline constexpr static std::size_t IA32_MAX_INST_LENGTH_ = 15;
  HANDLE hProcess_;
  std::vector<SeqUnit> seq_;
};

}  // namespace Seq
