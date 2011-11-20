// RFModException.h: RFModException クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFMODEXCEPTION_H__8707C12B_77F6_4B1A_A074_03E79928BDBE__INCLUDED_)
#define AFX_RFMODEXCEPTION_H__8707C12B_77F6_4B1A_A074_03E79928BDBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RConferException.h"

class RFModException : public RConferException  
{
public:
	RFModException()
	{
	}
	RFModException(unsigned long inErrorCode,std::string inErrorString)
	{
		setErrorCode(inErrorCode);
		setError(inErrorString);
	}
	RFModException(unsigned long inErrorCode,char* Format,...)
	{
		setErrorCode(inErrorCode);
		setError(Format,(char*)(&Format+1));
	}
	virtual ~RFModException()
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

#endif // !defined(AFX_RFMODEXCEPTION_H__8707C12B_77F6_4B1A_A074_03E79928BDBE__INCLUDED_)
