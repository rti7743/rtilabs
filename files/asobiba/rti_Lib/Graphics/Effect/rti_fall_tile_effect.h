#include "StartWithExeFile.h"

#ifdef FALLTILE_LIFE

#ifndef ___FILL_EFFECTH
#define ___FILL_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TFallTileEffect
{
private:
    void Init();
    BMD OutBmd; //出力
    BMD SpBuf; //すーたとしたときのスプライトバッファの内容のコピー
    int TableSize;
    struct _FallBlock;
    struct _FallBlock{
        bool alive;
        unsigned short start;
        short dx,dy;
        short x,y;
        short sizex,sizey;
        short speedx,speedy;
    };
    struct _FallBlock* FallBlockBuf;
    int  timecounter;
    int deedcount;
    short MaxXSize,MaxYSize;

public:
    TFallTileEffect();
    ~TFallTileEffect();
    void Set(BMD bmd,BMD _OutBmd,int sizex,int sizey,int speed,unsigned char kind,short xkankaku,short ykankaku);
    int Draw(GraphicsProc Proc);
};

typedef TFallTileEffect* EFILL;

#define FALLTILE_X_UP 1
#define FALLTILE_X_DOWN 2
#define FALLTILE_Y_UP 4
#define FALLTILE_Y_DOWN 8
#define FALLTILE_X_HALF_SPEED 16
#define FALLTILE_Y_HALF_SPEED 32

#endif

#endif //LIFE END

