// RGISSmallType.h: RGISSmallType クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLTYPE_H__BE327620_4F39_42AC_98C5_9C4AB2653FEE__INCLUDED_)
#define AFX_RGISSMALLTYPE_H__BE327620_4F39_42AC_98C5_9C4AB2653FEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGIS.h"
#include "RStdioMemory.h"
#include "RGISAllTest.h"





//ノードの管理
class RGISSmallNode
{
	unsigned char	Count;
	unsigned char*	Points;
public:
//	RGISSmallNode();
	~RGISSmallNode();

	void Create(RStdioMemory * inMemory , unsigned long inBlock) ;

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const unsigned char getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}
	void Delete();

	static void test();

	static UPOINT BlockToPoint(unsigned short inX , unsigned short inY, unsigned long inBlock);
private:

};

//複数点の管理
class RGISSmallType  
{
	unsigned char	Count;
	LPOINT*	Points;
public:
//	RGISSmallType();
	~RGISSmallType();

	void Create(RStdioMemory * inMemory , unsigned long inBlock) ;

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const LPOINT* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Delete();

	static void test();

	static LPOINT BlockToPoint(unsigned short inX , unsigned short inY, unsigned long inBlock);
	static LPOINT AvgCV(const LPOINT * inBlockStart , const char* inCVData , int inCVCount);
	//ブロックに分断
	static unsigned long selectBlock(const LPOINT * inPT) ;
private:

};

//単一点の管理.
class RGISSmallPoint
{
	LPOINT	Point;
public:
//	RGISSmallPoint();
	~RGISSmallPoint();

	void Create(RStdioMemory * inMemory , unsigned long inBlock) ;

	//地点の取得
	const LPOINT* getPoint() const
	{
		return &Point;
	}

	static void test();
private:
	LPOINT BlockToPoint(unsigned short inX , unsigned short inY, unsigned long inBlock) const;

};

#endif // !defined(AFX_RGISSMALLTYPE_H__BE327620_4F39_42AC_98C5_9C4AB2653FEE__INCLUDED_)
