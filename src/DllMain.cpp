#include "SeqMaker.h"
#include "SeqMaker.hpp"

#include <cstdint>
#include <stdexcept>

using namespace std;
using namespace Seq;

extern "C" {

BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/, DWORD /*fdwReason*/, LPVOID /*lpReserved*/) {
  return TRUE;
}

SEQ_MAKER_EXPORT SEQMAKER SeqMaker_Init(uint32_t pid) {
  SeqMaker* seq = nullptr;
  try {
    seq = new SeqMaker(pid);
    return static_cast<SEQMAKER>(seq);
  } catch (const runtime_error& e) {
    return static_cast<SEQMAKER>(nullptr);
  }
}

SEQ_MAKER_EXPORT void SeqMaker_DeInit(SEQMAKER seq) {
  if (seq == nullptr) {
    return;
  }

  delete static_cast<SeqMaker*>(seq);
}

SEQ_MAKER_EXPORT bool SeqMaker_AddInstruction(SEQMAKER seq, const Registers* regs) {
  if (seq == nullptr || regs == nullptr) {
    return true;
  }
  SeqMaker* p = static_cast<SeqMaker*>(seq);
  try {
    p->addInstruction(*regs);
  } catch (const runtime_error& e) {
    return true;
  }
  return false;
}

SEQ_MAKER_EXPORT const char* SeqMaker_CreateUniGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }
  SeqMaker* p = static_cast<SeqMaker*>(seq);
  try {
    auto&& ngram = p->createNGram<1>();
    return ngram.toString().c_str();
  } catch (const runtime_error& e) {
    return nullptr;
  }
}

SEQ_MAKER_EXPORT const char* SeqMaker_CreateBiGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }
  SeqMaker* p = static_cast<SeqMaker*>(seq);
  try {
    auto&& ngram = p->createNGram<2>();
    return ngram.toString().c_str();
  } catch (const runtime_error& e) {
    return nullptr;
  }
}

SEQ_MAKER_EXPORT const char* SeqMaker_CreateTriGram(SEQMAKER seq) {
  if (seq == nullptr) {
    return nullptr;
  }
  SeqMaker* p = static_cast<SeqMaker*>(seq);
  try {
    auto&& ngram = p->createNGram<3>();
    return ngram.toString().c_str();
  } catch (const runtime_error& e) {
    return nullptr;
  }
}
}
