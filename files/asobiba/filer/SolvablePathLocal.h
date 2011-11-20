// SolvablePathLocal.h: SolvablePathLocal クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHLOCAL_H__84065EEF_816C_43E7_A2FF_F94165E9A6E7__INCLUDED_)
#define AFX_SOLVABLEPATHLOCAL_H__84065EEF_816C_43E7_A2FF_F94165E9A6E7__INCLUDED_

class SolvablePathLocal;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


#include "SolvablePath.h"
#include "RfilesLocal.h"

#include "RfileLocalReport.h"
#include "RfileLocalBigIcon.h"

class SolvablePathLocal : public SolvablePath 
{
public:
	SolvablePathLocal(SolvablePath* inNext = NULL) : SolvablePath(inNext)
	{
	}
	virtual ~SolvablePathLocal()
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
			RFixedColumn(__FILECOLUMN_TYPE_EXT , "種類" , 0.15),
			RFixedColumn(__FILECOLUMN_TYPE_SIZE , "サイズ" , 0.15),
			RFixedColumn(__FILECOLUMN_TYPE_DATE , "更新時間" , 0.15),
			RFixedColumn(__FILECOLUMN_TYPE_ATTRIB , "属性",0.5),
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
			return new RfilesLocal(__RFILETYPE_LOCALFILE_REPORT, getPath() );
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			return new RfilesLocal(__RFILETYPE_LOCALFILE_BIGICON, getPath() );
		}
		throw RNotSupportException("指定されたディスプレイ %d は、サポート外です" , inType);
		return NULL;
	}
	//一つディレクトリを上がる
	virtual string UpDirectory() const ;

	static void Test();
};

#endif // !defined(AFX_SOLVABLEPATHLOCAL_H__84065EEF_816C_43E7_A2FF_F94165E9A6E7__INCLUDED_)
