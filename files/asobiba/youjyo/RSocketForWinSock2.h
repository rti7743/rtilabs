// RSocketForWinSock2.h: RSocketForWinSock2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSOCKETFORWINSOCK2_H__C22160FA_0325_4A87_AFEA_273CA5F0D002__INCLUDED_)
#define AFX_RSOCKETFORWINSOCK2_H__C22160FA_0325_4A87_AFEA_273CA5F0D002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotCloseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"
#include "RWin32Exception.h"

const int PADDED_ADDR_SIZE = sizeof(SOCKADDR_IN)+16;

class AsyncParam
{
public:
	AsyncParam()
	{
		Overlapped.Offset = 0;
		Overlapped.OffsetHigh = 0;
		Overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~AsyncParam()
	{
		CloseHandle(Overlapped.hEvent);
	}


	WSABUF			Buffer;
	WSAOVERLAPPED	Overlapped;
	DWORD			Size;
	DWORD			Flag;
};


class RSocketForWinSock2  
{
public:
	/**
	 * RSocket:		コンストラクタ
	 *				とりあえず初期化します。
	 */
	RSocketForWinSock2();
	/**
	 * RSocket:		コンストラクタ
	 *				指定されたソケットで初期化します。
	 *
	 * @param inNewSocket	既につながっているソケット.
	 * @return  
	 */
	RSocketForWinSock2(SOCKET inNewSocket);
	/**
	 * ~RSocket:	デストラクタ
	 *				
	 */
	virtual ~RSocketForWinSock2();

	/**
	 * Open:			ソケットを作成する.
	 *
	 * @param type		ソケットの種類
	 *						SOCK_STREAM : TCPで使用する
	 *						SOCK_DGRAM : UDPで使用する
	 * @param protocol	使用するプロトコル
	 * @throws			RIOCannotOpenException		開けなかったとき
	 */
	void Open( int type, int protocol ) throw(RIOCannotOpenException);
	/**
	 * Connect:				接続
	 *
	 * @param inHost		ホスト名
	 * @param inPort		ポート
	 * @throws				RIOException			接続できなかったとき
	 */
	void Connect(string inHost , int inPort)	throw(RIOException);
	/**
	 * Connect:			接続
	 *
	 * @param inSai		接続情報
	 * @throws			RIOException			接続できなかったとき
	 */
	void Connect(const SOCKADDR_IN * inSai) throw(RIOException);
	/**
	 * Bind:			bind(簡易版)
	 *
	 * @param inPort	bindするポート
	 * @throws			RIOException		bind失敗
	 */
	void Bind(int inPort) throw(RIOException);
	/**
	 * Bind:			bind
	 *
	 * @param inSai		bind情報
	 * @throws			RIOException		bind失敗
	 */
	void Bind(const SOCKADDR_IN * inSai) throw(RIOException);
	/**
	 * Listen:					listen
	 *
	 * @param inBacklog			接続要求ログを保存する（キューイングする）最大数
	 * @throws	RIOException	listen失敗	
	 */
	void Listen(int inBacklog = SOMAXCONN) throw(RIOException);
	/**	
	 * Accept:					accept
	 *
	 * @throws	RIOException	accept失敗
	 */
	void Accept(SOCKET inAcceptSocket ,
						PVOID lpOutputBuffer, DWORD * outReadByte,OVERLAPPED * outOverlapped) throw(RIOException);
	/**
	 * ShutDown:	切断.
	 *
	 */
	void ShutDown();
	/**
	 * Close:	ソケットクローズ.
	 *
	 * @throws	RIOCannotCloseException		切断に失敗
	 */
	void Close() throw(RIOCannotCloseException);
	/**
	 * Send:				送信
	 *
	 * @param ioParam		なぞ
	 * @return int 
	 * @throw	RIOCannotWriteException
	 */
	int RSocketForWinSock2::Send( AsyncParam * ioParam ) throw(RIOCannotWriteException);
	/**
	 * Reserve:				受信.
	 *
	 * @param ioParam		なぞ
	 * @return int 
	 * @throw	RIOCannotReadException
	 */
	int RSocketForWinSock2::Reserve( AsyncParam * ioParam ) throw(RIOCannotReadException);
	/**
	 * WaitForSignal:		受信、送信するまで待つ.
	 *
	 * @param ioParam		なぞ
	 * @param inTimeOut		タイムアウト.
	 * @return void 
	 * @throw	RIOException
	 */
	void RSocketForWinSock2::WaitForSignal(AsyncParam * ioParam,int inTimeOut ) throw(RIOException);
	/**
	 * WSAGetOverlappedResult:		受信、送信した結果を受け取る.
	 *
	 * @param ioParam				なぞ
	 * @return void 
	 * @throw	RIOException
	 */
	void RSocketForWinSock2::WSAGetOverlappedResult(AsyncParam * ioParam) throw(RIOException);

	bool RSocketForWinSock2::TransmitFile( HANDLE	inFileHandle , int inNumberOfBytesToWrite , int inNumberOfBytesPerSend,
									TRANSMIT_FILE_BUFFERS * inTransmitFileBufers , AsyncParam * ioParam )
									throw (RIOCannotReadException);
	/**
	 * getSocket:	ソケットハンドルの取得
	 *				カプセル化を壊すので使わないでほすぃ.
	 *
	 * @return SOCKET	ソケットハンドル
	 */
	SOCKET	getSocket()
	{
		return Socket;
	}

private:
	/**
	 * HoldTillReadyForWrite:		書き込みができるようになるまで待機.
	 *
	 * @param	inTimeOutS			タイムアウトする秒数
	 * @throws	RIOException	タイムアウト.
	 */
	void HoldTillReadyForWrite(long inTimeOutS) throw(RIOException);

	/**
	 * IntTime:	タイムアウトに使う時間の提供.
	 *
	 * @return DWORD	UNIX時刻
	 */
	DWORD IntTime();

protected:
	/**
	 * ErrorToMesage:	エラーメッセージを取得する.
	 *
	 * @return string	エラーメッセージ
	 */
	string ErrorToMesage();


private:
	//ソケットハンドル.
	SOCKET	Socket;
	//接続している場合 true.
	bool	IsConnect;

	enum { receives = 0, sends = 1, both = 2 };

};

#endif // !defined(AFX_RSOCKETFORWINSOCK2_H__C22160FA_0325_4A87_AFEA_273CA5F0D002__INCLUDED_)
