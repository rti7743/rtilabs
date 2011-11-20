// DisplayPDFView.cpp: DisplayPDFView クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayPDFView.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DisplayPDFView::DisplayPDFView
	(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{

	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//メインとなるリストビューの作成.
	PDFView.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL);

	//コールバック登録
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;

	//サイズを変更
	this->WindowResize();

}

//ウィンドウをリサイズする
void DisplayPDFView::WindowResize() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	PDFView.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayPDFView::~DisplayPDFView()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileOne = NULL;
}

//WindowProcを受け取る
//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
//自分は処理しない奴は false を返す。
bool DisplayPDFView::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( PDFView.getWindowHandle() ) )		WindowResize();
		break;
//	case WM_NOTIFY:
//		{
//		}
//		break;
	}
	return false;
}

//表示項目の設定
void DisplayPDFView::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ASSERT(inSize == 1);
	this->RfileOne = inRfileItems[0];

	string inFilename = this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);

	try
	{
		PDFView.Open( inFilename );
	}
	catch(RIOCannotOpenException e)
	{
		printf("Error: %s\n" , e.getMessage() );
	}
}

//アクティヴになりました
void DisplayPDFView::OnActive() const
{
	this->WindowResize();

	PDFView.Show();

	::SetFocus( PDFView.getWindowHandle() );
}

//非アクティヴになりました
void DisplayPDFView::OnNoActive() const
{
	PDFView.Hide();
}

