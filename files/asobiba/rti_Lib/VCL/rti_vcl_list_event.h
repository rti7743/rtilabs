/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef LIST_LIFE

#ifndef ___TLISTEVENTH
#define ___TLISTEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//リストボックスのイベントクラス
//リストボックスから作るコンポーネントはこれを継承させます.
//EDIT

class TListBoxEvent	: public TBaseObject
{
public:
	//リストの最後に追加
	void Add(char *String) 
	{SendMessage(hWnd,LB_ADDSTRING,0L,(LPARAM)String);};

	//リストのPosに追加
	void Insert(int Pos,char *String) 
	{SendMessage(hWnd,LB_INSERTSTRING,(WPARAM)Pos,(LPARAM)String);};

	//リストをクリア
	void Clear() 
	{SendMessage(hWnd,LB_RESETCONTENT,0L,0L);};

//		//指定されている項目が選択されているかどうか
//		void ListFindSelect(Index) \
//		SendMessage(hWnd,LB_GETSEL,(WPARAM)Index,0L)

	//現在選択されているところをかえしまっするるるるるー
	int GetSelect() 
	{return SendMessage(hWnd,LB_GETCURSEL,0L,0L);};

	//リストの項目数
	int GetMax() 
	{return SendMessage(hWnd,LB_GETCOUNT,0L,0L);};

	//リストより項目の削除
	void Delete(int Index) 
	{SendMessage(hWnd,LB_DELETESTRING,(WPARAM)Index,0L);};

	//リストのIndexのところをStringに変更する
	void Change(int Index,char *String) 
	{
		SendMessage(hWnd,LB_DELETESTRING,(WPARAM)Index,0L); 
		SendMessage(hWnd,LB_INSERTSTRING,(WPARAM)Index,(LPARAM)String);
	}

	//リストの Index 番目を選択させる
	void Select(int Index) 
	{SendMessage(hWnd, LB_SETCURSEL, (WPARAM)Index, 0L);};

	//リストの Index の内容をゲット
	int GetData(int Index,char *buf)
	{return SendMessage(hWnd,LB_GETTEXT,(WPARAM)Index,(LPARAM)buf);};

	TListBoxEvent();
	void WmCommandCome(WPARAM wParam);
	//LBN_SELCANCEL	項目の選択がキャンセルされたことを示します。 
	SimpleEvent OnCancel	 ;
	//LBN_SELCHANGE	選択項目が変更されようとしていることを示します。 
	SimpleEvent OnChange	 ;

};

#endif

#endif
