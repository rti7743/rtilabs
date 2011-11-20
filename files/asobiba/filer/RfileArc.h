// RfileArc.h: RfileArc クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEARC_H__79D4F7B5_D1D5_4E43_BC90_18CA75D6CA68__INCLUDED_)
#define AFX_RFILEARC_H__79D4F7B5_D1D5_4E43_BC90_18CA75D6CA68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "Rfile.h"

class RfileArc : public Rfile  
{
public:
	RfileArc();
	virtual ~RfileArc();

	//登録します。
	virtual void Attach(const char* inPath,const char* inName,DWORD inSize,DWORD inDate,WORD inRatio ,const char* inMode,const char* inAttribute) = 0;

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
};

#endif // !defined(AFX_RFILEARC_H__79D4F7B5_D1D5_4E43_BC90_18CA75D6CA68__INCLUDED_)
