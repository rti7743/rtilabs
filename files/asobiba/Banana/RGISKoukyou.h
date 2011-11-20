// RGISKoukyou.h: RGISKoukyou クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISKOUKYOU_H__7D712565_528D_40AA_A28B_8F750AF02563__INCLUDED_)
#define AFX_RGISKOUKYOU_H__7D712565_528D_40AA_A28B_8F750AF02563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RDrawObject.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"


struct KoukyouData
{
	GISType					SR;
	unsigned short			NameIndex;
	unsigned short			PT;
	bool					Use;
};


class RGISKoukyou  
{
public:
	RGISKoukyou();
	virtual ~RGISKoukyou();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();

	void SmallPack(const string & inDir,const RGISSlp * inSlp, const RGISNameDatabase* inNameData)  const throw(RException);
	JoinTempFormatScript getPackInfo() const;
	
	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const KoukyouData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

	static void test();

private:
	unsigned int			Count;
	KoukyouData*	Points;
};

#endif // !defined(AFX_RGISKOUKYOU_H__7D712565_528D_40AA_A28B_8F750AF02563__INCLUDED_)
