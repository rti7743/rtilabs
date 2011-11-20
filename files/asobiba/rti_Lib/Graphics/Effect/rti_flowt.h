#include "StartWithExeFile.h"
#ifdef FLOWT_LIFE

#ifndef ___FLOWT_EFFECTH
#define ___FLOWT_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TFlowtEffect
{
private:
    void Init();

    BMD OutBmd; //出力
    BMD bmd;
    int onesizew;
    int total;
    int xspeedmax;
    int yspeed;
    int flow;
    int areatparsent;
    int zureparsent;

    struct _FlowtChar;
    struct _FlowtChar{
        bool alive;
        short x,y;
        short xspeed,yspeed;
        short wsize;
    };
    struct _FlowtChar* FlowtChar;
public:
    TFlowtEffect();
    ~TFlowtEffect();
    void Set(BMD _bmd,BMD OutBmd,int _onesizew,int _total,int _yspeed,int _xspeedmax,int _flow,int _areatparsent,int _zureparsent);
	void Draw(GraphicsProc Proc,int x,int y,int w,int h);
};

typedef TFlowtEffect* FLOWT;

#endif

#endif //LIFE END
