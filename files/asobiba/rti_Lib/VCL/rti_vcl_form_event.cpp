/**********************************************************/
// フォームのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef FORM_LIFE

#include <.\VCL\rti_vcl_form_event.h>
#include <.\VCL\rti_vcl_apprication.h>

/**********************************************************/
/*コンストラクタ*/
/**********************************************************/
TFormEvent::TFormEvent()
{
#ifdef MENU_LIFE
	MainMenu	= NULL;
	PopupMenu	= NULL;
#endif
    //フォームをモーダルで開いたときに情報が格納されます.
    //つまり、ここが NULL だった場合は、モーダルではないってことです.
    ShowModalUnEnabledWindows = NULL;
}

/**********************************************************/
/*デストラクタ*/
/**********************************************************/
TFormEvent::~TFormEvent()
{
    HideModal();
}

/**********************************************************/
/*フォームのモーダルを解除します*/
/**********************************************************/
void TFormEvent::HideModal()
{
    if (ShowModalUnEnabledWindows == NULL) return; //モーダルではない
    for(int i = 0; i <= ShowModalUnEnabledWindowsCounter ; i++)
    {
        HWND hwnd = ShowModalUnEnabledWindows[i];
        if(IsWindow(hwnd))
        { //そのウィンドはまだ有効
            //有効にする
            EnableWindow(hwnd,TRUE);
        }
    }
	GlobalFree(ShowModalUnEnabledWindows);
    ShowModalUnEnabledWindows = NULL;

    //自分を無効にします.
    ShowWindow(GethWnd(),SW_HIDE);
}

/**********************************************************/
/*フォームをモーダルで開きます*/
/**********************************************************/
void TFormEvent::ShowModal()
{
    //自分以外のすべてのウィンドハンドルを取得しておきます.
    //また、自分以外をすべてのフォームを無効にします.
    int i = 0;
    ObjectIT it;
    HWND hwnd;

    ShowModalUnEnabledWindowsCounter = Application->HowMenyHaveControls();
	ShowModalUnEnabledWindows = (HWND*) 
        GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
			sizeof(HWND) * (ShowModalUnEnabledWindowsCounter +1) );

    
    for(it=Application->GetComponentFarst();
        it != Application->GetComponentEnd() ; it++)
        {
            hwnd = (HWND)( (*it)->GethWnd() );
            if(this->hWnd != hwnd)
            { //自分自身と比べないように...
				if (IsWindow(hwnd) && IsWindowEnabled(hwnd) && IsWindowVisible(hwnd))
				{
					ShowModalUnEnabledWindows[i++] = hwnd;
					//無効にする
					EnableWindow(hwnd,FALSE);
				}
            }
        }
    //自分を有効にします.
    ShowWindow( GethWnd() ,SW_SHOW);
}

#ifdef MENU_LIFE
/*******************************************************************/
//メニュー
/*******************************************************************/
void TFormEvent::SetMenu(TMenu *menu)
{
	MainMenu	= menu;
//    SetMenu(menu);
    DrawMenuBar(hWnd);
}

/*******************************************************************/
//ポップアップメニュー
/*******************************************************************/
void TFormEvent::SetPopupMenu(TMenu *menu,UINT flg )
{
	PopupMenu	= menu;
	PopupFlg    = flg;
}

/*******************************************************************/
//ポップアップメニューを表示
/*******************************************************************/
void TFormEvent::ShowPopupMenu(int x,int y,TMenu *menu)
{
	TrackPopupMenu(PopupMenu->GethWnd() , NULL,
		x, y, 0 ,hWnd, NULL);
}
#endif

/*******************************************************************/
//ポップアップメニューを自動的に動作するサービス
/*******************************************************************/
void TFormEvent::PopupMenuAutoShow(UINT pos,UINT flg)
{
#ifdef MENU_LIFE
	if (PopupMenu)
	{ //ポップアップメニューあり、表示していい.
		UINT disflg = PopupFlg & AUTOPOPUP_DISPLAYFLGS;
		if( (disflg&flg) == disflg)
		{
			POINT p;
			p.x  = LOWORD(pos);
			p.y  = HIWORD(pos);
			if (PopupFlg & AUTOPOPUP_CLIENTADD)
			{ //座標変換
				ClientToScreen(hWnd,&p);
			}
			ShowPopupMenu(p.x,p.y,PopupMenu);
		}
	}
#endif
}

#endif
