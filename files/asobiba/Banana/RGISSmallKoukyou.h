// RGISSmallKoukyou.h: RGISSmallKoukyou クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLKOUKYOU_H__9DF3CFF5_9F21_4E3D_BC10_A0B29587CEBF__INCLUDED_)
#define AFX_RGISSMALLKOUKYOU_H__9DF3CFF5_9F21_4E3D_BC10_A0B29587CEBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"
#include "RGISSmallNameDatabase.h"

struct SmallKoukyouDataOptimizMainData
{
	unsigned char			SR;
	unsigned char			LoopCount;
};
struct SmallKoukyouDataOptimizUniqData
{
	unsigned short			NameIndex;

	SPOINT					xy;
};


class RGISSmallKoukyou  
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmallKoukyou();
	virtual ~RGISSmallKoukyou();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName);

private:
	COLORREF selectColor(unsigned char inShubetsu) const;
	int RGISSmallKoukyou::selectIcon(unsigned char inShubetsu) const;

	//データの読み込み.
	void RGISSmallKoukyou::readDataMain(RStdioMemory * ioFile, SmallKoukyouDataOptimizMainData * outData) const;
	//データの読み込み.
	void RGISSmallKoukyou::readDataUniq(RStdioMemory * ioFile, SmallKoukyouDataOptimizUniqData * outData) const;


private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLKOUKYOU_H__9DF3CFF5_9F21_4E3D_BC10_A0B29587CEBF__INCLUDED_)
