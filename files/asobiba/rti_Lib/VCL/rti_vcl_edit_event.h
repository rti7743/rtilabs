/**********************************************************/
// エディトのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef EDIT_LIFE

#ifndef ___TEDITEVENTH
#define ___TEDITEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>

//エディトのイベントクラス
//エディトから作るコンポーネントはこれを継承させます.
//EDIT

class TEditEvent	: public TBaseObject
{
public:
	//最初の行の内容をバッファにコピー (1行入力専用に作ってみました)
	void Get(char *buf) 
	{SendMessage(hWnd,EM_GETLINE,0L,(LPARAM)buf);};
    
    //入力されているヤツを int 型に変換して返します
    int GetInt();

	//指定行の内容をバッファにコピー
	void GetNum(int Num,char *Buf) 
	{SendMessage(hWnd,EM_GETLINE,(WPARAM)Num,(LPARAM)Buf);};

	//エディットコントロールの行数を得る
	void GetRow() 
	{SendMessage(hWnd,EM_GETLINECOUNT,0L,0L);};

	//エディットコントロールの変更をする(変更=true)
	BOOL GetModify() 
	{return (BOOL)SendMessage(hWnd,EM_GETMODIFY,0L,0L) ;};

    //セット
    void Set(char *buf)
	{SetWindowText(hWnd, buf);};

	//エディットコントロールの変更フラグの変更(フラグfalseで現在変化なし)
	BOOL SetModify(BOOL flg) 
	{return (BOOL)SendMessage(hWnd,EM_SETMODIFY,(WPARAM)flg,0L);};

	//エディットコントロールの選択されている範囲を取得
	//(sp , ep は32ビットのアドレスで)
	void GetSel(int *sp,int *ep) 
	{SendMessage(hWnd,EM_GETSEL,(WPARAM)sp,(LPARAM)ep);};

	//エディットコントロールの選択されている範囲を設定
	void SetSel(int sp,int ep) 
	{SendMessage(hWnd,EM_GETSEL,(WPARAM)sp,(LPARAM)ep);};

	//エディットコントロールのすべてを選択
	void SetSelAll() 
	{SendMessage(hWnd,EM_GETSEL,(WPARAM)0L,(LPARAM)-1L);};

	//エディットコントロールの選択されているところを置き換える
	void ChangeString(char *String) 
	{SendMessage(hWnd,EM_REPLACESEL,(WPARAM)0L,(LPARAM)String);};

	//エディットコントロールに入力できる文字数を設定
	void SetLimit(int cchMax) 
	{SendMessage(hWnd,EM_LIMITTEXT,(WPARAM)cchMax,(LPARAM)0L);};

	//エディットコントロールをリードオンリー属性にする(リードオンリー = true )
	void SetReadOnly(bool fReadOnly) 
	{SendMessage(hWnd,EM_LIMITTEXT,(WPARAM)fReadOnly,(LPARAM)0L);};

	//エディットコントロールをアンドゥする
	void SetUndo() 
	{SendMessage(hWnd,EM_UNDO,(WPARAM)0L,(LPARAM)0L);};

	
	TEditEvent();
	void WmCommandCome(WPARAM wParam);
	//EN_CHANGE	編集されたとき
	SimpleEvent OnChange ;
	//EN_HSCROLL	ユーザーがスクロール バーをクリックしたことを示します。 
	SimpleEvent OnHScroll ;
	//EN_MAXTEXT	挿入部分が切り捨てられたことを示します。 
	SimpleEvent OnMaxText ;
	//EN_UPDATE	エディット コントロールが代わりのテキストを表示しようとしていることを示します。 
	SimpleEvent OnUpdate ;
	//EN_VSCROLL	ユーザーが垂直スクロール バーをクリックしたことを示します。 
	SimpleEvent OnVscroll ;
};




#endif

#endif
