// RTimer.h: RTimer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTIMER_H__F2C0A148_AD10_4F2C_8B05_6C0BA9AD5DBB__INCLUDED_)
#define AFX_RTIMER_H__F2C0A148_AD10_4F2C_8B05_6C0BA9AD5DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RTimer  
{
public:
	RTimer()
	{
		Window = NULL;
		TimerID = 0;
	}
	virtual ~RTimer()
	{
		Reset();
	}

	//失敗は 0 / 成功はタイマーID
	int Set(HWND inWin ,int inTimerID ,int inTime)
	{
		ASSERT(inWin != NULL);
		ASSERT(inTimerID != 0);
		ASSERT(inTime != 0);
		Reset();
		TimerID = ::SetTimer( inWin , inTimerID , inTime , NULL);
		Window = inWin;
		return TimerID;
	}
	//リセット.
	void Reset()
	{
		if (TimerID != 0)
			::KillTimer(Window , TimerID);
		TimerID = 0;
	}

private:
	HWND		Window;
	int			TimerID;

};

#endif // !defined(AFX_RTIMER_H__F2C0A148_AD10_4F2C_8B05_6C0BA9AD5DBB__INCLUDED_)
