#include "animequeue.h"
#include "node.h"
using namespace std;

bool edgeflag = false; //辺の張替えがまだ行われていない
bool colorflag = false; //色変えがまだ行われていない
bool valueflag = false;//価値の塗替えがまだ行われていない
float actiontimer = 0; //ある構造体の処理開始からのカウントダウン
bool actionflag = false; //行うべきアクションがある

Queue *queue = new Queue();

//アクション構造体の先頭ポインタを格納する
CurAction *ca = new CurAction();  //今処理中の構造体の情報を移す器

Queue::Queue(){
    this->head = nullptr;
    this->foot = nullptr;
}

//null代入
Action::Action(){
    this->nodes = nullptr;
    this->nodesize = 0;
    this->sort = '\n';
    this->tox = nullptr;
    this->toy = nullptr;
    this->dcolor = nullptr;
    this->dvalue = 0;
    this->leftedge = nullptr;
    this->rightedge = nullptr;
    this->dheight = 0;
    this->next = nullptr;
}

//null代入
CurAction::CurAction(){
    this->nodes = nullptr;
    this->nodesize = 0;
    this->sort = '\n';
    this->tox = nullptr;
    this->toy = nullptr;
    this->dcolor = nullptr;
    this->dvalue = 0;
    this->leftedge = nullptr;
    this->rightedge = nullptr;
    this->dheight = 0;
}
Action* Queue::searchspace(){
    int ite = 0;
    //footでない　かつ　nextが存在しない ->利用可能
    while(!(ite < ACTIONSIZE && (&(this->actions[ite]) != this->foot) && (!this->actions[ite].next))){
        ite++;
    }
    return &(this->actions[ite]);
}

//先頭のActionのアドレス値を返す
Action* Queue::poll(){
    Action *ans = this->head;
    //キューが空
    if(!ans)return nullptr;
    //queuetimer = 1.0f;
    //残り1つ
    if(ans == this->foot){
        this->head = nullptr;
        this->foot = nullptr;
        return ans;
    }
    //残り２つ
    if(ans->next == this->foot){
        this->head = this->foot;
        ans->next = nullptr;
        return ans;
    }
    this->head = this->head->next;
    //headとnextを切る
    ans->next = nullptr;
    return ans;
}

//moveアクションを追加
void Queue::pushM(Node **nodes,int nodesize , int *tox, int *toy){
    Action *newact = queue->searchspace();
    newact->nodes = (Node**)malloc(sizeof(Node*)*nodesize);
    //nodes sort nodesize
    newact->nodesize = nodesize;
    newact->sort = 'm';
    newact->tox = (int*)malloc(sizeof(int)*nodesize);
    newact->toy = (int*)malloc(sizeof(int)*nodesize);
    //tox toy vx, vy　の計算 nodes
    for(int i = 0;i<nodesize;i++){
        newact->nodes[i] = nodes[i];
        newact->tox[i] = tox[i];
        newact->toy[i] = toy[i];
    }
    if(this->foot){
        //footとnewactをつなげる
        this->foot->next = newact;
        this->foot = newact;
    }//空のとき
    else{
        this->head = newact;
        this->foot = newact;
    }
}

void Queue::pushE(Node **nodes,int nodesize ,Node *leftedge[], Node *rightedge[]){
    Action *newact = queue->searchspace();
    newact->nodes = (Node**)malloc(sizeof(Node*)*nodesize);
    //nodes sort nodesize
    newact->nodesize = nodesize;
    newact->sort = 'e';
    newact->leftedge = (Node**)malloc(sizeof(Node*)*nodesize);
    newact->rightedge = (Node**)malloc(sizeof(Node*)*nodesize);
    //node edgeを写す
    for(int i = 0;i<nodesize;i++){
        newact->nodes[i] = nodes[i];
        newact->leftedge[i] = leftedge[i];
        newact->rightedge[i] = rightedge[i];
    }
    if(this->foot){
        //footとnewactをつなげる
        this->foot->next = newact;
        this->foot = newact;
    }//空のとき
    else{
        this->head = newact;
        this->foot = newact;
    }
}

