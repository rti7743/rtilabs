#include "StartWithExeFile.h"
#ifdef CHECKBUTTON_LIFE

#ifndef ___TCHECKBUTTONH
#define ___TCHECKBUTTONH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_button_event.h>

class TCheckButton : public TButtonEvent
{
public:
    TCheckButton(TObject* hOwner,char *Caption,	DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TCheckButton();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};

#define CHECKBUTTON(object) ((TCheckButton*)(object))

#endif

#endif //LIFE END
