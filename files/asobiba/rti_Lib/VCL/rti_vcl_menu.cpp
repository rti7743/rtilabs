/**********************************************************/
// メニューの土台
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef MENU_LIFE

#include <.\VCL\rti_vcl_menu.h>
#include <.\VCL\rti_vcl_apprication.h>

void TMenu::CreateMenuObject(TObject* hOwner,char *Caption, DWORD dwStyle)
{
    //オーナー保存
    hMyOwner = hOwner;
    hWnd = NULL;
    if (hMyOwner==NULL || hMyOwner->GetKind() != OBJECT_KIND_MENU)
    { //トップ
        hWnd = CreateMenu();
        return;
    }
    //ポップアップを作る必要があるか..
    if(hMyOwner->GethWnd() == NULL)
    {
		char buf[256];
		UINT sstate;
        TObject *wDOwner = hMyOwner->GethOwer();
        //オーナーをホップアップ化させる.
	    ((TMenu*)hMyOwner)->SethWnd(CreatePopupMenu());
//        DEBUG3("確保 %d",(int)hMyOwner->GethWnd());

		//そのオーナーの文字列取得
		GetMenuString(wDOwner->GethWnd(),hMyOwner->GetId(),
			buf,255,MF_BYCOMMAND);
		//そのオーナーのステータス取得
		sstate =  GetMenuState(wDOwner->GethWnd(),
			hMyOwner->GetId(),MF_BYCOMMAND);
		//そのオーナーの属性を変更します
		ModifyMenu
			(wDOwner->GethWnd(),hMyOwner->GetId(),
			 MF_BYCOMMAND | MF_POPUP | sstate,
			 (UINT)hMyOwner->GethWnd(),buf);
//		((TMenu*)hMyOwner)->SetId ( (UINT)hMyOwner->GethWnd() ); 
    }
	this->Id = Application->GetId();
	//そうぢゃない ならば、最初の方は自分の親となるべきメニューである
    AppendMenu( hMyOwner->GethWnd(), dwStyle,(UINT)this->Id, Caption);

}

TMenu::TMenu(TMenu* whOwner,char *Caption,DWORD wdwStyle)
{
    MenuClick = NULL;

    CreateMenuObject(whOwner,Caption,wdwStyle);

    OwnerAdd(whOwner,this);

    Kind            = OBJECT_KIND_MENU;
}

TMenu::~TMenu()
{
    DestroyComponent();
}

/**********************************************************/
// 破棄
/**********************************************************/
void TMenu::DestroyComponent()
{
    OwerKill        = true;
    //自分の持っていたすべてのコントロールを閉じる
    ChildDestroy();
    //自分のオーナーに自分が消えることを通知
    IamDestroy();
    //メニュー破棄
    DestroyComponentMenu();
}

/**********************************************************/
// メニューの破棄
/**********************************************************/
void TMenu::DestroyComponentMenu()
{
    if(hWnd)
    { //自分はメニューハンドルを持っている.
//        DEBUG3("解放 %d",(int)hWnd);
        DestroyMenu(hWnd); //メニューハンドルを消滅させる.
    }
    else
    { //メニューハンドルを持たない項目である.
//        DeleteMenu(GethOwerWnd(),Id,MF_BYCOMMAND);
    }
    //自分のオーナーがメニューだったら
    if(hMyOwner && hMyOwner->GetKind() == OBJECT_KIND_MENU)
    { //ポップアップを失うかどうか検査
        if(hMyOwner->HowMenyHaveControls() == 0)
        { //オーナー(ポップアップ)はコントロールをすべて失いました.
//            PopUpCancle(hMyOwner); //ポップアップキャンセル
        }
    }
}

/**********************************************************/
// ポップアップキャンセル
/**********************************************************/
void TMenu::PopUpCancle(TObject *hOwner)
{
	char buf[256];
	UINT sstate;
    TObject *wDOwner = hOwner->GethOwer();
    //念のためそのオーナーのオーナーがメニューかどうか調べる
    if (!wDOwner || wDOwner->GetKind() != OBJECT_KIND_MENU) return ;

    //そのオーナーの文字列取得
	GetMenuString(wDOwner->GethWnd(),hOwner->GetId(),
		buf,255,MF_BYCOMMAND);
	//そのオーナーのステータス取得
	sstate =  GetMenuState(wDOwner->GethWnd(),
		hMyOwner->GetId(),MF_BYCOMMAND);
    //ポップアップ情報遮断
	sstate = sstate & ~(MF_POPUP) | MF_BYCOMMAND;

//	hMyOwner->SetId ( (UINT)hMyOwner->GetWnd() ); 
    //オーナーを一時的に消滅させる
    DestroyMenu(hOwner->GethWnd() ); //メニューハンドルを消滅させる.

    AppendMenu(wDOwner->GethWnd(), sstate,hOwner->GetId(),buf);

}


/**********************************************************/
//メニュー WM_COMMAND ディスパッチャ 
/**********************************************************/
void TMenu::WmCommandCome(WPARAM wParam)
{
    if(MenuClick) MenuClick(this);
//    DEBUG("Menu Event");
}


#endif // LIFE END

