// RfilesPreviewOne.h: RfilesPreviewOne クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESPREVIEWONE_H__9178FDED_DBB7_4EA4_B16A_42C810A47C10__INCLUDED_)
#define AFX_RFILESPREVIEWONE_H__9178FDED_DBB7_4EA4_B16A_42C810A47C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RTableOnlyOne.h"
#include "RfilePreviewOne.h"
#include "RfileFactory.h"

//プレビュー用の一つのファイルをあらわす.
class RfilesPreviewOne : public Rfiles  
{
private:
public:
	RfilesPreviewOne(string inPath);
	//final
	~RfilesPreviewOne();

	//一覧取得
	virtual void ls();
	//一覧取得
	virtual void ls(string inFilter);
	//削除
	virtual void rm(string inFilename);
	//リネーム
	virtual void rename(string inSrcFilename,string inDestFilename);
	//種類を返す.
	virtual __RFILETYPE		getRfileType()	const;
	//パスの取得
	virtual string getPath() const ;
	//パスの設定
	virtual void setPath(const string inPath)  ;

	//RTableを作成します。
	//ls を使う前に呼んでください。
	//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
	virtual void MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw();

	//テスト.
	static void test();
private:
	string		Path;
};

#endif // !defined(AFX_RFILESPREVIEWONE_H__9178FDED_DBB7_4EA4_B16A_42C810A47C10__INCLUDED_)
