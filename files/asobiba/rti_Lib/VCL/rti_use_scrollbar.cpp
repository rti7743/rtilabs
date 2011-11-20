/*****************************************************************/
//スクロールバーを作ったとくに使うやつ
/*****************************************************************/
// rti_vcl_scroll とは別個に作るのは、スクロールバーの性質からの問題
// ここの HWND の引数は、スクロールバーがあるウィンドのハンドル
//						 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "StartWithExeFile.h"
#ifdef SCROLLBAR_USE_LIFE
#include <windows.h>
#include <.\VCL\rti_use_scrollbar.h>

/*****************************************************************/
//スクロールバーイベント
/*****************************************************************/
void ScrollProc(HWND hwnd,int ScrollFlg,
	int wParam,short Range , short *ScreenPos,ScrollFeedBackProc FeedBackProc)
{
	short sUdLr;//増分
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:        //スクロール矢印
			sUdLr = -1;
			break;
		case SB_LINEDOWN:      //スクロール矢印
			sUdLr = 1;
			break;
		case SB_PAGEUP:        //スクロールシャフト 
            sUdLr = -10;
			break;
		case SB_PAGEDOWN:      //スクロールシャフト 
            sUdLr = 10;
			break;
		case SB_THUMBTRACK:    //スクロールつまみが押された
			sUdLr = 0;
			break;
		case SB_THUMBPOSITION: //離されたとき
            sUdLr = HIWORD(wParam)-*ScreenPos;
			break;
		default:
			sUdLr = 0;
			break;
		}
        sUdLr = max(-(*ScreenPos), min(sUdLr, Range - *ScreenPos ));
        if (sUdLr != 0) {
                *ScreenPos += sUdLr;
                SetScrollPos(hwnd, ScrollFlg, *ScreenPos, TRUE);

				//フィードバックルーチンに伝える
				FeedBackProc(hwnd,ScrollFlg,sUdLr,*ScreenPos);
//                ScrollWindow(hwnd, 0, -sUdLr, NULL, NULL);
//                UpdateWindow(hwnd);
        }
}
 
void ScrollReSize(HWND hwnd,int ScrollFlg,
				int lParam,short *Range , short *ScreenPos)
{
	if (ScrollFlg & SB_VERT)
	{ //縦
	    *Range = GetSystemMetrics(SM_CYSCREEN) - HIWORD(lParam);
	}
	else
	{ //横ということにする(手抜き)
	    *Range = GetSystemMetrics(SM_CXSCREEN) - LOWORD(lParam);
	}
    *ScreenPos = min(*ScreenPos, *Range);
    SetScrollRange(hwnd, ScrollFlg, 0, *Range, FALSE);
    SetScrollPos(hwnd, ScrollFlg, *ScreenPos, TRUE);
}

#endif
