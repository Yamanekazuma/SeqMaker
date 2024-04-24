#include <SeqUnit.hpp>

using namespace seq;

SeqUnit::SeqUnit(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst)
    : mnemonic_{inst.info.mnemonic}, operands_{}, allDestOps_{}, allSrcOps_{} {
  try {
    std::vector<OperandType> types{10};
    std::size_t cnt = 0;
    for (const auto& op : inst.operands) {
      if (op.visibility != ZYDIS_OPERAND_VISIBILITY_EXPLICIT) {
        continue;
      }

      if (op.actions & ZYDIS_OPERAND_ACTION_WRITE) {
        allDestOps_.emplace_back(hProcess, regs, inst.info, op);
        types[cnt++] = OperandType::Destination;
      } else if (op.actions & ZYDIS_OPERAND_ACTION_READ) {
        allSrcOps_.emplace_back(hProcess, regs, inst.info, op);
        types[cnt++] = OperandType::Source;
      }
    }

    allDestOps_.shrink_to_fit();
    allSrcOps_.shrink_to_fit();
    operands_.reserve(cnt);

    for (std::size_t i = 0, di = 0, si = 0; i < cnt; ++i) {
      if (types[i] == OperandType::Destination) {
        operands_[i] = &allDestOps_[di++];
      } else {
        operands_[i] = &allSrcOps_[si++];
      }
    }
  } catch (...) {
    throw;
  }
}

bool SeqUnit::isInstructionOf(ZydisMnemonic mnemonic) const noexcept {
  return mnemonic_ == mnemonic;
}

std::uint32_t SeqUnit::operand(std::size_t no) const {
  try {
    return (operands_.at(no))->accessingAddress();
  } catch (...) {
    throw;
  }
}
