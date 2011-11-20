// DisplayTextView.cpp: DisplayTextView クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayTextView.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DisplayTextView::DisplayTextView
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//メインとなるリストビューの作成.
	RichEdit.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL);

	//コールバック登録
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//サイズを変更
	this->WindowResize();

}

//ウィンドウをリサイズする
void DisplayTextView::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	RichEdit.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayTextView::~DisplayTextView()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProcを受け取る
//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
//自分は処理しない奴は false を返す。
bool DisplayTextView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( RichEdit.getWindowHandle() ) )		WindowResize();
		break;
//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}

//表示項目の設定
void DisplayTextView::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);

	try
	{
		RichEdit.ReadFile( inFilename );
	}
	catch(RIOCannotOpenException e)
	{
		printf("Error: %s\n" , e.getMessage() );
	}
}

//アクティヴになりました
void DisplayTextView::OnActive() const
{
	this->WindowResize();

	RichEdit.Show();

	::SetFocus( RichEdit.getWindowHandle() );
}

//非アクティヴになりました
void DisplayTextView::OnNoActive() const
{
	RichEdit.Hide();
}

