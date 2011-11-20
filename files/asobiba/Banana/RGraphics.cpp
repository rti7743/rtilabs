// RGraphics.cpp: RGraphics クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGraphics.h"

#include "RDebug.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGraphics::RGraphics()
{

}

RGraphics::~RGraphics()
{

}

void RGraphics::Clear(RDIBBitmap * inTarget)
{
	memset(inTarget->getImage() , 0 , inTarget->getHeight() * inTarget->getWidth() * (inTarget->getBit() / 8) );
}

void RGraphics::Clear(RDIBBitmap * inTarget , COLORREF16 inRGB)
{
//	memset(inTarget->getImage() , 0 , inTarget->getHeight() * inTarget->getWidth() * (inTarget->getBit() / 8) );
	unsigned short * image = (unsigned short * )inTarget->getImage();
	int count = inTarget->getHeight() * inTarget->getWidth() * (inTarget->getBit() / 8) / 2;
	for(int i = 0; i < count ; i++)
	{
		*image++ = inRGB;
	}
}

void RGraphics::WhiteClear(RDIBBitmap * inTarget)
{
	memset(inTarget->getImage() , 0xff , inTarget->getHeight() * inTarget->getWidth() * (inTarget->getBit() / 8) );
}

void RGraphics::PSet(RDIBBitmap * inTarget,int inX , int inY, COLORREF16 inRGB)
{
	if (inX < 0 || inY < 0 || inX >= inTarget->getWidth() || inY >= inTarget->getHeight() )
	{
//		ASSERT(0);
		return ;
	}
//	printf("%d,%d\n" , inX, inY);

	ASSERT(inTarget->getBit() == 16);
	*(unsigned short*)(inTarget->getImage() + (inX + inY * (inTarget->getWidth() )) * (inTarget->getBit() / 8) ) = inRGB;
//	SetPixel(inTarget->getDC() , inX,inY , RGB(255,255,255) );
}



void RGraphics::Paint(RDIBBitmap * inTarget , int inX, int inY , COLORREF16 inTargetColor ,const RECT * inClipper)
{
	PaintRev(inTarget , inX , inY , inTargetColor , inClipper);
}



void RGraphics::PaintRev(RDIBBitmap * inTarget , int inX, int inY , COLORREF16 inTargetColor,const RECT * inClipper)
{
//	if (inX < 0 || inY < 0 || inX >= Width || inY >= Height) return ;


	int l ,r , x;
	//左にいけるところまで行きます。
	for(l = inX ; l >= inClipper->left ; l-- )
	{
		if ( *inTarget->getImage( l , inY) == inTargetColor) break;
	}
	l++;

	//右にいけるところまで行きます。
	for(r = inX ; r < inClipper->right; r++ )
	{
		if ( *inTarget->getImage( r , inY) == inTargetColor) break;
	}
	r--;

	//OK. その間を塗りつぶします。
	for(int i = l  ; i <= r ; i++ )
	{
		PSet(inTarget , i , inY , inTargetColor);
	}

//	static iaaa = 0;
//	inTarget->Save("test/test" + num2str(iaaa++) + ".bmp");

	//で、その間にあったやつのひとつ上のピクセルで塗りつぶせるやつがあるんだったら塗りつぶす.
	if (inY > inClipper->top) 
	{
		int y = inY - 1;
		for(x = l ; x < r - 1; x++ )
		{
			if ( *inTarget->getImage(x ,  y) != inTargetColor ) 
				PaintRev(inTarget , x,y , inTargetColor ,  inClipper );
		}
	}
	//また、その間にあったやつのひとつ下のピクセルで塗りつぶせるやつがあるんだったら塗りつぶす.
	if (inY < inClipper->bottom-1) 
	{
		int y = inY + 1;
		for(x = l ; x < r - 1; x++ )
		{
			if ( *inTarget->getImage(x ,  y) != inTargetColor ) 
				PaintRev(inTarget , x,y , inTargetColor ,  inClipper );
		}
	}
}


