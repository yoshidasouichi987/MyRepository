#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__
#include "gamemain.h"

struct CharaDate;

void SetEnemy(int mx,int my);
void DrawEnemy(int ac);
CharaDate MoveCrabMonster(CharaDate cd);
CharaDate MoveOctoMonster(CharaDate cd);
CharaDate MoveBullet(CharaDate cd);
void AtariHeroAndMonster(int idx);

#endif