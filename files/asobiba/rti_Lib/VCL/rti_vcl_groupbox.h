#include "StartWithExeFile.h"
#ifdef GROUPBOX_LIFE

#ifndef TGROUPBOXH
#define TGROUPBOXH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_button_event.h>

class TGroupBox : public TButtonEvent
{
private:
public:
    TGroupBox(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TGroupBox();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    virtual void UserDestroy(){delete this;};
};


#endif


#endif //LIFE END
