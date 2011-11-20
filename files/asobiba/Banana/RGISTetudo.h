// RGISTetudo.h: RGISTetudo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISTETUDO_H__29FC0279_51F8_4943_BB6B_8897011DA632__INCLUDED_)
#define AFX_RGISTETUDO_H__29FC0279_51F8_4943_BB6B_8897011DA632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "GISType.h"
#include "RGISDraw.h"
#include "RGISSlp.h"
#include "RGISEki.h"
#include "RGISHashi.h"
#include "RGISTonneru.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"
#include "RGISFindHintEki.h"

struct TetudoData
{
	GISType		JT;
	GISType		SB;
	unsigned short			NameIndex;
	unsigned short			TR;
	unsigned short			HR;
	unsigned short			ER;
	GISINTArray	CV;
	//以下無視!
	bool					Use;
};

class RGISTetudo  
{
public:
	RGISTetudo();
	virtual ~RGISTetudo();

	void Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException);
	void Delete();
	void SmallPack(const string & inDir,const RGISSlp * inSlp,
		 const RGISNameDatabase* inNameData,const RGISEki * inEki,
		 RGISFindHintEki* ioFindHintEki,
		 const RGISHashi * inHashi,const RGISTonneru * inTonneru)  const throw(RException);
	JoinTempFormatScript getPackInfo() const;

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const TetudoData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void RGISTetudo::Draw(const RGISDraw * inDraw) const;

	static void test();
private:
	COLORREF selectColor(GISType* inShubetsu, int inER) const;
	COLORREF selectSize(GISType* inShubetsu, int inER) const;

private:
	unsigned int			Count;
	TetudoData*	Points;

};

#endif // !defined(AFX_RGISTETUDO_H__29FC0279_51F8_4943_BB6B_8897011DA632__INCLUDED_)
