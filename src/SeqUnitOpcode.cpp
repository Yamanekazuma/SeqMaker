#include <SeqUnitOpcode.hpp>

#include <format>

using namespace seq;

SeqUnitOpcode::SeqUnitOpcode(HANDLE hProcess, const Registers& regs, const ZydisDisassembledInstruction& inst)
    : SeqUnit{hProcess, regs, inst}, opcode_{inst.info.opcode} {}

std::string SeqUnitOpcode::makeString() noexcept {
  return std::format("{{opcode: {:02X}}}", opcode_);
}
