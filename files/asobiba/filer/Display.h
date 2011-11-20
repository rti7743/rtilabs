// Display.h: Display クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAY_H__E7A4406F_E7FD_4832_B13D_F94A7E9A6CE7__INCLUDED_)
#define AFX_DISPLAY_H__E7A4406F_E7FD_4832_B13D_F94A7E9A6CE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Display;
class Window;
class PathInterface;
#include "comm.h"
#include "Rfile.h"
#include "RFixedColumnArray.h"
#include "StringList.h"
#include "ReferenceCount.h"

#include "RWin32Exception.h"
#include "RNoComplateException.h"
#include "RNotFoundException.h"

enum __DISPLAYTYPE
{
	__DISPLAYTYPE_DEBUG,
	__DISPLAYTYPE_BLANK,
	__DISPLAYTYPE_LISTVIEW_REPORT,
	__DISPLAYTYPE_LISTVIEW_BIGICON,
	__DISPLAYTYPE_PREVIEW_TEXT,
	__DISPLAYTYPE_PREVIEW_BITMAP,
	__DISPLAYTYPE_PREVIEW_WEBBROWSER,
	__DISPLAYTYPE_PREVIEW_MEDIA,
	__DISPLAYTYPE_PREVIEW_PDF,
	__DISPLAYTYPE_PREVIEW_NISEOLE,
};

class Display  : public ReferenceCount
{
protected:
	//このクラスが管理されているウィンドウ.
	Window*				Win;
	//どのカラムをどこに配置するかを決めるクラス.
	RFixedColumnArray	FixedColumn;
	//現在表示している RFILE の種類
	__RFILETYPE			NavigateType;
	//参照カウント.
	int					UseCount;
protected:
	//FixedColumが変更されたときに呼ばれる.
	virtual void OnChangeFixedColum() = 0;
public:
	Display(Window* inWindow,__RFILETYPE inNavigateType) : ReferenceCount()
	{
		this->Win = inWindow;
		this->NavigateType = inNavigateType;
	}
protected:
	virtual ~Display()
	{
	}

public:
	//カラムのコピー.
	void setFixedCloumn(const RFixedColumn* inReadOnlyColumnes)
	{
		this->FixedColumn.setFixedCloumn(inReadOnlyColumnes);
		//固定カラムの状態が変わった
		this->OnChangeFixedColum();
	}

	//現在表示している　RFILE の種類を返します。
	__RFILETYPE getNowNavigateType() const
	{
		return NavigateType;
	}
	//ディスプレイの種類を返します。
	virtual __DISPLAYTYPE getType() const = 0;

	//アクティヴになりました
	virtual void OnActive() const = 0;

	//非アクティヴになりました
	virtual void OnNoActive() const = 0;

	//表示項目の設定
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems) = 0;

	//現在カーソルがあるファイル名を取得します。
	virtual string getActiveFilename() const = 0;

	//現在マークされているファイルを渡します。
	virtual StringList getMarkFilenames() const = 0;

	//ディスプレイウィンドウの幅と高さを取得
	virtual void GetWindowRect(RECT * outRC) const = 0;

	//指定したメニューをポップアップする.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) = 0;

	//タイトルを取得する
	virtual string getTitle() const = 0;

	//表示するものの作成.
	virtual	void CreateShowObject(const PathInterface* inPath) = 0;

};

#include "Window.h"
#include "PathInterface.h"


#endif // !defined(AFX_DISPLAY_H__E7A4406F_E7FD_4832_B13D_F94A7E9A6CE7__INCLUDED_)
