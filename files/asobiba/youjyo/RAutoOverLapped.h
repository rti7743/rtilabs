// RAutoOverLapped.h: RAutoOverLapped クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAUTOOVERLAPPED_H__E3DBD04F_9028_4FF1_8D9A_778158C1778A__INCLUDED_)
#define AFX_RAUTOOVERLAPPED_H__E3DBD04F_9028_4FF1_8D9A_778158C1778A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RAutoOverLapped  : public OVERLAPPED
{
public:
	RAutoOverLapped()
	{
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	~RAutoOverLapped()//継承禁止
	{
		CloseHandle(hEvent);
	}
};

#endif // !defined(AFX_RAUTOOVERLAPPED_H__E3DBD04F_9028_4FF1_8D9A_778158C1778A__INCLUDED_)
