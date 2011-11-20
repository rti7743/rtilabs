/**********************************************************/
// タブコントロールの土台
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef TOOLBAR_LIFE

#include <.\VCL\rti_vcl_toolbar.h>

TToolBar::TToolBar(TObject* hOwner,char *Caption,
				 DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,TOOLBARCLASSNAME);
	//ツールバーコントロール
    if ( !CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_TOOLBAR;
}

TToolBar::~TToolBar()
{
	DestroyComponent();
}
/**********************************************************/
/*
    追加
*/
/**********************************************************/
int TToolBar::Add(int iBitmap,BYTE fsState,BYTE fsStyle,DWORD dwData,
                   int iString,SimpleEvent CallRtn)
{
    TBBUTTON tbb;
    SendMessage(hWnd, TB_BUTTONSTRUCTSIZE,
        (WPARAM)sizeof(TBBUTTON), 0);
    tbb.iBitmap   = iBitmap; //ボタンイメージの番号　０から始まる
    tbb.idCommand = Application->GetId(); // WM_COMMANDの時のID 
    tbb.fsState   = fsState; // ボタンの状態　使用可能かなど 
    tbb.fsStyle   = fsStyle; // ボタンスタイル
    tbb.dwData    = dwData; // アプリケーション定義データ 
    tbb.iString   = iString; // ボタンストリング　０から始まる

    struct _ToolBarUsingStruct *tpuf;

    //各種設定
    tpuf = VToolBarUsing.at(ButtonCounter) ;
    tbb.iBitmap += tpuf->UsingBmpNo;
    tpuf->CallRtn = CallRtn;
    tpuf->idCommand = tbb.idCommand;

    //ボタンを増やす
    ButtonCounter++;

    return SendMessage(hWnd, TB_ADDBUTTONS, (WPARAM)1, 
           (LPARAM)&tbb);
}

/**********************************************************/
/*
 ビットマップ追加 最初がNULL だったら ファイル名を
    最初が NULL ではなかったら、ファイル名は NULL  
*/
/**********************************************************/
int TToolBar::AddBitMap(UINT nID,char *filename,int pace)
{
    TBADDBITMAP tbab;
    HINSTANCE   hInit;
    UINT nSetID;
    SendMessage(hWnd, TB_BUTTONSTRUCTSIZE,
        (WPARAM)sizeof(TBBUTTON), 0);
    if (nID == USER_DEFINED)
    {
        //ファイルリード
        hInit  = NULL;
        nSetID = (int) LoadImage(Application->GethInstance(),filename,IMAGE_BITMAP,
    	               0,0,LR_LOADFROMFILE);         //ビットマップハンドル制作
    }
    else
    {
        hInit  = HINST_COMMCTRL;
        nSetID = nID;
    }

    tbab.hInst =    hInit;
    tbab.nID   =    nSetID;
    //BITMAP登録
    int UsingNo = 
        SendMessage(hWnd, TB_ADDBITMAP, pace, (LPARAM)&tbab);

    struct _ToolBarUsingStruct *tpuf;
    for(int i = 0; i < pace;i ++)
    {
        tpuf = new _ToolBarUsingStruct();
        tpuf->UsingBmpNo = UsingNo;
        //最後尾に追加していく.
        VToolBarUsing.push_back( tpuf );

    }
    return UsingNo;
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TToolBar::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}
#endif //LIFE END
