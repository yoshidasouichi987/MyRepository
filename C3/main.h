#ifndef __MAIN_H__
#define __MAIN_H__
#include <DxLib.h>
#include "gamemain.h"
#include "loading.h"
//時間
extern int g_lasttime;
extern float g_frametime;
extern int g_timerstart;
//ゲーム状態
enum GameState{
    GAME_TITLE,GAME_MAIN,GAME_CLEAR,GAME_OVER
};
//画像
extern int g_gametitleimg;
extern int g_heroimg;
//ボタン状態
extern BOOL g_akey_prev;
//フォント
extern int g_middlefont;
extern int g_largefont;
//関数プロトタイプ宣言
void DrawGameTitle();
void DrawGameMain();
void DrawGameClear();
void DrawGameOver();
BOOL IsAKeyTrigger(int key);
#endif