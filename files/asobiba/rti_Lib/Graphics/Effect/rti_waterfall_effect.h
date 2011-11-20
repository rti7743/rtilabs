#include "StartWithExeFile.h"
#ifdef WATERFALL_LIFE

#ifndef ___WATERFALL_EFFECTH
#define ___WATERFALL_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TWaterFallEffect
{
private:
    void Init();

    BMD OutBmd; //出力
    BMD pbmd;
    int px;
    int py;
    int pw;
    int ph;
    int pspeed;
    int ppals;
    bool pud;
   
    int *speed_dim;
public:
    TWaterFallEffect();
    ~TWaterFallEffect();
    bool Draw(int ux,int uy);
    void Set(BMD bmd,BMD OutBmd,int x,int y,int w,int h,int speed,bool ud);
};

typedef TWaterFallEffect* EWATERFALL;

#endif


#endif  //LIFE END
