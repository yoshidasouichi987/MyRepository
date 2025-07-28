#include "enemy.h"
 
void SetEnemy(int mx,int my){
    char c = g_mapdate[my][mx];
    //空きを探す
    int i;
    for(i = 0;i<MAX_ENEMY;i++){
        if(g_stagedate.enemies[i].living == FALSE)break;
    }
    if(i<MAX_ENEMY){
        g_stagedate.enemies[i].living = TRUE;
        g_stagedate.enemies[i].x = (float)mx * IMG_CHIPSIZE;
        g_stagedate.enemies[i].y = (float)my * IMG_CHIPSIZE;
        g_stagedate.enemies[i].turn = FALSE;
        g_stagedate.enemies[i].noground = FALSE;
        g_stagedate.enemies[i].jumping = FALSE;
        g_stagedate.enemies[i].jumppower = 0;
        g_stagedate.enemies[i].jumpforward = 0;
        g_stagedate.enemies[i].type = (EnemyType)(c-'0');
        //生命力
        switch (g_stagedate.ememies[i].type){
            case ET_CRAB:g_stagedate.enemies.life = 4;break;
            case ET_OCT:g_stagedate.enemies.life = 10;break;
        }
        g_mapdate[my][mx] = '0';
    }
}
void DrawEnemy(int ac){
    for(int i = 0;i<MAX_ENEMY;i++){
        if(g_stagedate.enemies[i].living = TRUE){
            EnemyType type = g_stagedate.enemies[i].type;
            switch (type){
                case ET_CRAB: g_stagedate.enemies[i] = MoveCrabMonster(g_stagedate.enemies[i]);break;
                case RT_OCT:break;
            }
            //画面外にいるなら消す
            if((g_stagedate.ememies[i] < g_stagedate.scrollx - IMG_CHIPSIZE)
            ||g_stagedate.enemies[i] < g_stagedate.scrollx+1600){
                g_stagedate.enemies[i].living = FALSE;
            }

            DrawRotaGraph2(
                (int)g_stagedate.enemies[i].x,
                (int)g_stagedate.enemies[i].y,
                0,0,1,0,g_imghandles.monster[(int)type -2][ac %ANIMERATE]
            );
        }
    }
}
void MoveCrabMonster(Charadate cd){
    float mv = 100.0f * g_frametime;//移動量計算
    float hx = cd.x;
    float hy = cd.y;
    //ジャンプ処理
    if(cd.jumping == TRUE){
        mv = 0;
        cd.jumppower -= GRAVITY;
        hy -= (cd.jumppower * g_frametime);
        hx += (cd.jumpforward * g_frametime);
    }
    //移動
    if(cd.turn == FALSE)hx-=mv;
    else hx+=mv;
    //マップ当たり判定
    AtariInfo atari = CheckBlock(hy,hx,cd.x);
    if(cd.jumping == FALSE){//ジャンプ中は横向きの当たり判定がない
        if(cd.turn == TRUE){
            if(atari.DR == TRUE||atari.UR == TRUE)hx = cd.x;cd.turn = !cd.turn;
        }else{
            if(atari.DL == TRUE||atari.UL == TRUE)hx = cd.x;cd.turn = !cd.turn;
        }
    }
    //接地チェック
    if(atari.GL == TRUE||atari.GR == TRUE){
        cd.noground = FALSE;
        cd.jumping = FALSE;
        cd.jumppower = 0;
        cd.jumpforward = 0;
        //ジャンプ後に地面に埋まらないよう補正
        hy = (float)((int)(hy/IMG_CHIPSIZE)*IMG_CHIPSIZE);
    }else{
        cd.noground = TRUE;
        cd.jumping = TRUE;
    }
    cd.x = hx;
    cd.y = hy;
    return cd;
}
