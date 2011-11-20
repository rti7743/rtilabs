#include "StartWithExeFile.h"
#ifdef EDIT_LIFE

#ifndef ___TEDITH
#define ___TEDITH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_edit_event.h>

class TEdit : public TEditEvent
{
public:
    TEdit(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TEdit();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};


#endif


#endif //LIFE END
