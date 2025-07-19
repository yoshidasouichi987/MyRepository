#include "loading.h"
#ImageHandles g_imagehandles;
BOOL LoadGameImage(){
    g_imghandles.field = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.field == -1)return FALSE;
    //mapitems
    g_imghandles.mapitems[1] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[1] ==-1)return FALSE;
    g_imghandles.mapitems[2] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[2] ==-1)return FALSE;
    g_imghandles.mapitems[3] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[3] ==-1)return FALSE;
    g_imghandles.mapitems[4] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[4] ==-1)return FALSE;
    g_imghandles.mapitems[5] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[5] ==-1)return FALSE;
    g_imghandles.mapitems[6] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[6] ==-1)return FALSE;
    g_imghandles.mapitems[7] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[7] ==-1)return FALSE;
    g_imghandles.mapitems[8] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[8] ==-1)return FALSE;
     g_imghandles.mapitems[9] = LoadGraph("フォルダ名¥¥ファイル名");
    if(g_imghandle.mapitems[9] ==-1)return FALSE;
    
    return TRUE;
}

