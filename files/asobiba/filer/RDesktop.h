// RDesktop.h: RDesktop クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDESKTOP_H__2CE1BC94_FB65_4E69_9662_237AF08E3140__INCLUDED_)
#define AFX_RDESKTOP_H__2CE1BC94_FB65_4E69_9662_237AF08E3140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWindow.h"

//ゴミ
class RDesktop : public RWindow  
{
public:
	RDesktop();
	virtual ~RDesktop();

};

#endif // !defined(AFX_RDESKTOP_H__2CE1BC94_FB65_4E69_9662_237AF08E3140__INCLUDED_)
