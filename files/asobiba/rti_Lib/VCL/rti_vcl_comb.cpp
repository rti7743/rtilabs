/**********************************************************/
// コンボボックスの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef COMB_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_comb.h>
/*
        コンボボックスが挙動不審の場合は、
        縦幅を増やしてあげてみてね(^^;;;

        コンボボックスのばかぁ　(怒

*/


TComboBox::TComboBox(TObject* hOwner,char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"COMBOBOX");
    if (!CreateObject(hOwner,Caption,dwStyle | CBS_DROPDOWN | WS_VSCROLL,x,y,w,h,false,StaticProc) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_COMBOBOX;
}

TComboBox::~TComboBox()
{
	DestroyComponent();
}



/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TComboBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
