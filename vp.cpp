#include <stdlib.h>
#include <z80ex/z80ex.h>
#include "raylib.h"
#include <stdio.h>

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
	int memblocked;//set if cpu or dmc is accessing memory; reads 0 if data is accessed

	//display functions
	Color getPixelColor(){
		uint8_t amask = 0x80>>(7&aXscroll);
		return Color {0,0,0,0xff};
	}
	int disPixel(){
		DrawPixel(xpixel,ypixel+8,getPixelColor());
		if(++xpixel>=341)
			xpixel = 0;
		return 0;
	}
	void readByte(){
		switch(pixeloutof8){
			case(0):
				tID[0]=vram[0x2000+(aXscroll/8)+32*(aYscroll/8)];
				break;
			case(1):
				tID[1]=vram[0x2800+(aXscroll/8)+32*(aYscroll/8)];
				break;
			case(2):
				apixelbuffer[2]=vram[(tID[1]<<8)&1+tID[0]];
				break;
			case(3):
				apixelbuffer[3]=vram[(tID[1]<<8)&1+tID[0]]+1;
				break;
			case(4):
				tID[0]=vram[0x3000+(bXscroll/8)+32*(bYscroll/8)];
				break;
			case(5):
				tID[1]=vram[0x3800+(bXscroll/8)+32*(bYscroll/8)];
				break;
			case(6):
				bpixelbuffer[2]=1;
				break;
			case(7):
				bpixelbuffer[3]=1;
				break;
		}
		if(pixeloutof8++>=8)
			pixeloutof8=0;
	}





public:
	uint8_t *vram;
	vpState(){
		sprite = new uint8_t[512];
		palette = new uint8_t[64];
		xpixel = 0;
		ypixel = 0;
		pixeloutof8 = 0;
	}
	~vpState(){
		delete sprite;
		delete palette;
	}
	void clock(){
		disPixel();
		readByte();
	}
	void writeReg(){

	}
};





