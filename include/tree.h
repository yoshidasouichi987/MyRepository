#ifndef __TREE__
#define __TREE__
#include <stdio.h>
#include <iostream>
#include "DxLib.h"

//extern bool isnull;

using namespace std;
struct Node{
    int value, size;
    char color; 
    Node *parent, *left, *right;

    //描画に使用
    int x, y;//木における左から何番目かと上から何番目か
    int dx, dy;//描画用メモ
    Node *leftedge, *rightedge;
    char dcolor;
    int dvalue;
    //

    Node();
    void reNode();
    void update();
    void update_all();
    void rotateL();
    void rotateR();
    void sub_rotate(Node *me);
};
Node *get(int index, Node *root);

Node *settle_red(Node *node);

Node *settle_blackL(Node *p);
Node *settle_blackR(Node *p);

Node *insert(int value, Node *root, Node node[]);
Node *remove(int value, Node *root);

Node *sub_insert(Node *node, Node *root);
Node *sub_remove(Node *node, Node *root);
#endif