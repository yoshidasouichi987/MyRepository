#ifndef __LOADING_H__
#define __LOADING_H__
#include <DxLib.h>
struct ImageHandles{
    int field;//草原
    int mapitems[10];//アイテム
};
extern ImageHandles g_imghandles;
struct SoundHandles{
    int fire;
    int bgm;
    int gameover;
};
extern SoundHandles g_sndhandles;
BOOL LoadGameImage();
#endif