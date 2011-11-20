// YServer.cpp: YServer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YServer.h"
#define SOCKBUF_SIZE	(256 * 1024)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YServer::YServer()
{
}

YServer::~YServer()
{
}

/**
 * Run:	サーバ実行.
 *
 * @return virtual unsigned int 
 */
unsigned int YServer::Run()
{
	try
	{
		ListenSocket.Open(SOCK_STREAM,0);
		ListenSocket.Bind( YHttpdConf::getInstance()->getListenPort() );
		ListenSocket.Listen();

	}
	catch(RException e)
	{
		printf("初期化時に例外を感知しました : %s\n" ,e.getMessage() );
		ListenSocket.Close();
		return 0;
	}

	YConnectPack* ao = NULL;
	try
	{
		char		buff[2*PADDED_ADDR_SIZE];	
		DWORD		readByte;

		YConnectNotify* ycn = YConnectNotify::getInstance();
		while( this->IsHoldUp() )
		{
			if (ao == NULL)
			{
				ao = new YConnectPack ;
				ao->AcceptSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

				// Winsockで非ブロッキングのソケット通信を行うように設定する.
				unsigned long argp = 1;   // 非ブロッキングに設定 
				ioctlsocket( ao->AcceptSocket  , FIONBIO , &argp );
				//送信バッファを拡張
				int buf_size = SOCKBUF_SIZE;
				for ( ; buf_size > 0; buf_size /= 2)
				{
					if (setsockopt(ao->AcceptSocket, SOL_SOCKET, SO_SNDBUF, (char *)&buf_size, sizeof(buf_size)) == 0)	break;
				}
			}

			ListenSocket.Accept(ao->AcceptSocket , buff , &readByte , &ao->OverLapped);

			DWORD ret = WaitForSingleObject(ao->OverLapped.hEvent, INFINITE);
			if (ret != WAIT_OBJECT_0)
			{
				if (this->IsHoldUp())
				{
					closesocket(ao->AcceptSocket);
					delete ao;
					ao = NULL;
					break;
				}
			}
			else
			{
				//すでに閉じられている.
				if (ao->AcceptSocket == INVALID_SOCKET)
				{
//					printf("accept したソケット(%x)はすでに閉じられています\r\n",ao->AcceptSocket);
					continue;	//Accept しなおし.
				}
				//結果を受け取る.
				if (!GetOverlappedResult((HANDLE)ao->AcceptSocket , &ao->OverLapped, &readByte, FALSE))
				{
					//???
//					printf("accept したソケット(%x)の結果を受け取れませんでした\r\n",ao->AcceptSocket);

					//このソケットに対する再利用は不可。
					closesocket(ao->AcceptSocket);
					delete ao;
					ao = NULL;

					continue;	//Accept しなおし.
				}
				//リモートホストを求める.
				{
					SOCKADDR*			localSocket;
					int					localSocketLen;
					SOCKADDR*			remoteSocket;
					int					remoteSocketLen;
					//IP抜くぞｺﾞﾗｧ.
					GetAcceptExSockaddrs(buff , 0 ,PADDED_ADDR_SIZE , PADDED_ADDR_SIZE ,
							&localSocket , &localSocketLen , &remoteSocket , &remoteSocketLen);
					sockaddr_in* addrin = (sockaddr_in*)remoteSocket;
					ao->RemoteIP = addrin->sin_addr;
				}
				//受信したぞゴラァ
				ycn->PostAcceptNotify(ao);
				ao = NULL;
			}
		}
	}
	catch(RException e)
	{
		printf("実行時例外か、終了割り込みを感知しました. : %s\n" ,e.getMessage() );
	}

	if (ao)
	{
		closesocket(ao->AcceptSocket);
		delete ao;
	}

	return 0;
}


/**
 * Shutdown:	停止命令送信.
 *
 */
void YServer::Shutdown()
{
	ListenSocket.Close();
	this->HoldUp();
}

