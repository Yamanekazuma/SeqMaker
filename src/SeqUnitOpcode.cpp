#include <SeqUnitOpcode.hpp>

using namespace Seq;

SeqUnitOpcode::SeqUnitOpcode(HANDLE, const Registers&, ZydisDisassembledInstruction&& inst) : opcode_{inst.info.opcode} {}
