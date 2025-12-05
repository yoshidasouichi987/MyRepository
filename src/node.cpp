#include "node.h"
#include "tree.h"
using namespace std;
int height = 0;
float dheight = 0;
float dhtimer = 0;
float dhv = 0;
float dwv = 0;
float dwidth = 1;
int dhnext = 0;

//2^num
int getpow(int num){
    int ans = 1;
    for(int i = 0;i<num;i++)ans *= 2;
    return ans;
}

//indexを座標に変換
pair<int, int> getGrid(int num){
    int bi = 1;
    int d = 0;
    while(num >= bi*2){
        bi *= 2;
        d++;
    }
    return {num-bi, d};
}

//座標を番号に変換
int getIndex(int x, int y){
    int ans = 1;
    for(int i = 0;i<y;i++){
        ans += getpow(i);
    }
    ans += x;
    return ans;
}

//nodeに座標を与える.リセットする
void setNodeGrid(){
    for(int i = 1;i<FRAMESIZE;i++){
        if(!g_frame[i])continue;
        pair<int, int> p = getGrid(i);
        g_frame[i]->x = p.first;
        g_frame[i]->y = p.second;

        g_frame[i]->dx = -1;
        g_frame[i]->dy = -1;
        //一応　実際の子と　edgeの内容を一致させる
        g_frame[i]->leftedge = g_frame[i]->left;
        g_frame[i]->rightedge = g_frame[i]->right;
        //一応　実際の色と dcを一致させる
        g_frame[i]->dcolor = g_frame[i]->dcolor;
        //一応実際の価値と dvを一致させる
        g_frame[i]->dvalue = g_frame[i]->dvalue;
    }
}

//部分木に、どこに向かうべきかメモさせる
void setDisplacement(Node *node, int tox, int toy){
    if(!node)return;
    node->dx = tox;
    node->dy = toy;
    if(node->leftedge)setDisplacement(node->leftedge, tox*2, toy+1);
    if(node->rightedge)setDisplacement(node->rightedge, tox*2+1, toy+1);
}

//部分木のg_frameにおける位置をずらす
void setGrid(Node *node, int tox, int toy){
    if(!node)return;
    int index = getIndex(tox, toy);
    g_frame[index] = node;
    if(node->dx != -1){
        //かつてあったゴミポインタを消す
        int jndex = getIndex(node->x, node->y);
        g_frame[jndex] = nullptr;
    }
    node->x = tox;
    node->y = toy;
    node->dx = -1;
    node->dy = -1;
    if(node->leftedge)setGrid(node->leftedge, tox*2, toy+1);
    if(node->rightedge)setGrid(node->rightedge, tox*2+1, toy+1);
}