void RGraphics::Line(RDIBBitmap * inTarget, int inSX,int inSY , int inEX,int inEY , COLORREF16 inRGB , int inSize)
{
	ASSERT(inTarget->getBit() == 16);
/* 
	HPEN Pen = ::CreatePen( PS_SOLID, 1, inRGB );
	::SelectObject(inTarget->getDC() , Pen );
	::MoveToEx(inTarget->getDC() , inSX , inSY , NULL);
	::LineTo(inTarget->getDC() , inEX , inEY );
	::DeleteObject(Pen);
*/
//	RGraphics::OneLine(inTarget , inSX , inSY , inEX , inEY , COLORREF16(RGB(255,0,0)) );

//		static int ii = 0;
//		TRACE("ほし%d,%d - %d,%d\n" , inSX,inSY,inEX,inEY);
//		inTarget->Save("test/test" + num2str(ii++) +".bmp");
//		RGraphics::OneLine(inTarget , inSX , inSY , inEX , inEY , inRGB );	return ;
	if (inSize == 1)
	{
		RGraphics::OneLine(inTarget , inSX , inSY , inEX , inEY , inRGB );
	}
	else
	{
		RGraphics::MultiLine(inTarget , inSX , inSY , inEX , inEY , inRGB ,inSize);
	}
}

bool RGraphics::LineHosei(int * ioPX,int * ioPY,int inSX,int inSY , int inEX,int inEY , int inWidth , int inHeight)
{
	int t;
	bool XhoseiFalse = false;
	do
	{
		//X方向の補正.
		if (*ioPX > inWidth )
		{
			if (inEX != inSX)
			{
				t = (inEY - inSY) * ( inWidth - 1 - inSX) / (inEX - inSX)  + inSY;
				if (t < 0 || t >= inHeight)
				{
					XhoseiFalse = true;	//Xをあわせると Y がおかしくなる
					break;
				}
				*ioPY = t;
			}
			*ioPX = inWidth - 1;
			return true;
		}
		else if (*ioPX < 0)
		{
			if (inEX != inSX)
			{
				t = (inEY - inSY) * (0 - inSX) / (inEX - inSX) + inSY;
				if (t < 0 || t >= inHeight)
				{
					XhoseiFalse = true;	//Xをあわせると Y がおかしくなる
					break;
				}
				*ioPY = t;
			}
			*ioPX = 0;
			return true;
		}
	}while(0);

	do
	{
		if (*ioPY > inHeight )
		{
			if (inEX != inSX && inEY != inSY)
			{
				t = (inHeight - 1 - inSY) * (inEX - inSX)  / (inEY - inSY) + inSX;
				if (t < 0 || t >= inWidth)
				{
					XhoseiFalse = true;	//Yをあわせると X がおかしくなる
					break;
				}
				XhoseiFalse = false;
				*ioPX = t;
			}
			*ioPY = inHeight - 1;
			return true;
		}
		else if (*ioPY < 0)
		{ 
			if (inEX != inSX && inEY != inSY)
			{
				t = (0 - inSY) * (inEX - inSX)  / (inEY - inSY) + inSX;
				if (t < 0 || t >= inWidth)
				{
					XhoseiFalse = true;	//Yをあわせると X がおかしくなる
					break;
				}
				XhoseiFalse = false;
				*ioPX = t;
			}
			*ioPY = 0;
			return true;
		}
	}while(0);

	return (XhoseiFalse == false);
}

