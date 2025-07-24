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
    if(key & PAD_INPUT_LEFT){
        hx-=mv;
        g_stagedate.hero.turn = TRUE;
    }
    if(key & PAD_INPUT_RIGHT){
        hx +=mv;
        g_stagedate.hero.turn = FALSE;
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