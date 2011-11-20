// RGISDraw.h: RGISDraw クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISDRAW_H__C77F49D9_DBAC_4EF7_AEB9_8E6F32244AFE__INCLUDED_)
#define AFX_RGISDRAW_H__C77F49D9_DBAC_4EF7_AEB9_8E6F32244AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSlp.h"
#include "RGIS2500IntArray.h"
#include "RDrawObject.h"

class RGISDraw  
{
public:
	RGISDraw(const URECT * inDisplay , RDrawObject  * ioDraw,const RGISSlp * inSlp )
	{
		Display = inDisplay;
		Draw = ioDraw;
		Slp = inSlp;
	}
	virtual ~RGISDraw()
	{
	}

	void Polygon(const GISINTArray* inArray,int inSize , COLORREF inRGB ) const;
	void Polygon(const RGIS2500PointArray* inArray,int inSize , COLORREF inRGB ) const;
	void TexOut(UPOINT * inXY,const string & inStr , int inSize , COLORREF inRGB) const;

	
private:

	const URECT * Display;
	RDrawObject  * Draw;
	const RGISSlp * Slp;
};

#endif // !defined(AFX_RGISDRAW_H__C77F49D9_DBAC_4EF7_AEB9_8E6F32244AFE__INCLUDED_)
