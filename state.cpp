#include <stdint.h>
#include <z80ex/z80ex.h>

typedef struct{
public:
	uint8_t test;
}dmcState,sndState;


Z80EX_BYTE mread(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data);
void mwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data);
Z80EX_BYTE pread(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data);
void pwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data);
Z80EX_BYTE intread(Z80EX_CONTEXT *cpu, void *user_data);
void onClock(Z80EX_CONTEXT *cpu, void *user_data);


