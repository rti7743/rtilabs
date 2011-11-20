// RSetupLoad.h: RSetupLoad クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSETUPLOAD_H__8B06E6E4_0617_4AA0_9C38_644E0E5E0520__INCLUDED_)
#define AFX_RSETUPLOAD_H__8B06E6E4_0617_4AA0_9C38_644E0E5E0520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RCommKeyDown.h"
#include "MenuSentence.h"
#include "RNotFoundException.h"
#include "RParseException.h"
#include "RWin32Exception.h"

//標準のキーバイド、マウスダウン、ツールバーなどの場所.
const string FILER_DEFAULT_DEFINE_FILENAME = "filer.def";

typedef map<string ,MenuSentence*>	OneMenu;

class RSetupLoad  
{
public:
	RSetupLoad();
	virtual ~RSetupLoad();

	//読み込み.
	void Load(string inSetupFilename,string inSection) throw(RIOCannotOpenException,RIOCannotReadException,RParseException);

	//キーダウン.
	bool KeyDown(unsigned char inKeyCode ,MODIFY_KEY inModifyKey) const throw (RParseException);
	//キーダウン.
	bool KeyDown(unsigned char inKeyCode ) const throw (RParseException);

	//メニューを出す.
	string Popup(const string inMenuName, POINT inScreenPoint , HWND inOwner) const throw (RParseException,RNotFoundException,RWin32Exception);

private:
	//キーコードの変換
	unsigned char ToKey(string inKeyname) const;

private:
	RCommKeyDown	KeyDowns;
	OneMenu			Menus;
};

#endif // !defined(AFX_RSETUPLOAD_H__8B06E6E4_0617_4AA0_9C38_644E0E5E0520__INCLUDED_)
