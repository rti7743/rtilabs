// RSocketForWinSock2.cpp: RSocketForWinSock2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RSocketForWinSock2.h"
#include <stdio.h>
#include <sys\timeb.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")

//受信バッファの最大値.
const int RESERVE_BUFFER_SIZE	= 50000;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

/**
 * RSocketForWinSock2:		コンストラクタ
 *				とりあえず初期化します。
 */
RSocketForWinSock2::RSocketForWinSock2()
{
	Socket = INVALID_SOCKET;
	IsConnect = false;
}
/**
 * RSocketForWinSock2:		コンストラクタ
 *				指定されたソケットで初期化します。
 *
 * @param inNewSocket	既につながっているソケット.
 * @return  
 */
RSocketForWinSock2::RSocketForWinSock2(SOCKET inNewSocket)
{
	Socket = inNewSocket;
	IsConnect = true;
}


/**
 * ~RSocketForWinSock2:	デストラクタ
 *				
 */
RSocketForWinSock2::~RSocketForWinSock2()
{
}

/**
 * Open:			ソケットを作成する.
 *
 * @param type		ソケットの種類
 *						SOCK_STREAM : TCPで使用する
 *						SOCK_DGRAM : UDPで使用する
 * @param protocol	使用するプロトコル
 * @throws			RIOCannotOpenException		開けなかったとき
 */
void RSocketForWinSock2::Open( int type, int protocol ) throw(RIOCannotOpenException)
{
	Socket = socket(AF_INET,type,protocol);
	if (Socket == INVALID_SOCKET || Socket == NULL) throw RIOCannotOpenException(EXCEPTIONTRACE + "ソケットが開けませんでした" + ErrorToMesage() );

	// Winsockで非ブロッキングのソケット通信を行うように設定する.
	unsigned long argp = 1;   // 非ブロッキングに設定 
	if ( ioctlsocket( Socket , FIONBIO , &argp ) )
	{
		throw RIOException(EXCEPTIONTRACE + "非ブロッキングモードへ移動できません" + ErrorToMesage() );
	}

}

/**
 * Connect:				接続(簡易版)
 *
 * @param inHost		ホスト名
 * @param inPort		ポート
 * @throws				RIOException			接続できなかったとき
 */
void RSocketForWinSock2::Connect(string inHost , int inPort)	throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	//すでに接続しています。
	if (IsConnect)	return ;

	LPHOSTENT	HostEnt;
	SOCKADDR_IN	sai = {0};
	in_addr		iadr;

	iadr.s_addr=inet_addr( inHost.c_str() );
	if(iadr.s_addr==INADDR_NONE)
	{
		HostEnt=gethostbyname(inHost.c_str() );
		if(!HostEnt)
		{
			throw RIOException(EXCEPTIONTRACE + "ホスト名を解決できませんでした" + ErrorToMesage() );
		}
		sai.sin_addr=*((LPIN_ADDR)*HostEnt->h_addr_list);
	}
	else
	{
		sai.sin_addr = iadr;
	}

	sai.sin_port=htons((unsigned short)inPort); // 変換を忘れずに!
	sai.sin_family=AF_INET;

	this->Connect(&sai) ;
}

/**
 * Connect:			接続
 *
 * @param inSai		接続情報
 * @throws			RIOException			接続できなかったとき
 */
void RSocketForWinSock2::Connect(const SOCKADDR_IN * inSai) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	//すでに接続しています。
	if (IsConnect)	return ;

	// Winsockで非ブロッキングのソケット通信を行うように設定する.
	unsigned long argp = 1;   // 非ブロッキングに設定 
	if ( ioctlsocket( Socket , FIONBIO , &argp ) )
	{
		throw RIOException(EXCEPTIONTRACE + "非ブロッキングモードへ移動できません" + ErrorToMesage() );
	}

	//接続を試みる.
	int ConnectRet = connect(Socket,(LPSOCKADDR)inSai,sizeof(SOCKADDR_IN));
	if( ! (ConnectRet == 0 || ConnectRet == -1) ) //成功(0) か、まだ終わっていない(-1)でないならば...
	{	//接続に失敗しました.
		string error = ErrorToMesage();	//ブロックモードに戻すときにエラーが上書きされないように先に取得.

		argp = 0;   // ブロッキングに設定 
		ioctlsocket( Socket , FIONBIO , &argp );

		throw RIOException(EXCEPTIONTRACE + "接続に失敗しました" + error );
	}

	try
	{
		//書き込みができるようになるまで待機.
		//タイムアウトが発生すると RIOException が起きます。
		HoldTillReadyForWrite( 60 ) ;
	}
	catch(...)
	{
		argp = 0;   // ブロッキングに設定 
		ioctlsocket( Socket , FIONBIO , &argp );
		throw ;
	}

	argp = 0;   // ブロッキングに設定 
	if ( ioctlsocket( Socket , FIONBIO , &argp ) )
	{
		throw RIOException(EXCEPTIONTRACE + "ブロッキングモードへ移動できません" + ErrorToMesage() );
	}

	IsConnect = true;
}

