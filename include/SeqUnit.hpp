#pragma once

#include "Registers.hpp"

#include <Zydis/Zydis.h>

#include <windef.h>

#include <string>
#include <utility>

namespace seq {

class SeqUnit;

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
class SeqUnitFactory {
 public:
  static U create(HANDLE hProcess, const Registers& regs, ZydisDisassembledInstruction&& inst);
};

class SeqUnit {
 public:
  inline explicit operator std::string() const noexcept { return string(); }
  const std::string string() const noexcept { return str_; }

 private:
  std::string str_;

  // Factoryを介さずにSeqUnitクラス群を作成してほしくないので
  // Factoryをfriendクラスにしつつ、子クラスのコンストラクタをprivateに記述
  // friendは継承されないので各子クラスでもfriend宣言が必要なのがアレ
  template <class U>
    requires std::is_base_of_v<SeqUnit, U>
  friend class SeqUnitFactory;

  virtual std::string makeString() noexcept = 0;
};

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
U SeqUnitFactory<U>::create(HANDLE hProcess, const Registers& regs, ZydisDisassembledInstruction&& inst) {
  try {
    U unit{hProcess, regs, std::move(inst)};
    static_cast<SeqUnit&>(unit).makeString();
    return unit;
  } catch (...) {
    throw;
  }
}

};  // namespace seq
