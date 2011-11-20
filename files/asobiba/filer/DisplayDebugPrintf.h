// DisplayDebugPrintf.h: DisplayDebugPrintf クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYDEBUGPRINTF_H__D44F33B3_CC04_49CA_B6BE_3AF1FEE6250D__INCLUDED_)
#define AFX_DISPLAYDEBUGPRINTF_H__D44F33B3_CC04_49CA_B6BE_3AF1FEE6250D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayDebugPrintf;
#include "comm.h"
#include "Display.h"
#include "RfileLocalReport.h"
#include "RfileLocalBigIcon.h"
#include "RfileArcBigIcon.h"

//デバッグ表示用のファイル一覧表示.
class DisplayDebugPrintf : public Display
{
public:
	DisplayDebugPrintf(Window* inWindow = NULL ,__RFILETYPE inNavigateType = __RFILETYPE_LOCALFILE_REPORT);
	virtual ~DisplayDebugPrintf();

	//FixedColumが変更されたときに呼ばれる.
	virtual void OnChangeFixedColum(){}
	//アクティヴになりました
	virtual void OnActive() const{	}

	//非アクティヴになりました
	virtual void OnNoActive() const{	}

	string DebugBuffer;

	//ディスプレイの種類を返します。
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_DEBUG;
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

	//表示項目の設定
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems)
	{
		for (int i = 0; i < inSize ; i++)
		{
			const Rfile * p  = inRfileItems[i];
			DebugBuffer += p->getVars(__FILECOLUMN_TYPE_NAME) + " ★ " + p->getVars(__FILECOLUMN_TYPE_EXT) + " ";
			TRACE("filename %s ★ ext %s\n",
					p->getVars(__FILECOLUMN_TYPE_NAME).c_str() , p->getVars(__FILECOLUMN_TYPE_EXT).c_str() );
		}
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
	static void test();
};

#endif // !defined(AFX_DISPLAYDEBUGPRINTF_H__D44F33B3_CC04_49CA_B6BE_3AF1FEE6250D__INCLUDED_)
