// RExceptionUtil.cpp: RExceptionUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comm.h"
#include "RExceptionUtil.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RExceptionUtil::RExceptionUtil()
{

}

RExceptionUtil::~RExceptionUtil()
{

}

string RExceptionUtil::WindowsCodeToString(DWORD inErrorCode)
{
	LPVOID msgBuf;
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		inErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
		(LPTSTR) &msgBuf,
		0,
		NULL 
	);
	if (msgBuf == NULL)
	{
		msgBuf = "NULL";
	}

	string errorString;
#ifndef _WIN32_WCE
	errorString = PrintFormat("::[WIN32 FormatMessage] ErrorCode:%d(%s)" , inErrorCode , (const char*)msgBuf ) ;
#else
	MY_USES_CONVERSION;

	TCHAR* wmsg = (TCHAR*)msgBuf;
	errorString = PrintFormat("::[WIN32 FormatMessage] ErrorCode:%d(%s)" , inErrorCode ,  (const char*)W2A(wmsg) ) ;
#endif
	::LocalFree( msgBuf );

	return errorString;
}

string RExceptionUtil::ComErrorToString(_com_error &inComError)
{
	_bstr_t bstrSource(inComError.Source());
	_bstr_t bstrDescription(inComError.Description());

	string str = PrintFormat("COM エラー　Code = %08lx　Code meaning = %s　Source = %s　Description = %s",
		inComError.Error() , inComError.ErrorMessage() , (LPCSTR) bstrSource , (LPCSTR) bstrDescription );
	
	return str;
}
