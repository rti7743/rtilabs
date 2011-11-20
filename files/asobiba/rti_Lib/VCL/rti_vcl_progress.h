#include "StartWithExeFile.h"

#ifdef PROGRESS_LIFE

#ifndef ___TPROGRESSH
#define ___TPROGRESSH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_progress_event.h>

class TProgress : public TProgressEvent
{
public:
	void WmCommandCome(WPARAM wParam);
    TProgress(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TProgress();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};

#endif

#endif //LIFE END
