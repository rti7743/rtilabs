#include "StartWithExeFile.h"

#ifdef POWER_LIFE

#ifndef ___POWER_EFFECTH
#define ___POWER_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TPowerEffect
{
private:
    void Init();

    struct _PowerChar;
    struct _PowerChar{
        bool alive;
        int x,y,z;
        int xspeed,yspeed,zspeed;
    };
    struct _PowerChar* PowerChar;

    BMD OutBmd;
	BMD *BmdTable;

	int xpowerpoint;
	int ypowerpoint;
	int zpowerpoint;
	int samplingz; //z座標の奥行きをこれでわったサイズ分だけテクスチャの縮小バージョンを作ります
	int flow;      //とぶ粒子の数
	int areatparsent; //出現する確立
	int speed;

	int kankakuz;
public:
	TPowerEffect();
    ~TPowerEffect();
	void Set(BMD bmd,BMD _OutBmd,
						 int _xpowerpoint,int _ypowerpoint,int _zpowerpoint,
						 int _areatparsent,int _flow,int _speed);
    void Draw(GraphicsProc Proc,int x,int y,int w,int h);
};

typedef TPowerEffect* EPOWER;

#endif

#endif   // LIFE END
