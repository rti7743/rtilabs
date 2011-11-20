/**********************************************************/
// コンボボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef COMB_LIFE

#ifndef ___TCOMBEVENTH
#define ___TCOMBEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//コンボボックスのイベントクラス
//コンボボックスから作るコンポーネントはこれを継承させます.
//COMBOBOX

class TCombEvent	: public TBaseObject
{
public:
		//リストの最後に追加
		void Add(char *String) 
		{SendMessage(hWnd,CB_ADDSTRING,0L,(LPARAM)String);};

		//リストのPosに追加
		void Insert(int Pos,char *String) 
		{SendMessage(hWnd,CB_INSERTSTRING,(WPARAM)Pos,(LPARAM)String);};

		//リストをクリア
		void Clear() 
			{SendMessage(hWnd,CB_RESETCONTENT,0L,0L);};

		//現在選択されているところをかえしまっするるるるるー
		int GetSelect() 
			{return SendMessage(hWnd,CB_GETCURSEL,0L,0L);};

		//リストの項目数
		int GetMax()
			{return SendMessage(hWnd,CB_GETCOUNT,0L,0L);};

		//リストより項目の削除
		void Delete(int Index)
		{SendMessage(hWnd,CB_DELETESTRING,(WPARAM)Index,0L);};

		//リストのIndexのところをStringに変更する
		void Change(int Index,char *String) 
		{
			SendMessage(hWnd,CB_DELETESTRING,(WPARAM)Index,0L);
			SendMessage(hWnd,CB_INSERTSTRING,(WPARAM)Index,(LPARAM)String);
		};

		//リストの Index 番目を選択させる
		void Select(int Index)
		{SendMessage(hWnd, CB_SETCURSEL, (WPARAM)Index, 0L);};

		//リストの Index の内容をゲット
		char* GetData(int Index)
		{return (char*)SendMessage(hWnd, CB_GETITEMDATA, (WPARAM)Index, 0L);};

	TCombEvent();
	void WmCommandCome(WPARAM wParam);
	SimpleEvent	OnClose;
	SimpleEvent	OnDropDown;
	SimpleEvent	OnEditChange;
	SimpleEvent	OnEditUpDate;
	SimpleEvent	OnChange;
	SimpleEvent	OnCancel;
	SimpleEvent	OnOk;
};



#endif

#endif
