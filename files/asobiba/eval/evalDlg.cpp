// evalDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "eval.h"
#include "evalDlg.h"

#include "RecursiveDescentParsing.h"
#include "TokenParser.h"
#include "MSScriptEval.h"
#include "XLException.h"
#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvalDlg ダイアログ

CEvalDlg::CEvalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvalDlg)
	m_EvalString = _T("1+2*3+(1+1)*2");
	m_AnswerString = _T("");
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEvalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvalDlg)
	DDX_Text(pDX, IDC_EVALSTRING_EDIT, m_EvalString);
	DDX_Text(pDX, IDC_ANSWER_EDIT, m_AnswerString);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEvalDlg, CDialog)
	//{{AFX_MSG_MAP(CEvalDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDRECURSIVE_DESCENT_PARSING, OnDescentParsing)
	ON_BN_CLICKED(IDCHEART, OnCheart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvalDlg メッセージ ハンドラ

BOOL CEvalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CEvalDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CEvalDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//エラーの位置を視覚的に伝えるマーカー文字列を作成します.
CString CEvalDlg::MakeErrorMakerString(int inErrorPosstion) const
{	
	CString r;
	for(int i = 0 ; i < inErrorPosstion ; i ++)
	{
		r += ' ';
	}
	r += "↑ここら辺です";

	return r;
}

//再帰下降構文解析 で解く
void CEvalDlg::OnDescentParsing() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->UpdateData(TRUE);
	
	//計算するクラス
	RecursiveDescentParsing rdp;
	//字句パース
	TokenParser tp;
	bool r = tp.Set((const char*)this->m_EvalString);
	if (!r)
	{
		CString kokorahen = MakeErrorMakerString(tp.getErrorPosstion());
		
		CString message;
		message.Format("%d文字目付近を字句解析中にエラーが見つかりました\r\n%s\r\n%s"
			,tp.getErrorPosstion()
			,(const char*)this->m_EvalString
			,(const char*)kokorahen
			);
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}
	double ans = 0;
	try
	{
		//再帰下降構文解析
		ans = rdp.Run(&tp);
	}
	catch(XLException & e)
	{
		CString message;
		message.Format("式を実行中にエラーが発生しました\r\n%s" , e.getMessage() );
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}

	//結果の表示
	this->m_AnswerString.Format("%.10g" , ans);

	this->UpdateData(FALSE);
}

//チート で解く
void CEvalDlg::OnCheart() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	this->UpdateData(TRUE);

	//実行する jscript エンジン
	MSScriptEval mse;
	double ans = 0;
	try
	{
		//実行する
		ans = mse.Eval((const char*)this->m_EvalString);
	}
	catch(_com_error & e)
	{
		CString message;
		message.Format("実行中にエラーが発生しました\r\n%s" , e.ErrorMessage() );
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}
	if (mse.IsError())
	{
		CString kokorahen = MakeErrorMakerString(mse.getErrorPosstion());
		
		CString message;
		message.Format("%d文字目付近にエラーが見つかりました\r\nエラー内容:%s\r\n%s\r\n%s"
			,mse.getErrorPosstion()
			,mse.getErrorMessage().c_str()			//VC++ではこの書き方でも大丈夫なはず
			,(const char*)this->m_EvalString
			,(const char*)kokorahen
			);
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}

	//結果の表示
	this->m_AnswerString.Format("%.10g" , ans);

	this->UpdateData(FALSE);
}



void CEvalDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	//とりあえず、通常のやり方で計算式を解いてみる.
	this->OnDescentParsing();

//	CDialog::OnOK();
}

void CEvalDlg::OnCancel() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	CDialog::OnCancel();
}
