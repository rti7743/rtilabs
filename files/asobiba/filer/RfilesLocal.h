// RfilesLocal.h: RfilesLocal クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESLOCAL_H__C9F9AAF3_2667_4543_BB6A_C3B61BFE0033__INCLUDED_)
#define AFX_RFILESLOCAL_H__C9F9AAF3_2667_4543_BB6A_C3B61BFE0033__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RNotSupportException.h"
#include "RTableEasy.h"

#include "RfileFactory.h"
#include "RfileLocalBigIcon.h"
#include "RfileLocalReport.h"


//ローカルディスクファイル一覧の取得.
//final
class RfilesLocal : public Rfiles  
{
private:
	//よべませんよ
	RfilesLocal() {}
public:
	//初期化かできなかった場合は RNotSupportException を投げる.
	RfilesLocal(__RFILETYPE inRfileType,const string inPath) ;
	//final
	~RfilesLocal();

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
	//Rtable で保持している Rfile のタイプ
	__RFILETYPE		RfileType;
	//パス.
	string			Path;

};

#endif // !defined(AFX_RFILESLOCAL_H__C9F9AAF3_2667_4543_BB6A_C3B61BFE0033__INCLUDED_)