/**
 * Bind:			bind(簡易版)
 *
 * @param inPort	bindするポート
 * @throws			RIOException		bind失敗
 */
void RSocketForWinSock2::Bind(int inPort) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	//すでにbindしています。
	if (IsConnect)	return ;

	int	Opt = 1;
	setsockopt(Socket , SOL_SOCKET, SO_REUSEADDR, (const char*)&Opt , sizeof(int) );

	SOCKADDR_IN	sai = {0};
	sai.sin_family			= AF_INET;
	sai.sin_addr.s_addr	= htonl(INADDR_ANY);
	sai.sin_port			= htons((unsigned short)inPort);

	this->Bind( &sai );
}

/**
 * Bind:			bind
 *
 * @param inSai		bind情報
 * @throws			RIOException		bind失敗
 */
void RSocketForWinSock2::Bind(const SOCKADDR_IN * inSai) throw(RIOException)
{
	//すでにbindしています。
	if (IsConnect)	return ;

	int ret = bind(Socket , (struct sockaddr*) inSai , sizeof(sockaddr) );
	if (ret < 0)
	{
		throw RIOException(EXCEPTIONTRACE + "Bind に失敗しました" + ErrorToMesage() );
	}

	IsConnect = true;
}

/**
 * Listen:					listen
 *
 * @param inBacklog			接続要求ログを保存する（キューイングする）最大数
 * @throws	RIOException	listen失敗	
 */
void RSocketForWinSock2::Listen(int inBacklog ) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

	int ret = listen(Socket , inBacklog );
	if (ret < 0)
	{
		throw RIOException(EXCEPTIONTRACE + "Listen に失敗しました" + ErrorToMesage() );
	}
}

/**	
 * Accept:					accept
 *
 * @throws	RIOException	accept失敗
 */
void RSocketForWinSock2::Accept(SOCKET inAcceptSocket ,
								PVOID lpOutputBuffer, DWORD * outReadByte,OVERLAPPED * outOverlapped) throw(RIOException)
{
	ASSERT(Socket != INVALID_SOCKET);

    if (!AcceptEx(Socket, inAcceptSocket,
                  lpOutputBuffer,
                  0,
                  PADDED_ADDR_SIZE, 
                  PADDED_ADDR_SIZE,
                  outReadByte,
                  outOverlapped))
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING) return ;

		WSASetLastError(error);
		throw RIOException(EXCEPTIONTRACE + "accept に失敗しました" + ErrorToMesage() );
	}

}

/**
 * HoldTillReadyForWrite:		書き込みができるようになるまで待機.
 *
 * @param	inTimeOutS			タイムアウトする秒数
 * @throws	RIOException	タイムアウト.
 */
void RSocketForWinSock2::HoldTillReadyForWrite(long inTimeOutS) throw(RIOException)
{
	timeval UseTimeVal;
	FD_SET	WriteFlg;

	//セレクトの戻り値
	int	SelectRet;	
	int i;
	//短い時間でチェックします.
	for(i = 0 ; i < inTimeOutS ; i++)
	{
		//秒.
		UseTimeVal.tv_sec  = 1;
		//秒ms
		UseTimeVal.tv_usec = 0;
		FD_ZERO(&WriteFlg);
		FD_SET(Socket , &WriteFlg);

		SelectRet = select(2 , NULL , &WriteFlg , NULL , &UseTimeVal);
		//タイムアウトではなかったらループ離脱.
		if (SelectRet != 0) break;	
	}
	//タイムアウトです.
	if (i == inTimeOutS)
	{
		throw RIOException(EXCEPTIONTRACE + "タイムアウトが発生しました.");
	}

	if ( ! FD_ISSET(Socket , &WriteFlg) )
	{//ミス.
		throw RIOException(EXCEPTIONTRACE + "原因不明のエラーが発生しました" + ErrorToMesage() );
	}
}

