#include "draw.h"
#include "keyboard.h"
#include "node.h"
#include "animequeue.h"

using namespace std;

const char *comment[] = {"Red Black Tree","You Can Not Entry Any More Number","ComIterator2","comite3","comite4"};
const char *nums[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

int fonthandles[200];
int g_comite = 0;
float g_comtimer = 0.0f;
char mystring[MAXDIGITS+1] = {'\0'};


char debugstring[10];

//数値->文字列
int tostring(int num){
    //桁数を取得する
    int power = 10;//これより小さいやつは一桁
    int digit = 1;
    while(power < num){
        digit++;
        power*=10;
    }
    mystring[digit] = '\0';
    for(int i = digit-1;i >= 0;i--){
        mystring[i] = '0' + num%10;
        num /= 10;
    }
    return digit;
}

//数値(不動小数点数)->文字列
void tostring2(float num){
    //10をかけながら9桁を作る
    debugstring[0] = ((int)num + '0');
    num *= 10;
    debugstring[1] = '.';
    for(int i = 2;i<9;i++){
        debugstring[i] = ((int)num%10 + '0');
        num *= 10;
    }
}


void DrawPanel(){
    //描画
    const int white = GetColor(255,255,255);
    const int green = GetColor(100,255,0);
    const int black = GetColor(0,0,0);
		//add
		DrawBox(WIDTH-60, 0, WIDTH, 40, GetColor(100,100,100), TRUE);
		if(cursol == ADD)DrawBox(WIDTH-60, 0, WIDTH, 40, green, FALSE);
		DrawStringToHandle(WIDTH-60, 0, "Add", black, g_fonthandle);
		DrawStringToHandle(WIDTH-60, 15, "Node", black, g_fonthandle);
		//remove
		DrawBox(WIDTH-60, 60, WIDTH, 100, GetColor(200,200,200), TRUE);
		if(cursol == REMOVE)DrawBox(WIDTH-60, 60, WIDTH, 100, green, FALSE);
		DrawStringToHandle(WIDTH-60, 60,  "Remove", black, g_fonthandle);
		DrawStringToHandle(WIDTH-60, 75,  "Node", black, g_fonthandle);
		//入力された数字
		DrawBox(WIDTH-60, 120, WIDTH, 160, white, TRUE);
		DrawStringToHandle(WIDTH-60, 120, g_input, black, g_fonthandle);
		//0
		DrawBox(WIDTH-57, 183, WIDTH-40, 200, white, TRUE);
		if(cursol == ZERO)DrawBox(WIDTH-57, 183, WIDTH-40, 200, green, FALSE);
		DrawStringToHandle(WIDTH-57, 183, nums[0], black, g_fonthandle);
		//cancel
		DrawBox(WIDTH-17, 183, WIDTH, 200, white, TRUE);
		if(cursol == CANCEL)DrawBox(WIDTH-17, 183, WIDTH, 200, green, FALSE);
		DrawStringToHandle(WIDTH-17, 183, "x", black, g_fonthandle);
		//1~9
		for(int i = 0;i<3;i++){
			for(int j = 1;j<=3;j++){
				DrawBox(WIDTH-77+j*20, 203+i*20, WIDTH-60+j*20, 220+i*20, white, TRUE);
				if(cursol == (CURSOL)(CANCEL + i*3+j))DrawBox(WIDTH-77+j*20, 203+i*20, WIDTH-60+j*20, 220+i*20, green, FALSE);
				DrawStringToHandle(WIDTH-77+j*20, 203+i*20, nums[i*3+j], black, g_fonthandle);
			}
		}	
}
void DrawComment(){
	const int white = GetColor(255,255,255);
	//カウントダウン中か
    if(0 < g_comtimer){
		g_comtimer -= g_frametime;
	}//カウントダウンが終わっていたらコメントをもとに戻す
	else{
		g_comtimer = 0.0f;
		g_comite = 0;
	}    
	//comment
	DrawStringToHandle(0, 0, comment[g_comite], white, g_fonthandle);
}
void MakeFontHandles(){
	for(int i = 0;i<200;i++){
		fonthandles[i] = CreateFontToHandle(NULL, i, 3);
	}
}

//nodeを描画する
void DrawNodes(int radius){
    int fontsize = min(199, radius*2/5);
    const int white = GetColor(255,255,255);
    const int black = GetColor(0,0,0);
    const int red = GetColor(255,0,0);
    if(!dheight)return;
    for(int i = 1;i<FRAMESIZE;i++){
        if(!g_frame[i])continue;
        int x,y,xl,xr,yl,yr;
        int nx,ny,nxl,nxr,nyl,nyr;
        
        //自身の計算
        pair<int,int> p = getGrid(i);
        int w = p.first;
        int h = p.second;

        x = w*(WIDTH/(float)getpow(h)) + (WIDTH/(float)getpow(h))/2;
        y = h*(HEIGHT/dheight) + (HEIGHT/dheight)/2;
        int dx = g_frame[i]->dx;
        int dy = g_frame[i]->dy;
        //移動中であれば自身座標を加算する
        if((dx>=0) && (dy>=0)){
            nx = dx*((float)WIDTH/(float)getpow(dy)) + ((float)WIDTH/(float)getpow(dy))/2;
            ny = dy*((float)HEIGHT/dheight) + ((float)HEIGHT/dheight)/2;

            x += (nx-x)*((float)MOVETIME - actiontimer);
            y += (ny-y)*((float)MOVETIME - actiontimer);
        }    
        //leftedgeがあるならその座標を計算
        if(g_frame[i]->leftedge){
            int wl = g_frame[i]->leftedge->x;
            int hl = g_frame[i]->leftedge->y;
            xl = wl*(WIDTH/(float)getpow(hl)) + (WIDTH/(float)getpow(hl))/2;
            yl = hl*(HEIGHT/dheight) + (HEIGHT/dheight)/2;

            int dxl = g_frame[i]->leftedge->dx;
            int dyl = g_frame[i]->leftedge->dy;
            //移動中であればleftedgeの座標をずらす
            if(dxl != -1 && dyl != -1){
                nxl = dxl*((float)WIDTH/(float)getpow(dyl)) + ((float)WIDTH/(float)getpow(dyl))/2;
                nyl = dyl*((float)HEIGHT/dheight) + ((float)HEIGHT/dheight)/2;

                xl += (nxl-xl)*((float)MOVETIME - actiontimer);
                yl += (nyl-yl)*((float)MOVETIME - actiontimer);
            }
        }
        //rightedgeがあるならその座標を計算
        if(g_frame[i]->rightedge){
            int wr = g_frame[i]->rightedge->x;
            int hr = g_frame[i]->rightedge->y;
            xr = wr*(WIDTH/(float)getpow(hr)) + (WIDTH/(float)getpow(hr))/2;
            yr = hr*(HEIGHT/dheight) + (HEIGHT/dheight)/2;

            int dxr = g_frame[i]->rightedge->dx;
            int dyr = g_frame[i]->rightedge->dy;
            //移動中であればleftedgeの座標をずらす
            if(dxr != -1 && dyr != -1){
                nxr = dxr*((float)WIDTH/(float)getpow(dyr)) + ((float)WIDTH/(float)getpow(dyr))/2;
                nyr = dyr*((float)HEIGHT/dheight) + ((float)HEIGHT/dheight)/2;

                xr += (nxr-xr)*((float)MOVETIME - actiontimer);
                yr += (nyr-yr)*((float)MOVETIME - actiontimer);
            }
        }
        if(g_frame[i]->leftedge)DrawLine(x, y, xl, yl, white);
        if(g_frame[i]->rightedge)DrawLine(x, y, xr, yr, white);
        
        //円の描画
        //disguise1 disguise2 exist で描画 
        if(g_frame[i]->dcolor == 'R'){
            DrawCircle(x, y, radius, red, true);
        }else{
            DrawCircle(x, y, radius, black, true);
        }
        DrawCircle(x, y, radius, white, false);
        //数値の描画
        int digits = tostring(g_frame[i]->dvalue);
        DrawStringToHandle(x-(radius*29/30)*(digits/5), y-(radius/6),mystring , white, fonthandles[fontsize]);
    }
}
