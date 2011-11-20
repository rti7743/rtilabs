/*****************************************************************/
/*****************************************************************/
//タイルをスクロールさせます
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef SCROLL_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_scroll_effect.h>

TScrollEffect::TScrollEffect()
{
}

TScrollEffect::~TScrollEffect()
{
}

void TScrollEffect::Set(BMD _bmd,BMD _OutBmd,int _speedx,int _speedy)
{
    OutBmd = _OutBmd; //出力
 	bmd = _bmd;
	speedx = _speedx;
	speedy = _speedy;
    counterx = 0;
    countery = 0;
}

void TScrollEffect::Draw(GraphicsProc Proc,int x,int y,int w,int h)
{
    int wx,wy;
    int tw ,th; //テクスチャーの幅補正前
    int sx ,sy; //書き出すところ
    int ew,eh;
    tw = bmd->w;
    th = bmd->h;
    if (counterx >= 0) counterx=-tw+1;
    if (countery >= 0) countery=-th+1;

    sx = x+counterx;
    sy = y+countery;
    ew = w+tw;
    eh = h+th;
    for (wy = 0; wy < eh ;wy+=th)
    {
        for (wx = 0; wx < ew ;wx+=tw)
        {
    		Proc(bmd,0,0,
	    		 tw,th,
	  	    	 OutBmd,sx+wx,sy+wy);
        }
    }
    counterx +=speedx;
    countery +=speedy;
}

#endif  //LIFE END
