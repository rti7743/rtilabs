// RGISSmallTetudo.h: RGISSmallTetudo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLTETUDO_H__B4B37DDA_3CDD_41F3_BB57_5CE10982A7AF__INCLUDED_)
#define AFX_RGISSMALLTETUDO_H__B4B37DDA_3CDD_41F3_BB57_5CE10982A7AF__INCLUDED_

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
#include "RGISSmallFindObject.h"

struct SmallTetudoDataOptimizMainData
{
	unsigned char			SB;
//	unsigned short			HR;
//	unsigned short			TR;

	unsigned short			NameIndex;
	unsigned short			EkiNameIndex;
	unsigned char			LoopCount;
};
struct SmallTetudoDataOptimizUniqData
{
	unsigned char			CVCount;
	const char*				CV;
};


class RGISSmallTetudo  
{
public:
	RGISSmallTetudo();
	virtual ~RGISSmallTetudo();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//線/点の描画
	void Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
	//文字列の描画
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName);

	//駅検索.
	void FindEki(const LPOINT *inBlockStart ,FindPool * outFindData , 
		const string &inFindName , unsigned short inFindNameHash, const RGISSmallNameDatabase * inNameDB ) const;

private:
	COLORREF16 selectColor(unsigned char inShubetsu, unsigned short inER) const;
	COLORREF selectSize(unsigned char inShubetsu, unsigned short inER) const;
	//データの読み込み.
	void RGISSmallTetudo::readDataMain(RStdioMemory * ioFile, SmallTetudoDataOptimizMainData * outData) const;
	//データの読み込み.
	void RGISSmallTetudo::readDataUniq(RStdioMemory * ioFile, SmallTetudoDataOptimizUniqData * outData) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};
#endif // !defined(AFX_RGISSMALLTETUDO_H__B4B37DDA_3CDD_41F3_BB57_5CE10982A7AF__INCLUDED_)
