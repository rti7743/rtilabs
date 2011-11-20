#include "StartWithExeFile.h"
#ifdef TABCOONTROL_LIFE

#ifndef ___TTABCOONTROLX
#define ___TTABCOONTROLX
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_tabcontrol_event.h>

#define ALL_PAGE 0xffffffff

class TTabControl : public TTabControlObject
{
private:
	void ControlSetingPage();
public:
    TTabControl(TObject* hOwner,char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TTabControl();
    int Add(unsigned int mask,char* text);
	bool ControlReAllocPage(TObject* Obj,int page);
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};

#endif

#endif   // LIFE END
