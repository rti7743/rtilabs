// RGISSmallDraw.h: RGISSmallDraw クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLDRAW_H__C66A273E_3E87_4781_815A_1C75A3213110__INCLUDED_)
#define AFX_RGISSMALLDRAW_H__C66A273E_3E87_4781_815A_1C75A3213110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDrawObject.h"
#include "RGISSmallType.h"
#include "RGISSmallDrawEffect.h"
#include "RGISSmallDupKill.h"

class RGISSmallDraw  
{
public:
	RGISSmallDraw()	//デバッグ専用
	{
		Draw = NULL;
		Effect = NULL;
	}
	RGISSmallDraw(RDrawObject  * ioDraw,const RGISSmallDrawEffect* inEffect)
	{
		Draw = ioDraw;
		Effect = inEffect;

	}
	virtual ~RGISSmallDraw()
	{
	}

	void Polygon(const LPOINT * inBlockStart,const char* inCVData , int CVCount , int inSize , COLORREF16 inRGB ) const;
	void TextOut(const LPOINT * inBlockStart,const char* inCVData , int CVCount ,const char * inName , RGISSmallDupKill * ioDupKill) const;
	void TextOut(const LPOINT * inBlockStart ,const SPOINT * inXY,COLORREF16 inRGB, const char * inName) const;
	void Icon(const LPOINT * inBlockStart ,const SPOINT * inXY,int inBitmapNo ,COLORREF16 inRGB, const char * inName) const;

	void Paint(const LPOINT * inBlockStart,const char* inCVData , int CVCount , COLORREF16 inRGB) const;
	void Icon(const LPOINT * inXY,int inBitmapNo ,COLORREF16 inRGB,  const char * inName) const;
	void Point(const LPOINT * inBlockStart ,const SPOINT * inXY, int inSize , COLORREF16 inRGB , unsigned long inSuji) const;
	void Boxes(unsigned char * inLevel    ,const LPOINT * inBlockStart,const unsigned short * inPallet) const;
	void TranslucenceBox(const LPOINT * inXY,unsigned char inSize , unsigned char inColor);

	void DebugSave(const string & inFilename) const
	{
		Draw->DebugSave(inFilename);
	}

	//拡大したときにはしょるレベルの決定
	ParticularLevel getLevel() const
	{
		return Effect->getLevel();
	}

	void setPaintClip(const RECT * inClipper)
	{
		Draw->setPaintClip(inClipper);
	}

	static void test();

private:
	//inSrc と inDest の2点を通る直線上より内側に inTargetがありますか?
	bool IsLineOn(const POINT * inSrc , const POINT * inTarget , const POINT * inDest ) const;
private:

	RDrawObject  * Draw;
	const RGISSmallDrawEffect* Effect;
};

#endif // !defined(AFX_RGISSMALLDRAW_H__C66A273E_3E87_4781_815A_1C75A3213110__INCLUDED_)
