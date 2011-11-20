// RAnsyncStreamUtil.h: RAnsyncStreamUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANSYNCSTREAMUTIL_H__F2A1A38C_F979_4623_8D2B_11585D3EF7A9__INCLUDED_)
#define AFX_RANSYNCSTREAMUTIL_H__F2A1A38C_F979_4623_8D2B_11585D3EF7A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RStreamAsync.h"
#include "RStreamUtil.h"

class RAnsyncStreamUtil  
{
public:
	RAnsyncStreamUtil();
	virtual ~RAnsyncStreamUtil();
	/**
	 * Mount:		処理対象となるストリームをマウント.
	 *
	 * @param ioStream	処理対象となるストリーム
	 * @return void 
	 */
	void Mount(RStreamAsync *ioStream)
	{
		Stream = ioStream;
	}
	/**
	 * WriteBuffer:	書き込み予約
	 *
	 * @param	inBuffer		書き込むバッファ
	 * @param	inBufferSize	バッファサイズ
	 * @return  実際に書き込めたサイズ
	 * @throws	RException	書き込み中にエラーの場合.
	 */
	void RAnsyncStreamUtil::WriteBufferReservation(const char *inBuffer ,int inBufferSize) throw(RException);

	/**
	 * WriteBuffer:
	 *
	 * @param	inBuffer		書き込むバッファ
	 * @return  実際に書き込めたサイズ
	 * @throws	RException	書き込み中にエラーの場合.
	 */
	int RAnsyncStreamUtil::WriteBuffer(string inBuffer) throw(RException);

	/**
	 * WriteBuffer:
	 *
	 * @param	inBuffer		書き込むバッファ
	 * @return  実際に書き込めたサイズ
	 * @throws	RException	書き込み中にエラーの場合.
	 */
	int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer) throw(RException);

	/**
	 * WriteBuffer:	書き込み
	 *
	 * @param	inBuffer		書き込むバッファ
	 * @param	inBufferSize	バッファサイズ
	 * @return  実際に書き込めたサイズ
	 * @throws	RException	書き込み中にエラーの場合.
	 */
	int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer ,int inBufferSize) throw(RException);

	/**
	 * ReadBuffer:	読み込み予約
	 *
	 * @param	outBuffer		読み込まれるバッファ.
	 * @param	inBufferSize	バッファサイズ
	 * @return  読み込んだサイズ
	 * @throws	RException	書き込み中にエラーの場合.
	 */
	void RAnsyncStreamUtil::ReadBufferReservation(char *outBuffer ,int inBufferSize) throw(RException);


	/**
	 * ReadBuffer:	読み込み
	 *
	 * @param	outBuffer		読み込まれるバッファ.
	 * @param	inBufferSize	バッファサイズ
	 * @param	inStreamChecker	読み込んだバッファを調査するクラス.
	 * @return  読み込んだサイズ
	 * @throws	RException	書き込み中にエラーの場合.
	 */
	int RAnsyncStreamUtil::ReadBuffer(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException);


private:
	RStreamAsync	* Stream;

};

#endif // !defined(AFX_RANSYNCSTREAMUTIL_H__F2A1A38C_F979_4623_8D2B_11585D3EF7A9__INCLUDED_)
