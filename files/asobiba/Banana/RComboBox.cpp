// RComboBox.cpp: RComboBox クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RComboBox.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RComboBox::RComboBox()
{

}

RComboBox::~RComboBox()
{

}

//アイテムの追加.
bool RComboBox::AddItem(const string & inString)
{
#ifndef _WIN32_WCE
	SendMessage( CB_ADDSTRING,0L,(LPARAM)inString.c_str());
#else
	MY_USES_CONVERSION;
	SendMessage( CB_ADDSTRING,0L,(LPARAM)_A2W(inString.c_str() ) );
#endif
	return true;
}

//アイテムの挿入.
bool RComboBox::InsertItem(int inItem,const string & inString)
{
#ifndef _WIN32_WCE
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)inString.c_str());
#else
	MY_USES_CONVERSION;
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)_A2W(inString.c_str()));
#endif
	return true;
}

//アイテムの変更.
bool RComboBox::SetItem(int inItem,const string & inString)
{
	SendMessage( CB_DELETESTRING,(WPARAM)inItem,0L);
#ifndef _WIN32_WCE
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)inString.c_str());
#else
	MY_USES_CONVERSION;
	SendMessage( CB_INSERTSTRING,(WPARAM)inItem,(LPARAM)_A2W(inString.c_str()));
#endif
	return true;
}

//アイテムの取得
const string RComboBox::GetItem(int inItem)
{
	//選択されている文字列の長さを取得.
	int size = SendMessage( CB_GETLBTEXTLEN,(WPARAM)inItem,0);
#ifndef _WIN32_WCE
	_myAutoPtr<char> buffer ;
	SendMessage( CB_GETLBTEXT,(WPARAM)inItem,(LPARAM)buffer.Alloc(size + 1));

	return buffer.getCore();
#else
	_myAutoPtr<unsigned short> buffer ;
	SendMessage( CB_GETLBTEXT,(WPARAM)inItem,(LPARAM)buffer.Alloc(size + 1) );

	MY_USES_CONVERSION;
	return _W2A( buffer.getCore() );
#endif
}



//現在選択されているところの取得.
int	RComboBox::getNowSelected() const
{
	return SendMessage( CB_GETCURSEL, 0L, 0L);
}

//指定した場所を選択させる
void RComboBox::Select(int inItem)
{
	SendMessage( CB_SETCURSEL,(WPARAM)inItem,0L);
}

//アイテムの数の取得
int RComboBox::GetItemCount() const
{
	return SendMessage( CB_GETCOUNT,0L,0L);
}

//アイテムの削除
bool RComboBox::DeleteItem(int inItem)
{
	SendMessage( CB_DELETESTRING,(WPARAM)inItem,0L);
	return true;
}

//アイテムの削除
void RComboBox::DeleteItemAll()
{
	SendMessage( CB_RESETCONTENT,0L,0L);
}

