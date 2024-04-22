#include <SeqUnitOpcode.hpp>

#include <format>

using namespace seq;

SeqUnitOpcode::SeqUnitOpcode(HANDLE, const Registers&, ZydisDisassembledInstruction&& inst) : opcode_{inst.info.opcode} {}

std::string SeqUnitOpcode::makeString() noexcept {
  return std::format("{{opcode: {:02X}}}", opcode_);
}
