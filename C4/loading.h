#ifndef __LOADING_H__
#define __LOADING_H__
#include <DxLib.h>
#define ANIMFRATE 2
struct ImageHandles{
    int hero[ANIMEFRAME];
    int monster[2][ANIMEFRAME];
    int block,knife,bullet;
    int title;
};
extern ImageHandles g_imghandles;
struct SoundHandles{
    int bgm;
    int gameover;
    int attack;
    int bullet;
}
extern SoundHandles g_sndhandles;
BOOL LoadGameImage();
#endif