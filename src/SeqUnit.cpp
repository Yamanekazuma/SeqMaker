#include <SeqUnit.hpp>

using namespace seq;

SeqUnit::SeqUnit(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst)
    : length_{inst.info.length}, mnemonic_{inst.info.mnemonic}, operands_{hProcess, regs, inst}, violation_{[&]() -> decltype(violation_) {
        if (operands_.isReadAccessViolation()) {
          return SeqUnitViolation::Read;
        } else if (operands_.isWriteAccessViolation()) {
          return SeqUnitViolation::Write;
        } else {
          return SeqUnitViolation::None;
        }
      }()} {}

SeqUnit::~SeqUnit() {}

bool SeqUnit::isInstructionOf(ZydisMnemonic mnemonic) const noexcept {
  return mnemonic_ == mnemonic;
}

const DestInfo SeqUnit::destination(std::size_t no) const {
  try {
    return operands_.destAt(no);
  } catch (...) {
    throw;
  }
}

const SrcInfo SeqUnit::source(std::size_t no) const {
  try {
    return operands_.srcAt(no);
  } catch (...) {
    throw;
  }
}

bool SeqUnit::setNote(const char* note) noexcept {
  try {
    note_.assign(note);
    str_.assign(makeString());
    return false;
  } catch (...) {
    return true;
  }
}
