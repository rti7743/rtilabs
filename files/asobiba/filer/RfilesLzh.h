// RfilesLzh.h: RfilesLzh クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILESLZH_H__8EA6180D_F7EB_4519_99FE_6D12ED5C7D16__INCLUDED_)
#define AFX_RFILESLZH_H__8EA6180D_F7EB_4519_99FE_6D12ED5C7D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"

#include "Rfiles.h"
#include "RNotFoundException.h"
#include "DisplayDebugPrintf.h"
#include "RTableMuchDuplication.h"
#include "RTableOnlyOne.h"
#include "RfileArcBigIcon.h"
#include "RArchiveLha.h"
#include "RArchiveFactory.h"
#include "RfileFactory.h"
#include "RTableEasy.h"
#include "WindowsMain.h"
#include "VirtualPath.h"


//LZHファイル
class RfilesLzh : public Rfiles  
{
public:

	friend class ArcCacheFileFnctional;

	//サポートしていないやつは RNotSupportException
	//!初期化できなかったら RDoNotInitaliseException
	//!関数が読み込めなかったら RDoNotInitaliseException
	RfilesLzh(__ARCHIVETYPE inArchiveType,__RFILETYPE inRfileType,const string inPath) throw();
	//final
	~RfilesLzh();

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
	//RTableを作成します。
	//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
	RTable* CreateTable(__FILECOLUMN_TYPE inKey,bool IsAscending) const throw() ;

private:
	//Rtable で保持している Rfile のタイプ
	__RFILETYPE		RfileType;
	//パス.
	string			Path;
	//圧縮解答エンジン.
	RArchive*		ArchiveEngine;
};

//RfilesWithCache で使用するファンクタ
//RfilesLzhに対する friend class
class ArcCacheFileFnctional : public CacheFileFnctional
{
public:

	//Rfile の作成.
	virtual Rfiles* New(__RFILETYPE inRfileType,const string inSrcPath) const throw()
	{
		return new RfilesLzh(ArchiveType , inRfileType , inSrcPath );
	}

	//ソースパスの取得
	//c:\test3\test.zip::/新しいフォルダ/  ---> c:\test3\test.zip
	virtual string getSrcPath(const string inPath) const
	{
		VirtualPath vp(inPath);
		return vp.getNativePath();
	}

	//相対パスの取得
	//c:\test3\test.zip::\新しいフォルダ\  ---> 新しいフォルダ\  
	virtual string getRelativePath(const string inPath) const
	{
		VirtualPath vp(inPath);
		return vp.getLastPath();
	}

	//この inTargetFile を追加して・∀・）ｲｲ!!ですか?
	virtual bool IsAppend(const string inSrcPath , const Rfile* inTargetFile) const
	{
		const string name = inTargetFile->getVars(__FILECOLUMN_TYPE_NAME);
		const int hitPos = name.find(inSrcPath);
		return (hitPos < name.length() );
	}

	//RTable の作成.
	virtual RTable* ProxyCreateTable( const Rfiles* inSrcFiles,__FILECOLUMN_TYPE inKey,bool IsAscending) const
	{
		return ((RfilesLzh*)inSrcFiles)->CreateTable(inKey,IsAscending);
	}

	//アーカイヴァタイプ.
	__ARCHIVETYPE	ArchiveType;
};


#endif // !defined(AFX_RFILESLZH_H__8EA6180D_F7EB_4519_99FE_6D12ED5C7D16__INCLUDED_)
