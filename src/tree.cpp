#include "tree.h"
#include "node.h"
#include "animequeue.h"
using namespace std;
//bool isnull = false;
Node::Node(){
    this->size = 1;
    this->dcolor = 'R';
    this->color = 'R';
    this->parent = this->left = this->right = nullptr;
    this->leftedge = this->rightedge = nullptr;
    this->x = 0;
    this->y = 0;
    this->dx = -1;
    this->dy = -1;
    this->dvalue = 0;
    this->value = 0;
}
void Node::reNode(){
    this->size = 1;
    this->dcolor = 'R';
    this->color = 'R';
    this->parent = this->left = this->right = nullptr;
    this->leftedge = this->rightedge = nullptr;
    this->x = 0;
    this->y = 0;
    this->dx = -1;
    this->dy = -1;
    this->dvalue = 0;
    this->value = 0;
}
//部分木の最大の深さをとる
int getDeepest(Node *sub){
    Node *l = sub->left;
    Node *r = sub->right;
    int ld = (l?getDeepest(l):0);
    int rd = (r?getDeepest(r):0);
    return 1 + max(ld, rd);
}

pair<int, int> getTruePos(Node *root, int value){
    Node *now = root;
    int x=0,y=0;
    while(now){
        if(value < now->value){
            now = now->left;
            x = (x*2);
            y++;
        }else if(value > now->value){
            now = now->right;
            x = (x*2+1);
            y++;
        }else{
            return {x,y};
        }
    }
    return {0,0};
}

void Node::update(){
        this->size = 1;
        if(this->left)this->size += this->left->size;
        if(this->right)this->size += this->right->size;
}

void Node::update_all(){
        this->update();
        if(this->parent)this->parent->update_all();
    }

void Node::rotateL(){
    if(this->right)sub_rotate(this->right);
};

void Node::rotateR(){
    if(this->left)sub_rotate(this->left);
};

//要
void Node::sub_rotate(Node *me){
    Node *p, *pp, *c;
    p = this;
    pp = this->parent;

    //Edgeアクション
    Node *node2[3] = {pp,p,me};
    Node *leftedge[3];
    Node *rightedge[3];
    //Moveアクション
    pair<int, int> ppp = getTruePos(g_root, p->value);
    pair<int, int> mep = getTruePos(g_root, me->value);
    pair<int, int> cp;
    //me-c-p
    if(me == p->left){
        c = me->right;
        cp = c ? getTruePos(g_root, c->value) : pair<int,int>{0,0};
        me->right = p;
        p->left = c;
        leftedge[1]=c;
        rightedge[1]=p->right;
        leftedge[2]=me->left;
        rightedge[2]=p;
    }else{
        c = me->left;
        cp = c ? getTruePos(g_root, c->value) : pair<int,int>{0,0};
        me->left = p;
        p->right = c;
        leftedge[1]=p->left;
        rightedge[1]=c;
        leftedge[2]=p;
        rightedge[2]=me->right;
    }
    if(c)c->parent = p;
    p->parent = me;
    //me-p-pp
    me->parent = pp;
    if(pp && p == pp->left){
        pp->left = me;
        leftedge[0]=me;
        rightedge[0]=pp->right;
    }
    else if(pp && p == pp->right){
        pp->right = me;
        leftedge[0]=pp->left;
        rightedge[0]=me;
    }
    p->update();
    me->update();
    printf("come to rotate\n");
    printf("clear the first step");
    int px=ppp.first, py=ppp.second, mex=mep.first, mey=mep.second, cx=cp.first, cy=cp.second;
    printf("clear the second step");
    Node *node1[3] = {p,me,c};
    int tox[3] = {mex%2 ? mex-1 : mex+1 , px , cx%2 ? cx+1 : cx-1 };
    int toy[3] = {mey,py,cy};
    printf("go!rotate\n");
    //push
    queue->pushE(node2, 3, leftedge, rightedge);
    queue->pushM(node1, 3, tox, toy);
    printf("get through rotate\n");
}

