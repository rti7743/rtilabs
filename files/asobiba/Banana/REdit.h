// REdit.h: REdit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REDIT_H__00E033E7_D1AF_494C_A30B_6C7B303AC895__INCLUDED_)
#define AFX_REDIT_H__00E033E7_D1AF_494C_A30B_6C7B303AC895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindowBase.h"

class REdit : public RWindowBase  
{
public:
	REdit();
	virtual ~REdit();

	//作成. オーバーライド
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
};

#endif // !defined(AFX_REDIT_H__00E033E7_D1AF_494C_A30B_6C7B303AC895__INCLUDED_)
