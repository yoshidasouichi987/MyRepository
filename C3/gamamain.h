#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__
#include <DxLib.h>
#include "main.h"

#define IMG_CHIPSIZE 50
#define MAP_WIDTH 60
#define MAP_HEIGHT 12
#define MOVERATE 15
#define MAXENEMY 20
#define FIREEXPAND 3
#define FIRESHRINK 6//炎が伸び始めてから縮み切るまでの時間
#define FIRETOP 12
#define MAXSTAGE 2
//マップのアイテム位置
extern int g_mapdate[MAP_HEIGHT][MAP_WIDTH];
enum MapItem{
    MPITEM_NO,MPITEM_ROCK,MPITEM_HMONSTER,MPITEM_VMONSTER,MPITEM_DRAGON,MPITEM_HEROPOS = 7,MPITEM_KEY,MPITEM_GOAL
};
//人物データ
enum MonsterType{
    MT_HOLI = 2,MT_VERT,MT_DRAGON,MT_FIRE
};
struct CharaDate{
    int x,y;
    BOOL living;
    MonsterType type;
    int direction;
    int fireidx;//自身の炎が配列enemiesの何番目に格納されるかを記録
    int firetimer;//炎を出している時間のカウンタ
};
//ステージデータ
struct StageDate{
    int herox,heroy;
    int movecounter;
    CharaDate enemies[MAXENEMY];
    BOOL getkey;
    int stagenum;
};
extern StageDate g_stagedate;
//関数プロトタイプ宣言
void GameMain();
void DrawMap();
void InitStage();
void DrawEnemy();
void DrawDragonFire(int idx);
#endif