// RMessageBox.h: RMessageBox クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMESSAGEBOX_H__0E1B336C_48FF_46A2_88DC_5343A35F129E__INCLUDED_)
#define AFX_RMESSAGEBOX_H__0E1B336C_48FF_46A2_88DC_5343A35F129E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RMessageBox  
{
public:
	RMessageBox();
	virtual ~RMessageBox();

	static void Message(const string &inMsg , const string &inTitle = "Message");
};

#endif // !defined(AFX_RMESSAGEBOX_H__0E1B336C_48FF_46A2_88DC_5343A35F129E__INCLUDED_)
