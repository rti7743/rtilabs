// RListView.h: RListView クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLISTVIEW_H__63CE8750_6A13_4676_A5DE_FF71999BF165__INCLUDED_)
#define AFX_RLISTVIEW_H__63CE8750_6A13_4676_A5DE_FF71999BF165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RListView;

#include "comm.h"
#include "RWindowBase.h"

class RListView  : public RWindowBase
{
public:
	RListView();
	virtual ~RListView();

	//作成. オーバーライド
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();

	//拡張項目のセット.
	void setExtended(DWORD inStyle);
	//拡張項目のセット.
	DWORD getExtended();
	//仮想リストの場合の項目数の設定
	void 	SetItemCountEx(int inCount,DWORD inFlg);
	//仮想リストの場合の項目数の設定
	void 	SetItemCount(int inCount);

	//カラムをすべて削除する.
	void ClearColumn();
	//カラムアイテムの追加.
	bool InsertColumn(int inItem,int inSubItem,char* inString,int inWidth,UINT inMask,int inFmt);

	//アイテムの追加 / 変更.
	bool SetItem(int inItem,int inSubItem,const char* inString);
	//アイテムの追加 / 変更.
	bool SetItem(int inItem,int inSubItem,int inInt);
	//アイテムの取得
	string GetItem(int inItem,int inSubitem);

	//現在選択されているところの取得.
	int	getNowSelected() const;

	//指定したアイテムの状態を変更します。
	void SetItemState(int inItem , int inState,int inMask);
	//指定したアイテムの状態を取得します。
	int GetItemState(int inItem,int inMask) const;

	//アイテムの数の取得
	int GetItemCount() const;
	//アイテムの削除
	bool DeleteItem(int inItem);
	//アイテムの削除
	void DeleteItemAll();

	//イメージリストセット.
	bool setImageList(const HIMAGELIST inImageLIst, int inType);

	//アイテムとのヒットテスト
	int HitTest(const POINT inPoint) const;

	//すべての選択を解除.
	void ResetSelect();

	//現在マウスカーソルがのっかっているアイテムを取得します。
	int HitItem() const;
};

#endif // !defined(AFX_RLISTVIEW_H__63CE8750_6A13_4676_A5DE_FF71999BF165__INCLUDED_)
