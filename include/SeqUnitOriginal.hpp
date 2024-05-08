#pragma once

#include "SeqUnit.hpp"

#include "OperandInfo.hpp"
#include "Protection.hpp"
#include "Registers.hpp"

#include <format>
#include <optional>

namespace seq {

class SeqUnitOriginal : public SeqUnit {
 public:
  SeqUnitOriginal(SeqUnitOriginal&&) = default;
  SeqUnitOriginal(const SeqUnitOriginal&) = default;
  SeqUnitOriginal& operator=(SeqUnitOriginal&&) = default;
  SeqUnitOriginal& operator=(const SeqUnitOriginal&) = default;

 private:
  const std::string mnemonic_;
  DestInfo dest_;
  SrcInfo src1_;
  SrcInfo src2_;

  SeqUnitOriginal(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);

  template <class U>
    requires std::is_base_of_v<SeqUnit, U>
  friend class SeqUnitFactory;

  std::string makeString() noexcept override;
};

};  // namespace seq