void RGraphics::OneLine(RDIBBitmap * inTarget, int inSX,int inSY , int inEX,int inEY ,COLORREF16 inRGB )
{
	ASSERT(inTarget->getBit() == 16);

	//ともに範囲外であれば描画すらしない.
	if (inSX < 0 && inSY < 0 && inEX < 0 && inEY < 0) return ;
	if (inSX >= inTarget->getWidth() && inSY >= inTarget->getHeight() && inEX >= inTarget->getWidth() && inEY >= inTarget->getHeight() ) return ;
	if (inSX == inEX && inSY == inEY) return ;


	int sx = inSX;
	int sy = inSY;
	int ex = inEX;
	int ey = inEY;
	//開始地点の補正.
	if (! RGraphics::LineHosei(&sx , &sy , inSX , inSY , inEX , inEY , inTarget->getWidth() ,  inTarget->getHeight() ) )
	{
		return ;
	}
	//終了地点の補正.
	if (! RGraphics::LineHosei(&ex , &ey , inSX , inSY , inEX , inEY , inTarget->getWidth() ,  inTarget->getHeight() ) )
	{
		return ;
	}
	//補正したけどやっぱりだめ?
	if ( sx < 0 || sx >= inTarget->getWidth()  || ex < 0 || ex >= inTarget->getWidth() ||
		 sy < 0 || sy > inTarget->getHeight() || ey < 0 || ey > inTarget->getHeight() )
	{
//		ASSERT(0);
		return ;
	}
	if (sx == ex && sy == ey) return ;
/*
	if (inRGB == ConvertRGB(RGB(0,0,255)) )
	{
		static int ii = 0;
		TRACE("ほし%d,%d - %d,%d\n" , sx,sy,ex,ey);
		inTarget->Save("test/test" + num2str(ii++) +".bmp");
	}
*/
	//移動量の計算.
	int dx,dy,xincr,yincr;
	if( sx > ex ) 
	{
		xincr=-1;
		dx = sx - ex; 
	}
	else
	{
		xincr=1;
		dx = ex - sx;
	}
	if( sy > ey ) 
	{
		yincr=-1;
		dy = sy - ey; 
	}
	else
	{
		yincr=1;
		dy = ey - sy; 
	}


	int e2x = 2 * dx;
	int e2y = 2 * dy;

//	int x = sx;
//	int y = sy;

	//Yに移動したときに image ポインタを移動しなければならない量.
	int addYPitch = (inTarget->getWidth() ) * yincr;

	unsigned short debugColor = COLORREF16(RGB(255,0,0));

	//開始座標の位置を取得.
	unsigned short * image = (unsigned short *)inTarget->getImage(sx,sy);

    if (dy < dx)
    {                    // 点線にしない為
        //X 方向 を 増加する場合
        int e = -dx;
        for(int i = 0 ; i <= dx ; i++)
        {
//            PSet(inTarget  , x , y , debugColor );
			*image = inRGB;
 //           x = x + xincr;                 // X 方向
			image += xincr;

            e = e + e2y;             // DDA による補間
            if (e >= 0) 
            {
//                y = y + yincr ;            // Y方向
				image += addYPitch;
                e = e - e2x;         // DDA による補間
            }
        }
    }
    else
    {
        // Y 方向 を 増加する場合
        int e = -dy;
        for(int i = 0 ; i <= dy ; i ++)
        {
//            PSet(inTarget  , x , y , debugColor );
			*image = inRGB;
//            y = y + yincr;                  // Y 方向
			image += addYPitch;
            e = e + e2x;              // DDA による補間
            if (e >= 0) 
            {
//                x = x + xincr ;             // X 方向
				image += xincr;
                e = e - e2y;          // DDA による補間
            }
        }
    }
}

