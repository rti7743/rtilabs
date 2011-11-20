// RDrawObject.h: RDrawObject クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDRAWOBJECT_H__F5201049_3817_4384_9052_D901A00EA134__INCLUDED_)
#define AFX_RDRAWOBJECT_H__F5201049_3817_4384_9052_D901A00EA134__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RFontCache.h"
#include "RSujiFontCache.h"
#include "RMessageBox.h"
#include "RGraphics.h"
#include "RException.h"
#include "RGISSmallShareImage.h"
#include "BBananaConfig.h"

#include "RGISSmallDrawEffect.h"

class RDrawObject  
{
public:
	RDrawObject();
	virtual ~RDrawObject();

	void RDrawObject::Create(RDIBBitmap * poolBitmap, int inWidth , int inHeight, const RGISSmallShareImage* poolShareImage) throw (RException);
	void RDrawObject::Delete();
	void RDrawObject::Line(const POINT & inFrom , const POINT & inTo , COLORREF16 inColor , int inSize);
	void RDrawObject::TextOut(const POINT & inPoint,const string & inStr);
	void RDrawObject::SujiOut(const POINT & inPoint,unsigned int inSuji);
	void RDrawObject::Paint(const POINT & inPoint, COLORREF16 inDrawColoer ,COLORREF16 inTarget);
	void RDrawObject::setPixel(int x,int y , COLORREF16 inColor );
	unsigned short getPixel(int x,int y );

	void BitBlt(int sx,int sy,int sw,int sh,int ex,int ey);
	void RDrawObject::Black(COLORREF16 color);


	//ペイントで塗りつぶすときの制限値のセット.
	void setPaintClip(const RECT * inClipper)
	{
		Clipper = *inClipper;
	}

	void DebugSave(const string & inFilename) const
	{
		PoolBitmap->Save(inFilename);
	}

	int getWidth() const
	{
		return Width;
	}
	int getHeight() const
	{
		return Height;
	}

	void TranslucenceBox(const POINT & inPoint , int inSize , unsigned char inColor);

	void Boxes(const RGISSmallDrawEffect* inEffect ,unsigned char * inLevel    ,const LPOINT * inBlockStart,const COLORREF16 * inPallet) const;

	void setMark(const POINT & inPoint , int inBitmapNo);

	//中央に十字を書く
	void RDrawObject::CenterCross();
private:

	void RDrawObject::PaintRev(int inX, int inY , COLORREF inTarget , int inFlg);
private:
	RDIBBitmap	*	PoolBitmap;


	int Width , Height;
	RFontCache	Cache;
	RSujiFontCache	SujiCahce;
	RECT Clipper;

	const RGISSmallShareImage* PoolShareImage;
};

#endif // !defined(AFX_RDRAWOBJECT_H__F5201049_3817_4384_9052_D901A00EA134__INCLUDED_)
