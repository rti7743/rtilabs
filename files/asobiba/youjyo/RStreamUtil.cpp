// RStreamUtil.cpp: RStreamUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RStreamUtil.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


/**
 * WriteBuffer:
 *
 * @param	inBuffer		書き込むバッファ
 * @return  実際に書き込めたサイズ
 * @throws	RException	書き込み中にエラーの場合.
 */
int RStreamUtil::WriteBuffer(string inBuffer) throw(RException)
{
	return this->WriteBuffer(inBuffer.c_str() , inBuffer.size() );
}

/**
 * WriteBuffer:
 *
 * @param	inBuffer		書き込むバッファ
 * @return  実際に書き込めたサイズ
 * @throws	RException	書き込み中にエラーの場合.
 */
int RStreamUtil::WriteBuffer(const char *inBuffer) throw(RException)
{
	ASSERT(Stream != NULL);
	ASSERT(inBuffer != NULL);

	return this->WriteBuffer(inBuffer , strlen(inBuffer) );
}

/**
 * WriteBuffer:	書き込み
 *
 * @param	inBuffer		書き込むバッファ
 * @param	inBufferSize	バッファサイズ
 * @return  実際に書き込めたサイズ
 * @throws	RException	書き込み中にエラーの場合.
 */
int RStreamUtil::WriteBuffer(const char *inBuffer ,int inBufferSize) throw(RException)
{
	if (inBufferSize == 0) return 0;

	ASSERT(Stream != NULL);
	ASSERT(inBuffer != NULL);
	ASSERT(inBufferSize > 0);

	int		sended;			//今、送信できたサイズ.
	int		sendedLen;		//現在までに送信したサイズ.

	for(sendedLen = 0 ; sendedLen < inBufferSize ; sendedLen += sended)
	{
		//データ送信.
		sended = Stream->Write(inBuffer+sendedLen ,inBufferSize - sendedLen);
		if ( sended == 0 )
		{//読み込み終了
			break;
		}
		else if (sended < 0)
		{//異常事態
			throw RException(EXCEPTIONTRACE + "書き込み時にエラーが発生しました");
		}
	}
	return sended;
}


/**
 * ReadBuffer:	読み込み
 *
 * @param	outBuffer		読み込まれるバッファ.
 * @param	inBufferSize	バッファサイズ
 * @param	inStreamChecker	読み込んだバッファを調査するクラス.
 * @return  読み込んだサイズ
 * @throws	RException	書き込み中にエラーの場合.
 */
int RStreamUtil::ReadBuffer(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException)
{
	/*ChangeLog.txt:2003/01/25	読み込んだ合計サイズではなくて、一回に読めこめたサイズを返していたので修正。 よく動いていたな(w
	*/
	ASSERT(Stream != NULL);
	ASSERT(outBuffer != NULL);
	ASSERT(inStreamChecker != NULL);

	int		sended;			//今、送信できたサイズ.
	int		sendedLen;		//現在までに送信したサイズ.

	for(sendedLen = 0 ; sendedLen < inBufferSize ; sendedLen += sended)
	{
		//データ送信.
		sended = Stream->Read(outBuffer+sendedLen ,inBufferSize - sendedLen);
		if ( sended == 0 )
		{//読み込み終了
			break;
		}
		else if (sended < 0)
		{//異常事態
			throw RException(EXCEPTIONTRACE + "読み込み時にエラーが発生しました");
		}
		//今読み込んだ中に終了条件があるかチェック.
		if ( ! inStreamChecker->Check(outBuffer , sendedLen , sendedLen+sended) )
		{	//チェッカーによる停止.
			return sendedLen + sended;
		}
		
	}
	return sendedLen;
}

