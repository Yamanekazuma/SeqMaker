#pragma once

#ifdef DLL_SEQ_MAKER
#define SEQ_MAKER_EXPORT __declspec(dllexport)
#else
#define SEQ_MAKER_EXPORT __declspec(dllimport)
#endif

#include "types/Registers.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* SEQMAKER;

SEQ_MAKER_EXPORT SEQMAKER SeqMaker_Init(uint32_t pid);
SEQ_MAKER_EXPORT void SeqMaker_DeInit(SEQMAKER seq);
SEQ_MAKER_EXPORT bool SeqMaker_AddInstruction(SEQMAKER seq, const Registers* regs);
SEQ_MAKER_EXPORT const char* SeqMaker_CreateUniGram(SEQMAKER seq);
SEQ_MAKER_EXPORT const char* SeqMaker_CreateBiGram(SEQMAKER seq);
SEQ_MAKER_EXPORT const char* SeqMaker_CreateTriGram(SEQMAKER seq);

#ifdef __cplusplus
}
#endif
