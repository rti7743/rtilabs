#include "StartWithExeFile.h"
#ifdef RADIOBOX_LIFE

#ifndef TRADIOBOXH
#define TRADIOBOXH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_groupbox.h>
#include <.\VCL\rti_vcl_radiobutton.h>
#include <.\VCL\rti_vcl_button_event.h>


class TRadioBox : public TGroupBox
{
private:
	int ButtonPossion;
    unsigned long GetIndexMain(bool ByteReturn);
public:
    TRadioBox(TObject* hOwner,char *Caption,DWORD dwStyle,
		int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TRadioBox();
	TRadioButton* Add(char *Caption,DWORD dwStyle);
    //連続配置が必要なコンポーネントのコントロールの再配置を促す
    virtual void RefControl();
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};

    int GetIndexInt(){return (int) GetIndexMain(false);};
    unsigned long GetIndex(){return GetIndexMain(true);};
    void SetIndex(unsigned long bit);

    void (*OnChange)(TObject *my ,TObject *you);
};

#endif


#endif //LIFE END
