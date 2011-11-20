// YYoujyoKiller.h: YYoujyoKiller クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYOUJYOKILLER_H__123A734E_183E_4420_844F_FD547B0FB6CD__INCLUDED_)
#define AFX_YYOUJYOKILLER_H__123A734E_183E_4420_844F_FD547B0FB6CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RThread.h"

class YYoujyoKiller  : public RThread  
{
public:
	YYoujyoKiller();
	virtual ~YYoujyoKiller();

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
	char	Buffer[THREAD_WORK_AREA];
};

#endif // !defined(AFX_YYOUJYOKILLER_H__123A734E_183E_4420_844F_FD547B0FB6CD__INCLUDED_)
