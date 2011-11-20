// RNiseOle.h: RNiseOle クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNISEOLE_H__C1E91CFE_0249_4248_A95F_E542ABB02BC7__INCLUDED_)
#define AFX_RNISEOLE_H__C1E91CFE_0249_4248_A95F_E542ABB02BC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNoComplateException.h"
#include "RWin32Exception.h"
#include "RWindow.h"


//Kinderのソースを参考に作ってみたニセOLEです。
//楽しんでいただければ幸いです。(w
class RNiseOle  : public RWindow
{
public:
	RNiseOle();
	virtual ~RNiseOle();

	//起動
	void Exec(HINSTANCE inINstance,	HWND isParentWindow,const string inFilename) throw();
	//タイトルを取得する
	string getTitle() const ;
private:
	//拡張子に対応した実行ファイル名の取得
	string FindExecutable(const string inFilename) const;

	// EnumWindowsProcコールバック関数
	static BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam);
private:

	PROCESS_INFORMATION	Procinfo;
	HWND				TargetWindow;
};

#endif // !defined(AFX_RNISEOLE_H__C1E91CFE_0249_4248_A95F_E542ABB02BC7__INCLUDED_)