/**
 * ShutDown:	切断.
 *
 */
void RSocketForWinSock2::ShutDown()
{
	if (Socket == INVALID_SOCKET) return ;
	if (!IsConnect)	return ;

	shutdown(Socket , RSocketForWinSock2::sends);

	//この時間になってもつないでいる場合は、即切る.
	DWORD TimeOutTime = IntTime() + 10; //10秒待つ.

	//保留中のすべてのデータを受け取る.
	int		Ret;
	char	Buffer[RESERVE_BUFFER_SIZE];
	while(1)
	{
		Ret = recv(Socket , Buffer , RESERVE_BUFFER_SIZE*sizeof(char) , 0 );
		//TRACE("Shutdown 保留中のデータ %d を受信しました\r\n",Ret);
		if ( Ret <= 0 || Ret == SOCKET_ERROR)	break;

		//タイムアウトチェック
		if ( TimeOutTime < IntTime() ) 
		{
			//TRACE("Shutdown タイムアウトが発生しました\r\n");
			break;
		}
	}
	//TRACE("Shutdown が呼ばれました3");
	//データをこれ以上受信しないことを相手に伝える.
	shutdown(Socket , RSocketForWinSock2::both);

	IsConnect = false;
}

/**
 * Close:	ソケットクローズ.
 *
 * @throws	RIOCannotCloseException		切断に失敗
 */
void RSocketForWinSock2::Close() throw(RIOCannotCloseException)
{
	if(Socket == INVALID_SOCKET) return ;

	//切断する.
//	ShutDown();

	if( closesocket(Socket) )
	{
		throw RIOException(EXCEPTIONTRACE + "ソケットのクローズ時にエラーが発生しました" + ErrorToMesage() );
	}
	Socket = INVALID_SOCKET;

}

/**
 * Send:				送信
 *
 * @param ioParam		なぞ
 * @return int 
 * @throw	RIOCannotWriteException
 */
int RSocketForWinSock2::Send( AsyncParam * ioParam ) throw(RIOCannotWriteException)
{
	ASSERT(ioParam != NULL);
	if (this->Socket == INVALID_SOCKET) return 0;  

	int ret = ::WSASend( this->Socket, & ioParam->Buffer ,  1,
				&ioParam->Size,  ioParam->Flag,  &ioParam->Overlapped,  NULL	);
	if (ret < 0)
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING) return 0;

		WSASetLastError(error);
		throw RIOCannotWriteException(EXCEPTIONTRACE + "送信時にエラーが発生しました" + ErrorToMesage() );
	}

	return ret;
}

bool RSocketForWinSock2::TransmitFile( HANDLE	inFileHandle , int inNumberOfBytesToWrite , int inNumberOfBytesPerSend,
										TRANSMIT_FILE_BUFFERS * inTransmitFileBufers , AsyncParam * ioParam )
										throw (RIOCannotReadException)
{
	bool ret = Btob(::TransmitFile(this->Socket , 
			inFileHandle ,
			inNumberOfBytesToWrite ,
			inNumberOfBytesPerSend ,
			&ioParam->Overlapped ,
			inTransmitFileBufers ,
			0 ) );
	if (ret == false)
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING || error == WSA_IO_PENDING) return false;

		WSASetLastError(error);
		throw RIOCannotReadException(EXCEPTIONTRACE + "受信時にエラーが発生しました" + ErrorToMesage() );
	}

	return true;
}

/**
 * Reserve:				受信.
 *
 * @param ioParam		なぞ
 * @return int 
 * @throw	RIOCannotReadException
 */
