// RGISSlp.h: RGISSlp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSLP_H__C900AB00_D66C_4EFC_B6E6_F49200FF335B__INCLUDED_)
#define AFX_RGISSLP_H__C900AB00_D66C_4EFC_B6E6_F49200FF335B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISSlm.h"
#include "GISType.h"

//#define CLIPX(x)	((x-83260000)/4096)
//#define CLIPY(x)	(500-((x-547900000)/4096))
#define CLIPX(x)	(((x)-83260000)/1024)
#define CLIPY(x)	(500-(((x)-747900000)/1024))


class RGISSlp  
{
public:
	RGISSlp();
	virtual ~RGISSlp();

	void Create(const string & inFilename,const RGISSlm * inSlm) throw(RException);
	//スケールを変更し、ディスプレイ座標にします。
	void ConvertDisplayScal() throw(RException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	UPOINT getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);

		return Points[p];
	}
	//最大最小の場所の取得.
	LRECT getRect() const;

	bool TestCV(GISINTArray* inArray);


	static void test();
private:
	unsigned int			Count;
	UPOINT	*	Points;

};

#endif // !defined(AFX_RGISSLP_H__C900AB00_D66C_4EFC_B6E6_F49200FF335B__INCLUDED_)
