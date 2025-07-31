#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__
#include <DxLib.h>
#include "main.h"
#include "enemy.h"

#define IMG_CHIPSIZE 50
#define MAP_WIDTH 500
#define MAP_HEIGHT 12
#define SCR_WIDTH 16
#define SCROLL_STAPOS 500
#define ANIM_RATE 4
#define JUMP_POWER 750.0f
#define GRAVITY 30.0f
#define JUMP_FORWARD 200.0f
#define MAX_ENEMY 25
#define MAX_KNIFE 4
#define MAX_STAGE 1

extern char g_mapdate[MAP_HEIGHT][MAP_WIDTH+1];
struct CharaDate{
    float x,y;
    BOOL turn;
    BOOL noground;
    BOOL jumping;
    float jumppower,jumpforward;
    BOOL living;
    EnemyType type;
    int life;
};
struct StageDate{
    int stagenum;
    int mapwidth;
    int animcounter;
    CharaDate hero; 
    float scrollx//画面左端の座標
    CharaDate enemies[MAX_ENEMY];
    CharaDate knives[MAX_KNIFE];
    BOOL g_bkey_prev;
};
extern StageDate g_stagedate;
struct AtariInfo{
    BOOL UL,UR,DL,DR,GL,GR;
};
enum EnemyType{
    ET_CRAB = 2,
    ET_OCT = 3,
    ET_BULLET = 4
};
void GameMain();
void InitStage();
void DrawHero(int ac);
void DrawMap();
AtariInfo CheckBlock(float x,float y,float rx);
void DrawKnife(int key);
BOOL IsBkeyTrigger(int key);
#endif