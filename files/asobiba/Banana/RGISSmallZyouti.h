// RGISSmallZyouti.h: RGISSmallZyouti クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLZYOUTI_H__44B4382C_95D4_4549_8F9E_26AA5AF8B9ED__INCLUDED_)
#define AFX_RGISSMALLZYOUTI_H__44B4382C_95D4_4549_8F9E_26AA5AF8B9ED__INCLUDED_

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

struct SmallZyoutiDataOptimizMainData
{
	unsigned short			NameIndex;
	unsigned char			LoopCount;
};
struct SmallZyoutiDataOptimizUniqData
{
	unsigned char			CVCount;
	const char*				CV;
};


class RGISSmallZyouti  
{
public:
	RGISSmallZyouti();
	virtual ~RGISSmallZyouti();

	void Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException);
	//線/点の描画
	void Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw);
	//文字列の描画
	void Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName);

	//駅検索.
	void FindEki(const LPOINT *inBlockStart ,FindPool * outFindData , 
		const string &inFindName , unsigned short inFindNameHash, const RGISSmallNameDatabase * inNameDB ) const;

private:
	//データの読み込み.
	void readDataMain(RStdioMemory * ioFile, SmallZyoutiDataOptimizMainData * outData) const;
	//データの読み込み.
	void readDataUniq(RStdioMemory * ioFile, SmallZyoutiDataOptimizUniqData * outData) const;

private:
	const char *	Data;
	int				DataSize;
	int				DataCount;
};

#endif // !defined(AFX_RGISSMALLZYOUTI_H__44B4382C_95D4_4549_8F9E_26AA5AF8B9ED__INCLUDED_)
