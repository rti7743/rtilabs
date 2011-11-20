// eval.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "eval.h"
#include "evalDlg.h"

//テスト用
#include "TokenParser.h"
#include "RecursiveDescentParsing.h"
#include "MSScriptEval.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEvalApp

BEGIN_MESSAGE_MAP(CEvalApp, CWinApp)
	//{{AFX_MSG_MAP(CEvalApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvalApp クラスの構築

CEvalApp::CEvalApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CEvalApp オブジェクト

CEvalApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEvalApp クラスの初期化

BOOL CEvalApp::InitInstance()
{
	//COMの初期化
	CoInitialize(NULL);

	AfxEnableControlContainer();

	//テスト
	//テストはデバッグビルドだったら毎回実行します。
#ifdef _DEBUG
	TokenParser::test();
	RecursiveDescentParsing::test();
	MSScriptEval::test();
#endif



	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	CEvalDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//       記述してください。
	}

	//COMの開放
	CoUninitialize();

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
