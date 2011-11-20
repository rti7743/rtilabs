// RTaskViewWin32.cpp: RTaskViewWin32 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTaskViewWin32.h"
#include "WindowsMain.h"

#include "RDoNotInitaliseException.h"

#include "resource.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RTaskViewWin32::RTaskViewWin32() throw()
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();

	this->WindowHandle =
		::CreateDialog(mainSystem->getInstance() , "TASKVIEW", mainSystem->getWindowHandle() , (DLGPROC)DlgProcStatic);
	if (this->WindowHandle == NULL)
	{
        throw RDoNotInitaliseException("タスク管理ダイアログの作成に失敗 ErrorCode:%ld",::GetLastError() );
	}
	//class にコールバックするためにサブクラス化
	::SetWindowLong( this->WindowHandle , GWL_USERDATA,  (long)this);

	TaskList.MountWindow( ::GetDlgItem(this->WindowHandle , TASKLIST) );

	RECT rc;
	TaskList.GetClientRect(&rc);

	TaskList.InsertColumn( 0 , 0 , "ID" , rc.right * 0.20 , LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT ); 
	TaskList.InsertColumn( 1 , 0 , "進捗" , rc.right * 0.20 , LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT ); 
	TaskList.InsertColumn( 2 , 0 , "内容" , rc.right * 0.60 , LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT ); 
	//一行選択だったら、 Fixed をドラッグできたりできるようにします。
	TaskList.setExtended( TaskList.getExtended() | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );

	::ShowWindow( WindowHandle , SW_SHOW);

}

RTaskViewWin32::~RTaskViewWin32()
{
	::DestroyWindow( this->WindowHandle );
}

//新しいタスクを追加します。
void RTaskViewWin32::InsertTask(unsigned long inID,string inMessage)
{
	RAutoLock al(&Lock);

	int last = TaskList.GetItemCount();
	TaskList.SetItem( last,0 , inID);
	TaskList.SetItem( last,1 , 0);
	TaskList.SetItem( last,2 , inMessage.c_str());
}

//進捗の更新
void RTaskViewWin32::UpdateGoing(unsigned long inID,int inParsent)
{
	RAutoLock al(&Lock);

	//検索.
	int subitem = TaskList.SearchItem(0,inID);
	if (subitem == -1) return ;	//もう、ねーよ

	//変更しる!
	TaskList.SetItem(1 , subitem , inParsent);
}

//タスクの削除
void RTaskViewWin32::DeleteTask(unsigned long inID)
{
	RAutoLock al(&Lock);

	//検索.
	int subitem = TaskList.SearchItem(0,inID);
	if (subitem == -1) return ;	//もう、ねーよ

	//削除命令発行!
	TaskList.DeleteItem(subitem);
}


//ウィンドウプロシージャ.
LRESULT RTaskViewWin32::DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		return TRUE;
	}
	return FALSE;
}


//サイズが変更されたときに呼ばれる.
void RTaskViewWin32::OnSize() 
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	Filer * filer = Filer::getFiler();

	//フォーカスを調べる.
	bool left = filer->IsLeftFocus();

	//現在のフォーカスの反対側のウィンドウのサイズの取得.
	RECT MyClientRect,MyWindowRect,baseWindowRC;
	::GetClientRect( mainSystem->getBaseWindow( !left) , &baseWindowRC);
	::GetWindowRect( this->WindowHandle , &MyWindowRect);
	::GetClientRect( this->WindowHandle , &MyClientRect);

	//そのウィンドウの下が、このウィンドウの下の辺と同じになるように配置します。

	int windowWidth = MyWindowRect.right - MyWindowRect.left;
	int windowHeigh = MyWindowRect.bottom - MyWindowRect.top;
	MoveWindow( this->WindowHandle ,
		baseWindowRC.left , baseWindowRC.bottom - windowHeigh ,
		windowWidth , windowHeigh , TRUE );
}

