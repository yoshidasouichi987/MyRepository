#include "gamemain.h"
int g_mapdate[MAP_HEIGHT][MAP_WIDTH];
StageDate g_stagedate;
//ステージ初期化
void InitStage(){
    //マップデータの読み取り
    ZeroMemory(g_stagedate.enemies,sizeof(g_stagedate.enemies));
    char buf[256];
    sprintf_s(buf,256,"フォルダ名¥¥ファイル名",g_stagedate.stagemnum);
    int fh = FileRead_open(buf);//ファイルハンドル
    for(int y = 0;y < MAP_HEIGHT;y++){
        FileRead_gets(buf,256,fh);//一行読む
        for(int x = 0;x < MAP_WIDTH;x++){
            g_mapdate[y][x] = (int)(buf[x] - '0');//型変換
        }
    }
    FileRead_close(fh);
    int enemy = 0;
    //各マスについて
    for(int y = 0;y<MAP_HEIGHT;y++){
        for(int x = 0;x<MAP_WIDTH;x++){
            int c = g_mapdate[y][x];
            switch (c){
                case MAPITEM_HEROPOS:
                    g_stagedate.herox = x;
                    g_stagedate.heroy = y;
                    g_mapdate[y][x] = 0;
                    break;
                case MAPITEM_HMONSTER:
                case MAPITEM_YMONSTER:
                case MAPITEM_DRAGON:
                    g_stagedate.enemies[enemy].x = x;
                    g_stagedate.enemies[enemy].y = y;
                    g_stagedate.enemies[enemy].type = (MonsterType)c;
                    g_stagedate.enemies[enemy].living = TRUE;
                    g_stagedate.enemies[enemy].direction = -1;
                    g_mapdate[x][y] = 0;
                    enemy++;
                    break;
                default: break;
            }
        }
    }
    //炎を格納
    for(int i = 0;i<MAXENEMY;i++){
        if(g_stagedate.enemies[i].type == MT_DRAGON){
            g_stagedate.enemies[i].fireidx = enemy;
            g_stagedate.enemies[i].firetimer = 0;
            enemy+=FIREEXPAND;
        }
    }
}
void Gamemain(){
    //回数数え
    g_stagedate.movecounter++;
    g_stagedate.movecounter%=MOVERATE;
    DrawMap();
    DrawEnemy();
    //自キャラ移動
    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    //float mv = 80.0*g_frametime;
    if(g_stagedate.movecounter == 0){
        int hx = g_stagedate.herox;
        int hy = g_stagedate.heroy;
        if(key&PAD_INPUT_UP)hy-=1;
        if(key&PAD_INPUT_DOWN)hy+=1;
        if(key&PAD_INPUT_LEFT)hx+=1;
        if(key&PAD_INPUT_RIGHT)hx-=1;
    }
    //障害物
    if(g_mapdate[hx][hy]==MAPITEM_NO){
        g_stagedate.herox = hx;
        g_stagedate.heroy = hy;
    }
    //鍵
    if(g_mapdate[hx][hy]==MAPITEM_KEY){
        g_mapdate[hx][hy]=MAPITEM_NO;
        g_stagedate.getkey = TRUE;
        g_stagedate.herox = hx;
        g_stagedate.heroy = hy;
    }
    //ゴール
    if(g_mapdate[hx][hy]==MAPITEM_GOAL){
        if(g_stagedate.getkey==TRUE){
            g_gamestate = GAME_CLEAR;
            g_timerstart = g_lasttime;
        }
    }
    DrawGraph(g_stagedate.heroy*IMG_CHIPSIZE,g_stagedate.herox*IMG_CHIPSIZE,g_imghandles.mapitems[MPITEM_HEROPOS],TRUE);
}
void DrawMap(){
    for(int y = 0;y<MAP_HEIGHT;y++){
        for(int x = 0;x<MAP_WIDTH;x++){
            DrawGraph(x*IMG_CHIPSIZE,y*IMG_CHIPSIZE,g_imghandles.field,FALSE);
            int c = g_mapate[x][y];
            if(c>0){
                DrawGraph(x*IMG_CHIPSIZE,y*IMG_CHIPSIZE,g_imghandles.mapitems[c],TRUE);
            }
        }
    }
}
void DrawGameTitle(){
    DrawBox(0,0,800,600,GetColor(255,255,255),TRUE);
    DrawGraph(0,0,g_gametitleimg,TRUE);
    //テキスト表示
    DrawStringToHandle(100,400,"zキーでスタート",GetColor(0,0,0),g_middlefont);
    DrawStringToHandle(100,460,"カーソルキーで移動",GetColor(0,0,0),g_middlefont);
    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    if(IsAKeyTrigger(key)==True){
        //ゲーム開始
        g_gamestate = GAME_MAIN;
        //ステージ初期化
        InitStage();
        PlaySoundMem(g_sndhandles.bgm,DX_PLAYTYPE_LOOP);
    }
}
void DrawGameOver(){
    StopSoundMem(g_sndhandles.bgm);
    DrawBox(0,0,800,600,GetColor(0,0,0),TRUE);
    DrawStringToHandle(100,200,"ゲームオーバー",GetColor(255,0,0),g_largefont);
    if(g_lasttime - g_timerstart > 5000)g_gamestate = GAME_TITLE;
}
void DrawGameClear(){
    DrawBox(0,0,800,600,GetColor(255,255,255),TRUE);
    DrawStringToHandle(100,200,"ゲームクリア",GetColor(0,0,255),g_largefont);
    if(g_lasttime - g_timerstart > 5000){
        g_stagedate.stagenum++;
        g_stagedate.stagenum %= MAXSTAGE;
        if(g_stagedate.stagenum==0){
            g_gamestate = GAME_TITLE;
        }else{
            g_gamestate = GAME_MAIN;
            InitStage();
        }
    }

}
void DrawEnemy(){
    //すべてのモンスターについて
    for(int i = 0;i < MAXENEMY;i++){
        //死んでいたら放置
        if(g_stagedate.enemies[i].living == FALSE)continue;
        if(g_stagedate.movecounter == 0){
            int ex = g_stagedate.enemies[i].x;
            int ey = g_stagedate.enemies[i].y;
            //移動処理
            if(c==MT_HOLI)ex += g_stagedate.enemies[i].direction;
            if(c==MT_VERT)ey += g_stagedate.enemies[i].direction;
            if(c==MT_DRAGON)DrawDragonFire(i);
            if(g_mapdate[ex][ey]==MPITEM_NO){
                g_stagedate.enemies[i].x = ex;
                g_stagedate.enemies[i].y = ey;
            }else{
                g_stagedate.enemies[i].direction*=-1;
            }
            //当たっていたらgameover
            if(g_stagedate.enemies[i].x == g_stagedate.herox&&g_stagedate.enemies[i].y==g_stagedate.heroy){
                PlaySoundMem(g_sndhandles.gameover,DX_PLAYTPE_BACK);
                g_gamestate = GAME_OVER;
                g_timestart = g_lasttime;
            }
        }
        DrawGraph(g_stagedate.enemies[i].x*IMG_CHIPSIZE,g_stagedate.enemies[i].y*IMG_CHIPSIZE,g_imghandles.mapitems[(int)c],TRUE);
    }
}
void DrawDragonFire(int idx){ 
    int t = g_stagedate.enemies[idx].firetimer;
    int f = g_stagedate.enemies[idx].fireidx;
    if(t<FIREEXPAND){
        //炎を伸ばす
        for(int i = 0;i<FIREEXPAND;i++){//各炎につき
            if(i<=t){
                g_stagedate.enemies[f+i].living = TRUE;
                g_stagedate.enemies[f+i].type = MT_FIRE;
                g_stagedate.enemies[f+i].x = g_stagedate.enemies[idx].x - 1 - i;
                g_stagedate.enemies[f+i].y = g_stagedate.enemies[idx].y;
                PlaySoundMem(g_sndhandles.fire,DX_PLAYTYPE_BACK);
            }else{
               g_stagedate.enemies[f+i].living = FALSE;
            }
        }
    }else if(t<FIRESHRINK){
        //炎を縮める
        for(int i = 0;i<FIREEXPAND;i++){//各炎につき
            if(i<=(FIRESHRINK -1 - t)){
                g_stagedate.enemies[f+i].living = TRUE;
                g_stagedate.enemies[f+i].type = MT_FIRE;k
                g_stagedate.enemies[f+i].x = g_stagedate.enemies[idx].x - 1 - i;
                g_stagedate.enemies[f+i].y = g_stagedate.enemies[idx].y;
                PlaySoundMem(g_sndhandles.fire,DX_PLAYTYPE_BACK);
            }else{
                g_stagedate.enemies[f+i].living = FALSE;
            }
        }
    }else{
        //待機
        for(int i = 0;i<FIREEXPAND;i++){
            g_stagedate.enemies[f+i].living = FALSE;
        }
    }
    g_stagedate.enemies[idx].firetimer++;
    g_stagedate.enemies[idx].firetimer%=FIRESTOP;
}