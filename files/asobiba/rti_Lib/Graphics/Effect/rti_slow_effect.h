#include "StartWithExeFile.h"

#ifdef SLOW_LIFE

#ifndef ___SLOW_EFFECTH
#define ___SLOW_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス
#include <.\Graphics\Effect\rti_effect.h>

class TSlowEffect
{
private:
     BMD OutBmd;
	 BMD bmd;
	 int sizex;
	 int sizey;
	 int tsizex;
	 int tsizey;
	 int speed;
     int timecounter ;
	 int sizexhalf;
	 int sizeyhalf;

public:
    TSlowEffect();
    ~TSlowEffect();
    void Set(BMD _bmd,BMD _OutBmd,int _sizex,int _sizey,int _tsizex,int _tsizey,int _speed);

    bool Draw(GraphicsProc Proc,int x,int y);
};

typedef TSlowEffect* ESLOW;

#endif

#endif   // LIFE END
