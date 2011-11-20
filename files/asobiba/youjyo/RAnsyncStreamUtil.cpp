// RAnsyncStreamUtil.cpp: RAnsyncStreamUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RAnsyncStreamUtil.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RAnsyncStreamUtil::RAnsyncStreamUtil()
{
	Stream = NULL;
}

RAnsyncStreamUtil::~RAnsyncStreamUtil()
{

}

/**
 * WriteBuffer:	書き込み予約
 *
 * @param	inBuffer		書き込むバッファ
 * @param	inBufferSize	バッファサイズ
 * @throws	RException	書き込み中にエラーの場合.
 */
void RAnsyncStreamUtil::WriteBufferReservation(const char *inBuffer ,int inBufferSize) throw(RException)
{
	if (inBufferSize == 0) return ;

	ASSERT(Stream != NULL);
	ASSERT(inBuffer != NULL);
	ASSERT(inBufferSize > 0);

	Stream->Write(inBuffer ,inBufferSize );
}

/**
 * WriteBuffer:
 *
 * @param	inBuffer		書き込むバッファ
 * @return  実際に書き込めたサイズ
 * @throws	RException	書き込み中にエラーの場合.
 */
int RAnsyncStreamUtil::WriteBuffer(string inBuffer) throw(RException)
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
int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer) throw(RException)
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
int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer ,int inBufferSize) throw(RException)
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
		sended = Stream->WriteWaitForSignal();
		if ( sended == 0 )
		{//読み込み終了
			break;
		}
		else if (sended < 0)
		{//異常事態
			throw RException(EXCEPTIONTRACE + "書き込み時にエラーが発生しました");
		}
		//データ送信予約.
		Stream->Write(inBuffer+sendedLen ,inBufferSize - sendedLen);
	}
	return sended;
}

/**
 * ReadBuffer:	読み込み予約
 *
 * @param	outBuffer		読み込まれるバッファ.
 * @param	inBufferSize	バッファサイズ
 * @throws	RException	書き込み中にエラーの場合.
 */
void RAnsyncStreamUtil::ReadBufferReservation(char *outBuffer ,int inBufferSize) throw(RException)
{
	ASSERT(Stream != NULL);
	Stream->Read(outBuffer ,inBufferSize );
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
int RAnsyncStreamUtil::ReadBuffer(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException)
{
	ASSERT(Stream != NULL);
	ASSERT(outBuffer != NULL);
	ASSERT(inStreamChecker != NULL);

	int		sended;			//今、送信できたサイズ.
	int		sendedLen;		//現在までに送信したサイズ.

	for(sendedLen = 0 ; sendedLen < inBufferSize ; sendedLen += sended)
	{
		//データ送信.
		sended = Stream->ReadWaitForSignal();
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
			return sended;
		}
		//データ予約.
		Stream->Read(outBuffer+sendedLen ,inBufferSize - sendedLen);
	}
	return sended;
}

