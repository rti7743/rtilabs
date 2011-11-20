// RDialogUtil.h: RDialogUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDIALOGUTIL_H__36682679_01FE_4A0C_9DAF_9B54F6CD7052__INCLUDED_)
#define AFX_RDIALOGUTIL_H__36682679_01FE_4A0C_9DAF_9B54F6CD7052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RDialogUtil  
{
public:
	//ファイルを開くダイアログ
	static string FileOpenDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
		const string & inDefualtFilename ,const TCHAR* inFilter);
	//ファイルを保存ダイアログ
	static string FileSaveDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
		const string & inDefualtFilename ,const TCHAR* inFilter);
	//パス選択ダイアログ
	static string SelectPath(HWND inWindowHandle,const string &inTitle,const string &inPath);

};

#endif // !defined(AFX_RDIALOGUTIL_H__36682679_01FE_4A0C_9DAF_9B54F6CD7052__INCLUDED_)
