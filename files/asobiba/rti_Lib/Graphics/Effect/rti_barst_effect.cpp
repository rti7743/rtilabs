#include "StartWithExeFile.h"
#ifdef BARST_LIFE

/*****************************************************************/
/*****************************************************************/
//爆発と復帰 デス&リバース
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\Effect\rti_barst_effect.h>
#include <.\Base\rti_glookup.h>

TBarstEffect::TBarstEffect()
{
    list = NULL;
}

TBarstEffect::~TBarstEffect()
{
    Init();
}

void TBarstEffect::Init()
{
	if(list) free(list);
}

void TBarstEffect::Set(BMD bmd,BMD _OutBmd,bool desth,int x,int y,int w,int h,int basespeed)
{
    OutBmd = _OutBmd; //出力
	pbmd = bmd;  //絵に関係する情報は途中で変更することはできないので
	pw = bmd->w; //グローバルとしてもらっておく (^^;;
	ph = bmd->h; 
	pww = w;
	phh = h;

    Init();

	listmax =( (pw / pww) + (pw%pww ? 1 : 0) ) * ( (ph / phh) + (ph%phh ? 1 : 0) );
	list = (struct BarstStruct*)malloc( sizeof(struct BarstStruct) * (listmax + 1) );

	if (desth) //爆発
		ReSet(x,y,true,basespeed);
	else       //復原
		RevReSet(x,y,true,basespeed);
}

//爆発のリセット
void TBarstEffect::ReSet(int x,int y,bool set_possition,int basespeed) 
{
	int wx,wy,i;
	struct BarstStruct* puf;
    pbasepeed = basespeed ;

	i = 0;
	for (wy = ph-1; wy >= 0  ; wy-=phh)
	{
		for (wx = pw-1; wx >= 0  ; wx-=pww)
		{
				i ++;
				puf = &(list[i]);
				puf->org = PGet(pbmd,wx,wy);
				if (set_possition)
				{ //絵のある位置を指定して上げる
					puf->x = wx + x;
					puf->y = wy + y;
				}
				while ( (puf->sx = ( (rand()%4-2) * rand()%pbasepeed )  ) == 0);
				while ( (puf->sy = ( (rand()%4-2) * rand()%pbasepeed )  ) == 0);
				puf->w = pww;
				puf->h = phh;
				puf->alive = true;
		}
	}

	eopuf = 0;
}

//復帰のリセット
void TBarstEffect::RevReSet(int x,int y,bool set_possition,int basespeed) 
{
	struct BarstStruct* puf;
	int wx,wy,i;
	int ww,wh;
	px = x;
	py = y;
	ww = OutBmd->w -1;
	wh = OutBmd->h -1;
    pbasepeed = basespeed ;

	i = 0;
	for (wy = ph-1; wy >= 0  ; wy-=phh)
	{
		for (wx = pw-1; wx >= 0  ; wx-=pww)
		{
				i ++;
				puf = &(list[i]);
				puf->org = PGet(pbmd,wx,wy);
				if (set_possition)
				{  //適当に画面端に指定する
					if (rand() % 2)
					{ //X可変の 上下
						puf->x = rand() % ww + 1;
						if (rand() % 2)	puf->y = wh;
						else			puf->y = 1;
					}
					else
					{ //X可変の 左右
						puf->y = rand() % wh + 1;
						if (rand() % 2)	puf->x = ww;
						else			puf->x = 1;
					}
				}
				//移動量を求める
				path_finder(puf->x,puf->y, x+wx , y+wy ,
					&(puf->sx),&(puf->sy),pbasepeed);
				puf->w = pww;
				puf->h = phh;
				puf->alive = true;
		}
	}

	eopuf = 0;
}

//爆発絵画
int TBarstEffect::Draw()
{
	int i;
	int w,h;
	w = OutBmd->w-1;
	h = OutBmd->h-1;
	struct BarstStruct* puf;
	for (i = listmax-1 ; i >= 0  ; i --)
	{
		puf = &(list[i]);
		if (puf->alive && puf->org)
		{
			if (puf->x >= w || puf->x <= 0 || puf->y >= h || puf->y <= 0)
			{
				puf->alive = false;
				eopuf ++;
			}
			else
			{
                PSet(OutBmd,puf->x,puf->y,puf->org);

                puf->x += puf->sx;
				puf->y += puf->sy;
			}
		}
	}
	return  listmax - eopuf;
}


//復帰絵画
int TBarstEffect::RevDraw()
{
	int x,y;
	int ssx,ssy;
	int wy,wx,faj;
	int realx,realy;
	int w,h;
	w = OutBmd->w-1;
	h = OutBmd->h-1;
	struct BarstStruct* puf;
	struct BarstStruct* plist;
	faj = pbasepeed<<1;

	plist = list;
	for (wy = ph-1 ; wy >= 0;wy-=phh)
	{
		for (wx = pw-1; wx >=0 ; wx-=pww)
		{
			puf = plist++;
			if (puf->alive && puf->org)
			{
				x = puf->x += (ssx = puf->sx);    //移動 
				y = puf->y += (ssy = puf->sy);
				realx = px+wx;        //実際座標(目的地)
				realy = py+wy;        //実際座標(目的地)
				if ( (realx - faj) <= x && (realx + faj) >= x && (realy - faj) <= y && (realy + faj) >= y)
				{ //誤差は容認する
					puf->x = realx;
					puf->y = realy;
					puf->alive = false;
					eopuf ++;
				}
				if (realx > x && ssx < 0 ||
					realx < x && ssx > 0 ||
					realy > y && ssy < 0 ||
					realy < y && ssy > 0 )
				{
							path_finder(x,y, realx , realy ,
									&(puf->sx),&(puf->sy),pbasepeed);
				}
			}
			//一応クリッピングしておく..(good bye speed)
			if (puf->x < w && puf->x > 0 && puf->y < h && puf->y > 0)
                        PSet(OutBmd,puf->x,puf->y,puf->org);
		} 
	}
	return  listmax - eopuf;
}


#endif //LIFE END