Node *get(int index, Node *root){
    if(!root)return nullptr;
    if(index > root->size+1)return nullptr;
    Node *now = root;
    while(true){
        int lsize = (now->left?now->left->size:0);
        if(index < lsize){
            now = now->left;
        }else if(index > lsize){
            now = now->right;
            index -= (lsize + 1);
        }else{
            return now;
        }
    }
}

//要
Node *settle_red(Node *node){
    Node *p, *pp, *u;
    if(!node)return nullptr;
    //自身が根である
    if(!(p = node->parent)){
        printf("settlered1\n");
        //ColorAction
        {
            Node *nodes[1] = {node};
            char color[1] = {'B'};
            queue->pushC(nodes, 1, color);
        }
        node->color = 'B';
        return node;
    }
    //親が黒である場合(根である場合を含む)
    if(p->color == 'B')return p;
    pp = p->parent;
    if(p == pp->left)u = pp->right;
    if(p == pp->right)u = pp->left;
    //uが赤である場合
    if(u && u->color == 'R'){
        printf("settlered2(pp)\n");
        p->color = 'B';
        u->color = 'B';
        pp->color = 'R';
        //ColorAction
        {
            Node *nodes[3] = {p,u,pp};
            char color[3] = {'B','B','R'};
            queue->pushC(nodes, 3, color);
        }
        return settle_red(pp);
    }
    //me-p-ppが曲がっている
    if(node == p->left && p == pp->right){
        printf("settlered3\n");
        p->rotateR();
        pp->rotateL();
        node->color = 'B';
        pp->color = 'R';
        //ColorAction
        {
            Node *nodes[2] = {node,pp};
            char color[2] = {'B','R'};
            queue->pushC(nodes, 2, color);
        }
        return node;
    }
    if(node == p->right && p == pp->left){
        printf("settlered3\n");
        p->rotateL();
        pp->rotateR();
        node->color = 'B';
        pp->color = 'R';
        //ColorAction
        {
            Node *nodes[2] = {node,pp};
            char color[2] = {'B','R'};
            queue->pushC(nodes, 2, color);
        }
        return node;
    }
    //me-p-ppが直線(u黒)
    if(node == p->left && p == pp->left){
        printf("settlered4\n");
        pp->rotateR();
        pp->color = 'R';
        p->color = 'B';
        //ColorAction
        {
            Node *nodes[2] = {pp,p};
            char color[2] = {'R','B'};
            queue->pushC(nodes, 2, color);
        }
        return p;
    }
    if(node == p->right && p == pp->right){
        printf("settlered4\n");
        pp->rotateL();
        pp->color = 'R';
        p->color = 'B';
        //ColorAction
        {
            Node *nodes[2] = {pp,p};
            char color[2] = {'R','B'};
            queue->pushC(nodes, 2, color);
        }
        return p;
    }
    return nullptr;
}

