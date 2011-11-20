// MenuSentence.cpp: MenuSentence クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "MenuSentence.h"
#include "Filer.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

StringMap	MenuSentence::RunnbleScripts;


MenuSentence::MenuSentence()
{
	PopupMenu = ::CreatePopupMenu();
}
MenuSentence::~MenuSentence()
{
	::DestroyMenu(PopupMenu);
}


//ノードを作成.
void MenuSentence::CreateNode(string inLabel , string inScript)
{
	ASSERT(PopupMenu != NULL);

	//ウィンドウ"管理"人
	int id = WindowsMain::getID();
	::AppendMenu(PopupMenu, MF_STRING, id , inLabel.c_str() );
	RunnbleScripts[id] = inScript;
}

//入れ子になっているノードの作成.
void MenuSentence::CreateIncludeMenu(string inLabel , HMENU inNewPopup)
{
	ASSERT(PopupMenu != NULL);
	::AppendMenu(PopupMenu, MF_POPUP, (UINT_PTR)inNewPopup , inLabel.c_str() );
}

//ポップアップ.
HMENU MenuSentence::getMenu() const
{
	ASSERT(PopupMenu != NULL);
	return PopupMenu;
}
//実行
string MenuSentence::Run(int inID) const throw(RNotFoundException,RParseException)
{
	StringMap::const_iterator i = RunnbleScripts.find(inID);
	if (i == RunnbleScripts.end() ) throw RNotFoundException("選択された ID : %d に対応しているスクリプトはありません",inID );

	RFunctionScript rsf;
	return rsf.Interpreter((*i).second);
}