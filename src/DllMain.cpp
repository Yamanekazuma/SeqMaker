#include "SeqMaker.h"
#include "SeqMaker.hpp"
#include "SeqUnitMnemonic.hpp"
#include "SeqUnitOpcode.hpp"
#include "SeqUnitOriginal.hpp"

#include <cstdint>
#include <cstring>
#include <stdexcept>

using namespace std;
using namespace Seq;

class SEQMAKER_ {
public:
  SEQ_UNITS unit;
  void* seq;
};

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
  if (s->seq != nullptr && (s->unit == SEQ_UNIT_ORIGINAL || s->unit == SEQ_UNIT_MNEMONIC || s->unit == SEQ_UNIT_OPCODE)) {
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
  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  if (s->unit != SEQ_UNIT_ORIGINAL && s->unit != SEQ_UNIT_MNEMONIC && s->unit != SEQ_UNIT_OPCODE) {
    return true;
  }

  try {
    switch (s->unit) {
      case SEQ_UNIT_ORIGINAL:
        static_cast<SeqMaker<SeqUnitOriginal>*>(s->seq)->addInstruction(*regs);
        break;
      case SEQ_UNIT_MNEMONIC:
        static_cast<SeqMaker<SeqUnitMnemonic>*>(s->seq)->addInstruction(*regs);
        break;
      case SEQ_UNIT_OPCODE:
        static_cast<SeqMaker<SeqUnitOpcode>*>(s->seq)->addInstruction(*regs);
        break;
    }
  } catch (const runtime_error& e) {
    return true;
  }
  return false;
}

SEQ_MAKER_EXPORT char* SeqMaker_CreateUniGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }
  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  if (s->unit != SEQ_UNIT_ORIGINAL && s->unit != SEQ_UNIT_MNEMONIC && s->unit != SEQ_UNIT_OPCODE) {
    return nullptr;
  }

  const char* ngram_cstr = nullptr;
  try {
    switch (s->unit) {
      case SEQ_UNIT_ORIGINAL:
        ngram_cstr = static_cast<SeqMaker<SeqUnitOriginal>*>(seq)->createNGram<1>().toString().c_str();
        break;
      case SEQ_UNIT_MNEMONIC:
        ngram_cstr = static_cast<SeqMaker<SeqUnitMnemonic>*>(seq)->createNGram<1>().toString().c_str();
        break;
      case SEQ_UNIT_OPCODE:
        ngram_cstr = static_cast<SeqMaker<SeqUnitOpcode>*>(seq)->createNGram<1>().toString().c_str();
        break;
    }
  } catch (const runtime_error& e) {
    return nullptr;
  }

  return strdup(ngram_cstr);
}

SEQ_MAKER_EXPORT char* SeqMaker_CreateBiGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }
  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  if (s->unit != SEQ_UNIT_ORIGINAL && s->unit != SEQ_UNIT_MNEMONIC && s->unit != SEQ_UNIT_OPCODE) {
    return nullptr;
  }

  const char* ngram_cstr = nullptr;
  try {
    switch (s->unit) {
      case SEQ_UNIT_ORIGINAL:
        ngram_cstr = static_cast<SeqMaker<SeqUnitOriginal>*>(seq)->createNGram<2>().toString().c_str();
        break;
      case SEQ_UNIT_MNEMONIC:
        ngram_cstr = static_cast<SeqMaker<SeqUnitMnemonic>*>(seq)->createNGram<2>().toString().c_str();
        break;
      case SEQ_UNIT_OPCODE:
        ngram_cstr = static_cast<SeqMaker<SeqUnitOpcode>*>(seq)->createNGram<2>().toString().c_str();
        break;
    }
  } catch (const runtime_error& e) {
    return nullptr;
  }

  return strdup(ngram_cstr);
}

SEQ_MAKER_EXPORT char* SeqMaker_CreateTriGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }
  SEQMAKER_* s = static_cast<SEQMAKER_*>(seq);
  if (s->unit != SEQ_UNIT_ORIGINAL && s->unit != SEQ_UNIT_MNEMONIC && s->unit != SEQ_UNIT_OPCODE) {
    return nullptr;
  }

  const char* ngram_cstr = nullptr;
  try {
    switch (s->unit) {
      case SEQ_UNIT_ORIGINAL:
        ngram_cstr = static_cast<SeqMaker<SeqUnitOriginal>*>(seq)->createNGram<3>().toString().c_str();
        break;
      case SEQ_UNIT_MNEMONIC:
        ngram_cstr = static_cast<SeqMaker<SeqUnitMnemonic>*>(seq)->createNGram<3>().toString().c_str();
        break;
      case SEQ_UNIT_OPCODE:
        ngram_cstr = static_cast<SeqMaker<SeqUnitOpcode>*>(seq)->createNGram<3>().toString().c_str();
        break;
    }
  } catch (const runtime_error& e) {
    return nullptr;
  }

  return strdup(ngram_cstr);
}
}
