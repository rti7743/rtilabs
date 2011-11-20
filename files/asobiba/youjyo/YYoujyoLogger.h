// YYoujyoLogger.h: YYoujyoLogger クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYOUJYOLOGGER_H__5B730AA3_CC46_4A68_BD57_7598301B6ABD__INCLUDED_)
#define AFX_YYOUJYOLOGGER_H__5B730AA3_CC46_4A68_BD57_7598301B6ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotify.h"
#include "RThread.h"
#include "YConnectNotify.h"


class YLogNotify  
{
public:
	//つながったことを通知する
	void Post(string * logString)
	{
		Notify.Post(logString);
	}

	//この接続を破棄する
	void Shutdown()
	{
		Notify.Shutdown();
	}

	//つながったことが通知されるまで待て
	bool Wait(string ** logString)
	{
		return Notify.Wait((void**)logString);
	}

	static YLogNotify* getInstance();

private:
	RNotify		Notify;
};


//ログ書きをします.
class YYoujyoLogger  : public RThread  
{
public:
	YYoujyoLogger();
	virtual ~YYoujyoLogger();

	/**
	 * Run:	実行.
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
	FILE	* LogFile;
};

#endif // !defined(AFX_YYOUJYOLOGGER_H__5B730AA3_CC46_4A68_BD57_7598301B6ABD__INCLUDED_)
