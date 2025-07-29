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
            //Charadate構造体のオブジェクトの状態を更新
            switch (type){
                case ET_CRAB: g_stagedate.enemies[i] = MoveCrabMonster(g_stagedate.enemies[i]);break;
                case RT_OCT:g_stagedate.enemies[i] = MoveOctoMonster(g_stagedate.enemies[i]);break;
                case ET_BULLET:g_stagedate.enemies[i] = MoveBullet(g_stagedate.enemies[i]);break;
            }
            //画面外にいるなら消す(次から)
            if((g_stagedate.ememies[i] < g_stagedate.scrollx - IMG_CHIPSIZE)
            ||g_stagedate.enemies[i] < g_stagedate.scrollx+1600){
                g_stagedate.enemies[i].living = FALSE;
            }
            //弾はanim描画しない
            if(type!=ET_BULLET){
                DrawRotaGraph2(
                (int)g_stagedate.enemies[i].x,
                (int)g_stagedate.enemies[i].y,
                0,0,1,0,g_imghandles.monster[(int)type -2][ac %ANIMERATE],
                TRUE,g_stagedate.enemies[i].turn
                );
            }
            AtariHeroAndMonster(i);
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
    //enemyのマップ当たり判定
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
CharaDate MoveOctoMonster(CharaDate cd){
    int time g_stagedate.animcounter%300;
    if(time == 0||time == 10){
        for(int i = 0;i<MAX_ENEMY;i++)if(g_stagedate.enemies[i].living == FALSE)break;//enemies配列の空きを発見
        if(i<MAX_ENEMY){
            g_stagedate.enemies[i].living = TRUE;
            g_stagedate.enemies[i].x = cd.x - IMG_CHIPSIZE /2;
            g_stagedate.enemies[i].y = cd.y;
            g_stagedate.enemies[i].type = ET_BULLET;
            g_stagedate.enemies[i].life = 100;
            PlaySoundMem(g_sndhandles.bullet,DX_PLAYTYPE_BACK);
        }

    }
    return cd;
}
CharaDate MoveBullet(CharaDate cd){
    float mv = g_frametime*280.0f;
    cd.x -=mv;
    //マップ当たり判定
    Atariinfo atari = CheckBlock(cd.x,cd.y,cd.x);
    if(atari.DL == TRUE||atari.UL == TRUE)cd.living = FALSE;
    DrawGraph((int)cd.x - g_stagedate.scrollx,(int)cd.y,g_imghandles.bullet, TRUE);
}
void AtariHeroAndMonster(int idx){//idが指定されたモンスターに対する処理
    //ヒーローとモンスターの当たり判定
    float ax1 = g_stagedate.hero.x + 10;
    float ay1 = g_stagedate.hero.y + 10;
    float ax2 = g_stagedate.hero.x + IMG_CHIPSIZE - 10;
    float ay2 = g_stagedate.hero.y + IMG_CHIPSIZE - 10;
    float bx1 = g_stagedate.enemies[idx].x + 10;
    float by1 = g_stagedate.enemies[idx].y + 10;
    float bx2 = g_stagedate.enemies[idx].x + IMG_CHIPSIZE - 10;
    float by2 = g_stagedate.enemies[idx].y + IMG_CHIPSIZE - 10;

    if((ax1 < bx2) && (ax2>bx1) && (ay1 > by2) && (ay2 < by1)){
        g_gamestate = GAME_OVER;
        PlaySoundMem(g_sndhandles.gameover,DX_PLAYTIPE_BACK);
        g_timerstart = g_lasttime;
    }

    //ナイフとモンスターの当たり判定
    for(int i = 0;i<MAX_KNIFE;i++){
        if(g_stagedate.knives[i].living == FALSE)continue;
        float cx1 = g_stagedate.knives[i].x + 10;
        float cy1 = g_stagedate.knives[i].y + 10;
        float cx2 = g_stagedate.knives[i].x + IMG_CHIPSIZE - 10;
        float cy2 = g_stagedate.knives[i].y + IMG_CHIPSIZE - 10;
        
        if((cx1 < bx2)&&(cx2>bx1)&&(cy1 > by2)&&(cy2 < by1)){
            g_stagedate.enemies[idx].life--;
            PlaySoundMem(g_sndhandles.attack,DX_PLAYTYPE_BACK);
            if(g_stagedate.enemies[idx].life < 1)g_stagedate.enemies[idx].living = FALSE;
        }
        g_stagedate.knives[i].living = FALSE;//当たったらナイフも消える
    }
}