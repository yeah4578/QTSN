#include <z80ex/z80ex.h>
#include <stdlib.h>
#include <stdio.h>
#include "state.h"

Z80EX_BYTE mread(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data){
	cpuState* state = (cpuState*)user_data;
	Z80EX_BYTE readByte = state->memory[addr];
	if(addr>0xbfff)
		state->vp->memblocked = 1;
	return readByte;
}

void mwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data){
	cpuState* state = (cpuState*)user_data;
	if(addr>0x3fff){
		state->memory[addr] = value;
		if(addr>0xbfff)
			state->vp->memblocked = 1;
	}
}

Z80EX_BYTE pread(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data){
	return 0;
}

void pwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data){
}

Z80EX_BYTE intread(Z80EX_CONTEXT *cpu, void *user_data){
	return 0;
}

void onClock(Z80EX_CONTEXT *cpu, void *user_data){
	cpuState* state = (cpuState*)user_data;
	state->vp->clock();
}




