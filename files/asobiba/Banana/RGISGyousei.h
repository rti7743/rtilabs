// RGISGyousei.h: RGISGyousei クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISGYOUSEI_H__16A4E8AE_9EE0_4B82_AF4D_02B0A71C718B__INCLUDED_)
#define AFX_RGISGYOUSEI_H__16A4E8AE_9EE0_4B82_AF4D_02B0A71C718B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RGISDraw.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"

struct GyouseiData
{
	GISType		JT;
	GISType		SR;
	GISINTArray	CV;
	//以下無視!
};


class RGISGyousei  
{
public:
	RGISGyousei();
	virtual ~RGISGyousei();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();

	void SmallPack(const string & inFilename,const RGISSlp * inSlp)  const throw(RException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const GyouseiData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void RGISGyousei::Draw(const RGISDraw * inDraw) const;

	static void test();

private:
	unsigned int				Count;
	GyouseiData*	Points;
};

#endif // !defined(AFX_RGISGYOUSEI_H__16A4E8AE_9EE0_4B82_AF4D_02B0A71C718B__INCLUDED_)
