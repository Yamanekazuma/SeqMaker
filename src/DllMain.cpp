#include "SeqMaker/SeqMaker.h"

#include "SeqMaker.hpp"
#include "SeqUnitMnemonic.hpp"
#include "SeqUnitOpcode.hpp"
#include "SeqUnitOriginal.hpp"

#include <cstdint>
#include <stdexcept>

#ifdef NDEBUG
#define debug_print(e)
#else
#include <iostream>
static void debug_print(const std::exception& e) {
  std::cerr << e.what() << std::endl;
}
#endif

using namespace std;
using namespace seq;

class SEQMAKER_ {
 public:
  SEQ_UNITS unit;
  ISeqMaker* seq;
};

static ISeqMaker* ToSeqMaker(SEQMAKER_* s) noexcept;

static bool GetUnitDataInfo_is_call(const SeqUnit& unit, bool& output) noexcept;
static bool GetUnitDataInfo_is_jmp(const SeqUnit& unit, bool& output) noexcept;
static bool GetUnitDataInfo_branch_to(const SeqUnit& unit, uint32_t& output) noexcept;
static bool GetUnitDataInfo_inst_length(const SeqUnit& unit, size_t& output) noexcept;

extern "C" {

BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/, DWORD /*fdwReason*/, LPVOID /*lpReserved*/) {
  return TRUE;
}

SEQ_MAKER_EXPORT SEQMAKER SeqMaker_Init(uint32_t pid, SEQ_UNITS unit) {
  if (unit != SEQ_UNIT_ORIGINAL && unit != SEQ_UNIT_MNEMONIC && unit != SEQ_UNIT_OPCODE) {
    return static_cast<SEQMAKER>(nullptr);
  }

  SEQMAKER_* s = new SEQMAKER_();
  s->unit = unit;

  try {
    switch (unit) {
      case SEQ_UNIT_ORIGINAL:
        s->seq = new SeqMaker<SeqUnitOriginal>(pid);
        break;
      case SEQ_UNIT_MNEMONIC:
        s->seq = new SeqMaker<SeqUnitMnemonic>(pid);
        break;
      case SEQ_UNIT_OPCODE:
        s->seq = new SeqMaker<SeqUnitOpcode>(pid);
        break;
    }
  } catch (const runtime_error& e) {
    debug_print(e);
    delete s;
    return static_cast<SEQMAKER>(nullptr);
  }

  return static_cast<SEQMAKER>(s);
}

SEQ_MAKER_EXPORT void SeqMaker_DeInit(SEQMAKER seq) {
  if (seq == nullptr) {
    return;
  }

  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  if (s->unit == SEQ_UNIT_ORIGINAL || s->unit == SEQ_UNIT_MNEMONIC || s->unit == SEQ_UNIT_OPCODE) {
    switch (s->unit) {
      case SEQ_UNIT_ORIGINAL:
        delete static_cast<SeqMaker<SeqUnitOriginal>*>(s->seq);
        break;
      case SEQ_UNIT_MNEMONIC:
        delete static_cast<SeqMaker<SeqUnitMnemonic>*>(s->seq);
        break;
      case SEQ_UNIT_OPCODE:
        delete static_cast<SeqMaker<SeqUnitOpcode>*>(s->seq);
        break;
    }
  }
  s->seq = nullptr;
  delete s;
}

SEQ_MAKER_EXPORT SEQ_UNITDATA SeqMaker_AddInstruction(SEQMAKER seq, const Registers* regs) {
  if (seq == nullptr || regs == nullptr) {
    return nullptr;
  }

  auto p = ToSeqMaker(static_cast<SEQMAKER_*>(seq));
  if (p == nullptr) {
    return nullptr;
  }

  try {
    return static_cast<SEQ_UNITDATA>(&(p->addInstruction(*regs)));
  } catch (const runtime_error& e) {
    debug_print(e);
    return nullptr;
  }
}

SEQ_MAKER_EXPORT bool SeqMaker_GetUnitDataInfo(SEQ_UNITDATA unit, SEQ_UNITINFO_CONSTS info, void* output) {
  if (unit == nullptr || output == nullptr) {
    return true;
  }

  auto u = static_cast<const SeqUnit*>(unit);
  switch (info) {
    case SEQ_UNITINFO_IS_CALL:
      return GetUnitDataInfo_is_call(*u, *static_cast<bool*>(output));
    case SEQ_UNITINFO_IS_JMP:
      return GetUnitDataInfo_is_jmp(*u, *static_cast<bool*>(output));
    case SEQ_UNITINFO_BRANCH_TO:
      return GetUnitDataInfo_branch_to(*u, *static_cast<uint32_t*>(output));
    case SEQ_UNITINFO_INST_LENGTH:
      return GetUnitDataInfo_inst_length(*u, *static_cast<size_t*>(output));
    default:
      assert("It should be unreachable.");
      return true;
  }
}

SEQ_MAKER_EXPORT bool SeqMaker_AddNote(SEQ_UNITDATA unit, const char* note) {
  if (unit == nullptr) {
    return true;
  }

  auto u = const_cast<SeqUnit*>(static_cast<const SeqUnit*>(unit));
  return u->setNote(note);
}

SEQ_MAKER_EXPORT char* SeqMaker_CreateNGram(SEQMAKER seq, std::size_t n) {
  if (seq == nullptr) {
    return nullptr;
  }

  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  ISeqMaker* p = ToSeqMaker(s);
  if (p == nullptr) {
    return nullptr;
  }

  try {
    return p->createNGramString(n);
  } catch (const runtime_error& e) {
    debug_print(e);
    return nullptr;
  }
}
}

static ISeqMaker* ToSeqMaker(SEQMAKER_* s) noexcept {
  if (s == nullptr) {
    return nullptr;
  }

  switch (s->unit) {
    case SEQ_UNIT_ORIGINAL:
      return static_cast<SeqMaker<SeqUnitOriginal>*>(s->seq);
    case SEQ_UNIT_MNEMONIC:
      return static_cast<SeqMaker<SeqUnitMnemonic>*>(s->seq);
    case SEQ_UNIT_OPCODE:
      return static_cast<SeqMaker<SeqUnitOpcode>*>(s->seq);
    default:
      return nullptr;
  }
}

static bool GetUnitDataInfo_is_call(const SeqUnit& unit, bool& output) noexcept {
  output = unit.isInstructionOf(ZYDIS_MNEMONIC_CALL);
  return false;
}

static bool GetUnitDataInfo_is_jmp(const SeqUnit& unit, bool& output) noexcept {
  output = unit.isInstructionOf(ZYDIS_MNEMONIC_JMP);
  return false;
}

static bool GetUnitDataInfo_branch_to(const SeqUnit& unit, uint32_t& output) noexcept {
  if (!unit.isInstructionOf(ZYDIS_MNEMONIC_CALL) && !unit.isInstructionOf(ZYDIS_MNEMONIC_JMP)) {
    return true;
  }

  output = unit.source(0).value();
  return false;
}

static bool GetUnitDataInfo_inst_length(const SeqUnit& unit, size_t& output) noexcept {
  output = unit.length();
  return false;
}