void RGraphics::MultiLine(RDIBBitmap * inTarget, int inSX,int inSY , int inEX,int inEY ,COLORREF16 inRGB , int inSize)
{
	ASSERT(inTarget->getBit() == 16);

	//ともに範囲外であれば描画すらしない.
	if (inSX < 0 && inSY < 0 && inEX < 0 && inEY < 0) return ;
	if (inSX >= inTarget->getWidth() && inSY >= inTarget->getHeight() && inEX >= inTarget->getWidth() && inEY >= inTarget->getHeight() ) return ;
	if (inSX == inEX && inSY == inEY) return ;

	int halfSize = inSize / 2;

	int sx = inSX;
	int sy = inSY;
	int ex = inEX;
	int ey = inEY;
	//開始地点の補正.
	if (! RGraphics::LineHosei(&sx , &sy , inSX , inSY , inEX , inEY , inTarget->getWidth() ,  inTarget->getHeight() ) )
	{
		return ;
	}
	//終了地点の補正.
	if (! RGraphics::LineHosei(&ex , &ey , inSX , inSY , inEX , inEY , inTarget->getWidth() ,  inTarget->getHeight() ) )
	{
		return ;
	}
	//補正したけどやっぱりだめ?
	if ( sx < 0 || sx >= inTarget->getWidth()  || ex < 0 || ex >= inTarget->getWidth() ||
		 sy < 0 || sy > inTarget->getHeight() || ey < 0 || ey > inTarget->getHeight() )
	{
//		ASSERT(0);
		return ;
	}
	if (sx == ex && sy == ey) return ;
/*
	if (inRGB == ConvertRGB(RGB(0,0,255)) )
	{
		static int ii = 0;
		TRACE("ほし%d,%d - %d,%d\n" , sx,sy,ex,ey);
		inTarget->Save("test/test" + num2str(ii++) +".bmp");
	}
*/
	//移動量の計算.
	int dx,dy,xincr,yincr;
	if( sx > ex ) 
	{
		xincr=-1;
		dx = sx - ex; 
	}
	else
	{
		xincr=1;
		dx = ex - sx;
	}
	if( sy > ey ) 
	{
		yincr=-1;
		dy = sy - ey; 
	}
	else
	{
		yincr=1;
		dy = ey - sy; 
	}


	int e2x = 2 * dx;
	int e2y = 2 * dy;

//	int x = sx;
//	int y = sy;

	//Yに移動したときに image ポインタを移動しなければならない量.
	int addYPitch = (inTarget->getWidth() ) * yincr;
	int abs_addYPitch = abs(addYPitch);

	unsigned short debugColor = COLORREF16(RGB(255,0,0));

	//開始座標の位置を取得.
	unsigned short * image = (unsigned short *)inTarget->getImage(sx,sy);


    if (dy < dx)
    {                    // 点線にしない為
  		unsigned short * image00 = (unsigned short *)inTarget->getImage(0,0);
		unsigned short * imageXX = (unsigned short *)inTarget->getImage(inTarget->getWidth() - 1,inTarget->getHeight() - 1);

		//X 方向 を 増加する場合

		//ペンの太さ
		int penStart = abs_addYPitch * halfSize;
		int penEnd = abs_addYPitch * halfSize;

        int e = -dx;
        for(register int i = 0 ; i <= dx ; i++)
        {
//            PSet(inTarget  , x , y , debugColor );
			unsigned short * pen = image - penStart;
			unsigned short * penForEnd = image + penEnd;
			//Yの頭が画面外なら押し戻す
			for( ; pen < image00 ; pen += abs_addYPitch);
			//Yのけつが画面外なら押し戻す
			for( ; penForEnd > imageXX; penForEnd -= abs_addYPitch);

			for( ; pen < penForEnd ; pen += abs_addYPitch)
			{
				*pen = inRGB;
			}
//			*image = inRGB;
//            x = x + xincr;                 // X 方向
			image += xincr;

            e = e + e2y;             // DDA による補間
            if (e >= 0) 
            {
//                y = y + yincr ;            // Y方向
				image += addYPitch;
                e = e - e2x;         // DDA による補間
            }
        }
    }
    else
    {
  		unsigned short * image00 = (unsigned short *)inTarget->getImage(0,sy);

        // Y 方向 を 増加する場合

		//ペンの太さ
		int penStart = 1 * halfSize;
		int penEnd = 1 * halfSize;

        int e = -dy;
        for(register int i = 0 ; i <= dy ; i ++)
        {
//            PSet(inTarget  , x , y , debugColor );
			unsigned short * pen = image - penStart;
			unsigned short * penForEnd = image + penEnd;
			//Xの頭が画面外なら押し戻す
			for( ; pen < image00 ; pen += 1);
			//Xのけつが画面外なら押し戻す
			for( ; penForEnd > image00 + abs_addYPitch; penForEnd -= 1);

			for( ; pen < penForEnd ; pen += 1)
			{
				*pen = inRGB;
			}
//			*image = inRGB;
//            y = y + yincr;                  // Y 方向
			image += addYPitch;
			image00 += addYPitch;
            e = e + e2x;              // DDA による補間
            if (e >= 0) 
            {
//                x = x + xincr ;             // X 方向
				image += xincr;
                e = e - e2y;          // DDA による補間
            }
        }
    }
}
void RGraphics::Copy(const RDIBBitmap * inSrc ,int inX, int inY,int inW, int inH, RDIBBitmap * inDest , int inDX ,int inDY)
{
    //簡易クリッピング
	if ( !Clipping(inSrc,&inX, &inY,&inW ,&inH , inDest , &inDX , &inDY) )
	{
		return ;
	}

	//開始座標の位置を取得.
	const unsigned short * src = (unsigned short *)inSrc->getConstImage(inX,inY);
	unsigned short * dest = (unsigned short *)inDest->getImage(inDX,inDY);

    for (register int y = 0 ; y < inH ;y ++ )
    {
        const unsigned short * BackupPoint1 = src;
        unsigned short * BackupPoint2 = dest;
        for (register int x = 0 ; x < inW ;x ++ )
        {
			if ( *src != 0)
			{
				*dest = *src;
			}
            src++;
            dest++;
        }
        src = BackupPoint1 ;
        dest = BackupPoint2 ;
        src += inSrc->getWidth() ;
        dest += inDest->getWidth() ;
    }
}

