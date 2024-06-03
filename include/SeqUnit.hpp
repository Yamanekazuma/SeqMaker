#pragma once

#include "OperandInfo.hpp"
#include "Registers.hpp"

#include <Zydis/Zydis.h>

#include <windef.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace seq {

class SeqUnit;

template <class U>
  requires std::is_base_of_v<SeqUnit, U>
class SeqUnitFactory {
 public:
  static U create(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);
};

class SeqUnit {
 public:
  SeqUnit(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst);

  virtual ~SeqUnit() = 0;

  inline explicit operator std::string() const noexcept {
    return string();
  }
  inline const std::string& string() const noexcept {
    return str_;
  }

  bool isInstructionOf(ZydisMnemonic mnemonic) const noexcept;

  const DestInfo destination(std::size_t no) const;
  const SrcInfo source(std::size_t no) const;

  bool setNote(const char* note) noexcept;
  inline const std::string& note() const noexcept {
    return note_;
  }

  inline std::size_t length() const noexcept {
    return length_;
  }

  inline const OperandSet& operands() const noexcept {
    return operands_;
  }

 protected:
  inline const ZydisMnemonic& mnemonic() const noexcept {
    return mnemonic_;
  }

 private:
  std::string str_{};
  std::string note_{};
  const std::size_t length_;
  const ZydisMnemonic mnemonic_;
  const OperandSet operands_;

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
U SeqUnitFactory<U>::create(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst) {
  try {
    U unit{hProcess, regs, std::move(inst)};
    unit.str_ = static_cast<SeqUnit&>(unit).makeString();
    return unit;
  } catch (...) {
    throw;
  }
}

};  // namespace seq
