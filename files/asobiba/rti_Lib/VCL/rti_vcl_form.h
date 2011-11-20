#include "StartWithExeFile.h"
#ifdef FORM_LIFE

#ifndef ___TFORMH
#define ___TFORMH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_form_event.h>

class TForm : public TFormEvent
{
public:
    TForm(TObject* hOwner,char *Caption,DWORD dwStyle,int x,int y,int w,int h,char *classname,WNDPROC UserWndp = StaticProc);
    ~TForm();
	// ぴったりサイズに変更  
	void ReClasSize(int w , int h );
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};


#endif


#endif //LIFE END
