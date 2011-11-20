// SolvableFileText.h: SolvableFileText クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEFILETEXT_H__1C9BA1B0_23BB_4F4A_A60A_291DCE58D145__INCLUDED_)
#define AFX_SOLVABLEFILETEXT_H__1C9BA1B0_23BB_4F4A_A60A_291DCE58D145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "SolvableFile.h"
#include "RfilesLocal.h"

#include "RfileLocalReport.h"
#include "RfilesPreviewOne.h"

class SolvableFileText : public SolvableFile  
{
public:
	SolvableFileText()
	{
	}
	virtual ~SolvableFileText()
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
		case __DISPLAYTYPE_PREVIEW_TEXT:			//テキストビュー
			return true;
		}
		return false;
	}
	//最も推奨するディスプレイは、何ですか?
	virtual __DISPLAYTYPE WhatIsDisplayeRecommendedMost() const
	{
		return __DISPLAYTYPE_PREVIEW_TEXT;
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
		case __DISPLAYTYPE_PREVIEW_TEXT:
			return new RfilesPreviewOne( getPath() );
		}
		throw RNotSupportException("指定されたディスプレイ %d は、サポート外です" , inType);
		return NULL;
	}


	static void Test();
};

#endif // !defined(AFX_SOLVABLEFILETEXT_H__1C9BA1B0_23BB_4F4A_A60A_291DCE58D145__INCLUDED_)
