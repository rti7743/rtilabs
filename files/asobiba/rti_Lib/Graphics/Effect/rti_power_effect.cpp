/*****************************************************************/
/*****************************************************************/
//パワーチェェェェェェェェェェンジ(NOT POWER STONE)
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef POWER_LIFE

#include <windows.h>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_power_effect.h>


TPowerEffect::TPowerEffect()
{
    BmdTable  = NULL;
    PowerChar = NULL;
}

TPowerEffect::~TPowerEffect()
{
    Init();
}

void TPowerEffect::Init()
{
    if (BmdTable) free(BmdTable);
    if (PowerChar) free(PowerChar);
}

void TPowerEffect::Set(BMD bmd,BMD _OutBmd,
						 int _xpowerpoint,int _ypowerpoint,int _zpowerpoint,
						 int _areatparsent,int _flow,int _speed)
{
    OutBmd = _OutBmd; //出力
	xpowerpoint	= _xpowerpoint;
	ypowerpoint	= _ypowerpoint;
	zpowerpoint	= _zpowerpoint;
	flow	= _flow;
	areatparsent	= _areatparsent;
	samplingz	=  _zpowerpoint / _areatparsent + 1;
	speed = _speed;

    Init();

	int size = sizeof(struct _PowerChar)*flow;
	//粒子の基本データの確保
	PowerChar = (struct _PowerChar*) malloc(size + 1);
    ZeroMemory(PowerChar,size);

	//絵の配列
	BmdTable = (BMD*) 
		malloc(sizeof(BMD)*samplingz + 1);

    ZeroMemory(PowerChar,size);

    Fixed addw,addh;
	//どれだけzが移動したら変更するか
	Fixed kankaku = FixedDivSp(zpowerpoint , samplingz);

	Fixed x,y;
    int w,h;
    x = Int2Fixed(bmd->w);
    y = Int2Fixed(bmd->h);
    addw = FixedDiv(x,kankaku);
    addh = FixedDiv(y,kankaku);
	for(int i = 0 ; i < samplingz ; i ++)
	{
        w = Fixed2Int(x); 
        h = Fixed2Int(y);
		BmdTable[i] = BmpChar->New(bmd->w  , bmd->h);
        Cls(BmdTable[i]);
		//縮小します
		BigDraw(bmd,1,1,bmd->w-1,bmd->h-1,BmdTable[i],1,1,w-1,h-1);
		x -= addw;
		y -= addh;
	}
    kankakuz = Fixed2Int(kankaku);
}


void TPowerEffect::Draw(GraphicsProc Proc,int x,int y,int w,int h)
{
    struct _PowerChar* lpwPowerChar = PowerChar;
    int usebmp;
	BMD bmd;
	int xspeed;  //すぴーどx
	int yspeed;  //すぴーどy
	int zspeed;  //すぴーどy
    int dx,dy;
    int halfw,halfh;
    int i;
    bool allredy_make = false;
    halfw = w >>1 ,halfh = h >>1 ;
    for(i = 0 ;i < flow ; i ++)
    {
        if (lpwPowerChar->alive)
        {
			//XYZの速度を求める
			path_finder3D(
				lpwPowerChar->x,lpwPowerChar->y,lpwPowerChar->z,
				xpowerpoint,ypowerpoint,zpowerpoint,
				&xspeed,&yspeed,&zspeed,speed);

			lpwPowerChar->x += xspeed;
		 	lpwPowerChar->y += yspeed;
			lpwPowerChar->z += zspeed;
            dx = (halfw - (lpwPowerChar->x*10 / lpwPowerChar->z) );
            dy = (halfh - (lpwPowerChar->y*10 / lpwPowerChar->z) );
			//Zから見て正しいてくすちゃをはります
            usebmp = lpwPowerChar->z / kankakuz;
            if (usebmp>=samplingz-1) usebmp = samplingz-1;

			bmd = BmdTable[usebmp];
			if (HitCheckP(lpwPowerChar->x,lpwPowerChar->y,
				xpowerpoint,ypowerpoint,50) )
			{ //到達しているかチェック
				lpwPowerChar->alive = false;
			}
			else
			{
				Proc( bmd,0,0,bmd->w,bmd->h,OutBmd,dx,dy);
			}
        }
        else //空き発見
        {
            if ( rand100(areatparsent) && !allredy_make)
            { //登録
				if (randTF())
				{
					lpwPowerChar->x = randX(0,w);
					if (randTF()) lpwPowerChar->y = 0;
					else          lpwPowerChar->y = h;
				}
				else
				{
					if (randTF()) lpwPowerChar->x = 0;
					else          lpwPowerChar->x = w;
					lpwPowerChar->y = randX(0,h);
				}
				lpwPowerChar->z = 0;
				lpwPowerChar->alive = true;
                allredy_make = true;
            }
        }
        lpwPowerChar++;
    }
}

#endif  //LIFE END
