// SolvableFileLZH.h: SolvableFileLZH クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEFILELZH_H__ACB3E559_51BE_4E63_B911_8E69235F8329__INCLUDED_)
#define AFX_SOLVABLEFILELZH_H__ACB3E559_51BE_4E63_B911_8E69235F8329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "SolvableFile.h"
#include "RfilesLocal.h"

#include "RfileLocalReport.h"
#include "RfileArcBigIcon.h"
#include "RfilesLzh.h"
#include "RfilesWithCache.h"


class SolvableFileLZH : public SolvableFile  
{
public:
	SolvableFileLZH()
	{
	}
	virtual ~SolvableFileLZH()
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
//		case __DISPLAYTYPE_LISTVIEW_REPORT:
//			return new RfilesLzh<RfileLzhReport>( getPath() );
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			{
				ArcCacheFileFnctional * acff = new ArcCacheFileFnctional;
				acff->ArchiveType = __ARCHIVETYPE_LZH;
				return new RfilesWithCache(acff, __RFILETYPE_ARC_BIGICON , getPath() );
			}
			//return new RfilesLzh( __ARCHIVETYPE_LZH , __RFILETYPE_ARC_BIGICON , getPath() );
		}
		throw RNotSupportException("指定されたディスプレイ %d は、サポート外です" , inType);
		return NULL;
	}

	static void Test();

};

#endif // !defined(AFX_SOLVABLEFILELZH_H__ACB3E559_51BE_4E63_B911_8E69235F8329__INCLUDED_)
