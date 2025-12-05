#ifndef __ANIME_QUEUE__
#define __ANIME_QUEUE__
#include "shared.h"

extern float actiontimer; //あるAction構造体の処理開始からの時間
extern bool edgeflag;
extern bool colorflag;
extern bool actionflag;//行うべきアクションがある



struct Action{
    Node **nodes; //アクションを行うノード郡
    int nodesize;
    char sort; //アクションの種類
    int *tox; //目標index座標郡
    int *toy;
    char *dcolor; //描画色
    int dvalue; //描画価値
    Node **leftedge; //張替え先の辺
    Node **rightedge; 
    int dheight; //描画高さ　indexにも使いまわします
    Action *next; //次のAction
    Action();
};


struct Queue{
    Action *head;
    Action *foot;
    Action actions[ACTIONSIZE];
    Action *searchspace();
    Queue();
    Action* poll();
    void pushM(Node **node, int nodesize, int *tox, int *toy);
    void pushE(Node **node, int nodesize,  Node *leftedge[], Node *rightedge[]);
    void pushC(Node **node, int nodesize, char dcolor[]);
    void pushV(Node **node, int dvalue);
    void pushH(int dheight);
    void pushN(Node **node, int dv, int x, int y);
};
extern Queue *queue;

struct CurAction{
    Node **nodes;
    int nodesize;
    char sort; //アクションの種類
    int *tox; //目標座標
    int *toy;
    char *dcolor; //描画色
    int dvalue; //描画価値
    Node **leftedge; //張替え先の辺
    Node **rightedge; 
    int dheight;
    CurAction();
};
extern CurAction *ca;

//actionflagがtrueのとき行う 
void ProcessAction();
#endif