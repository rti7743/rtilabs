// RGISPgn.h: RGISPgn クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISPGN_H__35C7BA52_A5C8_49E4_B6C6_1F83B770E616__INCLUDED_)
#define AFX_RGISPGN_H__35C7BA52_A5C8_49E4_B6C6_1F83B770E616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGIS2500IntArray.h"
#include "RGISDraw.h"
#include "RGISArc.h"
#include "RGISAtr.h"
#include "RGISNameDatabase.h"
#include "RGIS2500Info.h"

struct PgnData
{
	int	Type;						//図式分類コード
	int No;							//個別番号
	RGIS2500IntArray	Array;		//当該ポリゴンを構成するアーク指定レコード
	UPOINT	Representative;			//代表点
};

class RGISPgn  
{
public:
	RGISPgn();
	virtual ~RGISPgn();
	void Create(const string & inFilename, int inScale) throw(RException);
	void Delete();

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const PgnData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Draw(const RGISDraw * inDraw, const RGISArc * inArc) const;

	void RGISPgn::SmallPack(const RGISArc * inArc ,const RGISAtr * inAtr , 
		const string & inPath  ,const string & inMark,RGISNameDatabase* ioNameDatabase,
		RGIS2500InfoVector* gis2500infoVector);

	void RGISPgn::SmallPackNoWrite(const RGISArc * inArc , const RGISAtr * inAtr , RGIS2500InfoVector* gis2500infoVector);

	static void test();
private:
	COLORREF selectColor(GISType* inShubetsu) const;
	COLORREF selectSize(GISType* inShubetsu) const;

private:
	unsigned int			Count;
	PgnData*	Points;
	string		Filename;

};

#endif // !defined(AFX_RGISPGN_H__35C7BA52_A5C8_49E4_B6C6_1F83B770E616__INCLUDED_)
