// DisplayWin32.cpp: DisplayWin32 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayWin32.h"
#include "RFunctionScript.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DisplayWin32::DisplayWin32(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{
	ASSERT(Win != NULL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//メインとなるリストビューを作ります。
	ListView.Create(mainSystem->getInstance(), 
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_TABSTOP | LVS_REPORT | LVS_OWNERDATA | WS_VISIBLE);

	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	//一行選択だったら、 Fixed をドラッグできたりできるようにします。
	ListView.setExtended( ListView.getExtended() | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );

	//真中に移動する.
	this->HarfWindow();
}

//ウィンドウを半分の位置に移動する.
void DisplayWin32::HarfWindow() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	ListView.MoveWindow(baseWindowRC.left , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);

}

DisplayWin32::~DisplayWin32()
{
	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileItems = NULL;
	this->RfileItemsSize = 0;
}

//WindowProcを受け取る
//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
//自分は処理しない奴は false を返す。
bool DisplayWin32::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)lparam;
			if (nmhdr->hwndFrom != ListView.getWindowHandle() )	return false;

			switch(nmhdr->code)
			{
			case LVN_GETDISPINFO:
				{
					if (this->OnGetDispInfo((NMLVDISPINFO*) lparam) )
					{
						*outRet = 0;
						return true;
					}
				}
				break;
			case NM_DBLCLK:	//ダブルクリックされたとき
				{
					if (this->OnDoubleClick() )
					{
						*outRet = 0;
						return true;
					}
				}
				break;
			case LVN_COLUMNCLICK:	//カラムをクリックされたとき
				{
					if (this->OnColumnClick((LPDRAWITEMSTRUCT) lparam ) )
					{
						*outRet = 0;
						return true;
					}
				}
				break;
			case LVN_KEYDOWN:
				{
					if (this->OnKeyDown((LPNMKEY) lparam) )
					{
						*outRet = 0;
						return true;
					}
				}
			case NM_SETFOCUS:
				{
					//フォーカスが変わったことを通知.
					Filer * filer = Filer::getFiler();
					filer->OnChangeFocus();
					*outRet = 0;
					return true;
				}
			}
		}
		break;
	}
	return false;
}

//FixedColumが変更されたときに呼ばれる.
void DisplayWin32::OnChangeFixedColum()
{
	RECT rc;
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &rc);


	int windowWidthSize = rc.left - rc.right - 16;

	//すべてのカラムの破棄
	ListView.ClearColumn();

	for(int i = 0; i < FixedColumn.getColumnSize() ; i++)
	{
		const RFixedColumn* colum = FixedColumn.getColumn(i);
		//これ以上要素ないよだったらループを抜けます。
		if (colum->getType() == __FILECOLUMN_TYPE_END) break;

		//各要素に分かれて処理します。
		switch( colum->getType() )
		{
		case __FILECOLUMN_TYPE_NAME:
		case __FILECOLUMN_TYPE_EXT:
		case __FILECOLUMN_TYPE_SIZE:
		case __FILECOLUMN_TYPE_DATE:
		case __FILECOLUMN_TYPE_ATTRIB:
			ListView.InsertColumn( i , 0 , (char*)colum->getName().c_str() ,
				colum->getWidthParsent() * windowWidthSize, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ,LVCFMT_LEFT );
			break;
		case __FILECOLUMN_TYPE_ICON:
			ListView.InsertColumn( i , 0 , (char*)colum->getName().c_str() , 
				16 , LVCF_FMT | LVCF_WIDTH | LVCF_IMAGE | LVCF_SUBITEM ,LVCFMT_LEFT );
			break;
		}
	}
}

//表示項目の設定
void DisplayWin32::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	ListView.SetItemCountEx(inSize,0);
	this->RfileItems = inRfileItems;
	this->RfileItemsSize = inSize;

	//イメージリストセット.
	ListView.setImageList(FileModifyChacheLargeWin32::getInstance()->getImageList() , LVSIL_NORMAL);
}


//ダブルクリック
bool DisplayWin32::OnDoubleClick()
{
	RFunctionScript	rfs;
	rfs.Interpreter("&VIEW $P$F");

	return true;
}

//カラムクリック
bool DisplayWin32::OnColumnClick(LPDRAWITEMSTRUCT inItem)
{
	return true;
}

//キーがなんか押されました
bool DisplayWin32::OnKeyDown(LPNMKEY inKey) 
{
	RFunctionScript	rfs;

	switch(inKey->nVKey & 0x00ff)
	{
	case VK_RETURN:
		rfs.Interpreter("&VIEW $P$F");
		return true;
	case VK_BACK:
		rfs.Interpreter("&UPDIR");
		return true;
	}
	return false;

}

//アクティヴになりました
void DisplayWin32::OnActive()  const
{
	ListView.Show();

	::SetFocus( ListView.getWindowHandle() );
}

//非アクティヴになりました
void DisplayWin32::OnNoActive() const
{
	ListView.Hide();
}


bool DisplayWin32::OnGetDispInfo(NMLVDISPINFO* inInfo)
{
	return false;
}


bool DisplayWin32::OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw )
{
	return false;
}


//現在カーソルがあるファイル名を取得します。
string DisplayWin32::getActiveFilename() const 
{
	int index = ListView.getNowSelected();
	if (index < 0 || index >= RfileItemsSize) return "";

	//Rfile 取得.
	const Rfile* rf = this->RfileItems[index];

	return rf->getVars(__FILECOLUMN_TYPE_NAME);
}

//現在マークされているファイルを渡します。
StringList DisplayWin32::getMarkFilenames() const 
{
	StringList selected;

	int index = -1;
	while((index = ListView_GetNextItem(ListView.getWindowHandle(), index, LVNI_ALL | LVNI_SELECTED))!=-1)
	{
		//念のため
		if (index < 0 || index >= RfileItemsSize) continue;

		//Rfile 取得.
		const Rfile* rf = this->RfileItems[index];

		selected.push_back( rf->getVars(__FILECOLUMN_TYPE_NAME) );
	}

	return selected;
}
