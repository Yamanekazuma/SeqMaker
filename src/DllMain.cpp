#include "SeqMaker.h"
#include "SeqMaker.hpp"
#include "SeqUnitMnemonic.hpp"
#include "SeqUnitOpcode.hpp"
#include "SeqUnitOriginal.hpp"

#include <cstdint>
#include <stdexcept>

using namespace std;
using namespace Seq;

class SEQMAKER_ {
public:
  SEQ_UNITS unit;
  ISeqMaker* seq;
};

static ISeqMaker* ToSeqMaker(SEQMAKER_* s) {
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

SEQ_MAKER_EXPORT bool SeqMaker_AddInstruction(SEQMAKER seq, const Registers* regs) {
  if (seq == nullptr || regs == nullptr) {
    return true;
  }

  ISeqMaker* p = ToSeqMaker(static_cast<SEQMAKER_*>(seq));
  if (p == nullptr) {
    return true;
  }

  try {
    p->addInstruction(*regs);
    return false;
  } catch (const runtime_error& e) {
    return true;
  }
}

SEQ_MAKER_EXPORT char* SeqMaker_CreateUniGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }

  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  ISeqMaker* p = ToSeqMaker(s);
  if (p == nullptr) {
    return nullptr;
  }

  try {
    return p->createUniGramString();
  } catch (const runtime_error& e) {
    return nullptr;
  }
}

SEQ_MAKER_EXPORT char* SeqMaker_CreateBiGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }

  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  ISeqMaker* p = ToSeqMaker(s);
  if (p == nullptr) {
    return nullptr;
  }

  try {
    return p->createBiGramString();
  } catch (const runtime_error& e) {
    return nullptr;
  }
}

SEQ_MAKER_EXPORT char* SeqMaker_CreateTriGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }

  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  ISeqMaker* p = ToSeqMaker(s);
  if (p == nullptr) {
    return nullptr;
  }

  try {
    return p->createTriGramString();
  } catch (const runtime_error& e) {
    return nullptr;
  }
}
}
