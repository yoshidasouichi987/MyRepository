#include "main.h"
#include "loading.h"
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