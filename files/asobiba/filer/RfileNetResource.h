// RfileNetResource.h: RfileNetResource クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILENETRESOURCE_H__5B34B5B9_3D77_4D9A_8E7C_8AAEE3F365D2__INCLUDED_)
#define AFX_RFILENETRESOURCE_H__5B34B5B9_3D77_4D9A_8E7C_8AAEE3F365D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rfile.h"

class RfileNetResource : public Rfile  
{
public:
	RfileNetResource();
	virtual ~RfileNetResource();

	//登録します。
	virtual void Attach(const char* inName,const char* inComment,DWORD inDisplayType ) = 0;
};

#endif // !defined(AFX_RFILENETRESOURCE_H__5B34B5B9_3D77_4D9A_8E7C_8AAEE3F365D2__INCLUDED_)
