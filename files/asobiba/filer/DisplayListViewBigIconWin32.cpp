// DisplayListViewBigIconWin32.cpp: DisplayListViewBigIconWin32 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayListViewBigIconWin32.h"
#include "RFunctionScript.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


DisplayListViewBigIconWin32::DisplayListViewBigIconWin32
(Window* inWindow,__RFILETYPE inNavigateType) : Display(inWindow,inNavigateType)
{
	ASSERT(Win != NULL);

	//Windowsのためのメイン.
	WindowsMain * mainSystem = WindowsMain::getMainSystem();

	//メインとなるリストビューの作成.
	ListView.Create(mainSystem->getInstance(),
		mainSystem->getBaseWindow( Win->getIsLeft() ),
		mainSystem->getID() , WS_CHILD | WS_TABSTOP | LVS_ICON | LVS_OWNERDATA | WS_VISIBLE );

	//コールバック登録
	mainSystem->registeCallbackWndProc(Win->getIsLeft(),this);

	//設定を読み込む.
	SetupLoad.Load(FILER_DEFAULT_DEFINE_FILENAME,"FileView");

	this->RfileItems = NULL;
	this->RfileItemsSize = 0;

	//真中に移動する.
	this->HarfWindow();
}


//ウィンドウを半分の位置に移動する.
void DisplayListViewBigIconWin32::HarfWindow() const
{
	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	RECT baseWindowRC ;

	ASSERT(Win != NULL);
	::GetClientRect( mainSystem->getBaseWindow( Win->getIsLeft() ) , &baseWindowRC);

	ListView.MoveWindow(0 , ITEM_PANEL_SIZE , baseWindowRC.right  , baseWindowRC.bottom - ITEM_PANEL_SIZE - STATUS_BAR_SIZE);
	
}

DisplayListViewBigIconWin32::~DisplayListViewBigIconWin32()
{
	//関連付けられているイメージリストを勝手に破棄しやがるので
	//無効なリストに摩り替えて、勝手には破棄されないようにします。
	//イメージリストは共有のものをつかっているからねー
	//破棄しちゃうともう一つのウィンドウでいちきなりイメージリストがエラーで大変なことになる。
	//このデバックで2時間かかったぢゃないかプンプン.
	ListView.setImageList(NULL , LVSIL_NORMAL);

	WindowsMain * mainSystem = WindowsMain::getMainSystem();
	mainSystem->UnregisteCallbackWndProc(Win->getIsLeft(),this);

	this->RfileItems = NULL;
	this->RfileItemsSize = 0;
}

//WindowProcを受け取る
//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
//自分は処理しない奴は false を返す。
bool DisplayListViewBigIconWin32::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet)
{
    switch(msg)
	{
	case WM_SIZE:
		if (IsWindowVisible( ListView.getWindowHandle() ) )		HarfWindow();
		break;
	case WM_NOTIFY:
		{
			if (!IsWindowVisible( ListView.getWindowHandle() ) ) return false;

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
			case NM_RCLICK:
				{
					if (this->OnRClick((LPNMHDR)lparam) )
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
					if (OnKeyDown(((LPNMKEY)lparam)->nVKey , MODIFY_KEY_NONE) ) 
					{
						*outRet = 0;
						return true;
					}
				}
			case NM_SETFOCUS:
				{
					//フォーカスが変わったことを通知.
					Filer * filer = Filer::getFiler();
					filer->OnChangeFocus( Win->getIsLeft() );
					*outRet = 0;
					return true;
				}
			}

		}
		break;
	}
	return false;
}

//キーが押されたときの処理
bool DisplayListViewBigIconWin32::OnKeyDown(unsigned char inKeyCode  , int inModifyKey ) const
{
	try
	{
		return this->SetupLoad.KeyDown(inKeyCode, (MODIFY_KEY) inModifyKey);
	}
	catch(RParseException e)
	{
		printf("エラーが発生しました:\n%s\n", e.getMessage() );
	}
	return false;
}

//FixedColumが変更されたときに呼ばれる.
void DisplayListViewBigIconWin32::OnChangeFixedColum()
{
}

