// RHTTPHeaderAttributeDate.h: RHTTPHeaderAttributeDate クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTTPHEADERATTRIBUTEDATE_H__72A6F2EC_47BB_48AC_81AA_C0695D4F15A3__INCLUDED_)
#define AFX_RHTTPHEADERATTRIBUTEDATE_H__72A6F2EC_47BB_48AC_81AA_C0695D4F15A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RHTTPHeaderAttributeDate  
{
public:
	RHTTPHeaderAttributeDate();
	virtual ~RHTTPHeaderAttributeDate();

	RHTTPHeaderAttributeDate& operator=(const char* inHeader);

	time_t getDate() const
	{
		return this->Date;
	}
	bool IsEnabled() const
	{
		return this->Enabled;
	}
private:
	time_t	Date;
	bool	Enabled;
};

#endif // !defined(AFX_RHTTPHEADERATTRIBUTEDATE_H__72A6F2EC_47BB_48AC_81AA_C0695D4F15A3__INCLUDED_)
