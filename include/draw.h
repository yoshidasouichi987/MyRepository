#ifndef __DRAW__
#define __DRAW__
#include "shared.h"
void DrawPanel();
void DrawComment();
void MakeFontHandles();
void DrawNodes(int radius);
extern int fonthandles[200];//0から199までの大きさ
extern char mystring[MAXDIGITS+1];
#endif