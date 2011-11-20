// RDialogUtil.cpp: RDialogUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RDialogUtil.h"
#include <shlobj.h>
#include <commdlg.h>
#include "RMessageBox.h"
#include "RAutoLibraryWithException.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//ファイルを開くダイアログ
string RDialogUtil::FileOpenDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
							 const string & inDefualtFilename , const TCHAR* inFilter)
{
#ifndef _WIN32_WCE
    OPENFILENAME ofn = { 0 };
	char retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		strcpy(retFilenameBuffer , inDefualtFilename.c_str() );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = inTitle.c_str();
	ofn.lpstrInitialDir = inPath.c_str();

    if (!GetOpenFileName(&ofn)) 
	{
		return "";
	}

	return retFilenameBuffer;
#else
	MY_USES_CONVERSION;

    OPENFILENAME ofn = { 0 };
	TCHAR retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		lstrcpy(retFilenameBuffer , _A2W(inDefualtFilename.c_str()) );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = _A2W(inTitle.c_str());
	ofn.lpstrInitialDir = _A2W(inPath.c_str());

	try
	{
		//gsgetfile.dll が使えるなら使う.
		RAutoLibraryWithException gsgetfile;
		gsgetfile.Create("gsgetfile.dll");
		typedef BOOL (WINAPI *gsGetOpenFileNameDef)(OPENFILENAME* pofn) ;

		gsGetOpenFileNameDef gsGetOpenFileName = (gsGetOpenFileNameDef) gsgetfile.GetProcAddress("gsGetOpenFileName");
		if (! gsGetOpenFileName(&ofn) )
		{
			return "";
		}
		return _W2A(retFilenameBuffer);
	}
	catch(RException)
	{
		//nop
	}

	//gsgetfile が使えないようなので、OSのを使う.
	if (!GetOpenFileName(&ofn)) 
	{
		return "";
	}


	return _W2A(retFilenameBuffer);
#endif
}

//ファイルを保存ダイアログ
string RDialogUtil::FileSaveDialog(HWND inWindowHandle,const string &inTitle,const string &inPath,
							 const string & inDefualtFilename ,const TCHAR* inFilter)
{
#ifndef _WIN32_WCE
    OPENFILENAME ofn = { 0 };
	char retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		strcpy(retFilenameBuffer , inDefualtFilename.c_str() );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    ofn.lpstrTitle = inTitle.c_str();
	ofn.lpstrInitialDir = inPath.c_str();

    if (!GetSaveFileName(&ofn)) 
	{
		return "";
	}

	return retFilenameBuffer;
#else
	MY_USES_CONVERSION;

    OPENFILENAME ofn = { 0 };
	TCHAR retFilenameBuffer[MAX_PATH] = {0};

    if(! inDefualtFilename.empty() )
	{
		lstrcpy(retFilenameBuffer , _A2W(inDefualtFilename.c_str()) );
	}

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = inWindowHandle;
    ofn.lpstrFilter = inFilter;
    ofn.lpstrFile = retFilenameBuffer;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    ofn.lpstrTitle = _A2W(inTitle.c_str());
	ofn.lpstrInitialDir = _A2W(inPath.c_str());

	try
	{
		//gsgetfile.dll が使えるなら使う.
		RAutoLibraryWithException gsgetfile;
		gsgetfile.Create("gsgetfile.dll");
		typedef BOOL (*gsGetSaveFileNameDef)(OPENFILENAME* pofn) ;
		gsGetSaveFileNameDef gsGetSaveFileName = (gsGetSaveFileNameDef) gsgetfile.GetProcAddress("gsGetSaveFileName");
		if (! gsGetSaveFileName(&ofn) )
		{
			return "";
		}
		return _W2A(retFilenameBuffer);
	}
	catch(RException)
	{
		//nop
	}

	//gsgetfile が使えないようなので、OSのを使う.
    if (!GetSaveFileName(&ofn)) 
	{
		return "";
	}

	return _W2A(retFilenameBuffer);
#endif
}

//パス選択ダイアログ
string RDialogUtil::SelectPath(HWND inWindowHandle,const string &inTitle,const string &inPath) 
{
#ifndef _WIN32_WCE
	char retPathBuffer[MAX_PATH] = {0};

	bool ret = false;
	IMalloc *m;
	if(SUCCEEDED(SHGetMalloc(&m)))
	{
		BROWSEINFO b;

		b.hwndOwner = inWindowHandle;
		b.pidlRoot = NULL;
		b.pszDisplayName = retPathBuffer;
		b.lpszTitle = inTitle.c_str() ;
		b.ulFlags = BIF_RETURNONLYFSDIRS; //フォルダのみ
		b.lpfn = NULL;
		b.lParam = 0;
		b.iImage = 0;
		LPITEMIDLIST id = SHBrowseForFolder(&b); //ダイアログの表示
		if(id)
		{
			SHGetPathFromIDList(id, retPathBuffer); //パス名を得る

			if (retPathBuffer[lstrlen(retPathBuffer)-1]!='\\') 
			{
				strcat(retPathBuffer,"\\"); //最後は \\ 
			}
			m->Free(id);
		}
		m->Release();
	}
	return retPathBuffer;
#else
	RMessageBox::Message("Wince ではこの機能は実装されていないので使えません。");
	return inPath;
#endif
}

