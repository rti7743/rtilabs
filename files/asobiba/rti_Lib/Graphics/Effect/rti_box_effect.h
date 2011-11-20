#include "StartWithExeFile.h"

#ifdef BOXEFFECT_LIFE

#ifndef ___BOX_EFFECTH
#define ___BOX_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TBoxEffect
{
private:
    void Init();
    BMD OutBmd; //出力
    BMD SpBuf; //すーたとしたときのスプライトバッファの内容のコピー
    int TableSize;
    struct _BoxBlock;
    struct _BoxBlock{
        bool alive;
        short x,y;      //スタート位置
        short nw,nh;    //書き込み範囲
        short sizex,sizey;
    };
    struct _BoxBlock* BoxBlockBuf;
    int Speed;
    int timecounter;
    int deedcount;
    short MaxXSize,MaxYSize;

public:
    TBoxEffect();
    ~TBoxEffect();
    void Set(BMD bmd,BMD _OutBmd,int sizex,int sizey,int speed,unsigned char kind);
    int Draw(int x,int y,GraphicsProc Proc);
};

typedef TBoxEffect* EBOX;

#endif

#endif //LIFE END

