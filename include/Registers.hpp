#pragma once

#include "SeqMaker/types/Registers.h"

#include <Zydis/Zydis.h>

#include <cstdint>

namespace seq {

class RegisterMaster {
 public:
  RegisterMaster() = delete;
  RegisterMaster(RegisterMaster&&) = delete;
  RegisterMaster(const RegisterMaster&) = delete;
  RegisterMaster operator=(RegisterMaster&&) = delete;
  RegisterMaster operator=(const RegisterMaster&) = delete;

  static std::uint32_t getRegisterValueFromZydis(const Registers& regs, const ZydisRegister& reg);
};

}  // namespace seq
