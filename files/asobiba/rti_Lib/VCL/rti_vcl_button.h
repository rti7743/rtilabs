#include "StartWithExeFile.h"
#ifdef BUTTON_LIFE

#ifndef ___TBUTTONH
#define ___TBUTTONH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_base.h>
#include <.\VCL\rti_vcl_button_event.h>

class TButton : public TButtonEvent 
{
public:
    TButton(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TButton();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};



#endif


#endif //LIFE END
