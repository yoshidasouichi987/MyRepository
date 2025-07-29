#include "main.h"
int g_lasttime = 0;
float g_frametime = 0;
int g_timerstart = 0;
GameState g_gamestate = GAME_TITLE;
int g_gametitileimg = LoadGraph("フォルダ名¥¥ファイル名");
int g_heroimg = LoadGraph("フォルダ名¥¥ファイル名");
BOOL g_akey_prev = FALSE;
int g_middlefont = CreateFontToHandle("メイリオ",42,-1,DX_FONTTYPE_ANTIALIASING);
int g_largefont = CreateFontToHandle("NULL",60,-1,DX_FONTTYPE_ANTIALIASING);

int WINAPI WinMain(HINSTANCE h1,HIMSTANCE hp,LPSTR lpC,int nC){
    //ウィンドウモードにする
    ChangeWindowMode(TRUE);
    //ウィンドウサイズを変更する
    SetGraphMode(800,600,32);
    //Dxライブラリ初期化
    if(DxLib_Init() == -1)return -1;
    //画像読み込み
    if(LoadGameImage() == FALSE) return -1;
    //初期化
    SetDrawScreen(DX_SCREEN_BACK);
    g_lasttime = GetNowCount();
    
    while(ProcessMessage()==0&&CheckHitKey(KEY_INPUT_ESCAPE)==0){
        //時間測定
        int curtime = GetNowCount();
        g_frametime = (float)(g_lasttime - curtime)/1000.0f;
        g_lasttime = curtime;
        switch(g_gamestate){
        case GAME_TITLE:DrawGameTitle();break;
        case GAME_MAIN:GameMain();break;
        case GAME_OVER:DrawGameOver();break;
        case GAME_CLEAR:DrawGameClear();break;
        }
        ScreenFlip();
    }
    WaitKey();
    //終了
    DxLib_end();
    return 0;
}
BOOL isAKayTrigger(int key){
    if(key&&PAD_INPUT_A){
        if(g_akey_prev==FALSE){
            g_akey_prev = TRUE;
            return TRUE;
        }
    }else{
            g_akey_prev = FALSE;
        }
    return FALSE;
}