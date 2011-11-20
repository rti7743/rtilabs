// RPDFView.cpp: RPDFView クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RPDFView.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RPDFView::RPDFView()
{

}

RPDFView::~RPDFView()
{
 
}

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RPDFView::Create(HINSTANCE inINstance,
	HWND isParentWindow,int inID , DWORD inFlg) throw()
{
	RActiveXWindow::Create(inINstance,"PDF.PdfCtrl.5" , PdfLib::DIID__DPdf, (void**)&PDF,isParentWindow,0, 0 ,WS_EX_STATICEDGE | WS_EX_ACCEPTFILES);

	puts("Control is OK");
}

//表示.
void RPDFView::Open(const string inFilename)
{
	USES_CONVERSION;

	BSTR a = T2OLE( inFilename.c_str() );
	PDF->LoadFile(a);
}

//タイトルを取得する
string RPDFView::getTitle() const 
{
	return "";
}