void RGraphics::AndCopy(const RDIBBitmap * inSrc ,int inX, int inY,int inW, int inH, RDIBBitmap * inDest , int inDX ,int inDY)
{
    //簡易クリッピング
	if ( !Clipping(inSrc,&inX, &inY,&inW ,&inH , inDest , &inDX , &inDY) )
	{
		return ;
	}

	//開始座標の位置を取得.
	const unsigned short * src = (unsigned short *)inSrc->getConstImage(inX,inY);
	unsigned short * dest = (unsigned short *)inDest->getImage(inDX,inDY);

    for (register int y = 0 ; y < inH ;y ++ )
    {
        const unsigned short * BackupPoint1 = src;
        unsigned short * BackupPoint2 = dest;
        for (register int x = 0 ; x < inW ;x ++ )
        {
			*dest = *dest & *src;
            src++;
            dest++;
        }
        src = BackupPoint1 ;
        dest = BackupPoint2 ;
        src += inSrc->getWidth() ;
        dest += inDest->getWidth() ;
    }
}
bool RGraphics::EasyClipingEx(const RDIBBitmap * bmd,int *rx1,int *ry1,int *rw1,int *rh1,
							 int rw2,int rh2,int rx2,int ry2,
							 int *sw,int *sh,int *ew,int *eh)
{
    //マイナス方向だったら補正.
    if(*rx1 < 0)
    {
        *rw1 += *rx1;  // *rx1 はマイナス.
        *rx1 = 0;
    }
    //マイナス方向だったら補正.
    if(*ry1 < 0)
    {
        *rh1 += *ry1;  // *ry1 はマイナス.
        *ry1 = 0;
    }
    //画像の横幅より大きければ　没
    if(*rx1 >= bmd->getWidth() )  return false;
    //画像の高さより大きければ　没
    if(*ry1 >= bmd->getHeight() )  return false;
    
    int p;
    p = rw2 - rx2 ;
    if (p < *rw1)    *ew = p;
    else             *ew = *rw1;
    if (rx2 < 0 ) *sw = abs(rx2) , *ew = *ew - *sw;
    else          *sw = 0;

    p = rh2 - ry2 ;
    if (p < *rh1)    *eh = p;
    else            *eh = *rh1;
    if (ry2 < 0 ) *sh = abs(ry2) , *eh = *eh - *sh;
    else          *sh = 0;

    if (*ew <= 0 || *eh <= 0 ) return false;
	return true;
}

