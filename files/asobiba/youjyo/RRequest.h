// RRequest.h: RRequest クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RREQUEST_H__326EE315_E250_4891_997D_7536A47C84AE__INCLUDED_)
#define AFX_RREQUEST_H__326EE315_E250_4891_997D_7536A47C84AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RThread.h"

class RRequest  
{
public:
	RRequest();
	virtual ~RRequest();

	//タスクの実行.
	virtual void Execute(RThread* ioResponsible) = 0;
};

#endif // !defined(AFX_RREQUEST_H__326EE315_E250_4891_997D_7536A47C84AE__INCLUDED_)
