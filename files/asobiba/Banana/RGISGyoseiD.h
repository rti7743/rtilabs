// RGISGyoseiD.h: RGISGyoseiD クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISGYOSEID_H__51079BAB_3F3F_41FB_99C6_3C82FF756368__INCLUDED_)
#define AFX_RGISGYOSEID_H__51079BAB_3F3F_41FB_99C6_3C82FF756368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISFindCityToNumberConvert.h"

struct GyoseiDData
{
	string					Name;
	unsigned short			PT;
	bool					Use;
};

//行政の範囲
class RGISGyoseiD
{
public:
	RGISGyoseiD();
	virtual ~RGISGyoseiD();

	void Create(const string & inFilename, unsigned long inCode) throw(RException);
	void Delete();

	void SmallPack(const string & inPath , RGISFindCityToNumberConvert * ioFindCityToNumberConvert) const throw(RException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const GyoseiDData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

private:
	unsigned int			Count;
	GyoseiDData*	Points;

	unsigned long	Code;

};


#endif // !defined(AFX_RGISGYOSEID_H__51079BAB_3F3F_41FB_99C6_3C82FF756368__INCLUDED_)
