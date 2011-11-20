// DisplayNiseOLE.cpp: DisplayNiseOLE クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayNiseOLE.h"

DisplayNiseOLE::DisplayNiseOLE
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	//コールバック登録
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//サイズを変更
	this->WindowResize();

}

//ウィンドウをリサイズする
void DisplayNiseOLE::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	NiseOLE.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);

}

DisplayNiseOLE::~DisplayNiseOLE()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProcを受け取る
//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
//自分は処理しない奴は false を返す。
bool DisplayNiseOLE::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( NiseOLE.getWindowHandle() ) )		WindowResize();
		break;
	case WM_PAINT:
	case WM_ERASEBKGND:
	case WM_SETFOCUS:
	case WM_SHOWWINDOW:
		//PostMessage(NiseOLE.getWindowHandle() , msg , wparam ,lparam);
		::InvalidateRect(NiseOLE.getWindowHandle()  , NULL , FALSE);
		break;

//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}


//表示項目の設定
void DisplayNiseOLE::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	const string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);
	NiseOLE.Exec(mainSystem->getInstance(),	mainSystem->getBaseWindow( Win->getIsLeft() ),inFilename);

	return ;
}

//アクティヴになりました
void DisplayNiseOLE::OnActive() const
{
	this->WindowResize();

	NiseOLE.Show();

	::SetFocus( NiseOLE.getWindowHandle() );
	//再描画
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	::InvalidateRect(mainSystem->getWindowHandle() , NULL ,TRUE);
}

//非アクティヴになりました
void DisplayNiseOLE::OnNoActive() const
{
	NiseOLE.Hide();
}

