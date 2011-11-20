// DisplayWebBrowserView.cpp: DisplayWebBrowserView クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayWebBrowserView.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


DisplayWebBrowserView::DisplayWebBrowserView
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//メインとなるビューの作成.
	WebBrowser.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_VISIBLE | WS_BORDER );
	//親のセット.
	WebBrowser.setParent(this);
	//コールバック登録
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//サイズを変更
	this->WindowResize();

}

//ウィンドウをリサイズする
void DisplayWebBrowserView::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	WebBrowser.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayWebBrowserView::~DisplayWebBrowserView()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProcを受け取る
//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
//自分は処理しない奴は false を返す。
bool DisplayWebBrowserView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( WebBrowser.getWindowHandle() ) )		WindowResize();
		break;
//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}

//表示項目の設定
void DisplayWebBrowserView::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);

	WebBrowser.Navigate2( inFilename );
}

//アクティヴになりました
void DisplayWebBrowserView::OnActive() const
{
	this->WindowResize();

	WebBrowser.Show();

	::SetFocus( WebBrowser.getWindowHandle() );
}

//非アクティヴになりました
void DisplayWebBrowserView::OnNoActive() const
{
	WebBrowser.Hide();
}


//こちらから指示したファイル以外のファイルをオープンしようとされた場合
//ブラウザの場合、別のリンクを踏んだ場合です
void DisplayWebBrowserView::OnQuickChange(string inPath)
{
	ASSERT(this->RfileOne);
	//どうせ、 RfilePreviewOne なので書き換えても問題なし.
	((RfilePreviewOne*)this->RfileOne)->Attach(inPath.c_str() );

	Win->OnQuickChange(inPath);
}


//タイトルが変更された場合
void DisplayWebBrowserView::OnTitleChange()
{
	Win->OnTitleChange();
}
