#include "tree.h"
#define MAXDIGITS 5
#define WIDTH 1000
#define HEIGHT 500
#define MAXNODENUM 64
#define FRAMESIZE 1024
#define MOVETIME 1.0F
#define EDGETIME  1.0F
#define COLORTIME  1.0F
#define VALUETIME 1.0F
#define HEIGHTTIME 1.0F
#define ACTIONSIZE 1024

extern Node* g_root;
extern Node g_node[MAXNODENUM];
extern Node* g_frame[FRAMESIZE];
extern int g_inputite;
extern char g_input[MAXDIGITS+1];

extern float g_frametime;
extern int g_fonthandle;
extern int g_comite;
extern float g_comtimer;

//indexを座標に変換
pair<int, int> getGrid(int index);

//座標をindexに変換
int getIndex(int x, int y);

//
int getpow(int num);