int RSocketForWinSock2::Reserve( AsyncParam * ioParam ) throw(RIOCannotReadException)
{
	ASSERT(ioParam != NULL);
	if (this->Socket == INVALID_SOCKET) return 0;  

	int ret = ::WSARecv( this->Socket, & ioParam->Buffer ,  1,
				&ioParam->Size,  &ioParam->Flag,  &ioParam->Overlapped,  NULL	);
	if (ret < 0)
	{
		const DWORD error = WSAGetLastError();
		if (error == ERROR_IO_PENDING) return 0;

		WSASetLastError(error);
		throw RIOCannotReadException(EXCEPTIONTRACE + "受信時にエラーが発生しました" + ErrorToMesage() );
	}

	return ret;
}

/**
 * WaitForSignal:		受信、送信するまで待つ.
 *
 * @param ioParam		なぞ
 * @param inTimeOut		タイムアウト.
 * @return void 
 * @throw	RIOException
 */
void RSocketForWinSock2::WaitForSignal(AsyncParam * ioParam,int inTimeOut ) throw(RIOException)
{
	ASSERT(ioParam != NULL);

	const DWORD ret = WSAWaitForMultipleEvents(1 ,& (ioParam->Overlapped.hEvent), TRUE, inTimeOut,TRUE);
	if (ret == WSA_WAIT_FAILED )
	{
		throw RIOException(EXCEPTIONTRACE + "WSAWaitForMultipleEventsがエラーを返しました" + ErrorToMesage() );
	}
	if (ret == WSA_WAIT_TIMEOUT)
	{
		throw RIOException(EXCEPTIONTRACE + "タイムアウトが発生しました.");
	}
}

/**
 * WSAGetOverlappedResult:		受信、送信した結果を受け取る.
 *
 * @param ioParam				なぞ
 * @return void 
 * @throw	RIOException
 */
void RSocketForWinSock2::WSAGetOverlappedResult(AsyncParam * ioParam) throw(RIOException)
{
	ASSERT(ioParam != NULL);

	if ( ::WSAGetOverlappedResult(this->Socket , &ioParam->Overlapped , &ioParam->Size ,TRUE , &ioParam->Flag ) == FALSE)
	{
		throw RIOException(EXCEPTIONTRACE + "WSAGetOverlappedResultがエラーを返しました" + ErrorToMesage() );
	}
}




/**
 * IntTime:	タイムアウトに使う時間の提供.
 *
 * @return DWORD	UNIX時刻
 */
DWORD RSocketForWinSock2::IntTime()
{
	struct _timeb Time;
	_ftime(&Time);

	return Time.time;
}
/**
 * ErrorToMesage:	エラーメッセージを取得する.
 *
 * @return string	エラーメッセージ
 */
