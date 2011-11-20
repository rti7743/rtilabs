#include "StartWithExeFile.h"
#ifdef CHECKBOX_LIFE

#ifndef TCHECKBOXH
#define TCHECKBOXH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_groupbox.h>
#include <.\VCL\rti_vcl_checkbotton.h>
#include <.\VCL\rti_vcl_button_event.h>


class TCheckBox : public TGroupBox
{
private:
	int ButtonPossion;
public:
    TCheckBox(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TCheckBox();
	TCheckButton* Add(char *Caption,DWORD dwStyle);
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};

    unsigned long GetIndex();
    void SetIndex(unsigned long bit);

    void (*OnChange)(TObject *my ,TObject *you);
};

#endif


#endif //LIFE END
