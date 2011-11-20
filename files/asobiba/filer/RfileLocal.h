// RfileLocal1.h: RfileLocal クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILELOCAL1_H__E05349BE_7983_4346_8AB7_680D922C8F61__INCLUDED_)
#define AFX_RFILELOCAL1_H__E05349BE_7983_4346_8AB7_680D922C8F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RfileLocal;

#define FILE_EXIST(name)	(0xFFFFFFFF != GetFileAttributes(name))

#include "comm.h"
#include "Rfile.h"
#include "Rfiles.h"
//ローカルファイルの読み込み、書き込みの部分を担当します。
//これだけではまだ使えないので、
//さらに上位に継承して使う必要があります。
class RfileLocal : public Rfile  
{
public:
	RfileLocal();
	virtual ~RfileLocal();

	//登録します。
	virtual void Attach(const Rfiles* inRfiles ,const char* inName,__int64 inSize,__int64 inDate,DWORD inAttrib) = 0;

	//オープン
	//@inPath	パス(Rfileはファイル名しか持っていない)
	//@inMode	開くモード
	//#開けなかった場合は RIOCannotOpenException を投げる
	// サポートされていない動作は RNotSupportException を投げる
	virtual void Open(string inPath ,__RFILEOPENTYPE inMode) throw();

	//クローズ
	//#閉じれなかった場合は RIOCannotCloseException を投げる
	// サポートされていない動作は RNotSupportException を投げる
	virtual void Close() throw();

	//リード
	//@outBuf		バッファ
	//@inBufferSize	バッファ長
	//#読み込み失敗は RIOCannotReadException を返す
	// サポートされていない動作は RNotSupportException を投げる
	virtual bool Read(char* outBuf,int inBufferSize) throw();

	//ライト
	//@outBuf		バッファ
	//@inBufferSize	バッファ長
	//?	>= 0 書き込んだバイト数
	//  <  0 Eof
	//#書き込み失敗は RIOCannotReadException を返す
	// サポートされていない動作は RNotSupportException を投げる
	virtual bool Write(char* outBuf,int inBufferSize) throw();

	//読み込み終了まで待つ.
	//#書き込み失敗は RIOException を返す
	virtual void WaitFor() throw();

	//終わったかどうか調べる
	//? true	終わったWaitFor() よんでちょ
	//? false	まだ
	//#書き込み失敗は RIOException を返す
	virtual bool Poll() throw();

	//処理の結果をもらいます。
	//Poll が true / または、WaitFor のリターン後に使ってください.
	//#書き込み失敗は RIOException を返す
	virtual unsigned long GetReport() throw();

private:
	HANDLE		Handle;
	OVERLAPPED	OverLapped;

};

#endif // !defined(AFX_RFILELOCAL1_H__E05349BE_7983_4346_8AB7_680D922C8F61__INCLUDED_)
