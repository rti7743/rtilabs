// YBuildThread.h: YBuildThread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YBUILDTHREAD_H__0A4C3A3F_B509_410B_B289_D063F4929C63__INCLUDED_)
#define AFX_YBUILDTHREAD_H__0A4C3A3F_B509_410B_B289_D063F4929C63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHtmlTemplate.h"
#include "RSpinLock.h"
#include "YSettingTXT.h"
#include "RHTTPPOSTParser.h"
#include "YLocalRule.h"

class YBuildThread 
{
public:
	YBuildThread();
	virtual ~YBuildThread();

	const string make(POST& ioPOST, YLocalRule* inLocalRule) const;
	void Load(const string inBoardName,const YSettingTXT*	inSettingTXT);

private:
	RHtmlTemplate			Template;
	const YSettingTXT*		SettingTXT;
	string					BoardName;

	mutable RSpinLock			Lock;
};

#endif // !defined(AFX_YBUILDTHREAD_H__0A4C3A3F_B509_410B_B289_D063F4929C63__INCLUDED_)
