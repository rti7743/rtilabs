// RSusieConferException.h: RSusieConferException クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSUSIECONFEREXCEPTION_H__F17020E1_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
#define AFX_RSUSIECONFEREXCEPTION_H__F17020E1_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RConferException.h"

class RSusieConferException : public RConferException  
{
public:
	RSusieConferException()
	{
	}
	RSusieConferException(unsigned long inErrorCode,std::string inErrorString)
	{
		setErrorCode(inErrorCode);
		setError(inErrorString);
	}
	RSusieConferException(unsigned long inErrorCode,char* Format,...)
	{
		setErrorCode(inErrorCode);
		setError(Format,(char*)(&Format+1));
	}
	virtual ~RSusieConferException()
	{
	}

	void setErrorCode(unsigned long	inErrorCode)
	{
		ErrorCode = inErrorCode;
	}
	unsigned long	getErrorCode()
	{
		return ErrorCode;
	}
private:
	//エラーコード.
	unsigned long	ErrorCode;

};

#endif // !defined(AFX_RSUSIECONFEREXCEPTION_H__F17020E1_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
