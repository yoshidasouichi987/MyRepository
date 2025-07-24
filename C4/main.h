#ifndef __MAIN_H__
#define __MAIN_H__
#include <DxLib.h>
#include "loading.h"

extern int g_lasttime;
extern float g_frametime;
extern int g_timerstart;

enum GameState{
    GAME_TITLE,GAME_MAIN,GAME_CLEAR,GAME_OVER
}
extern GameState g_gamestate;

extern int middlefont;
extern int largefont;
extern BOOL g_akey_prev;

void DrawGameMain();
void DrawGameTitle();
void DrawGameOver();
void DrawGameClear();

BOOL isAKeyTrigger(int key);