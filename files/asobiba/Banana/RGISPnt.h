// RGISPnt.h: RGISPnt クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISPNT_H__C344899C_44FC_4F59_A4E5_D05594FA3D73__INCLUDED_)
#define AFX_RGISPNT_H__C344899C_44FC_4F59_A4E5_D05594FA3D73__INCLUDED_

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

struct PntData
{
	int	Type;						//図式分類コード
	int No;							//個別番号
	LPOINT	Point;					//点
};

class RGISPnt  
{
public:
	RGISPnt();
	virtual ~RGISPnt();

	void Create(const string & inFilename, int inScale) throw(RException);
	void Delete();

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const PntData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}

	void SmallPack(const RGISArc * inArc ,const RGISAtr * inAtr , 
		const string & inPath  ,const string & inMark,RGISNameDatabase* ioNameDatabase,
		RGIS2500InfoVector* gis2500infoVector);

	void SmallPackNoWrite(const RGISArc * inArc , const RGISAtr * inAtr , RGIS2500InfoVector* gis2500infoVector);

	static void test();
private:

private:
	unsigned int			Count;
	PntData*	Points;

};

#endif // !defined(AFX_RGISPNT_H__C344899C_44FC_4F59_A4E5_D05594FA3D73__INCLUDED_)
