// RGraphics.h: RGraphics クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGRAPHICS_H__8FB3A22F_D0F4_48FF_AD0F_DC22D8D1F609__INCLUDED_)
#define AFX_RGRAPHICS_H__8FB3A22F_D0F4_48FF_AD0F_DC22D8D1F609__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDIBBitmap.h"

typedef unsigned short COLORREF16;
#define RGB24TOBGB16(x) ((COLORREF16) ( ((((x>>16)&0xff) >> 3) ) | ((((x>>8)&0xff) >> 3) << 5) | ((((x)&0xff) >> 3) << 10)  )) 


class RGraphics  
{
public:
	RGraphics();
	virtual ~RGraphics();

	static void Clear(RDIBBitmap * inTarget);
	static void Clear(RDIBBitmap * inTarget , COLORREF16 inRGB);
	static void WhiteClear(RDIBBitmap * inTarget);
	static void AndCopy(const RDIBBitmap * inSrc ,int inX, int inY,int inW, int inH, RDIBBitmap * inDest , int inDX ,int inDY);

	static void Copy(const RDIBBitmap * inSrc ,int inX, int inY,int inW, int inH, RDIBBitmap * inDest , int inDX ,int inDY);
	static void Line(RDIBBitmap * inTarget, int inSX,int inSY , int inEX,int inEY , COLORREF16 inRGB , int inSize);
	static void PSet(RDIBBitmap * inTarget,int inX , int inY, COLORREF16 inRGB);
	static void Paint(RDIBBitmap * inTarget , int inX, int inY , COLORREF16 inTargetColor ,const RECT * inClipper);
	static void TranslucenceBox(RDIBBitmap * ioDest ,int inX, int inY,int inW, int inH, COLORREF16 inRGB);

	static void test();
	static void TestLine(int x,int y , int x2,int y2 );

private:
	static bool RGraphics::EasyClipingEx(const RDIBBitmap * bmd,int *rx1,int *ry1,int *rw1,int *rh1,
							 int rw2,int rh2,int rx2,int ry2,
							 int *sw,int *sh,int *ew,int *eh);

	static bool RGraphics::Clipping(const RDIBBitmap * inSrc,int * ioSX,int * ioSY,int * ioW,int * ioH ,
							const RDIBBitmap * inDest,int * ioEX,int * ioEY);
	static bool RGraphics::Clipping(const RDIBBitmap * inSrc,int * ioSX,int * ioSY,int * ioW,int * ioH );



	static bool LineHosei(int * ioPX,int * ioPY,int inSX,int inSY , int inEX,int inEY , int inWidth , int inHeight);
	static void OneLine(RDIBBitmap * inTarget, int inSX,int inSY , int inEX,int inEY , COLORREF16 inRGB );
	static void MultiLine(RDIBBitmap * inTarget, int inSX,int inSY , int inEX,int inEY ,COLORREF16 inRGB , int inSize);
	static void PaintRev(RDIBBitmap * inTarget , int inX, int inY , COLORREF16 inTargetColor ,const RECT * inClipper);
};

#endif // !defined(AFX_RGRAPHICS_H__8FB3A22F_D0F4_48FF_AD0F_DC22D8D1F609__INCLUDED_)
