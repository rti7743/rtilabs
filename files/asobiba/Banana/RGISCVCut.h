// RGISCVCut.h: RGISCVCut クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISCVCUT_H__772EED5C_8D88_49CB_AEBC_FC86119F0352__INCLUDED_)
#define AFX_RGISCVCUT_H__772EED5C_8D88_49CB_AEBC_FC86119F0352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGIS.h"
#include "RGISSlp.h"
#include "RStdioFile.h"
#include "GISType.h"
#include "RGIS2500IntArray.h"
#include "RGISCVCutDataIml.h"
#include "RMmap.h"


class RGISCVCut  
{
	struct PointSet
	{
		UPOINT po;
		unsigned long lastBlock;
	};
public:
	RGISCVCut();
	virtual ~RGISCVCut();

	//書き込み
	void Write(const string & inDir , const RGISSlp * inSlp,
							const GISINTArray* inArray ,RGISCVCutDataIml * ioData);
	//書き込み
	void RGISCVCut::Write(const string & inDir , 
						  const RGIS2500PointArray* inArray ,RGISCVCutDataIml * ioData);
	//点の書き込み
	void Write(const string & inDir ,const UPOINT * inPoint,RGISCVCutDataIml * ioData);
	//ブロックに分断
	static unsigned long selectBlock(const UPOINT * inPT) ;

	static void RGISCVCut::test();
	static UPOINT* DebugRead(const string & inDir ,unsigned short inBlock ,const string & inData  );


	void RGISCVCut::Test2(const string & inDir ,unsigned long block , int inMeshKankaku);
private:
	void WriteMain(const string & inDir ,
		 PointSet* p ,unsigned int trueCVcount , RGISCVCutDataIml * ioData);
	//点の書き込み
	void PointWrite(RStdioFile *iofile , const UPOINT * inPtArray ,unsigned int inCount);
	//Src から Dest へ逝くときに境界線にぶつかるポイントの取得.
	UPOINT getMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const;
	//隣り合うブロックか?
	bool RGISCVCut::isTexanRude(unsigned long inA , unsigned long  inB) const;
	//Src から Dest へ逝くときに境界線の次にあるブロックの取得
	UPOINT RGISCVCut::getTexanRudeMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const;
	//-127 ～ 127 の範囲に収まるように補完.
	UPOINT RGISCVCut::LineHoseiChar(const UPOINT * inSrc , const UPOINT * inDest );
	//同じブロックの中で前後が char の範囲に収まるようにさらに分割
	void PointWrite256(RStdioFile *iofile , unsigned long inBlock , const UPOINT * inPtArray ,unsigned int inCount, RGISCVCutDataIml * ioData);
};

class RGISCVCutMesh
{
public:

	RGISCVCutMesh();
	~RGISCVCutMesh();

	//点の書き込み
	void Write(const string & inDir ,const UPOINT & inPoint,unsigned char inLevel);
private:
	RMmap				Mem;
	unsigned long		LastBlock;

};

#endif // !defined(AFX_RGISCVCUT_H__772EED5C_8D88_49CB_AEBC_FC86119F0352__INCLUDED_)
