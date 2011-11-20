// evalDlg.h : ヘッダー ファイル
//

#if !defined(AFX_EVALDLG_H__D892FCD6_B303_4B50_AA8C_5827CA713E3C__INCLUDED_)
#define AFX_EVALDLG_H__D892FCD6_B303_4B50_AA8C_5827CA713E3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEvalDlg ダイアログ

class CEvalDlg : public CDialog
{
// 構築
public:
	CEvalDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEvalDlg)
	enum { IDD = IDD_EVAL_DIALOG };
	CString	m_EvalString;
	CString	m_AnswerString;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEvalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEvalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDescentParsing();
	afx_msg void OnCheart();
	afx_msg void OnCancel();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//エラーの位置を視覚的に伝えるマーカー文字列を作成します.
	CString MakeErrorMakerString(int inErrorPosstion) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EVALDLG_H__D892FCD6_B303_4B50_AA8C_5827CA713E3C__INCLUDED_)
