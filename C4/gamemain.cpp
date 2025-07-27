#include "gamemain.h"
char g_mapdate[MAP_HEIGHT][MAP_WIDTH+1];
StageDate g_stagedate;
void InitStage(){
    char buf[256];
    sprintf_s(buf,256,"ファイル名¥¥フォルダ名%d",g_stagedate.stagenum+1);
    int fh = FileRead_open(buf);
    for(int y = 0;y<MAP_HEIGHT;y++){
        FileRead_gets(g_mapdate[y],256,fh);
    }
    FileRead_close(fh);
    g_stagedate.mapwidth = (int)strlen(g_mapdate[0]);
    g_stagedate.hero.x = 2*IMG_CHIPSIZE;
    g_stagedate.hero.y = 10*IMG_CHIPSIZE;
    g_stagedate.hero.turn = FALSE;
}
void GameMain(){
    g_stagedate.animcounter = (g_stagedate.animcounter+1)&MAXINT;
    int ac = g_stagedate.animecounter/ANIM_RATE;
    DrawMap();
    DrawHero();
}
void DrawMap(){
    int sc = (int)(g_stagedate.scrollx/IMG_CHIPSIZE);//画面左端のマス数
    int shiftx = (int)g_stagedate.scrollx % IMG_CHIPSIZE;//左端の写したくない部分
    for(int y = 0;y<MAP_HEIGHT;y++){
        for(int x = ;x<SCR_WIDTH+1;x++){
            if(x + sc >= g_stagedate.mapwidth)break;//右端には描けない
            if(g_mapdate[y][x+sc]=='1'){
                DrawGraph(x*IMG_CHIPSIZE - shiftx,y*IMG_CHIPSIZE,g_imghandles.block,TRUE);
            }
        }
    }
}
void DrawHero(int ac){
    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    float mv = g_frametime*80.0f;
    float hx = g_stagedate.hero.x;
    float hy = g_stagedate.hero.y;
    if(g_stagedate.hero.jumping==TRUE){
        mv=0;//ジャンプ中はキーによる移動はさせない
        g_stagedate.hero.jumppower -= GRAVITY;
        hy -= (g_stagedate.hero.jumppower *g_frametime);
        hx += (g_stagedate.hero.jumpforward *g_frametime);//ジャンプ中はjumpforward分移動する 
    }
    //移動
    if(key & PAD_INPUT_LEFT){
        hx-=mv;
        g_stagedate.hero.turn = TRUE;
    }
    if(key & PAD_INPUT_RIGHT){
        hx +=mv;
        g_stagedate.hero.turn = FALSE;
    }
    //マップ当たり判定
    AtariInfo atari = CheckBlock(hx,hy,g_stagedate.hero.x);
    if(g_stagedate.hero.turn == FALSE){
        if(atari.DR==TRUE||atari.UR==TRUE)hx = g_stagedate.hero.x;
    }else{
        if(atari.DL == TRUE||ataro.DR == TRUE)hx = g_stagedate.hero.x;
    }
    //接地チェック
    if(atari.GL == TRUE || atari.GR == TRUE){
        g_stagedate.hero.noground = FALSE;
        g_stagedate.hero.jumping = FALSE;
        g_stagedate.hero.jumppower = 0;
        g_stagedate.hero.jumpforward = 0;
        //ジャンプ後に地面に埋まらないよう補正
        hy = (float)((int)(hy/IMG_CHIPSIZE)*IMG_CHIPSIZE);
    }else{
        g_stagedate.hero.noground = TRUE;
        g_stagedate.hero.jumping = TRUE;
    }
    //天井チェック
    if(g_stagedate.hero.jumping == TRUE){
        if(atari.UL == TRUE || atari.UR == TRUE){
            g_stagedate.hero.jumppower = 0;
            g_stagedate.hero.jumpforward = 0;
        }
    }
    //ジャンプ開始処理
    if(g_stagedate.hero.jumping == FALSE){
        if(isAkeyTrigger(key) == TRUE && g_stagedate.hero.noground == FALSE){
            g_stagedate.hero.jumping = TRUE;
            g_stagedate.hero.jumppower = JUMP_POWER;
            if(key&PAD_INPUT_LEFT)g_stagedate.hero.jumpforward = -JUMP_FORWARD;
            if(key&PAD_INPUT_RIGHT)g_stagedate.hero.jumpforward = JUMP_FORWARD;
        }
    }
    //スクロール補正
    if(hx - g_stagedate.scrollx > SCROLL_STAPOS){
        g_stagedate.scrollx += (hx - g_stagedate.hero.x);//変位を足す
    }
    if(hx < g_stagedate.scrollx)hx = g_stagedate.hero.x;//画面端より左には行けない
    g_stagedate.hero.x = hx;
    g_stagedate.hero.y = hy;

    DrawRotaGraph2((int)(g_stagedate.hero.x-g_stagedate.scrollx),(int)g_stagedate.hero.y,0,0,1,0,g_imghandles.hero[ac%ANIMFRATE],TRUE,g_stagedate.hero.turn);
}
BOOL _CheckBlockSub(float x,float y){
    int mx = (int)(x/IMG_CHIPSIZE);
    int my = (int)(y/IMG_CHIPSIZE);
    //マップの範囲外ならTRUE
    if((mx < 0) || (mx >= g_stagedate.mapwidth) || (my >= g_stagedate.MAP_HEIGHT)){
        return TRUE;
    }
    if(my < 0)return FALSE;
    if(g_mapdate[mx][my] != '0')return TRUE;
    return FALSE;
}
AtariInfo CheckBlock(float x,float y,float rx){
    AtaroInfo result;
    result.UL = _CheckBlockSub(x,y);
    result.UR = _CheckBlockSub(x+IMG_CHIPSIZE - 1,y);
    result.DL = _CheckBlockSub(x,y + IMG_CHIPSIZE-1);
    result.DR = _CheckBlockSub(x+IMG_CHIPSIZE-1,y+IMG_CHIPSIZE-1);
    result.GL = _CheckBlockSub(rx,y+IMG_CHIPSIZE);
    result.GR = _CheckBlockSub(rx+IMG_CHIPSIZE-1,y+IMG_CHIPSIZE);
    return result;
}