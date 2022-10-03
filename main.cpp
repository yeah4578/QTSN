#include "state.h"
#include <stdio.h>
#include <stdlib.h>
#include <z80ex/z80ex.h>
#include <string>
#include "raylib.h"

Font FourBySix;
RenderTexture2D target;
const int canvasWidth = 256;
const int canvasHeight = 248;
float xScale = 2;
float yScale = 2;
int screenWidth = canvasWidth*xScale;
int screenHeight = canvasHeight*yScale;
int framecount = 0;

void onClock(Z80EX_CONTEXT *cpu, void *user_data){
	cpuState* state = (cpuState*)user_data;
	state->vp->clock();
}

bool mouseWithinRect(float x, float y, float width, float height){
	float mouseX = GetMouseX();
	float mouseY = GetMouseY();
	mouseX -= (screenWidth/yScale-canvasWidth)*0.5f;
	mouseY -= (screenHeight/xScale-canvasHeight)*0.5f;
	return CheckCollisionPointRec({mouseX,mouseY},{x,y,width,height});
}

void DisplayMenuBar(){
	DrawTextEx(FourBySix,"Load",(Vector2){0,0},6,1,mouseWithinRect(0,0,19,8)?BLUE:WHITE);
	DrawTextEx(FourBySix,"Save",(Vector2){32,0},6,1,mouseWithinRect(32,0,19,8)?BLUE:WHITE);
	DrawTextEx(FourBySix,"Options",(Vector2){64,0},6,1,mouseWithinRect(64,0,35,8)?BLUE:WHITE);
	DrawTextEx(FourBySix,"QTSN ver.0 ",(Vector2){201,0},6,1,DARKGRAY);
	DrawTextEx(FourBySix,TextFormat("%08d", framecount++),(Vector2){101,0},6,1,DARKGRAY);
	DrawTextEx(FourBySix,TextFormat("%d", GetFPS()),(Vector2){151,0},6,1,DARKGRAY);
	DrawLine(0,7,255,7,DARKGRAY);
}


int RunScanline(cpuState *state){
	int i = state->overCycle;
	for(;i<341;){
		i += z80ex_step(state->context);
	}
	return i-341;
}

void RunFrame(cpuState *state){
	for(int i=0;i<262;i++)
		state->overCycle = RunScanline(state);
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
	InitWindow(screenWidth,screenHeight,"QTSN");
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	ClearBackground(BLACK);

	SetMouseScale(1/xScale, 1/yScale);
	target = LoadRenderTexture(canvasWidth, canvasHeight);
	FourBySix = LoadFont("font.png");
}

int main(){
	cpuState CPU;
	vpState VPU;
	dmcState DMC;
	sndState SND;
	CPU.vp = &VPU;
	VPU.vram = CPU.memory+0xc000;
	if(loadPRG(CPU.memory, "boot.bin")==-1){
		return -1;
	}

	initializeWindow();

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if(IsWindowResized()){
			ClearBackground(BLACK);
			screenHeight = GetScreenHeight();
			screenWidth = GetScreenWidth();
			yScale=(float)screenHeight/canvasHeight;
			xScale=(float)screenWidth/canvasWidth;
			if(xScale>yScale)
				xScale=yScale;
			else
				yScale=xScale;
			SetMouseScale(1/xScale,1/yScale);

		}
		BeginTextureMode(target);
			ClearBackground(BLACK);
			DisplayMenuBar();
			RunFrame(&CPU);
		EndTextureMode();

		BeginDrawing();
		DrawTexturePro(target.texture,
			Rectangle{ 0, 0, (float)target.texture.width, (float)-target.texture.height },
			Rectangle{(screenWidth-canvasWidth*yScale)*0.5f,(screenHeight-canvasHeight*xScale)*0.5f,
				canvasWidth*xScale,canvasHeight*yScale},
			Vector2{}, 0, WHITE);
		EndDrawing();
	}
}
