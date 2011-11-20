// RfilesWithCache.h: RfilesWithCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESWITHCACHE_H__AB694DB9_2AA9_48F3_AD1A_05CF41B92E00__INCLUDED_)
#define AFX_RFILESWITHCACHE_H__AB694DB9_2AA9_48F3_AD1A_05CF41B92E00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RFiles.h"
#include "RCacheFS.h"

//テスト.
#include "RfilesLzh.h"

class RfilesWithCache : public Rfiles  
{
private:
	//よべませんよ
	RfilesWithCache(){}
public:
	RfilesWithCache(CacheFileFnctional* inCff , __RFILETYPE inRfileType , const string inPath) throw();

	virtual ~RfilesWithCache();


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
	//種類を返す.
	virtual string		getPath()	const;
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
	__RFILETYPE			RfileType;
	//パス.
	string				Path;
	//ファンクタ
	CacheFileFnctional*	Cff;
	//ソースパス(内部的に保持している Rfiles上でのパス)
	string				SrcPath;
	//内部的に保持しているオリジナルの Rfiles*
	Rfiles*				OrignalFiles;
	//内部的に保持している Rfiles に対しても MakeTable してあげなくてはいけない場合に
	//trueになるフラグです。
	bool				MostMakeRTable;

};

#endif // !defined(AFX_RFILESWITHCACHE_H__AB694DB9_2AA9_48F3_AD1A_05CF41B92E00__INCLUDED_)