//表示項目の設定
void DisplayListViewBigIconWin32::setRfileItems(int inSize,const Rfile** inRfileItems)
{
	this->RfileItems = inRfileItems;
	this->RfileItemsSize = inSize;

	//イメージリストセット.
	ListView.setImageList(FileModifyChacheLargeWin32::getInstance()->getImageList() , LVSIL_NORMAL);

	//これを呼び出すと、LVN_GETDISPINFO が呼ばれるらしいので、先に
	//RfileItems RfileItemsSize をセットしないと落ちでー
	ListView.SetItemCountEx(inSize ,0 );

	//すべての選択を解除します。
	ListView.ResetSelect();

	//アクティヴな選択を前回のディレクトリに合わせます。
	const string lastPath = Win->getLastPath();
	const string nowPath = Win->getNowPath();
	if (nowPath != lastPath.substr( 0 ,nowPath.size() ) )
	{	//新しく入ったディレクトリなので、先頭のアイテムにフォーカスを当てます。
		ListView.SetItemState(0 , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		TRACE("次のパスは一致しなかったたです %s -> %s\n" ,lastPath.c_str() ,  nowPath.c_str() );
	}
	else
	{	//対象ファイルをスキャンします。

		//サーチするターゲット.
		string filename = lastPath.substr( nowPath.size() );
		//ディレクトリなど、最後の \\ が入っている場合があるので切り落とします。
		if ( ! MultiString::IsPathToComplement(filename) )
		{
			filename = filename.substr(0 , filename.size() - 1 );
		}

		for(int i = 0; i<inSize ; i++)
		{
			if ( inRfileItems[i]->getVars(__FILECOLUMN_TYPE_NAME) == filename)
			{//ヒット.
				ListView.SetItemState(i , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				break;
			}
		}
		if (i==inSize)
		{//ヒットしなかった場合は、やはり、先頭に合わせます。
			ListView.SetItemState(0 , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

}



//ダブルクリック
bool DisplayListViewBigIconWin32::OnDoubleClick() const
{
	//マウスクリック関係はどこか選択されていることが条件です。
	//どこも選択されていない場合は、キャンセルする.
	if ( ListView.HitItem() == -1) return false;

	return OnKeyDown(VK_LBUTTON , MODIFY_KEY_DBCLICK);
}

//カラムクリック
bool DisplayListViewBigIconWin32::OnColumnClick(LPDRAWITEMSTRUCT inItem) const
{
	return true;
}

//アクティヴになりました
void DisplayListViewBigIconWin32::OnActive()  const
{
	HarfWindow();

	ListView.Show();

	::SetFocus( ListView.getWindowHandle() );
}

//非アクティヴになりました
void DisplayListViewBigIconWin32::OnNoActive() const
{
	ListView.Hide();
}


bool DisplayListViewBigIconWin32::OnGetDispInfo(NMLVDISPINFO* inInfo) const
{
	if (!RfileItems) return false;
	LVITEMA * item = &inInfo->item;

	const Rfile* rf = this->RfileItems[item->iItem];

	item->iImage = rf->getImageIndex();
	item->stateMask = 0;
	item->state = 0;
	item->pszText = (char*) rf->getVars(__FILECOLUMN_TYPE_NAME).c_str();
	return true;
}

bool DisplayListViewBigIconWin32::OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw ) const
{
	return false;
}

//現在カーソルがあるファイル名を取得します。
string DisplayListViewBigIconWin32::getActiveFilename() const 
{
	const int index = ListView.getNowSelected();
	if (index < 0 || index >= RfileItemsSize) return "";

	//Rfile 取得.
	const Rfile* rf = this->RfileItems[index];

	return rf->getVars(__FILECOLUMN_TYPE_NAME);
}

//現在マークされているファイルを渡します。
StringList DisplayListViewBigIconWin32::getMarkFilenames() const 
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


//右クリック
bool DisplayListViewBigIconWin32::OnRClick(const NMHDR * inNmhdr) 
{
	try
	{
		//どこも選択されていない場合は、現在位置のアイテムを選択させる.
		//クリックした位置にあるアイテムをアクティヴにする.
		const int item = ListView.HitItem();
		if (item != -1)
		{
			//その位置のアイテムは選択されていますか?
			if (! (ListView.GetItemState(item , LVIS_SELECTED) & (LVIS_SELECTED)) )
			{
				//選択されていないので、現在選択されているアイテムをすべて解除し、
				ListView.ResetSelect();

				//現在アイテムを選択させます。
				ListView.SetItemState(item , LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				
			}
		}

		//ポップアップ.
		PopupMenu("Popup");
	}
	catch(RException e)
	{
		printf("エラーが発生しました:\n%s",e.getMessage() );
		return false;
	}

	return true;	
}


//指定したメニューをポップアップする.
string DisplayListViewBigIconWin32::PopupMenu(const string inMenuName) const throw(RWin32Exception,RParseException,RNotFoundException) 
{
	POINT pt;
	GetCursorPos(&pt);

	return SetupLoad.Popup(inMenuName , pt , ListView.getWindowHandle());
}

//表示するものの作成.
void DisplayListViewBigIconWin32::CreateShowObject(const PathInterface* inPath) 
{

}

