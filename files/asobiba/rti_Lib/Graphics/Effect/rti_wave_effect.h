#include "StartWithExeFile.h"

#ifdef WAVE_LIFE

#ifndef ___WAVE_EFFECTH
#define ___WAVE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TWaveEffect
{
private:
    BMD OutBmd; //出力
    BMD pbmd;
    int px;
    int py;
    int ph;
    int pw;
    int phaba;
    int pkankaku;
    unsigned char counter;
    int pspeed;
     
public:
    TWaveEffect();
    ~TWaveEffect();

    void Set(BMD bmd,BMD _OutBmd,
        int x,int y,int w,int h,int haba,int kankaku,int speed);
    void DrawLR(int x,int y);
    void DrawUD(int x,int y);
};

typedef TWaveEffect* EWAVE;

#endif

#endif   // LIFE END
