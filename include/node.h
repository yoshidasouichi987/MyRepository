#ifndef __NODE_H__
#define __NODE_H__
#include "shared.h"

struct Node;

extern int height;//木の高さ
extern float dheight;//描画における木の高さ
extern float dhv; //変化速度
extern float dwv;
extern int dhnext; //次の目標
extern float dhtimer; //高さタイマー
extern float dwidth;//描画における木の最大幅

//nodeの実際の変数と描画変数を瞬時に一致させる
void setNodeGrid();

//部分木に、どこに向かうべきかメモさせる
void setDisplacement(Node *node, int tox, int toy);

//部分木のg_frameにおける位置をずらす
void setGrid(Node *node, int tox, int toy);

#endif