// BUserDataIconDlg.cpp: BUserDataIconDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BUserDataIconDlg.h"
#include "WindowsMain.h"
#include "resrc1.h"
#include "BConf.h"
#include "RMessageBox.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BUserDataIconDlg::BUserDataIconDlg()
{

}

BUserDataIconDlg::~BUserDataIconDlg()
{

}

void BUserDataIconDlg::Create(const RGISSmallShareImage* poolShareImage , int inNowIconNumber ,
							  const string & inName , const string & inCatego , const string & inSubCatego) 
{

	try
	{
		//説明ファイルの読み込み.
		Load();
	}
	catch(RException e)
	{
		RMessageBox::Message( e.getMessage() );
	}

	this->PoolShareImage = poolShareImage;
	this->NowIconNumber = inNowIconNumber;
	this->Name = inName;
	this->Catego = inCatego;
	this->SubCatego = inSubCatego;
}

void BUserDataIconDlg::OnInit() 
{
	ScrollBar.MountWindow( this->GetDlgItem(IDC_ICON_SCROLLBAR) );
	IconMessageEdit.MountWindow( this->GetDlgItem(IDC_ICON_MESSAGE_EDIT) );

	//アイコンの数は スクロールバーの範囲になります.
	const RDIBBitmap * icon = this->PoolShareImage->getIconImage();
	int scrollMax = icon->getWidth() / 16;

	SCROLLINFO info = {0};
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	info.nMin = 0;
	info.nMax = scrollMax - 1;
	info.nPos = this->NowIconNumber;
	ScrollBar.SetScrollInfo(SB_CTL , &info , TRUE);

	//選択されているアイコンの説明をテキストボックスに表示する.
	IconMessageEdit.setText( getIconMessage(this->NowIconNumber) );
}


void BUserDataIconDlg::OnPaint()
{
	RECT rc;
	this->getControlClientRect(this->GetDlgItem(IDC_ICON_PALLET) ,&rc);

	//横幅に入るだけアイコンを描く.
	int width = (rc.right - rc.left) / 16 * 16;

    HDC hdc;
    PAINTSTRUCT paint;

	WindowsMain * winMain = WindowsMain::getMainSystem();
	HWND window = this->getWindowHandle();
    hdc = BeginPaint( window , &paint);
	::PatBlt(hdc , rc.left , rc.top , rc.right - rc.left , 16 , BLACKNESS );
	::PatBlt(hdc , rc.left - 5, rc.top - 5, 16 + 5 , 16 + 5, WHITENESS );

	::BitBlt(hdc  , rc.left , rc.top , width , 16 , 
		PoolShareImage->getIconImage()->getConstDC() , 16 * NowIconNumber , 0 , SRCCOPY);
    EndPaint(window, &paint);
}

void BUserDataIconDlg::OnScroll(WPARAM inWParam)
{
	//現在の状態を取得.
	SCROLLINFO info = {0};
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	ScrollBar.GetScrollInfo(SB_CTL , &info );

	//スクロール量の計算
	if ( !ScrollBar.CalsScroll(&info , inWParam) )	return ;

	//スクロールバーを更新.
	ScrollBar.SetScrollInfo(SB_CTL , &info , TRUE);

	//現在選択されているアイコンの更新.
	this->NowIconNumber = info.nPos;

	//選択されているアイコンの説明をテキストボックスに表示する.
	IconMessageEdit.setText( getIconMessage(this->NowIconNumber) );

	//画面を更新.
	InvalidateRect( this->getWindowHandle() ,  NULL , FALSE);
}


