// DisplayListViewBigIconWin32.h: DisplayListViewBigIconWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYLISTVIEWBIGICONWIN32_H__986B4C37_6DEF_45E8_8C7A_7D477A0B03B9__INCLUDED_)
#define AFX_DISPLAYLISTVIEWBIGICONWIN32_H__986B4C37_6DEF_45E8_8C7A_7D477A0B03B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayListViewBigIconWin32;

#include "comm.h"
#include "Display.h"
#include "WindowsMain.h"
#include "RListView.h"
#include "RfileLocalReport.h"
#include "RListViewWithMouseGesture.h"
#include "WndProcCallable.h"
#include "RSetupLoad.h"

class DisplayListViewBigIconWin32  : 
	public Display,				//ディスプレイはこれを継承しなくてはならない。 DisplayListViewBigIconWin32 は Displayである。
	private WndProcCallable		//WndProcを受け取る事ができる Intefaceクラス.
{
public:
	DisplayListViewBigIconWin32(Window* inWindow,__RFILETYPE inNavigateType);
	virtual ~DisplayListViewBigIconWin32();

	//FixedColumが変更されたときに呼ばれる.
	virtual void OnChangeFixedColum();
	//表示項目の設定
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems);

	//ディスプレイの種類を返します。
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_LISTVIEW_BIGICON;
	}
	//アクティヴになりました
	virtual void OnActive() const;

	//非アクティヴになりました
	virtual void OnNoActive() const;

	//現在カーソルがあるファイル名を取得します。
	virtual string getActiveFilename() const ;

	//現在マークされているファイルを渡します。
	virtual StringList getMarkFilenames() const ;

	//ディスプレイウィンドウの幅と高さを取得
	virtual void GetWindowRect(RECT * outRC) const
	{
		ListView.GetWindowRect(outRC);
	}
	//指定したメニューをポップアップする.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RParseException,RNotFoundException) ;

	//タイトルを取得する
	virtual string getTitle() const 
	{
		return "";	//タイトルなんてないです.
	}

	//表示するものの作成.
	virtual	void CreateShowObject(const PathInterface* inPath) ;


protected:
	//WindowProcを受け取る
	//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
	//自分は処理しない奴は false を返す。
	virtual bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet);
private:
	//キーが押されたときの処理
	bool OnKeyDown(unsigned char inKeyCode  , int inModifyKey ) const;
	//ウィンドウを半分の位置に移動する.
	void HarfWindow() const;
	//ダブルクリック
	bool OnDoubleClick() const;
	//カラムクリック
	bool OnColumnClick(LPDRAWITEMSTRUCT inItem) const;
	//右クリック
	bool OnRClick(const NMHDR * inNmhdr) ;

	bool OnGetDispInfo(NMLVDISPINFO* inInfo) const;

	bool OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw ) const;

	const Rfile** RfileItems;
	int			RfileItemsSize;

	RListViewWithMouseGesture	ListView;

	//設定ファイルからロードできます。
	RSetupLoad			SetupLoad;

};

#endif // !defined(AFX_DISPLAYLISTVIEWBIGICONWIN32_H__986B4C37_6DEF_45E8_8C7A_7D477A0B03B9__INCLUDED_)