Node *settle_blackL(Node *p){
    if(!p)return nullptr;
    Node *s, *c1, *c2;
    s = p->right;
    if(!s){
        return nullptr;
    }
    c1 = s->left;
    c2 = s->right;
    //sが赤である
    if(s->color == 'R'){
        p->rotateL();
        s->color = 'B';
        p->color = 'R';
        //ColorAction
        {
            Node *nodes[2] = {s,p};
            char color[2] = {'B','R'};
            queue->pushC(nodes, 2, color);
        }
        settle_blackL(p);
        return s;
    }
    //c1が赤である
    if(c1 && c1->color == 'R'){
        s->rotateR();
        p->rotateL();
        //ColorAction
        {
            Node *nodes[2] = {c1,p};
            char color[2] = {p->color,'B'};
            queue->pushC(nodes, 2, color);
        }
        c1->color = p->color;
        p->color = 'B';
        return c1;
    }
    //c2が赤である
    if(c2 && c2->color == 'R'){
        p->rotateL();
        //ColorAction
        {
            Node *nodes[3] = {s,p,c2};
            char color[3] = {p->color,'B','B'};
            queue->pushC(nodes, 3, color);
        }
        s->color = p->color;
        p->color = 'B';
        c2->color = 'B';
        return s;
    }
    //c1 c2 u が黒
    //pが赤
    s->color = 'R';
    if(p->color == 'R'){
        p->color = 'B';
        //ColorAction
        {
            Node *nodes[2] = {s,p};
            char color[2] = {'R','B'};
            queue->pushC(nodes, 2, color);
        }
        return p;
    }
    //pが黒
    if(p->color == 'B'){
        //ColorAction
        {
            Node *nodes[1] = {s};
            char color[1] = {'R'};
            queue->pushC(nodes, 1, color);
        }
        if(!(p->parent))return p;
        if(p == p->parent->left)return settle_blackL(p->parent);
        if(p == p->parent->right)return settle_blackR(p->parent);
    }
    return nullptr;
}

Node *settle_blackR(Node *p){
    if(!p)return nullptr;
    Node *s, *c1, *c2;
    s = p->left;
    if(!s)return nullptr;
    c1 = s->right;
    c2 = s->left;
    //uが赤である
    if(s->color == 'R'){
        p->rotateR();
        s->color = 'B';
        p->color = 'R';
        //ColorAction
        {
            Node *nodes[2] = {s,p};
            char color[2] = {'B','R'};
            queue->pushC(nodes, 2, color);
        }
        settle_blackR(p);
        return s;
    }
    //c1が赤である
    if(c1 && c1->color == 'R'){
        s->rotateL();
        p->rotateR();
        //ColorAction
        {
            Node *nodes[2] = {c1,p};
            char color[2] = {p->color,'B'};
            queue->pushC(nodes, 2, color);
        }
        c1->color = p->color;
        p->color = 'B';
        return c1;
    }
    //c2が赤である
    if(c2 && c2->color == 'R'){
        p->rotateR();
        //ColorAction
        {
            Node *nodes[3] = {s,p,c2};
            char color[3] = {p->color,'B','B'};
            queue->pushC(nodes, 3, color);
        }
        s->color = p->color;
        p->color = 'B';
        c2->color = 'B';
        return s;
    }
    //c1 c2 u が黒
    s->color = 'R';
    //pが赤
    if(p->color == 'R'){
        p->color = 'B';
        //ColorAction
        {
            Node *nodes[2] = {s,p};
            char color[2] = {'R','B'};
            queue->pushC(nodes, 2, color);
        }
        return p;
    }
    //pが黒
    if(p->color == 'B'){
        //ColorAction
        {
            Node *nodes[1] = {s};
            char color[1] = {'R'};
            queue->pushC(nodes, 1, color);
        }
        if(!(p->parent))return p;
        if(p == p->parent->left)return settle_blackL(p->parent);
        if(p == p->parent->right)return settle_blackR(p->parent);
    }
    return nullptr;
}

Node *sub_insert(Node *node, Node *root){
    Node *p = node->parent;
    //親が黒である
    if(p->color == 'B'){
        printf("sub_insert\n");
        return root;
    }
    //親が赤である
    else{
        printf("sub_settle_red MyParentIs%d\n",node->parent->dvalue);
        Node *ans = settle_red(node);
        //heightを更新
        if(!(ans->parent)){
            printf("NextRootIs %d\n",ans->dvalue);
            height = getDeepest(ans);
            queue->pushH(height);
            return ans;
        }
        else{
            printf("my parent is %d\n",ans->parent->dvalue);
            height = getDeepest(root);
            queue->pushH(height);
            return root;
        }
    }
}

