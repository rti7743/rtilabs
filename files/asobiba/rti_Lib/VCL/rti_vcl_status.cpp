/**********************************************************/
//ステータスバーの土台
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef STATUS_LIFE

#include <.\VCL\rti_vcl_status.h>

TStatusBar::TStatusBar(TObject* hOwner,char *Caption,DWORD dwStyle,
					   int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"STATUSCLASSNAME");
	//ステータスバー
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_STATUS;
}

TStatusBar::~TStatusBar()
{
	DestroyComponent();
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TStatusBar::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
