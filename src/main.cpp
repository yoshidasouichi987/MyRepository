#include "main.h"
#include <iostream>
using namespace std;

int g_fonthandle = 0;
int lasttime = 0;
float g_frametime = 0.0f;


Node g_node[MAXNODENUM];
Node *g_frame[FRAMESIZE];
Node *g_root = nullptr;

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow)
{
	//特に意味はない
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(WIDTH,HEIGHT,32);
	if(DxLib_Init() == -1)return -1;
	
	g_fonthandle =  CreateFontToHandle(NULL, 15, 3);
	MakeFontHandles();
	int radius;

	while(ProcessMessage() == 0){
		//時間計算
		int curtime = GetNowCount();
		g_frametime = (float)(curtime - lasttime)/1000.0f;
		lasttime = curtime;
		//actiontimer
		if(actiontimer > 0){
			actiontimer -= g_frametime;
		}
		//height width操作
		if(0 < dhtimer){
			//高さを変化
			dheight += dhv*g_frametime;
			dwidth += dwv*g_frametime;
			dhtimer -= g_frametime;
		}else if(dhtimer < 0){
			//完成した高さに固定
			dheight = (float)dhnext;
			dwidth = getpow(dhnext);
			dhtimer = 0;
		}
		//アクションが溜まったか
		if(queue->head){
			actionflag = true;
		}
		//入力状況
		int inputstate = GetJoypadInputState(DX_INPUT_KEY);
		MoveCursol(inputstate);
		ProcessZkey();
		if(actionflag)ProcessAction();
		ClearDrawScreen();
		DrawPanel();
		DrawComment();
		if(dheight&&dwidth)radius = min((HEIGHT/dheight)/3, (WIDTH/dwidth)/3);
		else radius = HEIGHT/3;
		DrawNodes(radius);
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
//libfreetype6 libfontconfig1 