// DisplayBlank.h: DisplayBlank クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYBLANK_H__986A6AFA_449F_48D6_A19C_FE738A4AE1D0__INCLUDED_)
#define AFX_DISPLAYBLANK_H__986A6AFA_449F_48D6_A19C_FE738A4AE1D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayBlank;
#include "comm.h"
#include "Display.h"

class DisplayBlank : public Display  
{
public:
	DisplayBlank(Window* inWindow,__RFILETYPE inNavigateType)
		: Display(inWindow,inNavigateType)
	{
	}
	virtual ~DisplayBlank()
	{
	}

	//ディスプレイの種類を返します。
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_BLANK;
	}


	//アクティヴになりました
	virtual void OnActive() const
	{
	}

	//非アクティヴになりました
	virtual void OnNoActive() const
	{
	}

	//表示項目の設定
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems) 
	{
	}

	//現在カーソルがあるファイル名を取得します。
	virtual string getActiveFilename() const 
	{
		return "";
	}

	//現在マークされているファイルを渡します。
	virtual StringList getMarkFilenames() const 
	{
		return StringList();	//そんなものはない
	}

	//ディスプレイウィンドウの幅と高さを取得
	virtual void GetWindowRect(RECT * outRC) const
	{
		ZeroMemory(outRC , sizeof(RECT) );
	}

	//指定したメニューをポップアップする.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) 
	{
		return "";	//あとで実装する.
	}

	//タイトルを取得する
	virtual string getTitle() const 
	{
		return "";
	}
	//表示するものの作成.
	virtual	void CreateShowObject(const PathInterface* inPath) 
	{
	}

protected:
	//FixedColumが変更されたときに呼ばれる.
	virtual void OnChangeFixedColum() 
	{
	}

};

#endif // !defined(AFX_DISPLAYBLANK_H__986A6AFA_449F_48D6_A19C_FE738A4AE1D0__INCLUDED_)
