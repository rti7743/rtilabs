/*****************************************************************/
/*****************************************************************/
//　ゆっくり ぼやぁ～と絵画します
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef SLOW_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_slow_effect.h>

TSlowEffect::TSlowEffect()
{
}

TSlowEffect::~TSlowEffect()
{
}

void TSlowEffect::Set(BMD _bmd,BMD _OutBmd,int _sizex,int _sizey,int _tsizex,int _tsizey,int _speed)
{
    OutBmd = _OutBmd; //出力
 	bmd = _bmd;
    timecounter = 0;
	sizex = _sizex;
	sizey = _sizey;
	tsizex = _tsizex;
	tsizey = _tsizey;
	speed = _speed;
	sizexhalf =_sizex>>1;
	sizeyhalf =_sizey>>1;
}


bool TSlowEffect::Draw(GraphicsProc Proc,int x,int y)
{
	short wy,wx;  
	short sy,sx;  //絵画開始点
	short gw,gh;  //絵画幅
	short ty,tx;  //絵画軸をずらす
    short bmdw,bmdh;  //絵のサイズのテンポラリ
	bool end_of_flg = false;  //終了フラグ
	bool ret = true;          //戻り値

	//高速化のために 絵の高さと幅をテンポラリよに
	bmdw = bmd->w;
	bmdh = bmd->h;

	//絵画軸をずらす
	tx = tsizex+(timecounter>>2);
	ty = tsizey+(timecounter>>2);
	//簡易クリッピングその1
	if (tx > sizexhalf) 
	{
		tx = sizexhalf;
		end_of_flg = true;
	}
	if (ty > sizeyhalf) 
	{
		if (end_of_flg) ret= false; //終了
		ty = sizeyhalf;
	}
			
    for(wy=0 ; wy < bmdh ; wy +=sizey )
    {
        for(wx=0 ; wx < bmdw ; wx +=sizex )
        {
			//   |そのタイルの中心| これが基本型
			sx = wx + sizexhalf - (tx);
			sy = wy + sizeyhalf - (ty);
			gw = tx<<1;
			gh = ty<<1;
			//速度は落ちるがクリッピングしておく
			if (sx+gw >= bmdw) gw = bmdw - sx;
			if (sy+gh >= bmdh) gh = bmdh - sy;
			if (gw > 0 && gh > 0)
			{
				Proc(bmd,sx,sy,gw,gh, OutBmd,x + sx,y + sy);
			}
        }
    }
    timecounter +=speed ;
	return ret;
}

#endif  //LIFE END
