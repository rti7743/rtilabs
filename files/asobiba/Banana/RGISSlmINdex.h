// RGISSlmINdex.h: RGISSlmINdex クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSLMINDEX_H__9F26EFC4_AED6_4963_AD5C_24B890167EB6__INCLUDED_)
#define AFX_RGISSLMINDEX_H__9F26EFC4_AED6_4963_AD5C_24B890167EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSlm.h"

typedef vector<RGISSlm*>	RGISSlms;

//すべての slm ファイルを読み込む.
class RGISSlmINdex  
{
public:
	RGISSlmINdex();
	virtual ~RGISSlmINdex();

	//すべての slm ファイルを読み込む.
	void Create(const string & inPath ,const RStringList & inTargetList , int inScale) ;
	//すべて破棄.
	void Destroy();

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Points.size();
	}
	//地点の取得
	const RGISSlm* getPoint(unsigned int p) const
	{
		ASSERT(p < getCount() );
		return Points[p];
	}


private:
	RGISSlms	Points;
};

#endif // !defined(AFX_RGISSLMINDEX_H__9F26EFC4_AED6_4963_AD5C_24B890167EB6__INCLUDED_)
