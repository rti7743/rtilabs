/**********************************************************/
// フォームの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef FORM_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_form.h>
#include <.\Base\rti_debug.h>

TForm::TForm(TObject* hOwner,char *Caption,DWORD dwStyle,
             int x,int y,int w,int h,char *classname,WNDPROC UserWndp)
{
    //クラスを作る
    if (classname==NULL)
    {
        CreateOlignalClass(UserWndp);
    }
    else lstrcpy(ClassName,classname);

	//フォームを作る
    if (! CreateObject(hOwner,Caption,dwStyle,x,y,w,h,true,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_FORM;

}

TForm::~TForm()
{
	DestroyComponent();
    if (Application->GethWnd() == hWnd)
    {
		PostQuitMessage(0);   //終了メッセージの発行
    }
}
    
/**********************************************************/
/* ぴったりサイズに変更  */
/**********************************************************/
void TForm::ReClasSize(int w , int h )
{
	DWORD dwStyle = GetWindowLong(GethWnd(),GWL_STYLE);
	BOOL fMenu = FALSE;	//ディフォではメニューなし.

#ifdef MENU_LIFE	//メニューを使う?
	if (MainMenu) fMenu = TRUE;	//で、なおかつメニューある?
#endif
	RECT Oldrc;
	//現在の位置の取得.
	GetWindowRect( GethWnd() ,&Oldrc);

	Oldrc.right  = w + Oldrc.left;
	Oldrc.bottom = h + Oldrc.top; 
	AdjustWindowRect( &Oldrc, dwStyle, fMenu );
	Oldrc.right  -= Oldrc.left;
	Oldrc.bottom -= Oldrc.top; 
	SetWindowPos( GethWnd() ,HWND_TOP	, Oldrc.left,Oldrc.top,Oldrc.right,Oldrc.bottom ,SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOCOPYBITS);

}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TForm::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
	UINT AutoPopupFlg = 0;
    switch(message)
    {
	case WM_PARENTNOTIFY:
		switch (LOWORD(wParam))
		{
		case WM_LBUTTONDOWN	:
			PopupMenuAutoShow(lParam,MK_RBUTTON	 | AUTOPOPUP_PARENT);
			break;
		case WM_RBUTTONDOWN	:
			PopupMenuAutoShow(lParam,MK_LBUTTON	 | AUTOPOPUP_PARENT);
		}
		break;
    case WM_CREATE:
        return 0;
        break;
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}
#endif //LIFE END
