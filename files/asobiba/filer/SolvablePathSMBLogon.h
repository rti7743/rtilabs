// SolvablePathSMBLogon.h: SolvablePathSMBLogon クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHSMBLOGON_H__F06BD4AA_6846_4ECE_A773_8228D51D704D__INCLUDED_)
#define AFX_SOLVABLEPATHSMBLOGON_H__F06BD4AA_6846_4ECE_A773_8228D51D704D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


#include "SolvablePath.h"
#include "RfilesSMBLogon.h"


class SolvablePathSMBLogon : public SolvablePath  
{
public:
	SolvablePathSMBLogon(SolvablePath* inNext = NULL) : SolvablePath(inNext)
	{
	}
	virtual ~SolvablePathSMBLogon()
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
		return __RFILETYPE_SMBLOGON_BIGICON;
	}
	//このディスプレイはサポートしてますか?
	virtual bool IsDisplaySupported(__DISPLAYTYPE inType) const
	{
		switch(inType)
		{
		case __DISPLAYTYPE_LISTVIEW_REPORT:			//詳細
		case __DISPLAYTYPE_LISTVIEW_BIGICON:		//大きなアイコン
			return true;
		}
		return false;
	}
	//最も推奨するディスプレイは、何ですか?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const
	{
		//return __DISPLAYTYPE_LISTVIEW_REPORT;
		return __DISPLAYTYPE_LISTVIEW_BIGICON;
	}
	//最も推奨するディスプレイでのカラムを教えてください
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const
	{
		static RFixedColumn fixedColumn[] = 
		{
			RFixedColumn(__FILECOLUMN_TYPE_ICON , "" , 0),
			RFixedColumn(__FILECOLUMN_TYPE_NAME , "ファイル名" , 0.50),
			RFixedColumn(__FILECOLUMN_TYPE_COMMENT , "コメント" , 0.50),
			RFixedColumn(__FILECOLUMN_TYPE_END , "" , 0),
		};
		return (const RFixedColumn*)fixedColumn;
	}
	//ファイル取得
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const throw()
	{
		switch(inType)
		{
		case __DISPLAYTYPE_LISTVIEW_REPORT:
			return new RfilesSMBLogon(__RFILETYPE_SMBLOGON_REPORT, getPath() );
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			return new RfilesSMBLogon(__RFILETYPE_SMBLOGON_BIGICON, getPath() );
		}
		throw RNotSupportException("指定されたディスプレイ %d は、サポート外です" , inType);
		return NULL;
	}
	//一つディレクトリを上がる
	virtual string UpDirectory() const ;

	static void Test();

};

#endif // !defined(AFX_SOLVABLEPATHSMBLOGON_H__F06BD4AA_6846_4ECE_A773_8228D51D704D__INCLUDED_)