string RSocketForWinSock2::ErrorToMesage()
{

	//http://kaf.tri6.net/code/wsock_r.html
	DWORD error = WSAGetLastError();
	if (error == 0) return "";
	switch(error)
	{
	case WSAEINTR: return string(" 関数呼び出しが中断された。\nWinSock 1.1 のブロッキング関数呼び出しが、WSACancelBlockingCall() によって中断された。");
	case WSAEBADF: return string("無効なファイルハンドル。\nソケット関数に渡されたファイルハンドル（ソケットハンドル）が不正である。（WSAENOTSOCKと同義で用いられる）");
	case WSAEACCES: return string("アクセスが拒否された。\nconnect()：ブロードキャストアドレスで接続しようとしたソケットの SO_BROADCASTオプション が有効になっていない。\nsendto()：ブロードキャストアドレスに送信しようとしたソケットの SO_BROADCASTオプションが有効になっていない。\naccept()：受付しようとした接続要求がすでにタイムアウトになっている、もしくは取り消されている。");
	case WSAEFAULT: return string("無効なバッファアドレス\n関数に渡されたバッファを指すポインタが無効なアドレス空間を指している。またはバッファのサイズが小さすぎる。");
	case WSAEINVAL: return string("無効な引数が渡された。\nlisten()を呼び出さずにaccept()を呼び出した。\nバインド済みのソケットに対してbind()しようとした。\nlisten()で待機中のソケットでconnect()しようとした。\n関数に渡されたパラメータに無効な値が含まれている。");
	case WSAEMFILE: return string("使用中のソケットの数が多すぎる。\n同時に利用できるソケット数の最大に達しており、新たにソケットを作成できない。");
	case WSAEWOULDBLOCK: return string("操作はブロッキングされる。\nノンブロッキングモードでソケット関数が呼び出されため、関数は即座にリターンしたが要求された操作は完了していない。");
	case WSAEINPROGRESS: return string("すでにブロッキング手続きが実行されている。\nブロッキング関数が実行中であるか、サービスプロバイダがコールバック関数を処理中である。WinSockでは１つのプロセスで同時に複数のブロッキング操作をすることはできないため、前回呼び出した操作が完全に終了するまで次の操作を開始することはできない。");
	case WSAEALREADY: return string("要求された操作は既に実行中、または実行済み。\nキャンセルしようとした非同期操作が既にキャンセルされている。\nconnect()を呼び出したが、既に前回の呼び出しによって接続処理中である。");
	case WSAENOTSOCK: return string("指定されたソケットが無効である。\nソケットハンドルとして無効な値がソケット関数に渡された。");
	case WSAEMSGSIZE: return string("メッセージサイズが大きすぎる。\n送信、または受信しようとしたメッセージが、基盤となるプロトコルのサポートしている最大メッセージサイズを超えている。ioctlsocket()でバッファサイズを増やすことで回避可能なこともある。");
	case WSAEPROTOTYPE: return string("ソケットは要求されたプロトコルに適合していない。\nsocket()を呼び出すときに指定したプロトコルタイプがソケットタイプと適合していない。メッセージ型プロトコルでストリームソケットを作成しようとした場合など。");
	case WSAENOPROTOOPT: return string("不正なプロトコルオプション。\n指定したソケットオプションまたはioctlsocket()での操作が、そのプロトコルではサポートされてない。\nストリームソケットに SO_BROADCAST を指定しようとした。");
	case WSAEPROTONOSUPPORT: return string("プロトコルがサポートされていない。\nサポートされていないプロトコルを指定した、またはプロトコルのサポートしていない操作を実行しようとした。");
	case WSAESOCKTNOSUPPORT: return string("指定されたソケットタイプはサポートされていない。");
	case WSAEOPNOTSUPP: return string("要求された操作はサポートされていない。\nバンド外データを送信/受信しようとしたソケットでそれがサポートされていない。\n非接続型(UDPなど)ソケットでaccept()を呼び出した。");
	case WSAEPFNOSUPPORT: return string("プロトコルファミリがサポートされていない。");
	case WSAEAFNOSUPPORT: return string("アドレスファミリがサポートされていない。");
	case WSAEADDRINUSE: return string("アドレスは既に使用中である。\nbind()しようとしたアドレスは、既にほかのソケットで使われている。同じローカルアドレスを複数のソケットで使うためには、SO_REUSEADDRソケットオプションをTRUEにする必要がある。");
	case WSAEADDRNOTAVAIL: return string("無効なネットワークアドレス。\nソケット関数に渡されたネットワークアドレスに無効な部分がある。");
	case WSAENETDOWN: return string("ネットワークがダウンしている。\nネットワークシステムが何らかの障害を起こし、機能停止しているため要求された操作が実行できない。");
	case WSAENETUNREACH: return string("指定されたネットワークホストに到達できない。\nWSAEHOSTUNREACHと同義で使われている。");
	case WSAENETRESET: return string("ネットワーク接続が破棄された。\nキープアライブを行っている接続で、何らかの障害のためタイムアウトが検出され接続が閉じられた。");
	case WSAECONNABORTED: return string("ネットワーク接続が破棄された。");
	case WSAECONNRESET: return string("ネットワーク接続が相手によって破棄された。");
	case WSAENOBUFS: return string("バッファが不足している。\nメモリ不足のためWinSockが操作に必要なバッファを確保することができない。または、バッファを必要とする関数呼び出しで、小さすぎるバッファが渡された。");
	case WSAEISCONN: return string("ソケットは既に接続されている。\n接続中のソケットに対してconnect()、listen()、bind()等の操作を行おうとした。\n接続中のソケットでsendto()を実行しようとした。");
	case WSAENOTCONN: return string("ソケットは接続されていない。\n接続されていないソケットでsend()、getpeername()等の接続されたソケットを必要とする操作を実行しようとした。");
	case WSAESHUTDOWN: return string("ソケットはシャットダウンされている。\n既にシャットダウンされて送信/受信が停止されているソケットで、送信または受信の操作を実行しようとした。");
	case WSAETOOMANYREFS: return string("参照の数が多すぎる。");
	case WSAETIMEDOUT: return string("接続要求がタイムアウトした。\nconnect()を呼び出して接続を試みたが、（相手の応答がない等で）処理がタイムアウトになり接続要求がキャンセルされた。");
	case WSAECONNREFUSED: return string("接続が拒否された。");
	case WSAELOOP: return string("ループ。");
	case WSAENAMETOOLONG: return string("名前が長すぎる。");
	case WSAEHOSTDOWN: return string("ホストがダウンしている。\nWSAETIMEDOUTと同義。");
	case WSAEHOSTUNREACH: return string("ホストへの経路がない。\nネットワークの構造上到達できないホストに対して操作を実行しようとした。またはアドレスが不正である。");
	case WSAENOTEMPTY: return string("ディレクトリが空ではない。");
	case WSAEPROCLIM: return string("プロセスの数が多すぎる。\nWSAStartup()：既にWinSockが管理できる最大プロセス数に達しており処理が実行できない。");
	case WSAEUSERS: return string("ユーザーの数が多すぎる。");
	case WSAEDQUOT: return string("ディスククォータ。");
	case WSAESTALE: return string("実行しようとした操作は廃止されている。");
	case WSAEREMOTE: return string("リモート。");
	case WSASYSNOTREADY: return string("ネットワークサブシステムが利用できない。\nWSAStartup()：ネットワークサブシステムが利用できない、または正しくセットアップされていないため機能していない。");
	case WSAVERNOTSUPPORTED: return string("Winsock.dllのバージョンが範囲外である。\nWSAStartup()：要求したWinSockバージョンは、現在の実装ではサポートされていない。");
	case WSANOTINITIALISED: return string("WinSockシステムが初期化されていない。\nWinSock関数を実行しようとしたが、WSAStartup()が正常に実行されていないため機能しない。");
	case WSAEDISCON: return string("シャットダウン処理中。\n接続が相手の処理によって切断されようとしている。");
	case WSAENOMORE: return string("データはこれ以上存在しない。\nWSA_E_NO_MOREと同義。");
	case WSAECANCELLED: return string("操作は取り消された。\nWSA_E_CANCELLEDと同義。");
	case WSAEINVALIDPROCTABLE: return string("サービスプロバイダの関数テーブルが無効。");
	case WSAEINVALIDPROVIDER: return string("サービスプロバイダが無効。");
	case WSAEPROVIDERFAILEDINIT: return string("サービスプロバイダの初期化に失敗した。");
	case WSASYSCALLFAILURE: return string("システムコールに失敗した。\n決して失敗しないはずのシステムコールが何らかの理由により失敗した。");
	case WSASERVICE_NOT_FOUND: return string("サービスが見つからない。\n要求されたサービスが標準的なサービスでなく、指定した名前空間からサービスを見つけることができない。");
	case WSATYPE_NOT_FOUND: return string("タイプが見つからない。");
	case WSA_E_NO_MORE: return string("データはこれ以上存在しない。\nWSALookupServiceNext()：情報はこれ以上存在しない。");
	case WSA_E_CANCELLED: return string("検索がキャンセルされた。\nWSALookupServiceNext()：サービスの検索途中に、WSALookupServiceEnd()によって処理がキャンセルされた。");
	case WSAEREFUSED: return string("操作は拒否された。");
	case WSAHOST_NOT_FOUND: return string("ホストが見つからない。\nDNSなどの名前解決サービスが指定されたホストを見つけられなかった。プロトコルやサービスのクエリー操作においても返される。");
	case WSATRY_AGAIN: return string("指定されたホストが見つからない、またはサービスの異常。");
	case WSANO_RECOVERY: return string("回復不能なエラーが発生した。\nデータベース検索の場合は、名前解決サービスが使用できないことを意味する。");
	case WSANO_DATA: return string("要求されたタイプのデータレコードが見つからない。");
	default:	return string("未定義のエラー" + num2str(error) + "が発生しました");

	}
}
 