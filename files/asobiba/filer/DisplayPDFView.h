// DisplayPDFView.h: DisplayPDFView クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYPDFVIEW_H__5BE134B9_BA4C_48B1_BAEA_3C0B42849D64__INCLUDED_)
#define AFX_DISPLAYPDFVIEW_H__5BE134B9_BA4C_48B1_BAEA_3C0B42849D64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "Display.h"
#include "WndProcCallable.h"
#include "RPDFView.h"
#include "RfilePreviewOne.h"

class DisplayPDFView  :
	public Display,				//ディスプレイはこれを継承しなくてはならない。 DisplayTextView は Displayである。
	private WndProcCallable		//WndProcを受け取る事ができる Intefaceクラス.
{
public:
	DisplayPDFView(Window* inWindow,__RFILETYPE inNavigateType);
	virtual ~DisplayPDFView();

	//ディスプレイの種類を返します。
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_PREVIEW_PDF;
	}

	//FixedColumが変更されたときに呼ばれる.
	virtual void OnChangeFixedColum()
	{
	}

	//表示項目の設定
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems);

	//アクティヴになりました
	virtual void OnActive() const;

	//非アクティヴになりました
	virtual void OnNoActive() const;

	//現在カーソルがあるファイル名を取得します。
	virtual string getActiveFilename() const 
	{
		return this->RfileOne->getVars(__FILECOLUMN_TYPE_NAME);
	}

	//現在マークされているファイルを渡します。
	virtual StringList getMarkFilenames() const 
	{
		return StringList();	//そんなものはない
	}
	//ディスプレイウィンドウの幅と高さを取得
	virtual void GetWindowRect(RECT * outRC) const
	{
		PDFView.GetWindowRect(outRC);
	}
	//指定したメニューをポップアップする.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) 
	{
		return "";	//あとで実装する.
	}
	//タイトルを取得する
	virtual string getTitle() const 
	{
		return PDFView.getTitle();
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
	//ウィンドウをリサイズする
	void WindowResize() const;
	//通常クリックとか、リターンキーとかでのアクション
	void NormalAction();
	//一つ前のディレクトリに戻る
	void ReturnAction();

	const Rfile* RfileOne;
	RPDFView	PDFView;

};

#endif // !defined(AFX_DISPLAYPDFVIEW_H__5BE134B9_BA4C_48B1_BAEA_3C0B42849D64__INCLUDED_)