//ウィンドウプロシージャ.
LRESULT BUserDataIconDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
		OnInit() ;
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDOK:	//書き込み
				this->EndDialog(1);
				break;
			case IDCANCEL:	//ｷｬﾝｾﾙ
				this->EndDialog(0);
				break;
			case IDC_AUTO_BUTTON:	//自動選択.
				{
					int no = AutoMatch(Name , Catego , SubCatego);
					if (no != -1)
					{
						//設定の更新
						this->NowIconNumber = no;

						//スクロールバーの設定
						ScrollBar.setPos(no , SB_CTL);

						//選択されているアイコンの説明をテキストボックスに表示する.
						IconMessageEdit.setText( getIconMessage(this->NowIconNumber) );

						//画面を更新.
						InvalidateRect( this->getWindowHandle() ,  NULL , FALSE);
					}
				}
				break;
			}
		}
		break;
	case WM_HSCROLL:
		OnScroll(inWParam);
		break;
	case WM_PAINT:
		OnPaint();
		break;
	}
	return FALSE;
}


//説明の読み込み
void BUserDataIconDlg::Load()
{
	RStdioFile file;
	file.Open(BConf::getConf()->getCurrentDirectory() + "tool_bit.txt" , "rb");

	char messageBuffer[256];
	char matchListBuffer[256];
	
	Text.clear();

	char spaceBuffer[1024];

	for(int nowLine = 0 ;  !file.IsEof() ; nowLine ++ )
	{
		string line = file.ReadLine();
		//空行または、コメントは無視.
		if ( line.empty() || line[0] == '#' || line == "\r\n") continue;

		int r = sscanf(line.c_str() , "%256s%1024[ ]%s256", messageBuffer , spaceBuffer , matchListBuffer);
		if (r < 1) 
		{
			throw RException(EXCEPTIONTRACE + "tool_bit.txt ファイルの" + num2str(nowLine) + "行目の読み込みに失敗しました");
		}

		//説明がない場合も問題がおきないように。
		if (r != 3)
		{
			matchListBuffer[0] = '\0';
		}
		//格納.
		BToolBitText* p = new BToolBitText;
		p->Create(messageBuffer , matchListBuffer);
		Text.push_back( p );
	}
}

//自動選択
int BUserDataIconDlg::AutoMatch(const string & inName , const string & inCatego , const string & inSubCatego) const
{
	if ( Text.empty() ) return -1;

	newVector<BToolBitText>::const_iterator i = Text.begin();
	newVector<BToolBitText>::const_iterator e = Text.end();

	//ヒット回数保存庫.
	int * hitCount = new int[ Text.size() + 1 ];
	memset(hitCount , 0 , Text.size() * sizeof(int) );

	int num;
	for(num = 0 ; i != e ; i++ , num ++)
	{
		const RStringList* list = (*i)->getMatchList();
		//名前にヒットしていたら 1点
		if ( TextFind(list , inName) )
		{
			hitCount[num] += 1;
		}
		//カテゴリにヒットしていたら 2点
		if ( TextFind(list , inCatego) )
		{
			hitCount[num] += 2;
		}
		//サブカテゴリにヒットしていたら 2点
		if ( TextFind(list , inSubCatego) )
		{
			hitCount[num] += 2;
		}
	}

	//一番ヒットしていそうなやつを選ぶ.
	int maxIndex = - 1;
	int max = 0;
	for( ; num >= 0; num --)
	{
		if (hitCount[num] > max)
		{
			max = hitCount[num] ;
			maxIndex = num;
		}
	}

	//一番ヒットしていたやつを帰す.
	return maxIndex;
}

//リストの中の文字列に inName の部分が一致しているか検索.
bool BUserDataIconDlg::TextFind(const RStringList* inList , const string & inName) const
{
	RStringList::const_iterator i = inList->begin();
	RStringList::const_iterator e = inList->end();

	for( ; i != e ; i ++)
	{
		string aaa = *i;

		unsigned int size = inName.size();
		if ( i->size() >= size) continue;

		//最後が一致しているか検索.
		const char * p = inName.c_str() + inName.size() - i->size();
		if ( strcmp( i->c_str() , p ) == 0 ) return true;
	}

	return false;
}


//指定されたアイコンの説明の取得
const string BUserDataIconDlg::getIconMessage(int inIconSelect) const
{
	if (Text.size() <= (unsigned int) inIconSelect ) return "";

	return Text[inIconSelect]->getMessage();
}
