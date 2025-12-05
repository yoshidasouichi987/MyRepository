#include "keyboard.h"
using namespace std;

bool z_keyprev=false, up_keyprev=false, down_keyprev=false, left_keyprev=false, right_keyprev=false;
char g_input[MAXDIGITS+1] = {'\0'};
int g_inputite = 0;

CURSOL cursol = ADD;

int SGV(){
	//inputをintになおす
	int value = 0;
	int power = 1;
	for(int i = g_inputite-1;i>=0;i--){
		value += (int)(g_input[i] - '0')*power;
		power *= 10;
	}
	return value;
}

//zキーが押されたら 
void ProcessZkey(){
	//Zキーが押されていたら
    if(isZkey(&z_keyprev)){
		//追加
		if(cursol == ADD){
			int value = SGV();
			//ノード数が限界を超えていたら
			if(g_root && g_root->size >= MAXNODENUM){
				//コメント2を3秒表示
				g_comite = 2;
				g_comtimer = 3;
			}else{
				//ノードの挿入
				g_root = insert(value, g_root, g_node);
			}
		}//削除
		else if(cursol == REMOVE){
			int value = SGV();
			//ノードの削除
			g_root = remove(value, g_root);
		}//✕
		else if(cursol == CANCEL){
			InputValue(-1);
		}
		else{
			//桁数が最大 コメント1を3秒表示
			if(g_inputite >= MAXDIGITS){
				g_comite = 1;
				g_comtimer = 3;
			}//カーソルが0
			else if(cursol == (int)ZERO){
				InputValue(0);
			}//0 cancel 1　の順になっている..
			else{
				InputValue((int)(cursol-CANCEL));
			}
		}
    }
}
//カーソルを動かす
void MoveCursol(int inputstate){
	//add remove は1下げ 0 は2下げ cancel~6は3下げ 7~9はadd
	if(iskey(&down_keyprev ,inputstate, PAD_INPUT_DOWN)){
		if((int)cursol <= (int)REMOVE)cursol = (CURSOL)((cursol+1)%13);
		else if((int)cursol == (int)ZERO)cursol = (CURSOL)((cursol+2)%13);
		else if((int)cursol <= (int)SIX)cursol = (CURSOL)((cursol+3)%13);
		else cursol = ADD;
	}
	//add remove 0は1上げ cancelはremove 1~2は0 3~9は3上げ
	if(iskey(&up_keyprev, inputstate, PAD_INPUT_UP)){
		if((int)cursol <= (int)ZERO)cursol = (CURSOL)((13+cursol-1)%13);
		else if((int)cursol == CANCEL)cursol = REMOVE;
		else if((int)cursol <= (int)TWO)cursol = ZERO;
		else cursol = (CURSOL)((13+cursol-3)%13);
	}
	if(iskey(&left_keyprev, inputstate, PAD_INPUT_LEFT)){
		cursol = (CURSOL)((13+cursol-1)%13);
	}
	if(iskey(&right_keyprev, inputstate, PAD_INPUT_RIGHT)){
		cursol = (CURSOL)((cursol+1)%13);
	}
}

//入力に応じて数字を書き込む
void InputValue(int num){
	if(num == -1){
		if((g_inputite) > 0){
			g_input[g_inputite] = '\n';
			(g_inputite)--;
			g_input[g_inputite] = '\0';
		}
	}
	else{
		g_input[g_inputite] = (char)('0'+num);
		(g_inputite)++;
		g_input[g_inputite] = '\0';
	}
}
BOOL iskey(bool *prev, int state, int target){
	if(!(*prev)){
		if(state&target){
			*prev = true;
			return true;
		}else{
			*prev = false;
		}
	}else{
		if(!(state&target)){
			*prev = false;
		}
	}
	return false;
}

//zキー入力判定
BOOL isZkey(bool *prev){
	if(!(*prev)){
		if(CheckHitKey(KEY_INPUT_Z)){
			*prev = true;
			return true;
		}else{
			*prev = false;
		}
	}else{
		if(!CheckHitKey(KEY_INPUT_Z)){
			*prev = false;
		}
	}
	return false;
}
