// NazoNeetThread.h: NazoNeetThread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NazoNeetThread_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_)
#define AFX_NazoNeetThread_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RThread.h"
class NazoNeetThread : public RThread
{
public:
	unsigned int Run();
};


#endif // !defined(AFX_NazoNeetThread_H__03C22686_63AC_43EE_923F_A07C5AA003D7__INCLUDED_)