//!!!!!
Node *insert(int value, Node *root, Node node[]){
    int ite = 0;
    //保存場所を探す
    while(!(node[ite].color == 'R' && node[ite].parent == nullptr)){
        ite++;
    }
    Node *newnode = &node[ite];
    //いちおう
    newnode->reNode();
    newnode->value = value;
    //rootが存在しない
    if(!root){
        //Exist
        Node *nodes[1] = {newnode};
        queue->pushN(nodes, value , 0, 0);
        //
        newnode->color = 'B';
        //ColorAction
        char color[1] = {'B'};
        queue->pushC(nodes, 1, color);

        height=1;
        queue->pushH(1);
        //
        return newnode;
    }
    //rootの下に挿入する//挿入位置で深さを更新
    Node *now = root;
    //
    int depth = 1;
    //
    while(true){
        depth++;
        if(value < now->value){
            if(!(now->left)){
                newnode->parent = now;
                now->left = newnode;
                now->update_all();

                //HEIGHT
                if(depth>height){
                    height = depth;
                    queue->pushH(height);
                }
                
                //EXIST 
                {
                    Node *nodes[1] = {newnode};
                    pair<int, int> newp = getTruePos(g_root, newnode->value);
                    queue->pushN(nodes, newnode->value , newp.first, newp.second);
                }
                
                //EDGE
                {
                    Node *nodes[1] = {now};
                    Node *leftedge[1] = {newnode};
                    Node *rightedge[1] = {now->right};
                    queue->pushE(nodes, 1, leftedge, rightedge);
                }
                return sub_insert(newnode, root);
            }else{
                now = now->left;
            }
        }else if(value > now->value){
            if(!(now->right)){
                newnode->parent = now;
                now->right = newnode;
                now->update_all();
                //HEIGHT
                if(depth>height){
                    height = depth;
                    queue->pushH(height);
                }
                //EXIST
                {
                    Node *nodes[1] = {newnode};
                    pair<int, int> newp = getTruePos(g_root, newnode->value);
                    queue->pushN(nodes, value , newp.first, newp.second);
                }
                //EDGE
                {
                    Node *nodes[1] = {now};
                    Node *leftedge[1] = {now->left};
                    Node *rightedge[1] = {newnode};
                    queue->pushE(nodes, 1, leftedge, rightedge);
                }
                return sub_insert(newnode, root);
            }else{
                now = now->right;
            }
        }//挿入値が他のノードと重複する
        else{
            return root;
        }
    }
}

Node *remove(int value, Node *root){
    Node *now = root;
    while(now){
        if(value < now->value){
            now = now->left;
        }else if(value > now->value){
            now = now->right;
        }else{
            return sub_remove(now, root);
        }
    }
    //削除値が存在しない
    return root;
}

