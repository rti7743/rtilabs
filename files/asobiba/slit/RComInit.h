// RComInit.h: RComInit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOMINIT_H__AA441886_D909_4F98_961D_699D29661ACE__INCLUDED_)
#define AFX_RCOMINIT_H__AA441886_D909_4F98_961D_699D29661ACE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WCE

#include <ole2.h>


class RComInit  
{
private:
	RComInit();
public:
	virtual ~RComInit();

	static void UseCom();
};

#endif

#endif // !defined(AFX_RCOMINIT_H__AA441886_D909_4F98_961D_699D29661ACE__INCLUDED_)
