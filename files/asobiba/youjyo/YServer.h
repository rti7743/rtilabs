// YServer.h: YServer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSERVER_H__FF1C36F5_B469_4C29_BEB4_228B3406C873__INCLUDED_)
#define AFX_YSERVER_H__FF1C36F5_B469_4C29_BEB4_228B3406C873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RThread.h"
#include "RSocketForWinSock2.h"
#include "YMain.h"
#include "YThreadWithTls.h"
#include "RAutoOverLapped.h"
#include "YConnectNotify.h"
#include "RSimpleThreadPool.h"
#include "YHttpdConf.h"
#include "RMimeType.h"

class YServer : public RThread  
{
public:
	YServer();
	virtual ~YServer();

	/**
	 * Run:	サーバ実行.
	 *
	 * @return virtual unsigned int 
	 */
	virtual unsigned int Run();

	/**
	 * Shutdown:	停止命令送信.
	 *
	 */
	void Shutdown();

private:
	RSocketForWinSock2	ListenSocket;
//	RSocketForWinSock2	AcceptSocket;

	RSimpleThreadPool	Pool;
};

#endif // !defined(AFX_YSERVER_H__FF1C36F5_B469_4C29_BEB4_228B3406C873__INCLUDED_)
