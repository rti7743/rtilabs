// DisplayWin32.h: DisplayWin32 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYWIN32_H__5100E289_5937_49DC_A9E9_C81675A4EA91__INCLUDED_)
#define AFX_DISPLAYWIN32_H__5100E289_5937_49DC_A9E9_C81675A4EA91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayWin32;
#include "comm.h"
#include "Display.h"
#include "WindowsMain.h"
#include "RListViewWithMouseGesture.h"
#include "RfileLocalReport.h"
#include "WndProcCallable.h"

class DisplayWin32  :
	public Display,				//ディスプレイはこれを継承しなくてはならない。 DisplayListViewBigIconWin32 は Displayである。
	private WndProcCallable		//WndProcを受け取る事ができる Intefaceクラス.
{
public:
	DisplayWin32(Window* inWindow,__RFILETYPE inNavigateType);
	virtual ~DisplayWin32();

	//FixedColumが変更されたときに呼ばれる.
	virtual void OnChangeFixedColum();
	//表示項目の設定
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems);

	//ディスプレイの種類を返します。
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_LISTVIEW_REPORT;
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
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) 
	{
		return "";	//あとで実装する.
	}
	//タイトルを取得する
	virtual string getTitle() const 
	{
		return "";	//タイトルなんてないです.
	}
	//表示するものの作成.
	virtual	void CreateShowObject(const PathInterface* inPath) 
	{
	}

protected:
	//WindowProcを受け取る
	//自分が処理する場合は true を返す。 戻り値は outRet に代入する。
	//自分は処理しない奴は false を返す。
	virtual bool WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,LRESULT* outRet);
private:
	//ウィンドウを半分の位置に移動する.
	void HarfWindow() const;
	//ダブルクリック
	bool OnDoubleClick() ;
	//カラムクリック
	bool OnColumnClick(LPDRAWITEMSTRUCT inItem) ;
	//キーがなんか押されました
	bool OnKeyDown(LPNMKEY inKey) ;
	bool OnGetDispInfo(NMLVDISPINFO* inInfo);
	bool OnCustomDraw(NMLVCUSTOMDRAW* inCustomDraw );

	const Rfile** RfileItems;
	int			RfileItemsSize;
	RListViewWithMouseGesture	ListView;
};

#endif // !defined(AFX_DISPLAYWIN32_H__5100E289_5937_49DC_A9E9_C81675A4EA91__INCLUDED_)
