#include <stdint.h>
#include <z80ex/z80ex.h>

class vpState{
public:
	uint8_t* vram,*sprite,*palette;
	uint8_t apixelbuffer[4],bpixelbuffer[4];
	uint8_t aXscroll,aYscroll,bXscroll,bYscroll;

	uint8_t pixeloutof8;
	uint8_t xpixel,ypixel;
	uint8_t tID[2];



	int memblocked;//set if cpu or dmc is accessing memory; reads 0 if data is accessed
	vpState(){
		sprite = new uint8_t[512];
		palette = new uint8_t[64];
		xpixel,ypixel = 0;
		pixeloutof8 = 0;
	}

	~vpState(){
		delete sprite;
		delete palette;
	}
};

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

class cpuState	{
public:
	uint8_t* memory;
	Z80EX_CONTEXT* context;
	vpState* vp;
	dmcState* dmc;
	sndState* snd;

	cpuState(){
		memory = new uint8_t[0x10000];
		context = z80ex_create(mread,this,mwrite,this,pread,this,pwrite,this,intread,this);
		z80ex_set_tstate_callback(context,onClock,this);
	}

	~cpuState(){
		delete memory;
		z80ex_destroy(context);
	}
};

