// RArchiveException.h: RArchiveException クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RARCHIVEEXCEPTION_H__77141ACA_FB7B_4185_9677_19D4A01715F9__INCLUDED_)
#define AFX_RARCHIVEEXCEPTION_H__77141ACA_FB7B_4185_9677_19D4A01715F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RConferException.h"


class RArchiveException : public RConferException  
{
public:
	RArchiveException(unsigned long inErrorCode,char* Format,...);
	virtual ~RArchiveException();
private:
	bool		FatalError;
};

#endif // !defined(AFX_RARCHIVEEXCEPTION_H__77141ACA_FB7B_4185_9677_19D4A01715F9__INCLUDED_)
