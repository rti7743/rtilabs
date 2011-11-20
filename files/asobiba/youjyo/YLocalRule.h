// YLocalRule.h: YLocalRule クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YLOCALRULE_H__CC5969FE_2B2B_4264_9756_AEF1DA940F72__INCLUDED_)
#define AFX_YLOCALRULE_H__CC5969FE_2B2B_4264_9756_AEF1DA940F72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RSpinLock.h"

class YLocalRule  
{
public:
	YLocalRule();
	virtual ~YLocalRule();

	//ローカルルールの読み込み.
	void Load(const string inBoardName);

	const string getLocalRule()  const
	{
		RAutoReadSpinLock al(&Lock);
		return this->LocalRule;
	}

private:
	string		LocalRule;
	mutable RSpinLock	Lock;
};

#endif // !defined(AFX_YLOCALRULE_H__CC5969FE_2B2B_4264_9756_AEF1DA940F72__INCLUDED_)
