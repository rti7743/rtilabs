#include "StartWithExeFile.h"
#ifdef SCROLLBAR_LIFE

#ifndef ___TSCROLLH
#define ___TSCROLLH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_scroll_event.h>

class TScrollBar : public TScrollEvent
{
public:
	TScrollBar(TObject* hOwner,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TScrollBar();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};

#define ScrollSetRange(Object,Min,Max) SetScrollRange(Object->hWnd,Min,Max,TRUE)

#endif


#endif //LIFE END
