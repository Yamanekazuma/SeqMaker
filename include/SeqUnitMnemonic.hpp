#pragma once

#include "SeqUnit.hpp"

namespace seq {

class SeqUnitMnemonic : public SeqUnit {
 public:
  SeqUnitMnemonic(SeqUnitMnemonic&&) = default;
  SeqUnitMnemonic(const SeqUnitMnemonic&) = default;
  SeqUnitMnemonic& operator=(SeqUnitMnemonic&&) = delete;
  SeqUnitMnemonic& operator=(const SeqUnitMnemonic&) = delete;

 private:
  std::string mnemonic_;

  SeqUnitMnemonic(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);

  template <class U>
    requires std::is_base_of_v<SeqUnit, U>
  friend class SeqUnitFactory;

  std::string makeString() noexcept override;
};

};  // namespace seq
