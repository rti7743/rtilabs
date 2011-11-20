// RGISSmallDouro.h: RGISSmallDouro クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLDOURO_H__0FCFAA24_B6E9_4E0F_A6E1_5A7D804A54F5__INCLUDED_)
#define AFX_RGISSMALLDOURO_H__0FCFAA24_B6E9_4E0F_A6E1_5A7D804A54F5__INCLUDED_

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
#include "RGISSmallNavigation.h"

struct SmallDouroDataOptimizMainData
{
	unsigned char			SB;
	unsigned char			FI;
	unsigned short			HR;
	unsigned short			TR;
	unsigned short			NameIndex;

	unsigned char			LoopCount;
};
struct SmallDouroDataOptimizUniqData
{
	unsigned char			NodeCount;
	const char*				Node;
	unsigned char			CVCount;
	const char*				CV;
};


class RGISSmallDouro  
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmallDouro();
	virtual ~RGISSmallDouro();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	void Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName);

	//ナビゲーションをする際の情報の収集.
//	void MakPathFinder(const LPOINT *inBlockStart , SmallPathfinderData * outPathFinder) const;

private:
	COLORREF selectColor(unsigned char inShubetsu) const;
	COLORREF selectSize(unsigned char inShubetsu) const;
	//データの読み込み.
	void readDataMain(RStdioMemory * ioFile, SmallDouroDataOptimizMainData * outData) const;
	//データの読み込み.
	void readDataUniq(RStdioMemory * ioFile, SmallDouroDataOptimizUniqData * outData) const;
	//接続情報の取得.
//	int FindConnect(SmallPathfinderData::SmallPathfinderDataOne::_Node::_Connect * outConnect , SPOINT * inPos , int inMyDouroNumber) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};
#endif // !defined(AFX_RGISSMALLDOURO_H__0FCFAA24_B6E9_4E0F_A6E1_5A7D804A54F5__INCLUDED_)
