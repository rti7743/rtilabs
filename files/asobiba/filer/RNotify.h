// RNotify.h: RNotify クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNOTIFY_H__F9E0A432_2018_48B1_952F_1189A1FE1649__INCLUDED_)
#define AFX_RNOTIFY_H__F9E0A432_2018_48B1_952F_1189A1FE1649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WindowsMain.h"

#define WM_TASKEND	(WM_USER+1000)	//タスクの終了.

template <class FILES , class WIN>
class RNotify  
{
public:
	RNotify()
	{
	}
	virtual ~RNotify()
	{
	}


	void CallBack(FILES* inFiler , WIN* inWin)
	{
		::PostMessage( WindowsMain::getMainSystem()->getWindowHandle() ,
			WM_TASKEND , (WPARAM)inFiler , (LPARAM)inWin );
			
	}

};

#endif // !defined(AFX_RNOTIFY_H__F9E0A432_2018_48B1_952F_1189A1FE1649__INCLUDED_)
