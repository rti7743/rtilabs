/*****************************************************************/
/*****************************************************************/
//　揺らす
//      うーむ、ちょっとバクありかもしれない (汗汗
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef WAVE_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_wave_effect.h>

TWaveEffect::TWaveEffect()
{
}

TWaveEffect::~TWaveEffect()
{
}

void TWaveEffect::Set(BMD bmd,BMD _OutBmd,
                      int x,int y,int w,int h,int haba,int kankaku,int speed)
{
    OutBmd = _OutBmd; //出力
    pbmd = bmd;
    px = x;
    py = y;
    pw = w;
    ph = h;
    counter = 0;
    phaba = haba;
    pkankaku = kankaku;
    pspeed = speed;
}

void TWaveEffect::DrawLR(int x,int y)
{
    int wy,wx;
	int TranceBaseByte;
	int TranceByte;
	int SkipByte;
	int ReserveAdW;
	int AddPicth1 ;
	int AddPicth2 ;
	BYTE *data1,*data2;
	AddPicth1 = pbmd->w * 3;
	AddPicth2 = OutBmd->w * 3;
	TranceBaseByte = pw * 3;
	
	CommDraw->GraphicsLock(pbmd);
	CommDraw->GraphicsLock(OutBmd);
	data1 = pbmd->bm + px*3 + py*AddPicth1; //ここは普通に転送する場所まで移動
	data2 = OutBmd->bm+y*AddPicth2; //縦幅だけスタートにあわせる

    for (wy = 0 ; wy < ph ; wy++ 
		,data2 += AddPicth2 ,	data1 += AddPicth1)
    {
        wx = (int)(LTBSin256[ ((counter+wy) )  & 0xff  ] * (double)phaba) ;
		ReserveAdW = (wx + x) *3; //転送を受けるとところ
		//先方チェック!
		if (ReserveAdW < 0)
		{
			//注意:ReserveAdWはマイナスである。

			TranceByte += ReserveAdW; //転送サイズを減らします (^^;;
			if (TranceByte <= 0 ) continue; //転送するものがないので離脱
			SkipByte = -ReserveAdW;   //減らしたぶんだけ転送元の画像のアドレスを進める
			ReserveAdW = 0;           //転送スタートは 0 だよ
		}
		else
		{
			SkipByte = 0; //スキップなし
			//しんがりチェック!
			if (AddPicth2 < ReserveAdW+TranceBaseByte)
			{ //最低のラインを転送する
				TranceByte = AddPicth2 - ReserveAdW;
			}
			else
			{ //通常転送
				TranceByte = TranceBaseByte;
			}
		}

		//コピー
		memcpy( (data2 + ReserveAdW ) , (data1 +SkipByte) ,TranceByte);
    }
	CommDraw->GraphicsUnLock(OutBmd);
	CommDraw->GraphicsUnLock(pbmd);

    counter+= pspeed;
}

void TWaveEffect::DrawUD(int x,int y)
{
    int wy,wx,wh;
    int s,h;
	int eh;
	int ReserveH;
	int AddPicth1;
	int AddPicth2;
	BYTE *data1;
	BYTE *data2;
	BYTE *backupdata1;
	BYTE *backupdata2;

    //簡易終了位置クリッピング
    int sw,ew,p;
    p = OutBmd->w - x ;
    if (p < pw)    ew = p;
    else           ew = pw;
    if (x < 0 )    sw = abs(x) , ew = ew - sw;
    else           sw = 0;

	AddPicth1 = pbmd->w*3;
	AddPicth2 = OutBmd->w*3;
	h = OutBmd->h;
	
	CommDraw->GraphicsLock(pbmd);
	CommDraw->GraphicsLock(OutBmd);
	data1 = (pbmd->bm +(px+sw)*3+py* AddPicth1);
	data2 = (OutBmd->bm + (x+sw)*3+ y* AddPicth2);

    for (wx = 0 ; wx < ew ; wx++,
							data1+=3 ,	data2+=3)
    {
        s = ( ((counter+wx) )  &0xff);
        wy = (int)(LTBSin256[s] * (double)phaba) ;

		backupdata1 = data1;
		backupdata2 = data2;

		//上のクリッピング
		eh = ph;
		ReserveH = y+wy;
		if (ReserveH > h) continue; //転送する部分がない
		if (ReserveH < 0)
		{
			wy = 0;
			eh += ReserveH;
			data1+= (-ReserveH) * AddPicth1;
		}
		//下のクリッピング
		if (ReserveH+eh > h)
		{
			eh = eh - ((ReserveH+eh)-h);
		}
		data2 += wy * AddPicth2; //受信開始アドレス
		for (wh = 0 ; wh < eh ; wh++)
		{
			*(data2) = *(data1);
			*(data2+1) = *(data1+1);
			*(data2+2) = *(data1+2);
			data1 += AddPicth1;
			data2 += AddPicth2;
		}
		data1 = backupdata1 ;
		data2 = backupdata2 ;
    }
	CommDraw->GraphicsUnLock(OutBmd);
	CommDraw->GraphicsUnLock(pbmd);

    counter+= pspeed;
}
#endif  //LIFE END
