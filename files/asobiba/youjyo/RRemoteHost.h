// RRemoteHost.h: RRemoteHost クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RREMOTEHOST_H__B25E702A_0824_4F7A_AAC6_99FF03EC7390__INCLUDED_)
#define AFX_RREMOTEHOST_H__B25E702A_0824_4F7A_AAC6_99FF03EC7390__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RRemoteHost  
{
public:
	RRemoteHost();
	virtual ~RRemoteHost();

	const string getIP() const
	{
		return RemoteIP;
	}
	const in_addr getRawIP() const
	{
		return RawIP;
	}
	void setIP(in_addr inAddr);
private:
	string RemoteIP;
	in_addr	RawIP;
};

#endif // !defined(AFX_RREMOTEHOST_H__B25E702A_0824_4F7A_AAC6_99FF03EC7390__INCLUDED_)
