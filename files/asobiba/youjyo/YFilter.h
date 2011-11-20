// YFilter.h: YFilter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YFILTER_H__6ADC5428_D352_484B_A246_D7F995715958__INCLUDED_)
#define AFX_YFILTER_H__6ADC5428_D352_484B_A246_D7F995715958__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIPFilter.h"

class YFilter  
{
private:
	YFilter();		//singleton
public:
	virtual ~YFilter();

	void Load();
	bool IsJapanIP(in_addr inIP) const
	{
		return Japan.IsBan(inIP);
	}
	bool IsDenyIP(in_addr inIP) const
	{
		return Deny.IsBan(inIP);
	}

	static YFilter* getInstance();

private:
	RIPFilter	Japan;
	RIPFilter	Deny;
};

#endif // !defined(AFX_YFILTER_H__6ADC5428_D352_484B_A246_D7F995715958__INCLUDED_)
