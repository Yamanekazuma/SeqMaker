#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  SEQ_UNIT_ORIGINAL,
  SEQ_UNIT_MNEMONIC,
  SEQ_UNIT_OPCODE,
} SEQ_UNITS;

typedef enum {
  SEQ_UNITINFO_IS_CALL,
  SEQ_UNITINFO_IS_JMP,
  SEQ_UNITINFO_BRANCH_TO,
} SEQ_UNITINFO_CONSTS;

#ifdef __cplusplus
}
#endif
