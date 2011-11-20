// RWaitCursor.h: RWaitCursor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWAITCURSOR_H__792610DB_7566_45A8_B30B_6FE160BCD78A__INCLUDED_)
#define AFX_RWAITCURSOR_H__792610DB_7566_45A8_B30B_6FE160BCD78A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RWaitCursor  
{
public:
	RWaitCursor();
	virtual ~RWaitCursor();

	HCURSOR OldCursor;
};

#endif // !defined(AFX_RWAITCURSOR_H__792610DB_7566_45A8_B30B_6FE160BCD78A__INCLUDED_)
