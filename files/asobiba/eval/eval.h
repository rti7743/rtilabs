// eval.h : EVAL アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_EVAL_H__A66E0DF5_9F85_44A6_B48D_B7355E3A3CE2__INCLUDED_)
#define AFX_EVAL_H__A66E0DF5_9F85_44A6_B48D_B7355E3A3CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CEvalApp:
// このクラスの動作の定義に関しては eval.cpp ファイルを参照してください。
//

class CEvalApp : public CWinApp
{
public:
	CEvalApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEvalApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CEvalApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EVAL_H__A66E0DF5_9F85_44A6_B48D_B7355E3A3CE2__INCLUDED_)