void Queue::pushC(Node **nodes,int nodesize ,char dcolor[]){
    Action *newact = queue->searchspace();
    newact->nodes = (Node**)malloc(sizeof(Node*)*nodesize);
    //nodes sort nodesize
    newact->nodesize = nodesize;
    newact->sort = 'c';
    newact->dcolor = (char*)malloc(sizeof(char)*nodesize);
    //nodeを写す
    for(int i = 0;i<nodesize;i++){
        newact->nodes[i] = nodes[i];
        newact->dcolor[i] = dcolor[i];
    }
    if(this->foot){
        //footとnewactをつなげる
        this->foot->next = newact;
        this->foot = newact;
    }//空のとき
    else{
        //head = foot = newact
        this->head = newact;
        this->foot = newact;
    }
}

void Queue::pushV(Node **node, int dvalue){
    Action *newact = queue->searchspace();
    newact->nodes = (Node**)malloc(sizeof(Node*));
    //nodes sort nodesize
    newact->nodes[0] = node[0];
    newact->nodesize = 1;
    newact->sort = 'v';
    newact->dvalue = dvalue;
    //nodeを写す
    newact->nodes[0] = node[0];
    if(this->foot){
        //footとnewactをつなげる
        this->foot->next = newact;
        this->foot = newact;
    }//空のとき
    else{
        //head = foot = newact
        this->head = newact;
        this->foot = newact;
    }
}

void Queue::pushH(int dheight){
    Action *newact = queue->searchspace();
    //nodes sort nodesize　ノード不要
    newact->sort = 'h';
    newact->dheight = dheight;
    newact->nodesize = 0;
    newact->nodes = nullptr;
    if(this->foot){
        //footとnewactをつなげる
        this->foot->next = newact;
        this->foot = newact;
    }//空のとき
    else{
        //head = foot = newact
        this->head = newact;
        this->foot = newact;
    }
}

void Queue::pushN(Node **node, int dv, int x, int y){
    Action *newact = queue->searchspace();
    newact->nodes = (Node**)malloc(sizeof(Node*));
    //nodes sort nodesize
    newact->nodes[0] = node[0];
    newact->sort = 'n';
    newact->nodesize = 1;
    newact->dvalue = dv;
    newact->dheight = getIndex(x, y);

    if(this->foot){
        //footとnewactをつなげる
        this->foot->next = newact;
        this->foot = newact;
    }//空のとき
    else{
        //head = foot = newact
        this->head = newact;
        this->foot = newact;
    }
}

