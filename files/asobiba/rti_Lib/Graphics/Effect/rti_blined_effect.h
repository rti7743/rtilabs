#include "StartWithExeFile.h"

#ifdef BLINED_LIFE

#ifndef ___BLINE_EFFECTH
#define ___BLINE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TBLinedEffect
{
private:
    //初期化
    void Init();
    BMD OutBmd; //出力
    BMD bmd;
	struct _BLined;
	struct _BLined{
		bool alive;
		short start;
		short x;
		short toutatu;
		short linew;
	};
	struct _BLined *BLined;
	int linehaba;
	int speed;
	int size;
	int counter;
	int deedcount;
public:
    TBLinedEffect();
    ~TBLinedEffect();
    void Set(BMD _bmd,BMD _OutBmd,int _linehaba,int _linekankaku,int _speed);
    int Draw(GraphicsProc Proc,int x,int y);
};

typedef TBLinedEffect* EBLINED;

#endif

#endif   // LIFE END
