#include <stdio.h>
#include <inttypes.h>
#include <Zydis/Zydis.h>

static const ZyanU8 SAMPLE_DATA[] = {0x60, 0x61, 0x83, 0xEC, 0x18, 0xC7, 0x04, 0x24, 0x31, 0xEB, 0x47, 0x00, 0xC3};

static const ZyanU32 SAMPLE_RUNTIME_ADDRESS = 0x400000;

int main(void) {
	ZydisDisassembledInstruction inst;
	ZyanUSize offset = 0;
	ZyanU32 address = SAMPLE_RUNTIME_ADDRESS;
	while (ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_COMPAT_32, address, &SAMPLE_DATA[offset], sizeof(SAMPLE_DATA) - offset, &inst))) {
		printf("%08" PRIx32 " %s\n", address, inst.text);
		offset += inst.info.length;
		address += inst.info.length;
	}
}

