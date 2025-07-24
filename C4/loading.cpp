#include "loading.h"
Imagehandles g_imghandles;
BOOL LoadGameImage(){
    if(LoadDivGraph("フォルダ名¥¥ファイル名",2,2,1,50,50,g_imghandles.hero)==-1)return FALSE;
    if(LoadDivGraph("フォルダ名¥¥ファイル名",2,2,1,50,50,g_imghandles.monester[0])==-1)return FALSE;
    if(LoadDivGraph("フォルダ名¥¥ファイル名",2,2,1,50,50,g_imghandles.monster[1]==-1))return FALSE;
    if((g_imghandles.block = LoadGraph("フォルダ名¥¥ファイル名"))==-1)return FALSE;
    if((g_imghandles.bullet = LoadGraph("フォルダ名¥¥ファイル名"))==-1)return FALSE;
    if((g_imghandles.knife = LoadGraph("フォルダ名¥¥ファイル名"))==-1)return FALSE;
    if((g_imghandles.title = LoadGraph("フォルダ名¥¥ファイル名"))==-1)return FALSE;
    return TRUE;
}
