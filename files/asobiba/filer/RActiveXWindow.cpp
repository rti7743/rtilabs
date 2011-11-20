// RActiveXWindow.cpp: RActiveXWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RActiveXWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RActiveXWindow::RActiveXWindow()
{
	this->Unknown = NULL;
}

RActiveXWindow::~RActiveXWindow()
{
	if (this->Unknown)
	{
		this->Unknown->Release();
		this->Unknown = NULL;
	}
}
#include <Exdisp.h>

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RActiveXWindow::Create(HINSTANCE inINstance, const string inProgID, IID inIID , void** ioInterface,
					   HWND isParentWindow,int inID, DWORD inFlg,DWORD inExStyle) 
					   throw(RNotSupportException,RDoNotInitaliseException)
{
	USES_CONVERSION;
	AtlActiveXBrige *aaxb = AtlActiveXBrige::getInstance();

	HWND hwndChild = ::CreateWindowEx(inExStyle,"AtlAxWin",
                                    inProgID.c_str() , 
                                    inFlg | WS_CHILD|WS_VISIBLE,
                                    0,0,0,0,
                                    isParentWindow,
									(HMENU)inID,
                                    inINstance,
                                    NULL);

	MountWindow(hwndChild);

	ASSERT(Unknown == NULL);
	if (!SUCCEEDED(aaxb->AtlAxGetControl( this->getWindowHandle()  ,(IUnknown **) &Unknown)))
	{
		throw RNotSupportException("AtlAxGetControlに失敗しました");
	}

	
    // get an interface .
	if ( !SUCCEEDED( Unknown->QueryInterface(inIID, ioInterface) ) )
	{
		throw RNotSupportException("インターフェース構築に失敗しました");
	}

	if (! (*ioInterface))
	{
		throw RNotSupportException("インターフェース構築(QueryInterface)は SUCCEEDED ですが、実際に構築されていません");
	}

}
