#pragma once

#include "SeqUnit.hpp"

#include <format>

namespace Seq {

class SeqUnitMnemonic : public SeqUnit {
 public:
  inline SeqUnitMnemonic() noexcept {}
  SeqUnitMnemonic(HANDLE hProcess, const Registers& regs, ZydisDisassembledInstruction&& inst);

  SeqUnitMnemonic(SeqUnitMnemonic&&) = default;
  SeqUnitMnemonic(const SeqUnitMnemonic&) = default;
  SeqUnitMnemonic& operator=(SeqUnitMnemonic&&) = default;
  SeqUnitMnemonic& operator=(const SeqUnitMnemonic&) = default;

  inline const std::string toString() const noexcept override {
    /*
    {
      mnemonic: ニーモニック
    }
    */
    return std::format("{{mnemonic: {}}}", mnemonic_);
  }

 private:
  std::string mnemonic_;
};

};  // namespace Seq