bool RGraphics::Clipping(const RDIBBitmap * inSrc,int * ioSX,int * ioSY,int * ioW,int * ioH ,
					const RDIBBitmap * inDest,int * ioEX,int * ioEY)
{
	//開始位置補正
	if (*ioSX < 0)
	{
		*ioW += *ioSX;	//ioSXはマイナス
		*ioSX = 0;
	}
	else if (*ioSX >= inSrc->getWidth() )
	{
		*ioW -= (*ioSX - inSrc->getWidth() );
		*ioSX = inSrc->getWidth() - 1;
	}
	if (*ioW + *ioSX > inSrc->getWidth())
	{
		*ioW = inSrc->getWidth() - *ioSX ;
	}
	if (*ioSY < 0)
	{
		*ioH += *ioSY;	//ioSYはマイナス
		*ioSY = 0;
	}
	else if (*ioSY >= inSrc->getHeight() )
	{
		*ioH -= (*ioSY - inSrc->getHeight() );
		*ioSY = inSrc->getHeight() - 1;
	}
	if (*ioH + *ioSY > inSrc->getHeight())
	{
		*ioH = inSrc->getHeight() - *ioSY ;
	}

	//終了位置による補正
	if (*ioEX < 0)
	{
		*ioSX -= *ioEX;	//ioEXはマイナス.
		*ioW += *ioEX;	//ioEXはマイナス.
		*ioEX = 0;
	}
	else if (*ioEX >= inDest->getWidth() )
	{
		*ioW -= (*ioEX - inDest->getWidth() );
		*ioEX = inDest->getWidth() - 1;
	}
	if (*ioW + *ioEX > inDest->getWidth())
	{
		*ioW = inDest->getWidth() - *ioEX ;
	}

	if (*ioEY < 0)
	{
		*ioSY -= *ioEY;	//ioEYはマイナス.
		*ioH += *ioEY;	//ioEYはマイナス.
		*ioEY = 0;
	}
	else if (*ioEY >= inDest->getHeight() )
	{
		*ioH -= (*ioEY - inDest->getHeight() );
		*ioEY = inDest->getHeight() - 1;
	}
	if (*ioH + *ioEY > inDest->getHeight())
	{
		*ioH = inDest->getHeight() - *ioEY ;
	}

	if (*ioW < 0 || *ioW > inSrc->getWidth()  || *ioW > inDest->getWidth()  || 
		*ioH < 0 || *ioH > inSrc->getHeight() || *ioH > inDest->getHeight() )
	{
		return false;
	}

	return true;
}

bool RGraphics::Clipping(const RDIBBitmap * inSrc,int * ioSX,int * ioSY,int * ioW,int * ioH )
{
	//開始位置補正
	if (*ioSX < 0)
	{
		*ioW += *ioSX;	//ioSXはマイナス
		*ioSX = 0;
	}
	else if (*ioSX >= inSrc->getWidth() )
	{
		*ioW -= (*ioSX - inSrc->getWidth() );
		*ioSX = inSrc->getWidth() - 1;
	}
	if (*ioW + *ioSX > inSrc->getWidth())
	{
		*ioW = inSrc->getWidth() - *ioSX ;
	}
	if (*ioSY < 0)
	{
		*ioH += *ioSY;	//ioSYはマイナス
		*ioSY = 0;
	}
	else if (*ioSY >= inSrc->getHeight() )
	{
		*ioH -= (*ioSY - inSrc->getHeight() );
		*ioSY = inSrc->getHeight() - 1;
	}
	if (*ioH + *ioSY > inSrc->getHeight())
	{
		*ioH = inSrc->getHeight() - *ioSY ;
	}

	if (*ioW < 0 || *ioW > inSrc->getWidth()  || 
		*ioH < 0 || *ioH > inSrc->getHeight() )
	{
		return false;
	}

	return true;
}

void RGraphics::TranslucenceBox(RDIBBitmap * ioDest ,int inX, int inY,int inW, int inH, COLORREF16 inRGB)
{
    //簡易クリッピング
	if ( !Clipping(ioDest,&inX, &inY,&inW ,&inH ) )
	{
		return ;
	}

	//開始座標の位置を取得.
	unsigned short * src = (unsigned short *)ioDest->getConstImage(inX / 2 * 2,inY);
	COLORREF16 inRGBHalf  = ((inRGB >> 1) & 0xBDEF);

    for (register int y = 0 ; y < inH ;y ++ )
    {
        unsigned short * BackupPoint1 = src;
        for (register int x = 0 ; x < inW ;x ++ )
        {
//			unsigned char b = (inRGB >> 10) & 0x07
//BDEF 1 01111 01111 01111
//			*src = ((((*src >> 1) & 0xBDEF) + inRGBHalf)  << 1);
			*src = inRGB;
			src++;
        }
        src = BackupPoint1 ;
        src += ioDest->getWidth() ;
    }
}



void RGraphics::TestLine(int x,int y , int x2,int y2 )
{
	TRACE("%d,%d - %d,%d\n" , x,y,x2,y2);

	RDIBBitmap target;
	target.Create(676,748 , 16);
	RGraphics::Line( &target , x,y,x2, y2, COLORREF16(RGB(255,255,255)) ,1);
	target.Save("test/test1.bmp");

	RDIBBitmap target2;
	target2.Create(676,748 , 16);
	HPEN Pen = ::CreatePen( PS_SOLID, 1, RGB(255,255,255) );
	::SelectObject(target2.getDC() , Pen );
	::MoveToEx(target2.getDC() , x , y , NULL);
	::LineTo(target2.getDC() , x2 , y2 );
	::DeleteObject(Pen);

	target2.Save("test/test2.bmp");

	RDebug::FileVerify("test/test1.bmp" , "test/test2.bmp");
}

