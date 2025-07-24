#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__
#include <DxLib.h>
#include "main.h"

#define IMG_CHIPSIZE 50
#define MAP_WIDTH 500
#define MAP_WEIGHT 12
#define MAXSTAGE 1
#define SCR_WIDTH 16
#define ANIM_RATE 4
#define SCLOLL_STAPOS 500
extern char g_mapdate[MAP_HEIGHT][MAP_WIDTH+1];
struct CharaDate{
    float x,y;
    BOOL turn;
};
struct StageDate{
    int stagenum;
    int mapwidth;
    int animcounter;
    CharaDate hero; 
    float scrollx//画面左端の座標
};
extern StageDate g_stagedate;
void GameMain();
void InitStage();
void DrawHero(int ac);
void DrawMap();
#endif