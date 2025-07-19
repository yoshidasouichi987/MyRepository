#ifndef __LOADING_H__
#define __LOADING_H__
#include <DxLib.h>
struct ImageHandles{
    int field;
    int mapitems[10];
}
extern ImageHandles g_imghandles;
BOOL LoadGameImage();
#endif