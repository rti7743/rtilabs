// SolvablePathDebug.h: SolvablePathDebug クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHDEBUG_H__D93B10A7_75F2_4B8D_932B_088989A5A41A__INCLUDED_)
#define AFX_SOLVABLEPATHDEBUG_H__D93B10A7_75F2_4B8D_932B_088989A5A41A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SolvablePathDebug;

#include "comm.h"


#include "SolvablePath.h"

class SolvablePathDebug : public SolvablePath  
{
public:
	SolvablePathDebug(SolvablePath* inNext = NULL) : SolvablePath(inNext)
	{
	}
	virtual ~SolvablePathDebug()
	{
	}

	//そのパスを解決してみる
	//解決できた場合は解決した結果のパスは メンバに setPath でセットすること.
	//www.yahoo.co.jp ---> http://www.yahoo.co.jp/ などとなる
	//解決できた場合は true 解決できなかった場合は false
	virtual bool DoSolvable(string inPath) ;
	//種類を返す.
	virtual __RFILETYPE getType() const
	{
		return __RFILETYPE_LOCALFILE_REPORT;
	}

	//このディスプレイはサポートしてますか?
	virtual bool IsDisplaySupported(__DISPLAYTYPE inType) const
	{
		switch(inType)
		{
		case __DISPLAYTYPE_DEBUG:
			return true;
		}
		return false;
	}
	//最も推奨するディスプレイは、何ですか?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const
	{
		return __DISPLAYTYPE_DEBUG;
	}
	//最も推奨するディスプレイでのカラムを教えてください
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const
	{
		static RFixedColumn fixedColumn[] = 
		{
			RFixedColumn(__FILECOLUMN_TYPE_NAME , "ファイル名" , 0),
			RFixedColumn(__FILECOLUMN_TYPE_EXT , "種類", 0),
			RFixedColumn(__FILECOLUMN_TYPE_END , "", 0),
		};
		return (const RFixedColumn*)fixedColumn;
	}
	//対応する Rfiles の作成.
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const
	{
		return NULL;
	}
	//一つディレクトリを上がる
	virtual string UpDirectory() const 
	{
		return "";
	}


};

#endif // !defined(AFX_SOLVABLEPATHDEBUG_H__D93B10A7_75F2_4B8D_932B_088989A5A41A__INCLUDED_)