//g_actionflagがtrueのとき行ってください
void ProcessAction(){
    //このアクションが終わっている/新しく始まる
    if(actiontimer <= 0){
        //moveの場合 位置を微調整する
        if(ca->sort == 'm'){
            for(int i = 0;i<ca->nodesize;i++){
                if(!ca->nodes[i])continue;
                setGrid(ca->nodes[i], ca->tox[i], ca->toy[i]);
            }
            //caの配列を解放する
            if(ca->tox){
                free(ca->tox);
                ca->tox = nullptr;
            }
            if(ca->toy){
                free(ca->toy);
                ca->toy = nullptr;
            }
        }
        //edgeの場合
        else if(ca->sort == 'e'){
            if(ca->leftedge){
                free(ca->leftedge);
                ca->leftedge = nullptr;
            }
            if(ca->rightedge){
                free(ca->rightedge);
                ca->rightedge = nullptr;
            }
        }
        //colorの場合
        else if(ca->sort == 'c'){
            if(ca->dcolor){
                free(ca->dcolor);
                ca->dcolor = nullptr;
            }
        }
        //atanyrate
        if(ca->nodes){
            free(ca->nodes);
            ca->nodes = nullptr;
        }
        //新しいアクションを取り出す
        Action *newa = queue->poll();
        //すべてのアクションをやりきった ->　リセット
        if(!newa){
            actionflag = false;
            actiontimer = 0;
            return;
        }
        //newa(新しいアクション)を処理する
        else{
            actionflag = true;
            //caの新しい配列を作る
            //node nodesize sort
            ca->nodes = (Node**)malloc(sizeof(Node*)*(newa->nodesize));
            ca->nodesize = newa->nodesize;
            ca->sort = newa->sort;
            //nodesをうつす
            if(ca->nodes && newa->nodes){
                for(int i = 0;i<(ca->nodesize);i++){
                    ca->nodes[i] = newa->nodes[i];
                }
            }
            //move
            if(newa->sort == 'm'){
                ca->tox = (int*)malloc(sizeof(int)*newa->nodesize);
                ca->toy = (int*)malloc(sizeof(int)*newa->nodesize);
                for(int i = 0;i<(newa->nodesize);i++){
                    ca->tox[i] = newa->tox[i];
                    ca->toy[i] = newa->toy[i];
                }
                //newa解放
                free(newa->tox);
                free(newa->toy);

                //部分木に、どこに向かうべきかメモさせる
                for(int i = 0;i<newa->nodesize;i++){
                    setDisplacement(ca->nodes[i], ca->tox[i], ca->toy[i]);
                }
                actiontimer = MOVETIME;
            }
            //edgeの場合左右の辺アドレス
            else if(newa->sort == 'e'){
                ca->leftedge = (Node**)malloc(sizeof(Node*)*ca->nodesize);
                ca->rightedge = (Node**)malloc(sizeof(Node*)*ca->nodesize);
                for(int i = 0;i<(ca->nodesize);i++){
                    ca->leftedge[i] = newa->leftedge[i];
                    ca->rightedge[i] = newa->rightedge[i];
                }
                //newa解放
                free(newa->leftedge);
                free(newa->rightedge);

                actiontimer = EDGETIME;
                edgeflag = true;
            }
            //color 
            else if(newa->sort == 'c'){
                ca->dcolor = (char*)malloc(sizeof(char)*(newa->nodesize));
                for(int i = 0;i<(ca->nodesize);i++){
                    ca->dcolor[i] = newa->dcolor[i];
                }
                free(newa->dcolor);
                actiontimer = COLORTIME;
                colorflag = true;
            }//value
            else if(newa->sort == 'v'){
                ca->dvalue = newa->dvalue;
                actiontimer = VALUETIME;
                valueflag = true;
            }
            //以下,caを更新しない
            //dheightを一定ループあとまでに変更する
            else if(newa->sort == 'h'){
                ca->dheight = newa->dheight;
                dhnext = ca->dheight;
                dhv = ((float)(dhnext) - dheight)/HEIGHTTIME;
                dwv = ((float)(getpow(dhnext) - getpow((int)dheight)))/HEIGHTTIME;
                dhtimer = HEIGHTTIME;
            }//フレームにノードの追加
            else if(newa->sort == 'n'){
                printf("addingNode\n");
                //ca->nodes[0]->reNode();
                //該当ノード(nullptrの場合あり)をframeに入れる
                ca->dvalue = newa->dvalue;
                //座標
                ca->dheight = newa->dheight;
                int index = ca->dheight;
                pair<int,int> p = getGrid(index);
                int x = p.first;
                int y = p.second;
                printf("put!");
                //nullptr/g_nodeのアドレス
                if(g_frame[index])g_frame[index]->reNode();
                g_frame[index] = ca->nodes[0];
                if(g_frame[index]){
                    g_frame[index]->dvalue = ca->dvalue;
                    g_frame[index]->dcolor = 'R';
                    g_frame[index]->x = x;
                    g_frame[index]->y = y;
                    g_frame[index]->dx = g_frame[index]->dy = -1;
                    g_frame[index]->leftedge = g_frame[index]->rightedge = nullptr;
                }
                //次のアクションにいく
            }
        }
        free(newa->nodes);
        //delete newa;
    }
    //アクション中である
    else{
        //move
        if(ca->sort == 'm'){
            //特になし
        }//edge
        else if(ca->sort == 'e'){
            if(actiontimer * 2 <= EDGETIME && edgeflag){
                edgeflag = false;
                for(int i = 0;i<ca->nodesize;i++){
                    if(!ca->nodes[i])continue;
                    (ca->nodes[i])->leftedge = ca->leftedge[i];
                    (ca->nodes[i])->rightedge = ca->rightedge[i];
                }
            }
        }//color
        else if(ca->sort == 'c'){
            if(actiontimer * 2 <= COLORTIME && colorflag){
                colorflag = false;
                for(int i = 0;i<ca->nodesize;i++){
                    ca->nodes[i]->dcolor = ca->dcolor[i];
                }
            }
        }//value
        else if(ca->sort == 'v'){
            if(actiontimer * 2 <= VALUETIME && valueflag){
                valueflag = false;
                ca->nodes[0]->dvalue = ca->dvalue;
            }
        }//その他 特になし
    }
}