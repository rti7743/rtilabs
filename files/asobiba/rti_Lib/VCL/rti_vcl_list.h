#include "StartWithExeFile.h"
#ifdef LIST_LIFE

#ifndef ___TLISTBOXH
#define ___TLISTBOXH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_list_event.h>

class TListBox : public TListBoxEvent
{
public:
    TListBox(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TListBox();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};



#endif


#endif //LIFE END
