/**********************************************************/
// リストボックスの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef LIST_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_list.h>

TListBox::TListBox(TObject* hOwner,char *Caption,
				   DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"LISTBOX");
	//リストボックスを作る
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_LISTBOX;
}

TListBox::~TListBox()
{
	DestroyComponent();
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TListBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}


#endif //LIFE END
