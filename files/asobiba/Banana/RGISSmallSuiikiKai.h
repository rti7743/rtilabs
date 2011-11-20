// RGISSmallSuiikiKai.h: RGISSmallSuiikiKai クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLSUIIKIKAI_H__47989235_82A6_48DC_AB12_6B25278A57DC__INCLUDED_)
#define AFX_RGISSMALLSUIIKIKAI_H__47989235_82A6_48DC_AB12_6B25278A57DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "comm.h"
#include "RGISSmallType.h"
#include "RException.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISAllTest.h"

struct SmallSuiikiKaiDataOptimizMainData
{
	unsigned char			SR;

	unsigned char			LoopCount;
};
struct SmallSuiikiKaiDataOptimizUniqData
{
	unsigned char			CVCount;
	const char*				CV;
};

class RGISSmallSuiikiKai  
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmallSuiikiKai();
	virtual ~RGISSmallSuiikiKai();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//線/点の描画
	void Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
	//塗りつぶし
	void Draw2(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);

private:
	COLORREF selectColor(unsigned char inShubetsu) const;
	COLORREF selectSize(unsigned char inShubetsu) const;
	//データの読み込み.
	void readDataMain(RStdioMemory * ioFile, SmallSuiikiKaiDataOptimizMainData * outData) const;
	//データの読み込み.
	void readDataUniq(RStdioMemory * ioFile, SmallSuiikiKaiDataOptimizUniqData * outData) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLSUIIKIKAI_H__47989235_82A6_48DC_AB12_6B25278A57DC__INCLUDED_)
