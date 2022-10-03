#include "raylib.h"
#include <stdint.h>
#include <z80ex/z80ex.h>

class vpState{
private:
	uint8_t *sprite,*palette;
	uint8_t apixelbuffer[4],bpixelbuffer[4];
	uint8_t aXscroll,aYscroll,bXscroll,bYscroll;

	int pixeloutof8;
	uint16_t xpixel;
	uint16_t ypixel;
	uint8_t tID[2];
	uint8_t attr[4];

	Color getPixelColor();
	int disPixel();
	void readByte();
public:
	uint8_t *vram;
	int memblocked;//set if cpu or dmc is accessing memory; reads 0 if data is accessed
	vpState();
	~vpState();
	void clock();
	void writeReg();
};

class dmcState{
	uint16_t dmabuffer[32];//buffer space for 8 DMA commands
	uint8_t aiobuffer[32];//buffer space for 16 AIO commands
	uint16_t IRQreload;//value to put into IRQtmr after it hits zero
	uint16_t IRQtmr;
	uint8_t IRQctrl;


	uint8_t byteCount;//counter for AIO or DMA bytes written

	void genIRQ();
	void clockDMA();
	void clockAIO();
	void clockTimer();

public:
	dmcState();
	~dmcState();
	void clock();
	void writeIO(int port, uint8_t value);
};

typedef struct{
	uint8_t test;
}sndState;


Z80EX_BYTE mread(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data);
void mwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data);
Z80EX_BYTE pread(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data);
void pwrite(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data);
Z80EX_BYTE intread(Z80EX_CONTEXT *cpu, void *user_data);
void onClock(Z80EX_CONTEXT *cpu, void *user_data);

class cpuState	{
public:
	uint8_t* memory;
	int overCycle;
	vpState *vp;
	Z80EX_CONTEXT* context;
	void interrupt();
	cpuState();
	~cpuState();
};

typedef struct{
	vpState VP;
	cpuState CPU;
	dmcState DMC;
	sndState SND;
}sysContainer;
