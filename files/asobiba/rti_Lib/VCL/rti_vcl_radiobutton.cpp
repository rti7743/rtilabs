/**********************************************************/
// ラジオボタンの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef RADIOBUTTON_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_radiobutton.h>

TRadioButton::TRadioButton(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"BUTTON");
	//ボタンを作る
    if ( !CreateObject(hOwner,Caption,dwStyle | BS_AUTORADIOBUTTON,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_RADIOBUTTON;

}

TRadioButton::~TRadioButton()
{
	DestroyComponent();
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TRadioButton::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
