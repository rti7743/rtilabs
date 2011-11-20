// RSocket.h: RSocket クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSOCKET_H__F4099A31_C639_4C24_A646_B6D617BCCB6F__INCLUDED_)
#define AFX_RSOCKET_H__F4099A31_C639_4C24_A646_B6D617BCCB6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotCloseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotWriteException.h"


/**
 * class RSocket:	ソケット
 *
 */
class RSocket  
{
public:
	/**
	 * RSocket:		コンストラクタ
	 *				とりあえず初期化します。
	 */
	RSocket();
	/**
	 * RSocket:		コンストラクタ
	 *				指定されたソケットで初期化します。
	 *
	 * @param inNewSocket	既につながっているソケット.
	 * @return  
	 */
	RSocket(SOCKET inNewSocket);
	/**
	 * ~RSocket:	デストラクタ
	 *				ひらきっぱの場合はかってに閉じます。
	 */
	virtual ~RSocket();

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
	 * @return RSocket*			接続されたクライアント
	 * @throws	RIOException	accept失敗
	 */
	RSocket* Accept() throw(RIOException);
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
	* Send:				送信.
	*
	* @param inBuffer		送信するバッファ
	* @param inBufferLen	送信するバッファのサイズ
	* @param inFlags		フラグ
	* @param inTimeoutS	タイムアウト秒数 * 5
	* @return int			送信したサイズ.
	* @throws	RIOException	タイムアウト.
	*/
	int Send( const char* inBuffer ,int inBufferLen , int inFlags = 0,int inTimeoutS = 1) throw(RIOException,RIOCannotWriteException);
	/**
	 * Reserve:				受信.
	 *
	 * @param outBuffer		受信するバッファ
	 * @param inBufferLen	受信するバッファのサイズ
	 * @param inFlags		フラグ
	 * @param inTimeoutS	タイムアウト秒数 * 5
	 * @return int			受信したサイズ.
	 * @throws	RIOException	タイムアウト.
	 */
	int Reserve( char* outBuffer ,int inBufferLen , int inFlags = 0,int inTimeoutS = 1) throw(RIOException,RIOCannotReadException);
private:
	/**
	 * HoldTillReadyForRead:	読み込みができるようになるまで待機.
	 *
	 * @param	inTimeOutS		タイムアウトする秒数 * 5
	 * @throws	RIOException	タイムアウト.
	 */
	void HoldTillReadyForRead(long inTimeOutS) throw(RIOException);
	/**
	 * HoldTillReadyForWrite:		書き込みができるようになるまで待機.
	 *
	 * @param	inTimeOutS			タイムアウトする秒数 * 5
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
	//ソケットハンドル.
	SOCKET	Socket;
	//接続している場合 true.
	bool	IsConnect;

	enum { receives = 0, sends = 1, both = 2 };
};

#endif // !defined(AFX_RSOCKET_H__F4099A31_C639_4C24_A646_B6D617BCCB6F__INCLUDED_)
