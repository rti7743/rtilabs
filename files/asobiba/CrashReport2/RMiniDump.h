// RCrashReport.h: RCrashReport クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCRASHREPORT_H__65536149_7484_40D9_BE34_E2DDA9A9712D__INCLUDED_)
#define AFX_RCRASHREPORT_H__65536149_7484_40D9_BE34_E2DDA9A9712D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include <Dbghelp.h>

class RMiniDump  
{
private:
	RMiniDump()
	{
	}
public:
	virtual ~RMiniDump()
	{
	}

	//任意に mini dumpを作成する.
	static bool MiniDump(const string & inFilename , MINIDUMP_TYPE inFlag,EXCEPTION_POINTERS *poolExceptionInfo = NULL);

};

#endif // !defined(AFX_RCRASHREPORT_H__65536149_7484_40D9_BE34_E2DDA9A9712D__INCLUDED_)
