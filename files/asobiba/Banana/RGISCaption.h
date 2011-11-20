// RGISCaption.h: RGISCaption クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISCAPTION_H__8FCA3A8E_F856_42F2_AFCD_1396E76C4099__INCLUDED_)
#define AFX_RGISCAPTION_H__8FCA3A8E_F856_42F2_AFCD_1396E76C4099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGraphics.h"
#include "RMathAngle.h"
#include "RFont.h"

class RGISCaption  
{
public:
	RGISCaption();
	virtual ~RGISCaption();

	void Create(RDIBBitmap * ioCanvas);
	void Black(COLORREF16 color);
	void Houi(RAngle deg);
	void Zahyou(const LPOINT * inXY);
	void ScaleLine(int inScale);
private:
	void RotateLine(RDIBBitmap * ioCanvas , int sx,int sy ,int ex, int ey , COLORREF16 color , int size ,RAngle deg,int xjiku , int yjiku) const;

	const RMathAngleFixed	* MathAngle;
	RFont		NewFont;

	RDIBBitmap * Canvas;
};

#endif // !defined(AFX_RGISCAPTION_H__8FCA3A8E_F856_42F2_AFCD_1396E76C4099__INCLUDED_)
