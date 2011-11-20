// Filer.cpp: Filer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "Filer.h"
#include "RTaskViewWin32.h"
#include "RTaskManager.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Filer::Filer()
{
	Left = NULL;
	Right = NULL;

	LeftWindowFocus = true;	//とりあえず
}

Filer::~Filer()
{
}

//singleton
Filer* Filer::getFiler() 
{
	static Filer filer;
	return &filer;
}

//ウィンドウがリサイズされたときに呼ばれる
void Filer::OnWindowResize()
{
	Left->OnWindowResize();
	Right->OnWindowResize();

	TaskView->OnSize();
}

//キーが押されたとき
void Filer::OnKeyDown(unsigned char inKey)
{
	switch(inKey)
	{
	case 'L':
		Left->NewWindow("c:\\WINNT") ;
		break;
	case 'H':
		Left->NewWindow("http://www.yahoo.co.jp/") ;
		break;
	case 'K':
		Left->NewWindow("c:\\test3\\kibafla2.swf") ;
		break;
	case 'J':
		Left->NewWindow("c:\\test3\\01 - 世界はNEOHAPPY.mp3") ;
		break;
	case 'I':
		Left->NewWindow("c:\\test3\\pdftest.pdf") ;
		break;
	case 'U':
		Left->NewWindow("c:\\test3\\純愛アルバム - 田原あゆみ.RM") ;
		break;
	case 'Y':
		Left->NewWindow("c:\\test3\\g09.jpg") ;
		break;
	case 'T':
		Left->NewWindow("c:\\test3\\mozilla.txt") ;
		break;
	case 'R':
		Left->NewWindow("\\\\192.168.1.10") ;
		break;
	case 'E':
		Left->NewWindow("smb://") ;
		break;
	case 'W':
		Left->NewWindow("c:\\test3\\サポテック織り.bmp") ;
		break;
	case VK_F5:
		Left->NewWindow("e:\\down") ;
		break;
	}
}

//有効なウィンドウかどうか調べる.
bool Filer::IsValidWindow(const Window* inWin)
{
	return (Left->Search(inWin) || Right->Search(inWin));
}

//開始処理
void Filer::OnCreate()
{
	Left = new FilerSub(true);
	Right = new FilerSub(false);
	TaskView = new RTaskViewWin32;
}

//終了処理
void Filer::OnDestory()
{
	delete Left;
	delete Right;
	delete TaskView;

	Left = NULL;
	Right = NULL;
	TaskView = NULL;
}

//左側にフォーカスがありますか?
bool Filer::IsLeftFocus() const
{
	return LeftWindowFocus;
}

//フォーカスをもつウィンドウが移動したときに呼ばれる.
void Filer::OnChangeFocus(bool isLeftWindow)
{
	LeftWindowFocus = isLeftWindow;
}

//フォーカスが変わったことを通知.
void Filer::OnChangeFocus()
{
	TaskView->OnSize();
}

