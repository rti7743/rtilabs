// YYoujyoGC.h: YYoujyoGC クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYOUJYOGC_H__5D6E29A7_9302_4E28_AAF2_463D6CC0CF2D__INCLUDED_)
#define AFX_YYOUJYOGC_H__5D6E29A7_9302_4E28_AAF2_463D6CC0CF2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RThread.h"

class YYoujyoGC  : public RThread  
{
public:
	YYoujyoGC();
	virtual ~YYoujyoGC();

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

};

#endif // !defined(AFX_YYOUJYOGC_H__5D6E29A7_9302_4E28_AAF2_463D6CC0CF2D__INCLUDED_)
