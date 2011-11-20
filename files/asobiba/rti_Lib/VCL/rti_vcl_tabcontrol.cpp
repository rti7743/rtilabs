/**********************************************************/
// タブコントロールの土台
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef TABCOONTROL_LIFE
#include <.\VCL\rti_vcl_tabcontrol.h>

TTabControl::TTabControl(TObject* hOwner,char *Caption,
				 DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,WC_TABCONTROL);
	//タブコントロール
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_TABCONTROL;
}

TTabControl::~TTabControl()
{
	DestroyComponent();
}

int TTabControl::Add(unsigned int mask,char* text)
{
    static int page = -1;
    TC_ITEM tcItem;

    page++;
    tcItem.mask = mask;
    tcItem.pszText = text;
    SendMessage(this->hWnd, TCM_INSERTITEM, (WPARAM)page, (LPARAM)&tcItem);

    return page;
}
/**********************************************************/
// コントロールを指定したページと関連付けます
/**********************************************************/
bool TTabControl::ControlReAllocPage(TObject* Obj,int page)
{
	if(Obj->GethOwer() != this)
	{ //自分をオーナーとして作成していないコンポーネントは知らん
		return false;
	}
	//オーナーが使用する領域を使用させてもらう.
	Obj->SetOwerUse(page); //このページで使うんだぴょん
	                          // 0 は無効なので + 1 する.
	return true;
}

/**********************************************************/
// コントロールを指定したページを表示します
/**********************************************************/
void TTabControl::ControlSetingPage()
{
    TBaseObject* TheObject;

    ObjectIT it;
    for(it=GetComponentFarst(); it != GetComponentEnd() ; it++)
    {
		TheObject = (TBaseObject*) (*it);
		if (TheObject->GetOwerUse()==GetPage() ||
			           TheObject->GetOwerUse()==ALL_PAGE)
		{ //現在表示されているページ
            TheObject->Show();
            TheObject->Enabled();
		}
		else
		{
            TheObject->Hide();
            TheObject->UnEnabled();
		}
    }
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TTabControl::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    case WM_PAINT:
		//コントロールを有効、無効にする
    	ControlSetingPage();
        break;
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}
#endif //LIFE END
