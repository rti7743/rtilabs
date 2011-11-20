// RGISNode.h: RGISNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISNODE_H__A2A529EC_D50B_424A_B918_28D37AC01D1A__INCLUDED_)
#define AFX_RGISNODE_H__A2A529EC_D50B_424A_B918_28D37AC01D1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RStdioFile.h"
#include "RGISSlp.h"
#include "RGISGyousei.h"

class RGISNodeIntArray
{
	unsigned int	Count;
	unsigned long*	Points;
public:
	RGISNodeIntArray();
	~RGISNodeIntArray();
	void Create(const char* inStr) throw (RException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	int getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return Points[p];
	}
	void Delete();



	static void test();

};



struct NodeData
{
	unsigned long			PT;
	//以下無視!
	bool					Use;
};


class RGISNode  
{
	unsigned int	Count;
	NodeData*	Points;
public:
	RGISNode();
	~RGISNode();
	void Create(const string & inFilename) throw (RException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const NodeData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Delete();

	//ファイルにノード情報を書き込む.
	static void WriteToFile(RStdioFile * ioFile ,const RGISNode * inNode , const RGISNodeIntArray* inNodeArray ,
		const UPOINT*  writePointArray ,int inPointCount , const RGISSlp* inSlp,const RGISGyousei* inGyousei) throw(RException);

	static void test();

};

#endif // !defined(AFX_RGISNODE_H__A2A529EC_D50B_424A_B918_28D37AC01D1A__INCLUDED_)
