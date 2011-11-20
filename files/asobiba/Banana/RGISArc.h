// RGISArc.h: RGISArc クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISARC_H__86C56552_8872_417C_8E9E_A1DB6BF65F38__INCLUDED_)
#define AFX_RGISARC_H__86C56552_8872_417C_8E9E_A1DB6BF65F38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISDraw.h"
#include "RGIS2500IntArray.h"

struct ArcData
{
	int	Type;
	int	LineTag;
	int No;
	RGIS2500PointArray	Array;
};

class RGISArc  
{
public:
	RGISArc();
	virtual ~RGISArc();

	void Create(const string & inFilename, int inScale) throw(RException);
	void Delete();


	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const ArcData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Draw(const RGISDraw * inDraw,const RGIS2500IntArray* inArray , int inType) const;

	static void test();
private:
	COLORREF selectColor(GISType* inShubetsu) const;
	COLORREF selectSize(GISType* inShubetsu) const;

private:
	unsigned int			Count;
	ArcData*	Points;

};

#endif // !defined(AFX_RGISARC_H__86C56552_8872_417C_8E9E_A1DB6BF65F38__INCLUDED_)
