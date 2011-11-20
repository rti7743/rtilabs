// RNotify.h: RNotify クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNOTIFY_H__277C31EC_873D_46E0_8E9B_B059861CFC03__INCLUDED_)
#define AFX_RNOTIFY_H__277C31EC_873D_46E0_8E9B_B059861CFC03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RNotify  
{
public:
	RNotify();
	virtual ~RNotify();

	//つながったことを通知する
	void Post(void * inConnectPack);

	//この接続を破棄する
	void Shutdown();
	
	//つながったことが通知されるまで待て
	bool Wait(void ** outConnectPack);

private:
	HANDLE		NotifyHandle;
};

#endif // !defined(AFX_RNOTIFY_H__277C31EC_873D_46E0_8E9B_B059861CFC03__INCLUDED_)
