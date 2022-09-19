#include <stdlib.h>
#include <z80ex/z80ex.h>
#include "state.h"
#include "raylib.h"

Color getPixelColor(vpState *state){
	Color color = {10,20,30};
	return color;
}

int disPixel(vpState *state){
	DrawPixel(state->xpixel,state->ypixel+8,getPixelColor(state));
	return 0;
}

void readByte(vpState *state){
	switch(state->pixeloutof8){
		case(0):
			state->tID[0]=state->vram[0x2000+(state->aXscroll/8)+32*(state->aYscroll/8)];
			break;
		case(1):
			state->tID[1]=state->vram[0x2800+(state->aXscroll/8)+32*(state->aYscroll/8)];
			break;
		case(2):
			state->apixelbuffer[2]=state->vram[(state->tID[1]<<8)&1+state->tID[0]];
			break;
		case(3):
			state->apixelbuffer[3]=state->vram[(state->tID[1]<<8)&1+state->tID[0]]+1;
			break;
		case(4):
			state->tID[0]=state->vram[0x3000+(state->bXscroll/8)+32*(state->bYscroll/8)];
			break;
		case(5):
			state->tID[1]=state->vram[0x3800+(state->bXscroll/8)+32*(state->bYscroll/8)];
			break;
		case(6):
			state->bpixelbuffer[2]=1;
			break;
		case(7):
			state->bpixelbuffer[3]=1;
			break;
	}
	state->pixeloutof8+=state->pixeloutof8<8?1:-8;
}

void clockVP(vpState *state){
	disPixel(state);
	readByte(state);
}
