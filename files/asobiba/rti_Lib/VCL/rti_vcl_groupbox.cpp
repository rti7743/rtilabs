/**********************************************************/
// グループボックスの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef GROUPBOX_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_groupbox.h>

TGroupBox::TGroupBox(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"BUTTON");
	//ボタンを作る
    if (!CreateObject(hOwner,Caption,dwStyle |BS_GROUPBOX,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_GROUPBOX;

}

TGroupBox::~TGroupBox()
{
	DestroyComponent();
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TGroupBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
