#pragma once

#include "Registers.hpp"

#include <Zydis/Zydis.h>

#include <windef.h>
#include <winnt.h>
#include <string>

namespace Seq {

class SeqUnit {
 public:
  inline explicit virtual operator std::string() const noexcept { return string(); }
  virtual const std::string string() const noexcept = 0;
};

};  // namespace Seq
