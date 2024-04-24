#pragma once

#include "SeqUnit.hpp"

namespace seq {

class SeqUnitOpcode : public SeqUnit {
 public:
  SeqUnitOpcode(SeqUnitOpcode&&) = default;
  SeqUnitOpcode(const SeqUnitOpcode&) = default;
  SeqUnitOpcode& operator=(SeqUnitOpcode&&) = default;
  SeqUnitOpcode& operator=(const SeqUnitOpcode&) = default;

 private:
  ZyanU8 opcode_;

  SeqUnitOpcode(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);

  template <class U>
    requires std::is_base_of_v<SeqUnit, U>
  friend class SeqUnitFactory;

  std::string makeString() noexcept override;
};

};  // namespace seq
