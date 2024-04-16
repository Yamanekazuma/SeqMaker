#include <SeqUnitOpcode.hpp>

using namespace seq;

SeqUnitOpcode::SeqUnitOpcode(HANDLE, const Registers&, ZydisDisassembledInstruction&& inst) : opcode_{inst.info.opcode} {}
