#pragma once

#include "SeqUnit.hpp"

#include <format>

namespace Seq {

class SeqUnitOpcode : public SeqUnit {
public:
  inline SeqUnitOpcode() noexcept {}
  SeqUnitOpcode(HANDLE hProcess, const Registers& regs, ZydisDisassembledInstruction&& inst);

  SeqUnitOpcode(SeqUnitOpcode&&) = default;
  SeqUnitOpcode(const SeqUnitOpcode&) = default;
  SeqUnitOpcode& operator=(SeqUnitOpcode&&) = default;
  SeqUnitOpcode& operator=(const SeqUnitOpcode&) = default;

  inline const std::string toString() const noexcept override {
    /*
    {
      opcode: オペコード
    }
    */
    return std::vformat("{opcode: {:02X}}", std::make_format_args(opcode_));
  }

private:
  ZyanU8 opcode_;
};

};  // namespace Seq
