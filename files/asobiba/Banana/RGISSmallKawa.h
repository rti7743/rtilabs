// RGISSmallKawa.h: RGISSmallKawa クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLKAWA_H__583223F9_883E_4F62_BB4C_3DF0BAA0CA14__INCLUDED_)
#define AFX_RGISSMALLKAWA_H__583223F9_883E_4F62_BB4C_3DF0BAA0CA14__INCLUDED_

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



struct SmallKawaDataOptimizMainData
{
	unsigned char			SB;
	unsigned short			NameIndex;

	unsigned char			LoopCount;
};
struct SmallKawaDataOptimizUniqData
{
	unsigned char			CVCount;
	const char*				CV;
};

class RGISSmallKawa  
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmallKawa();
	virtual ~RGISSmallKawa();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//線/点の描画
	void Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
	//文字列の描画
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName);
private:
	COLORREF selectColor(unsigned char inShubetsu) const;
	COLORREF selectSize(unsigned char inShubetsu) const;
	//データの読み込み.
	void readDataMain(RStdioMemory * ioFile, SmallKawaDataOptimizMainData * outData) const;
	//データの読み込み.
	void readDataUniq(RStdioMemory * ioFile, SmallKawaDataOptimizUniqData * outData) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLKAWA_H__583223F9_883E_4F62_BB4C_3DF0BAA0CA14__INCLUDED_)
