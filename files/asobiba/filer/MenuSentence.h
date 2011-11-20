// MenuSentence.h: MenuSentence クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUSENTENCE_H__7FE726B3_8ECC_4983_9ED4_6B5313EBD9E2__INCLUDED_)
#define AFX_MENUSENTENCE_H__7FE726B3_8ECC_4983_9ED4_6B5313EBD9E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "StringList.h"
#include "RFunctionScript.h"
#include "RNotFoundException.h"


class MenuSentence
{
public:
	MenuSentence();
	~MenuSentence();
	//ノードを作成.
	void CreateNode(string inLabel , string inScript);
	//入れ子になっているノードの作成.
	void CreateIncludeMenu(string inLabel , HMENU inNewPopup);
	//ポップアップ.
	HMENU getMenu() const;
	//実行
	string Run(int inID) const throw(RNotFoundException,RParseException);
private:
	static StringMap	RunnbleScripts;
	HMENU		PopupMenu;
};


#endif // !defined(AFX_MENUSENTENCE_H__7FE726B3_8ECC_4983_9ED4_6B5313EBD9E2__INCLUDED_)
