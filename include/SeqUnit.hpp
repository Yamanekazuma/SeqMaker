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

  inline explicit operator std::string() const noexcept { return string(); }
  const std::string string() const noexcept { return str_; }

  bool isInstructionOf(ZydisMnemonic mnemonic) const noexcept;
  std::uint32_t operand(std::size_t no) const;

  bool setNote(const char* note) noexcept;
  const std::string note() const noexcept { return note_; }

  std::size_t length() const noexcept { return length_; }

 protected:
  const ZydisMnemonic mnemonic_;
  std::vector<IOperandInfo*> operands_;
  std::vector<DestInfo> allDestOps_;
  std::vector<SrcInfo> allSrcOps_;

 private:
  std::string str_{};
  std::string note_{};
  std::size_t length_{};

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
    static_cast<SeqUnit&>(unit).makeString();
    return unit;
  } catch (...) {
    throw;
  }
}

};  // namespace seq
