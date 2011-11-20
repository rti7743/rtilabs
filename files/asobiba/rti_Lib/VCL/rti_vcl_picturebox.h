#include "StartWithExeFile.h"
#ifdef PICTUREBOX_LIFE

#ifndef TPICTUREBOXH
#define TPICTUREBOXH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_groupbox.h>
#include <.\Graphics\rti_graphics.h>

class TPictureBox : public TGroupBox
{
private:
	HDC memDC;
	HBITMAP hBit;
	void DestroyPicture();
	int w,h;
public:
    TPictureBox(TObject* hOwner,char *Caption,
		DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp = StaticProc);
    ~TPictureBox();
	void SetPicture(char *FileName);
	void SetPicture(BMD bmd);
    //VCLによるサブクラス化で実行されるプロシージャ
	//ディスパッチャー
	virtual LRESULT UserDisPacth
		(HWND hwnd , UINT message,WPARAM wParam,LPARAM lParam);
    //終了させる
    virtual void UserDestroy(){delete this;};
};

#endif


#endif //LIFE END
