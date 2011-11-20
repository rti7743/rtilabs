#include "StartWithExeFile.h"
#ifdef SCROLLBAR_USE_LIFE

#ifndef ___SCROLLBAR_USEH
#define ___SCROLLBAR_USEH
#include <windows.h>

typedef void (*ScrollFeedBackProc)
			(HWND hwnd,int ScrollFlg,short sUdLr,short pos);

//スクロールバー
void ScrollProc(HWND hwnd,int ScrollFlg,
		int wParam,short Range , short *ScreenPos,ScrollFeedBackProc FeedBackProc);
void ScrollReSize(HWND hwnd,int ScrollFlg,
				int lParam,short *Range , short *ScreenPos);

#endif

#endif //LIFE END
