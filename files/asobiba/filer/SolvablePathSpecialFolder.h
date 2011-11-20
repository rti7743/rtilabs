// SolvablePathSpecialFolder.h: SolvablePathSpecialFolder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHSPECIALFOLDER_H__D0894669_5320_4C1C_B159_C920352E521B__INCLUDED_)
#define AFX_SOLVABLEPATHSPECIALFOLDER_H__D0894669_5320_4C1C_B159_C920352E521B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SolvablePath.h"
#include "RfilesSMBLogon.h"
#include <ShlObj.h>

#include "RfileNetResourceBigIcon.h"

class SolvablePathSpecialFolder : public SolvablePath  
{
public:
	SolvablePathSpecialFolder();
	virtual ~SolvablePathSpecialFolder();

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
			RFixedColumn(__FILECOLUMN_TYPE_NAME , "名前", 0.30),
			RFixedColumn(__FILECOLUMN_TYPE_EXT , "種類" , 0.15),
			RFixedColumn(__FILECOLUMN_TYPE_SIZE , "合計サイズ",0.15),
			RFixedColumn(__FILECOLUMN_TYPE_FREE , "空き領域",0.15),
			RFixedColumn(__FILECOLUMN_TYPE_COMMENT , "コメント",0.10),
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
			return new RfilesSMBLogon( __RFILETYPE_LOCALFILE_REPORT, getPath() );
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			return new RfilesSMBLogon( __RFILETYPE_LOCALFILE_BIGICON, getPath() );
		}
		throw RNotSupportException("指定されたディスプレイ %d は、サポート外です" , inType);
		return NULL;
	}
	//一つディレクトリを上がる
	virtual string UpDirectory() const ;

	//スペシャルフォルダの名前の取得
	static string getSpecialFolder( int inFolder );
	static string TranslationFileName(LPITEMIDLIST p_pIDlist, LPSTRRET p_pStrret);

	static void Test();
};

#endif // !defined(AFX_SOLVABLEPATHSPECIALFOLDER_H__D0894669_5320_4C1C_B159_C920352E521B__INCLUDED_)
