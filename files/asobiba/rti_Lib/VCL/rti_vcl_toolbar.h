#include "StartWithExeFile.h"
#ifdef TOOLBAR_LIFE

#ifndef ___TTOOLBAR_LIFEX
#define ___TTOOLBAR_LIFEX
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_toolbar_event.h>

#define USER_DEFINED 0xffffffff

class TToolBar : public TToolBarEvent
{
private:
public:
    TToolBar(TObject* hOwner,char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TToolBar();
    int AddBitMap(UINT nID,char *filename,int pace);
    int Add(int iBitmap,BYTE fsState,BYTE fsStyle,DWORD dwData,
                   int iString,SimpleEvent CallRtn);
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};

#endif

#endif   // LIFE END
