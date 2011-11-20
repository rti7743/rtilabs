// RComboBox.h: RComboBox クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOMBOBOX_H__404DD9E2_248A_4280_B834_057333EC6DF2__INCLUDED_)
#define AFX_RCOMBOBOX_H__404DD9E2_248A_4280_B834_057333EC6DF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWindowBase.h"

class RComboBox : public RWindowBase  
{
public:
	RComboBox();
	virtual ~RComboBox();

	//作成. オーバーライド
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//アイテムの追加.
	bool AddItem(const string & inString);
	//アイテムの挿入.
	bool InsertItem(int inItem,const string & inString);
	//アイテムの変更.
	bool SetItem(int inItem,const string & inString);
	//アイテムの取得
	const string GetItem(int inItem);

	//現在選択されているところの取得.
	int	getNowSelected() const;

	//指定した場所を選択させる
	void Select(int inItem);

	//アイテムの数の取得
	int GetItemCount() const;
	//アイテムの削除
	bool DeleteItem(int inItem);
	//アイテムの削除
	void DeleteItemAll();
};

#endif // !defined(AFX_RCOMBOBOX_H__404DD9E2_248A_4280_B834_057333EC6DF2__INCLUDED_)
