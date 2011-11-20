/*****************************************************************/
/*****************************************************************/
//ÇΩÇÍÇÈÇÊÇ§Ç…äGâÊ
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef WATERFALL_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_waterfall_effect.h>
#include <math.h>
TWaterFallEffect::TWaterFallEffect()
{
    speed_dim = NULL;
}

TWaterFallEffect::~TWaterFallEffect()
{
    Init();
}

void TWaterFallEffect::Init()
{
    if(speed_dim) free(speed_dim);
}


void TWaterFallEffect::Set(BMD bmd,BMD _OutBmd,
                           int x,int y,int w,int h,int speed,bool ud)
{
    OutBmd = _OutBmd; //èoóÕ
    pbmd = bmd;
    px = x;
    py = y;
    pw = w;
    ph = h;
    pspeed = speed;
    pud = ud;
    ppals = 10;


    Init();
    speed_dim = (int*) malloc(sizeof(int) * pw + 1);
    int ww;
	int *p_speed_dim;
	p_speed_dim = speed_dim;
    for (ww = 0 ; ww < pw ; ww++ )
    {   //äÓñ{ë¨ìxÇçÏÇÈ
        
        *p_speed_dim = (int)(sin( RRR((ww*(20)%360))  )* (rand()%ppals) );
        if (*p_speed_dim >= ph) *p_speed_dim = ph;
		p_speed_dim ++; //éüÇÃóvëfÇ÷
    }
}


bool TWaterFallEffect::Draw(int ux,int uy)
{
    int wx,wy;
    int ew,eh;
	int AddPicth1;
	int AddPicth2;
	int *p_speed_dim;
    int hpspeed;
	p_speed_dim = speed_dim;

	BYTE *data1;
	BYTE *data2;
	BYTE *backupdata1;
	BYTE *backupdata2;
	AddPicth1 = pbmd->w*3;
	AddPicth2 = OutBmd->w*3;

    hpspeed = pspeed;
    ew = pw;
    eh = ph;

	CommDraw->GraphicsLock(pbmd);
	CommDraw->GraphicsLock(OutBmd);
	data1 = (pbmd->bm +(px)*3+py* AddPicth1);
	data2 = (OutBmd->bm + (ux)*3+ uy* AddPicth2);
    for(wx = 0 ;wx < pw; wx++)
    {
		backupdata1 = data1;
		backupdata2 = data2;
        for(wy = 0 ;wy < *p_speed_dim; wy++)
        {
    		*(data2) = *(data1);
	    	*(data2+1) = *(data1+1);
		    *(data2+2) = *(data1+2);
			data1 += AddPicth1;
			data2 += AddPicth2;
        }
		data1 = backupdata1 ;
		data2 = backupdata2 ;
        data1 += 3;
        data2 += 3;
        *p_speed_dim += pspeed;
        if (*p_speed_dim >= ph) *p_speed_dim = ph;
		p_speed_dim ++;
    }
	CommDraw->GraphicsUnLock(OutBmd);
	CommDraw->GraphicsUnLock(pbmd);
return false;
}


#endif  //LIFE END