void RGraphics::test()
{
//	RGraphics::TestLine(87,28 , 77,0); 
	
//	RGraphics::TestLine(511,743 , 515,748); 
	{
		RDIBBitmap canvas;

		canvas.Create(200,200,16);
		Clear(&canvas , COLORREF16(RGB(0,0,0)) );

		TranslucenceBox(&canvas , 10,10 ,100, 100, COLORREF16(RGB(255,255,255)) );
		canvas.Save("test/copytest7.bmp");
//		RDebug::FileVerify("test/copytest7.bmp" , "test/copytest7_t.bmp");
	}
	{
		RDIBBitmap target;
		target.Create(676,716 , 16);
		RGraphics::MultiLine( &target , -1 , 307 , 63 , 262, COLORREF16(RGB(255,255,255)) ,2);
		target.Save("test/test1.bmp");	
	}
	{
		RDIBBitmap canvas;
		RDIBBitmap src;

		canvas.Create(131,131,16);
		Clear(&canvas , COLORREF16(RGB(255,255,255)) );
		src.Create(10,10,16);
		Clear(&src , COLORREF16(RGB(255,0,0)) );

		Copy(&src,0,0,8,8,&canvas , 20, 20);
		canvas.Save("test/copytest6.bmp");
//		RDebug::FileVerify("test/copytest6.bmp" , "test/copytest6_t.bmp");
	}
	{
		RDIBBitmap canvas;
		RDIBBitmap src;

		canvas.Create(100,100,16);
		Clear(&canvas , COLORREF16(RGB(255,255,255)) );
		src.Create(10,10,16);
		Clear(&src , COLORREF16(RGB(255,0,0)) );

		Copy(&src,0,0,10,10,&canvas , 0, 95);
		canvas.Save("test/copytest5.bmp");
		RDebug::FileVerify("test/copytest5.bmp" , "test/copytest5_t.bmp");
	}
	{
		RDIBBitmap canvas;
		RDIBBitmap src;

		canvas.Create(100,100,16);
		Clear(&canvas , COLORREF16(RGB(255,255,255)) );
		src.Create(10,10,16);
		Clear(&src , COLORREF16(RGB(255,0,0)) );

		Copy(&src,0,0,10,10,&canvas , 0, -5);
		canvas.Save("test/copytest4.bmp");
		RDebug::FileVerify("test/copytest4.bmp" , "test/copytest4_t.bmp");
	}
	{
		RDIBBitmap canvas;
		RDIBBitmap src;

		canvas.Create(100,100,16);
		Clear(&canvas , COLORREF16(RGB(255,255,255)) );
		src.Create(10,10,16);
		Clear(&src , COLORREF16(RGB(255,0,0)) );

		Copy(&src,0,0,10,10,&canvas , -5, 0);
		canvas.Save("test/copytest3.bmp");
		RDebug::FileVerify("test/copytest3.bmp" , "test/copytest3_t.bmp");
	}
	{
		RDIBBitmap canvas;
		RDIBBitmap src;

		canvas.Create(100,100,16);
		Clear(&canvas , COLORREF16(RGB(255,255,255)) );
		src.Create(10,10,16);
		Clear(&src , COLORREF16(RGB(255,0,0)) );

		Copy(&src,0,0,10,10,&canvas , 95, 0);
		canvas.Save("test/copytest2.bmp");
		RDebug::FileVerify("test/copytest2.bmp" , "test/copytest2_t.bmp");
	}
	{
		RDIBBitmap canvas;
		RDIBBitmap src;

		canvas.Create(100,100,16);
		Clear(&canvas , COLORREF16(RGB(255,255,255)) );
		src.Create(10,10,16);
		Clear(&src , COLORREF16(RGB(255,0,0)) );

		Copy(&src,0,0,10,10,&canvas , 0, 0);
		canvas.Save("test/copytest1.bmp");
		RDebug::FileVerify("test/copytest1.bmp" , "test/copytest1_t.bmp");
	}
	return ;
}
