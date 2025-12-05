#ifndef __KEYBOARD__
#define __KEYBOARD__

#include "shared.h"

//前回のキーの状態
extern bool up_keyprev, down_keyprev, z_keyprev, left_keyprev, right_keyprev;

//Zキーが押されたら木を操作する
void ProcessZkey();

//カーソルを動かす
void MoveCursol(int inputstate);

//zキー判定
BOOL isZkey(bool *prev);

//キー判定
BOOL iskey(bool *prev, int state, int target);

//数値入力を受け取る
void InputValue(int num);

//列挙体
enum CURSOL{
	ADD = 0, REMOVE = 1, ZERO = 2, CANCEL = 3, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE
};
//現在のカーソル位置
extern CURSOL cursol;

#endif