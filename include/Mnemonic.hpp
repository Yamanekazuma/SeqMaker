#pragma once

#include <Zydis/Zydis.h>

#include <string>

namespace seq {

class Mnemonic {
 public:
  Mnemonic(const ZydisDisassembledInstruction& inst);

  inline operator std::string() const noexcept { return string(); }
  const std::string string() const noexcept;

 private:
  std::string mnemonic_;
};

};  // namespace seq