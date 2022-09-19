#include "state.h"
#include <stdio.h>
#include <z80ex/z80ex.h>
#include <string>
#include "raylib.h"

Font FourBySix;
RenderTexture2D target;
const int canvasWidth = 256;
const int canvasHeight = 256;
float xScale = 2;
float yScale = 2;
int screenWidth = canvasWidth*xScale;
int screenHeight = canvasHeight*yScale;

void DisplayMenuBar(){
	DrawTextEx(FourBySix,"Load",(Vector2){0,0},6,1,WHITE);
	DrawTextEx(FourBySix,"Save",(Vector2){32,0},6,1,WHITE);
	DrawTextEx(FourBySix,"Options",(Vector2){64,0},6,1,WHITE);
}

void RunFrame(){


}


int loadPRG(uint8_t* rom, std::string filename){
	FILE* bootCode;
	if(bootCode = fopen(filename.c_str(),"r")){
		if(0x4000==fread(rom,1,0x4001,bootCode))//0x4001 to check for oversized ROM file
			return 0;
		printf("Init failed: Invalid boot.bin.\n");
		return -1;
	}
	printf("Init failed: Can't find boot.bin.\n");
	return -1;
}

void initializeWindow(){
	InitWindow(screenWidth,screenHeight,"amiongus");
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	ClearBackground(BLACK);

	SetMouseScale(1/xScale, 1/yScale);
	target = LoadRenderTexture(canvasWidth, canvasHeight);
	FourBySix = LoadFont("font.png");
}

int main(){
	vpState VP;
	dmcState DMC;
	sndState SND;
	cpuState CPU;
	CPU.vp = &VP;
	CPU.dmc = &DMC;
	CPU.snd = &SND;
	VP.vram = CPU.memory+0xc000;
	if(loadPRG(CPU.memory, "boot.bin")==-1){
		return -1;
	}

	initializeWindow();

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if(IsWindowResized()){
			ClearBackground(DARKGRAY);
			screenHeight = GetScreenHeight();
			screenWidth = GetScreenWidth();
			yScale=(float)screenHeight/canvasHeight;
			xScale=(float)screenWidth/canvasWidth;
			if(xScale>yScale)
				SetMouseScale(1/yScale,1/yScale);
			else
				SetMouseScale(1/xScale,1/xScale);
		}
		BeginTextureMode(target);
		ClearBackground(BLACK);
		DisplayMenuBar();
// 		RunFrame();
		EndTextureMode();

		BeginDrawing();
		DrawTexturePro(target.texture,
			       Rectangle{ 0, 0, (float)target.texture.width, (float)-target.texture.height },
			       xScale>yScale
			       ?Rectangle{screenWidth/2-canvasWidth/2*yScale,0,canvasWidth*yScale,(float)screenHeight}
			       :Rectangle{0,screenHeight/2-canvasHeight/2*xScale,(float)screenWidth,canvasHeight*xScale},
			       Vector2{}, 0, WHITE);
		EndDrawing();
	}
}
