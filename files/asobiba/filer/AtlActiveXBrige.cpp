// AtlActiveXBrige.cpp: AtlActiveXBrige クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "AtlActiveXBrige.h"

CComModule _Module;


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AtlActiveXBrige::AtlActiveXBrige()
{
	ExportAtlAxWinInit = NULL;
	ExportAtlAxGetControl = NULL;

	if (! Lib.Create("atl.dll") ) return ;
	ExportAtlAxWinInit = (ATLAXWININIT) Lib.GetProcAddress("AtlAxWinInit") ;
	ExportAtlAxGetControl = (ATLAXGETCONTORL) Lib.GetProcAddress("AtlAxGetControl") ;
	ExportAtlAxCreateControl = (ATLAXCREATECONTORL) Lib.GetProcAddress("AtlAxCreateControl") ;
	ExportAtlAxAttachControl = (ATLAXATTACHCONTROL) Lib.GetProcAddress("AtlAxAttachControl") ;

	//クラスの作成.
	if (ExportAtlAxWinInit) ExportAtlAxWinInit();
}

AtlActiveXBrige::~AtlActiveXBrige()
{

}

AtlActiveXBrige* AtlActiveXBrige::getInstance() 
{
	static AtlActiveXBrige aaxb;
	return &aaxb;
}


HRESULT AtlActiveXBrige::AtlAxGetControl(HWND inWindow , IUnknown** outPP)
{
	ASSERT(ExportAtlAxGetControl != NULL);
	if (!ExportAtlAxGetControl) return -1;

	return ExportAtlAxGetControl(inWindow , outPP);
}

HRESULT AtlActiveXBrige::AtlAxCreateControl(LPCOLESTR inName, HWND inWindow, IStream* inStream, IUnknown** outUnkContainer)
{
	ASSERT(ExportAtlAxCreateControl != NULL);
	if (!ExportAtlAxCreateControl) return -1;

	return ExportAtlAxCreateControl(inName, inWindow, inStream, outUnkContainer);
}

HRESULT AtlActiveXBrige::AtlAxAttachControl(IUnknown* inControl, HWND inWindow, IUnknown** outUnkContainer)
{
	ASSERT(ExportAtlAxAttachControl != NULL);
	if (!ExportAtlAxAttachControl) return -1;

	return ExportAtlAxAttachControl(inControl , inWindow , outUnkContainer);
}
