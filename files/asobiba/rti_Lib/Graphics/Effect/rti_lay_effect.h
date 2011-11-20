#include "StartWithExeFile.h"

#ifdef LAY_LIFE

#ifndef ___LAY_EFFECTH
#define ___LAY_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

#define LAY_LEFT    3
#define LAY_RIGHT   1
#define LAY_UP      0
#define LAY_DOWN    2

class TLayEffect
{
private:
    int X1,Y1,W1,H1,X2,Y2;
    BMD Bmd1,Bmd2;
    int Possition;
    int Speed;
    unsigned char Kind;
    bool PickDrawLR();
    bool PickDrawUD();
public:
	TLayEffect();
    ~TLayEffect();
	void Set(BMD _bmd1,int _x1,int _y1,int _w1,int _h1,
        BMD _bmd2,int _x2,int _y2,unsigned char _kind,int _speed);
	void Draw();
};

typedef TLayEffect* LAY;

#endif

#endif   // LIFE END
