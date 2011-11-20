#include "StartWithExeFile.h"

#ifdef CRASH_LIFE

#ifndef ___CRASH_EFFECTH
#define ___CRASH_EFFECTH

#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TCrashEffect
{
private:
    BMD OutBmd;
    int X,Y;
    int ShiftX,ShiftY;
	int Speedx;
	int Speedy;
    unsigned char Flg;
public:
    TCrashEffect();
    ~TCrashEffect();
    void Set(BMD _bmd,unsigned int _speedx,unsigned int _speedy,
        unsigned int _shiftx,unsigned int _shifty);
    bool Draw();
    bool CrashMove(int *_P,int *_Speed,int _Shift,unsigned char ReturnFlg);
    void MoveDraw(BMD bmd1,int rx2,int ry2);
};

typedef TCrashEffect* ECRASH;

#endif

#endif   // LIFE END
