// RRichEdit.h: RRichEdit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RRICHEDIT_H__75405421_E243_40C0_886B_2902636D4A24__INCLUDED_)
#define AFX_RRICHEDIT_H__75405421_E243_40C0_886B_2902636D4A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RRichEdit;

#include "comm.h"
#include "RWindow.h"
#include "RAutoLibraryWithException.h"
#include "RDoNotInitaliseException.h"
#include "RNotSupportException.h"
#include "RIOCannotOpenException.h"

//リッチエディットコントロールの作成.
//参考 http://www.kumei.ne.jp/c_lang/sdk2/sdk_130.htm
//     http://www.kumei.ne.jp/c_lang/sdk2/sdk_131.htm
class RRichEdit : public RWindow  
{
public:
	RRichEdit();
	virtual ~RRichEdit();

	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();

	//読み込み
	//失敗 RIOCannotOpenException
	void ReadFile(string inFilename) throw();

	//DLLの読み込み、エクスポートを超簡単に行うクラス。
	static RAutoLibraryWithException AutoLib;
	//読み込みのコールバック.
	static DWORD CALLBACK ReadingProc(DWORD dwCookie, LPBYTE pbBuf, LONG cb, LONG *pcb);
	//タイトルを取得する
	string getTitle() const ;
private:

private:
	HANDLE ReadFileHandle;
};

#endif // !defined(AFX_RRICHEDIT_H__75405421_E243_40C0_886B_2902636D4A24__INCLUDED_)
