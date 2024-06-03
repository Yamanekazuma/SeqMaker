#pragma once

#ifdef DLL_SEQ_MAKER
#define SEQ_MAKER_EXPORT __declspec(dllexport)
#else
#define SEQ_MAKER_EXPORT __declspec(dllimport)
#endif

#include "types/Registers.h"
#include "types/SeqUnit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* SEQMAKER;
typedef const void* SEQ_UNITDATA;

SEQ_MAKER_EXPORT SEQMAKER SeqMaker_Init(uint32_t pid, SEQ_UNITS unit);
SEQ_MAKER_EXPORT void SeqMaker_DeInit(SEQMAKER seq);

SEQ_MAKER_EXPORT SEQ_UNITDATA SeqMaker_AddInstruction(SEQMAKER seq, const Registers* regs);
SEQ_MAKER_EXPORT bool SeqMaker_GetUnitDataInfo(SEQ_UNITDATA unit, SEQ_UNITINFO_CONSTS info, void* output);
SEQ_MAKER_EXPORT bool SeqMaker_AddNote(SEQ_UNITDATA unit, const char* note);

SEQ_MAKER_EXPORT char* SeqMaker_CreateNGram(SEQMAKER seq, size_t n);

#ifdef __cplusplus
}
#endif
