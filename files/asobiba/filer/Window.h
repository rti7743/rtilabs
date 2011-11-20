// Window.h: Window クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOW_H__C152CCE8_4D60_4A49_86CA_3890E2151DED__INCLUDED_)
#define AFX_WINDOW_H__C152CCE8_4D60_4A49_86CA_3890E2151DED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Window;

#include "comm.h"


#include "Rfiles.h"
#include "Display.h"
#include "RTaskManager.h"
#include "FilerSub.h"
#include "AddressHistory.h"

//主ウィンドウ
class Window
{
private:
	//ファイル一覧管理
	Rfiles*		Files;
	//ディスプレイ
	Display*	Disp;
	//親のFilerSub.
	FilerSub*	ParentFSub;
	//アドレスの履歴管理
	AddressHistory	History;
	//サーチ結果をマスクする
	string			Mask;
	//直前のパス
	string			LastPath;

public:
	//左に置くか、右に置くかを決定します。
	Window(FilerSub* inFilerSub,string inPath);
	virtual ~Window();

	//アップデートします。
	void UpdateWindow(Rfiles* inFiles , Display* inDisp);

	//ディスプレイウィンドウの幅と高さを取得
	void GetWindowRect(RECT * outRC) const;

	//左にあんの?
	bool getIsLeft() const;
	//アクティヴになりました.
	void OnActive();
	//非アクティヴになりました.
	void OnNoActive() const;
	//ディレクトリを一つ上がる.
	void UpDirectory();
	//一つ前に進めるの?
	bool IsHistoryNext() const			{	return History.IsNext();	}

	//一つ前に戻れる?
	bool IsHistoryBack() const			{	return History.IsBack();	}

	//プレビューしてダメだったらディレクトリとしてスキャン.
	bool PreviewOrLs(string inPath,bool isNOTRecodeHistory = false)
	{
		//直前のパスを保存
		this->LastPath = this->getNowPath();
		//プレビューしてダメだったらディレクトリとしてスキャン.
		if ( Preview(inPath,isNOTRecodeHistory) )	return true;
		if ( ls(inPath,isNOTRecodeHistory) )	return true;
		return false;
	}

	//現在のディスプレイ上でのパスが変更になりました。
	//たとえばブラウザでリンクをクリックされたとき、
	//現在のディスプレイを使ってパスとヒストリーだけ記録します。
	void OnQuickChange(string inPath,bool isNOTRecodeHistory = false);
	//タイトルが変更された場合
	void OnTitleChange();

	//現在保持しているファイルシステムでの現在パスを取得.
	string getNowPath() const
	{
		if (this->Files) return this->Files->getPath();
		return "";
	}
	//タイトルの取得.
	string getTitle() const;

	//現在カーソルがあるファイル名を取得します。
	string getActiveFilename() const
	{
		return Disp->getActiveFilename();
	}
	//現在マークされているファイルを渡します。
	StringList getMarkFilenames() const
	{
		return Disp->getMarkFilenames();
	}
	//読み直し.
	void Refresh() 
	{
		PreviewOrLs( getNowPath() );
	}
	//指定したメニューをポップアップする.
	string PopupMenu(const string inMenuName) throw(RWin32Exception,RNoComplateException,RNotFoundException)
	{
		return Disp->PopupMenu(inMenuName);
	}
	//マスクの取得
	string getMask() const
		{		return this->Mask;	}
	//マスクをセットする.
	void setMask(const string inMask)
		{		this->Mask = inMask;	}

	//自分が所有しているタスクをすべてとめます。
	void StopTasks() const;

	//戻れ!
	void HistoryBack();
	//進め!
	void HistoryNext();

	//一つ前のパスを取得します。
	string getLastPath() const
		{		return this->LastPath;	}

private:
	//パスで内部ビューワーでプレビューします。
	bool Preview(string inPath,bool isNOTRecodeHistory = false) ;
	//ディレクトリスキャン.
	bool ls(string inPath,bool isNOTRecodeHistory = false) ;

};

#endif // !defined(AFX_WINDOW_H__C152CCE8_4D60_4A49_86CA_3890E2151DED__INCLUDED_)
