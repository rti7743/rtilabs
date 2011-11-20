// SolvablePathWeb.h: SolvablePathWeb クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATHWEB_H__4D592BAB_C5FF_4A2B_AA6E_E66368F35927__INCLUDED_)
#define AFX_SOLVABLEPATHWEB_H__4D592BAB_C5FF_4A2B_AA6E_E66368F35927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SolvablePath.h"

#include "comm.h"


#include "SolvablePath.h"
#include "RfilesLocal.h"

#include "RfilePreviewOne.h"
#include "RfilesPreviewOne.h"


class SolvablePathWeb : public SolvablePath  
{
public:
	SolvablePathWeb(SolvablePath* inNext = NULL) : SolvablePath(inNext)
	{
	}
	virtual ~SolvablePathWeb()
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
		return __RFILETYPE_PREVIEWONE;
	}
	//このディスプレイはサポートしてますか?
	virtual bool IsDisplaySupported(__DISPLAYTYPE inType) const
	{
		switch(inType)
		{
		case __DISPLAYTYPE_PREVIEW_WEBBROWSER:			//Web
			return true;
		}
		return false;
	}
	//最も推奨するディスプレイは、何ですか?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const
	{
		return __DISPLAYTYPE_PREVIEW_WEBBROWSER;
	}
	//最も推奨するディスプレイでのカラムを教えてください
	virtual const RFixedColumn* LetKnowColumnesInDisplayRecommendedMost() const
	{
		static RFixedColumn fixedColumn[] = 
		{
			RFixedColumn(__FILECOLUMN_TYPE_END , "" , 0),
		};
		return (const RFixedColumn*)fixedColumn;
	}
	//ファイル取得
	virtual Rfiles* RfilesFactory(__DISPLAYTYPE inType) const throw()
	{
		switch(inType)
		{
		case __DISPLAYTYPE_PREVIEW_WEBBROWSER:
			return new RfilesPreviewOne( getPath() );
		}
		throw RNotSupportException("指定されたディスプレイ %d は、サポート外です" , inType);
		return NULL;
	}
	//一つディレクトリを上がる
	virtual string UpDirectory() const ;

	static void Test();

};

#endif // !defined(AFX_SOLVABLEPATHWEB_H__4D592BAB_C5FF_4A2B_AA6E_E66368F35927__INCLUDED_)
