#include "StartWithExeFile.h"
#ifdef RADIOBUTTON_LIFE

#ifndef ___TRADIOBUTTONH
#define ___TRADIOBUTTONH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_button_event.h>

class TRadioButton : public TButtonEvent
{
public:
    TRadioButton(TObject* hOwner,char *Caption,
		DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TRadioButton();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};

#define RADIOBUTTON(object) ((TRadioButton*)(object))

#endif


#endif //LIFE END
