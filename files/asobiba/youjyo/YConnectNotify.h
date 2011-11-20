// YConnectNotify.h: YConnectNotify クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YCONNECTNOTIFY_H__BEA50F73_1FC3_4DF1_9DD6_C8AB6FB03EF4__INCLUDED_)
#define AFX_YCONNECTNOTIFY_H__BEA50F73_1FC3_4DF1_9DD6_C8AB6FB03EF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotify.h"
#include "RSocket.h"
#include "RAutoOverLapped.h"

struct YConnectPack 
{
	SOCKET				AcceptSocket;
	RAutoOverLapped		OverLapped;
	in_addr				RemoteIP;
};

class YConnectNotify  
{
public:
	YConnectNotify()
	{
	}
	virtual ~YConnectNotify()
	{
	}

	//つながったことを通知する
	void PostAcceptNotify(YConnectPack * inConnectPack)
	{
		Notify.Post(inConnectPack);
	}

	//この接続を破棄する
	void PostShutdownNotify()
	{
		Notify.Shutdown();
	}

	//つながったことが通知されるまで待て
	bool WaitAcceptNotify(YConnectPack ** outConnectPack)
	{
		return Notify.Wait((void**)outConnectPack);
	}

	static YConnectNotify* getInstance();

private:
	RNotify		Notify;
};

#endif // !defined(AFX_YCONNECTNOTIFY_H__BEA50F73_1FC3_4DF1_9DD6_C8AB6FB03EF4__INCLUDED_)