//!!!!!
Node *sub_remove(Node *node, Node *root){
    Node *l, *r, *p;
    l = node->left;
    r = node->right;
    p = node->parent;
    //根を消す場合
    if(!p){
        //右子も左子も存在しない
        if(!l && !r){
            node->color = 'R';
            //ColorAction
            {
                Node *nodes[1] = {node};
                char color[1] = {'R'};
                queue->pushC(nodes, 1, color);
            }
            height = 0;
            queue->pushH(0);
            //EXIST
            {
                Node *nodes[1] = {nullptr};
                pair<int, int> np = getTruePos(g_root, node->value);
                queue->pushN(nodes, 0 , np.first, np.second);
            }
            return nullptr;
        }
        //右子が存在しない
        if(!r){
            node->left = nullptr;
            //EdgeAction
            {
                Node *nodes[1] = {node};
                Node *leftedge[1] = {nullptr};
                Node *rightedge[1] = {nullptr};
                queue->pushE(nodes, 1, leftedge, rightedge);
            }
            //ColorAction
            {
                Node *nodes[2] = {node,l};
                char color[2] = {'R','B'};
                queue->pushC(nodes, 2, color);
            }
            //EXIST
            {
                Node *nodes[1] = {nullptr};
                pair<int, int> np = getTruePos(g_root, node->value);
                queue->pushN(nodes, 0 ,np.first, np.second);
            }
            node->update();
            node->color = 'R';
            l->parent = nullptr;
            l->color = 'B';
            height = 1;
            queue->pushH(1);
            return l;
        }
        
    }
    //左側を消す場合
    else if(node == p->left){
        //右子も左子も存在しない
        if(!l && !r){
            //EXIST
            {
                Node *nodes[1] = {nullptr};
                pair<int,int> np = getTruePos(root, node->value); 
                queue->pushN(nodes, 0, np.first, np.second);
            }
            p->left = nullptr;
            p->update_all();
            //EdgeAction
            {
                Node *nodes[1] = {p};
                Node *leftedge[1] = {nullptr};
                Node *rightedge[1] = {p->right};
                queue->pushE(nodes, 1, leftedge, rightedge);
            }
            node->parent = nullptr;
            if(node->color == 'R'){
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
            Node *ans = settle_blackL(p);
            if(!(ans->parent)){
                height = getDeepest(ans);
                queue->pushH(height);
                return ans;
            }
            else{
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
        }
        //右子が存在しない
        if(!r){
            p->left = l;
            l->parent = p;
            p->update_all();
            //EdgeAction
            {
                Node *nodes[2] = {p,node};
                Node *leftedge[2] = {l,nullptr};
                Node *rightedge[2] = {p->right,node->right};
                queue->pushE(nodes, 2, leftedge, rightedge);
            }
               //EXIST
            {
                Node *nodes[1] = {nullptr};
                pair<int,int> np = getTruePos(root, node->value); 
                queue->pushN(nodes, 0, np.first, np.second);
            }
            //Move
            {
                Node *nodes[1] = {l};
                pair<int, int> lp = getTruePos(g_root, l->value);
                int tox[1] = {lp.first};
                int toy[1] = {lp.second};
                queue->pushM(nodes, 1, tox, toy);

            }
            node->parent = nullptr;
            node->left = nullptr;
            node->update();
            //削除ノードが赤
            if(node->color == 'R'){
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
            node->color = 'R';
            //削除ノードの左子が赤
            if(l->color == 'R'){
                l->color = 'B';
                 //ColorAction
                {
                    Node *nodes[2] = {node, l};
                    char color[2] = {'R','B'};
                    queue->pushC(nodes, 2, color);
                }
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
            //削除ノードも左子も黒
            Node *ans = settle_blackL(p);
             //ColorAction
            {
                Node *nodes[1] = {p};
                char color[1] = {'R'};
                queue->pushC(nodes, 1, color);
            }
            if(!(ans->parent)){
                height = getDeepest(ans);
                queue->pushH(height);
                return ans;
            }
            else{
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
        }
    }
    //右側を消す場合
    else if(node == p->right){
        //右子も左子も存在しない
        if(!l && !r){
             //EXIST
            {
                Node *nodes[1] = {nullptr};
                pair<int,int> np = getTruePos(root, node->value); 
                queue->pushN(nodes, 0, np.first, np.second);
            }
            p->right = nullptr;
            p->update_all();
            node->parent = nullptr;
            //EdgeAction
            {
                Node *nodes[1] = {p};
                Node *leftedge[1] = {nullptr};
                Node *rightedge[1] = {nullptr};
                queue->pushE(nodes, 1, leftedge, rightedge);
            }
            if(node->color == 'R'){
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
            Node *ans = settle_blackR(p);
            if(!(ans->parent)){
                height = getDeepest(ans);
                queue->pushH(height);
                return ans;
            }
            else{
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
        }
        //右子が存在しない
        if(!r){
            p->right = l;
            l->parent = p;
            p->update_all();
            node->parent = nullptr;
            node->left = nullptr;
            node->update();
            //EdgeAction
            {
                Node *nodes[2] = {p,node};
                Node *leftedge[2] = {p->left,nullptr};
                Node *rightedge[2] = {l, node->right};
                queue->pushE(nodes, 2, leftedge, rightedge);
            }
            //EXIST
            {
                Node *nodes[1] = {nullptr};
                pair<int,int> np = getTruePos(root, node->value); 
                queue->pushN(nodes, 0, np.first, np.second);
            }
            //Move
            {
                pair<int, int> pp = getTruePos(g_root, p->value);
                Node *nodes[1] = {l};
                int tox[1] = {pp.first};
                int toy[1] = {pp.second};
                queue->pushM(nodes, 1, tox, toy);
            }
            //削除ノードが赤
            if(node->color == 'R'){
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
            node->color = 'R';
            //削除ノードの左子が赤
            if(l->color == 'R'){
                l->color = 'B';
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
            //削除ノードも左子も黒
            Node *ans = settle_blackR(p);
            if(!(ans->parent)){
                height = getDeepest(ans);
                queue->pushH(height);
                return ans;
            }
            else{
                height = getDeepest(root);
                queue->pushH(height);
                return root;
            }
        }
    }
    //次のノードを代わりに消す
    Node *next, *nr ,*np;

    next = get(0,r);
    nr = next->right;
    np = next->parent;

    //描画EXISTように　身代わりノードの実座標x,yを取得
    pair<int,int> nep = getTruePos(g_root, next->value);
    int nx = nep.first;
    int ny = nep.second;
    //

    int tmp = node->value;
    node->value = next->value;
    next->value = tmp;

    //VALUE
    {
    Node *nodes[1] = {node};
    queue->pushV(nodes,node->value);
    }

    next->right = nullptr;
    next->update();
    next->parent = nullptr;

    //EdgeAction
    Node *nodes[2] = {np, next};
    Node *leftedge[2];leftedge[1]=nullptr;
    Node *rightedge[2];rightedge[1]=nullptr;

    if(nr)nr->parent = np;
    //
    if(np == node){
        leftedge[0]=np->left;
        rightedge[0]=nr;
    }
    else{
        leftedge[0]=nr;
        rightedge[0]=np->right;
    }
    queue->pushE(nodes, 2, leftedge, rightedge);

    //EXIST
    {
        //nextを消す
        //pair<int, int> nnp = getTruePos(g_root, node->value);
        Node *nodes1[1] = {nullptr};
        //int nw,nh;
        //nw = (np==node ? nnp.first*2+1 : nnp->first*2);
        queue->pushN(nodes1, 0, nx, ny);
    
    //MOVE
        Node *nodes2[1] = {nr};
        int tox[1] = {nep.first};
        int toy[1] = {nep.second};
        queue->pushM(nodes2 , 1 , tox, toy);
    }

    if(np == node){
        np->right = nr;
    }
    else{
        np->left = nr;
    }
    
    np->update_all();
    //nextが赤
    if(next->color == 'R'){
        height = getDeepest(root);
        queue->pushH(height);
        return root;
    }
    next->color = 'R';
    
    //nrが赤
    if(nr && nr->color == 'R'){
        nr->color = 'B';
         //ColorAction
        {
            Node *nodes[2] = {next,nr};
            char color[2] = {'R','B'};
            queue->pushC(nodes, 2, color);
        }
        height = getDeepest(root);
        queue->pushH(height);
        return root;
    }

    Node *ans;
    //
    if(np == node)ans = settle_blackR(np);
    else ans = settle_blackL(np);
    //
     //ColorAction
    {
        Node *nodes[1] = {next};
        char color[1] = {'R'};
        queue->pushC(nodes, 1, color);
    }
    if(!(ans->parent)){
        height = getDeepest(ans);
        queue->pushH(height);
        return ans;
    }
    else{
        height = getDeepest(root);
        queue->pushH(height);
        return root;
    }
}
