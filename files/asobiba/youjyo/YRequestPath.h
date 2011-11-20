// YRequestPath.h: YRequestPath クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YREQUESTPATH_H__1AA0B09D_A6B5_49CC_9A91_49432B3D9DCE__INCLUDED_)
#define AFX_YREQUESTPATH_H__1AA0B09D_A6B5_49CC_9A91_49432B3D9DCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

enum REQUESTTYPE
{
	REQUESTTYPE_ERROR,
	REQUESTTYPE_SUBJECT,
	REQUESTTYPE_INDEX,
	REQUESTTYPE_DAT,
	REQUESTTYPE_CGIALL,
	REQUESTTYPE_CGILAST,
	REQUESTTYPE_CGIONE,
	REQUESTTYPE_CGIFORM,
	REQUESTTYPE_CGIFORMTO,
	REQUESTTYPE_CGITO,
	REQUESTTYPE_BBSCGI,
	REQUESTTYPE_SUBBACK,
	REQUESTTYPE_KAKO,
};

class YRequestPath  
{
public:
	YRequestPath();
	virtual ~YRequestPath();

	bool PathParse(const string &inPath);

	DATNUMBER getDatNumber()	const	{		return DatNumber;	}
	string getBoardName()	const	{		return BoardName;	}
	REQUESTTYPE getRequestType()	const	{		return RequestType;	}
	int getwParam()	const	{		return wParam;	}
	int getlParam()	const	{		return lParam;	}

	static void test();
private:
	DATNUMBER	DatNumber;
	string		BoardName;
	REQUESTTYPE	RequestType;

	int			wParam;
	int			lParam;
};

#endif // !defined(AFX_YREQUESTPATH_H__1AA0B09D_A6B5_49CC_9A91_49432B3D9DCE__INCLUDED_)
