// RGISChimei.h: RGISChimei クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISCHIMEI_H__EA6748B3_2201_43D4_9E54_8BFA48E174B8__INCLUDED_)
#define AFX_RGISCHIMEI_H__EA6748B3_2201_43D4_9E54_8BFA48E174B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RDrawObject.h"
#include "RGISSlp.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"

struct ChimeiData
{
	GISType					SR;
	unsigned short			NameIndex;
	unsigned short			PT;
	bool					Use;
};


class RGISChimei
{
public:
	RGISChimei();
	virtual ~RGISChimei();

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
	const ChimeiData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

	static void test();

private:
	unsigned int			Count;
	ChimeiData*	Points;

};


#endif // !defined(AFX_RGISCHIMEI_H__EA6748B3_2201_43D4_9E54_8BFA48E174B8__INCLUDED_)
