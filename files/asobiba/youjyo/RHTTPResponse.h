// RHTTPResponse.h: RHTTPResponse クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTTPRESPONSE_H__3A325AFA_39CD_472D_AE6C_7C3D8F52ACDF__INCLUDED_)
#define AFX_RHTTPRESPONSE_H__3A325AFA_39CD_472D_AE6C_7C3D8F52ACDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStreamUtil.h"

class RHTTPResponse  
{
public:

	static string OK()
	{
		return	"HTTP/1.1 200 OK\r\n";
	}
	static string RangeOK()
	{
		return	"HTTP/1.1 206 Partial Content\r\n";
	}
	static string Location(const string inPath )
	{
		return	string() + 
				"HTTP/1.1 302 Moved Temporarily\r\n" + 
				string("Location: " + inPath + "\r\n");
	}
	static string NotModified()
	{
		return	"HTTP/1.1 304 Not Modified\r\n";
	}
	static string ResponseEnd()
	{
		return	"\r\n";
	}

	static string ServerName()
	{
		return	"Server: Youjyo\r\n";
	}

};

#endif // !defined(AFX_RHTTPRESPONSE_H__3A325AFA_39CD_472D_AE6C_7C3D8F52ACDF__INCLUDED_)
