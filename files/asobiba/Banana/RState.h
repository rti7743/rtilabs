// RState.h: RState クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTATE_H__93D255DB_ED9B_4EF4_AD47_C6F4AFB61EFE__INCLUDED_)
#define AFX_RSTATE_H__93D255DB_ED9B_4EF4_AD47_C6F4AFB61EFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RWindowBase.h"

class RState : public RWindowBase  
{
public:
	RState();
	virtual ~RState();

	//作成. オーバーライド
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();

};

#endif // !defined(AFX_RSTATE_H__93D255DB_ED9B_4EF4_AD47_C6F4AFB61EFE__INCLUDED_)
