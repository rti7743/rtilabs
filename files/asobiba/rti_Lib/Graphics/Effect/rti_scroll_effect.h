#include "StartWithExeFile.h"

#ifdef SCROLL_LIFE

#ifndef ___SCROLL_EFFECTH
#define ___SCROLL_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス
#include <.\Graphics\Effect\rti_effect.h>

class TScrollEffect
{
private:
     BMD OutBmd;
	 BMD bmd;
	 int speedx;
	 int speedy;
     int counterx ;
     int countery ;

public:
    TScrollEffect();
    ~TScrollEffect();
    void Set(BMD _bmd,BMD _OutBmd,int _speedx,int _speedy);

    void Draw(GraphicsProc Proc,int x,int y,int w,int h);
};

typedef TScrollEffect* ESCROLL;

#endif

#endif   // LIFE END
