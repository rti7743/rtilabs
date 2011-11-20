// Rfile.h: Rfile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILE_H__9B1C25E7_7C42_4D62_A184_723E88D7C983__INCLUDED_)
#define AFX_RFILE_H__9B1C25E7_7C42_4D62_A184_723E88D7C983__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Rfile;

#include "comm.h"


#include "RFixedColumn.h"
#include "RNotSupportException.h"
#include "RIOCannotWriteException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotCloseException.h"


enum __RFILETYPE
{
	__RFILETYPE_DEBUG,		//デバッグ
	__RFILETYPE_NONE_DUMMY,			//BLANKページ
	__RFILETYPE_LOCALFILE_REPORT,	//詳細表示
	__RFILETYPE_LOCALFILE_BIGICON,	//アイコン表示
	__RFILETYPE_PREVIEWONE,			//プレビューするためのファイル
	__RFILETYPE_ARC_BIGICON,		//圧縮ファイル名アイコン表示
	__RFILETYPE_SMBLOGON_REPORT,	//Windows共有のログイン部分
	__RFILETYPE_SMBLOGON_BIGICON,	//Windows共有のログイン部分
};

enum __RFILEOPENTYPE
{
	__RFILEOPENTYPE_READ,	//読み込み
	__RFILEOPENTYPE_WRITE,	//書き込み
};


class Rfile
{
public:
	Rfile()
	{
	}
	virtual ~Rfile()
	{
	}
public:

	//ソートキー
	const void*		getSortKey() const	{	return SortKey;	}

	//種類を返す.
	virtual __RFILETYPE	getType() const	= 0;

	//ディレクトリかどうか判断.
	virtual bool IsDirectory() const = 0;

	//値の取得.
	virtual const string getVars(__FILECOLUMN_TYPE inType) const = 0;
	//イメージの取得.
	virtual const int getImageIndex() const = 0;
	//ソートキーを登録します。 Attchの後に呼んでください.
	virtual void SortAttach(__FILECOLUMN_TYPE inKey) = 0;

	//オープン
	//@inPath	パス(Rfileはファイル名しか持っていない)
	//@inMode	開くモード
	//#開けなかった場合は RIOCannotOpenException を投げる
	// サポートされていない動作は RNotSupportException を投げる
	virtual void Open(string inPath ,__RFILEOPENTYPE inMode) throw()
	{
		throw RNotSupportException("オープンはサポートされていません");
	}

	//クローズ
	//#閉じれなかった場合は RIOCannotCloseException を投げる
	// サポートされていない動作は RNotSupportException を投げる
	virtual void Close() throw()
	{
		throw RNotSupportException("クローズはサポートされていません");
	}
	//リード
	//@outBuf		バッファ
	//@inBufferSize	バッファ長
	//#読み込み失敗は RIOCannotReadException を返す
	// サポートされていない動作は RNotSupportException を投げる
	virtual bool Read(char* outBuf,int inBufferSize) throw()
	{
		throw RNotSupportException("読み込みはサポートされていません");
		return false;
	}

	//ライト
	//@outBuf		バッファ
	//@inBufferSize	バッファ長
	//#書き込み失敗は RIOCannotReadException を返す
	// サポートされていない動作は RNotSupportException を投げる
	virtual bool Write(char* outBuf,int inBufferSize) throw()
	{
		throw RNotSupportException("書き込みはサポートされていません");
		return false;
	}

	//読み込み終了まで待つ.
	//#書き込み失敗は RIOException を返す
	virtual void WaitFor() throw()
	{
	}

	//終わったかどうか調べる
	//? true	終わったWaitFor() よんでちょ
	//? false	まだ
	//#書き込み失敗は RIOException を返す
	virtual bool Poll() throw()
	{
		return false ;
	}

	//処理の結果をもらいます。
	//Poll が true / または、WaitFor のリターン後に使ってください.
	//#書き込み失敗は RIOException を返す
	virtual unsigned long GetReport() throw()
	{
		return 0;
	}


protected:
	const void*		SortKey;	//ソートに使う要素
};


#endif // !defined(AFX_RFILE_H__9B1C25E7_7C42_4D62_A184_723E88D7C983__INCLUDED_)
