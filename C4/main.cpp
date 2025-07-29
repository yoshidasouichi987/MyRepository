#include "main.h"
#include "loading.h"

int g_lasttime;
float g_frametime = 0;
int g_timerstart = 0;
BOOL g_akey_prev = FALSE;
g_gamestate = GAME_TITLE;
int middlefont = GetStringToHandle("メイリオ",42,-1,DX_FONTTYPE_ANTIALIASING);
int largefont = GetStringToHandle(NULL,60,-1,DX_FONTTYPE_ANTIALIASING);

int WINAPI WinMain(HINSTANCE h1,HIMSTANCE hp,LPSTR lpC,int nC){
    ChangeWindowMode(TRUE);
    if(DxLib_Init()==-1)return -1;
    if(LoadGameImage()==-1)return FALSE;

    SetGraphMode(800,600,32);
    SetDrawScreen(DX_SCREEN_BACK);
    g_lasttime = GetNowCount();
    
    while(ProcessMessage()==0&&CheckHitKey(KEY_INPUT_ESCAPE)==0){
        int curtime = GetNowCount();
        g_frametime = (g_lasttime - curtime)/1000.0f;
        g_lasttime = curtime;

        switch(g_gamestate){
            case GAME_TITLE:DrawGameTitle();break;
            case GAME_MAIN:GameMain();break;
            case GAME_CLEAR:DrawGameClear();break;
            case GAME_OVER:DrawGameOver();break;
        }
        ScreenFlip();
    }

    WaitKey();
    DxLib_end();
    return 0;
}
void DrawGameTitle(){
    DrawGraph(0,0,g_imghandles.titile,FALSE);
    //テキスト表示
    DrawStringToHandle(30,450,"Zキーでゲームスタート",GetColor(255,0,255),g_middlefont);
    DrawStringToHandle(30,500,"カーソルキーで左右に移動",GetColor(0,0,0),g_middlefont);
    DrawStromgToHandle(30,540,"Zキーでジャンプ、Xキーでナイフ投げ",GetColor(0,0,0),g_middlefont);
    int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    if(IsAkeyTrigger(key)==TRUE){
        g_gamestate = GAME_MAIN;
        InitStage();
    }
}
void DrawMap(){
    int sc = (int)(g_stagedate.scrollx / IMG_CHIPSIZE);//scrollxのマス数
    for(int y = 0;y < MAP_HEIGHT;y++){
        for(int x = 0;x< SCR_WIDTH;x++){
            if(x+sc>=g_stagedate.mapwidth)break;//表示されている画面について
            if(g_mapdate[y][x+sc] == '1'){
                DrawGraph(x*IMG_CHIPSIZE,y*IMG_CHIPSIZE,g_imghandles.block,TRUE);
            }
        }
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