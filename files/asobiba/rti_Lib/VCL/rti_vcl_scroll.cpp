/**********************************************************/
// スクロールバーの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef SCROLLBAR_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_scroll.h>

TScrollBar::TScrollBar(TObject* hOwner,
	DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"SCROLLBAR");
	//スクロールバーを作る
    if ( !CreateObject(hOwner,NULL,dwStyle,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_SCROLL;

}

TScrollBar::~TScrollBar()
{
	DestroyComponent();
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TScrollBar::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
