// RfileArc.cpp: RfileArc クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileArc.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RfileArc::RfileArc()
{

}

RfileArc::~RfileArc()
{

}

//オープン
//@inPath	パス(Rfileはファイル名しか持っていない)
//@inMode	開くモード
//#開けなかった場合は RIOCannotOpenException を投げる
// サポートされていない動作は RNotSupportException を投げる
void RfileArc::Open(string inPath ,__RFILEOPENTYPE inMode) throw()
{
}

//クローズ
//#閉じれなかった場合は RIOCannotCloseException を投げる
// サポートされていない動作は RNotSupportException を投げる
void RfileArc::Close() throw()
{
}

//リード
//@outBuf		バッファ
//@inBufferSize	バッファ長
//#読み込み失敗は RIOCannotReadException を返す
// サポートされていない動作は RNotSupportException を投げる
bool RfileArc::Read(char* outBuf,int inBufferSize) throw()
{
	return true;
}

//ライト
//@outBuf		バッファ
//@inBufferSize	バッファ長
//?	>= 0 書き込んだバイト数
//  <  0 Eof
//#書き込み失敗は RIOCannotReadException を返す
// サポートされていない動作は RNotSupportException を投げる
bool RfileArc::Write(char* outBuf,int inBufferSize) throw()
{
	return true;
}

//読み込み終了まで待つ.
//#書き込み失敗は RIOException を返す
void RfileArc::WaitFor() throw()
{
}

//終わったかどうか調べる
//? true	終わったWaitFor() よんでちょ
//? false	まだ
//#書き込み失敗は RIOException を返す
bool RfileArc::Poll() throw()
{
	return false;
}


//処理の結果をもらいます。
//Poll が true / または、WaitFor のリターン後に使ってください.
//#書き込み失敗は RIOException を返す
unsigned long RfileArc::GetReport() throw()
{
	return 0;
}
