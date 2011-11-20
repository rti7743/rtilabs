// RRichEdit.cpp: RRichEdit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RRichEdit.h"
#include "richedit.h"

RAutoLibraryWithException RRichEdit::AutoLib;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RRichEdit::RRichEdit()
{
	this->ReadFileHandle = NULL;
}

RRichEdit::~RRichEdit()
{
	if (this->ReadFileHandle) ::CloseHandle(this->ReadFileHandle);
}

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RRichEdit::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{
	if ( ! AutoLib.IsReading() )
	{
		AutoLib.Create("RICHED32.DLL");
	}

	RWindow::Create(inINstance,"RICHEDIT",isParentWindow,inID,inFlg,0);

}

//読み込み.
//失敗 RIOCannotOpenException
void RRichEdit::ReadFile(string inFilename) throw()
{
	if (this->ReadFileHandle) throw RIOCannotOpenException("すでに ファイルは開かれています。");

	this->ReadFileHandle = ::CreateFile(inFilename.c_str() , GENERIC_READ | GENERIC_WRITE , 0 ,NULL ,OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL);
	if (this->ReadFileHandle == NULL) throw RIOCannotOpenException("ファイル %s を開けませんでした" , inFilename);

	EDITSTREAM eds = {0};
	eds.dwCookie = (DWORD)this->ReadFileHandle;
	eds.dwError = 0;
	eds.pfnCallback = ReadingProc;

	::SendMessage(this->getWindowHandle() , EM_STREAMIN, SF_TEXT, (LPARAM)&eds);
}

DWORD CALLBACK RRichEdit::ReadingProc(DWORD dwCookie, LPBYTE pbBuf, LONG cb, LONG *pcb)
{
	::ReadFile( (HANDLE)dwCookie, pbBuf, cb, (LPDWORD)pcb, NULL);
	return FALSE; 
}


//タイトルを取得する
string RRichEdit::getTitle() const 
{
	return "";
}