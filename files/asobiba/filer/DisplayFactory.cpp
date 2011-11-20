// DisplayFactory.cpp: DisplayFactory クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "DisplayFactory.h"
#include "DisplayDebugPrintf.h"
#include "DisplayWin32.h"
#include "DisplayBlank.h"
#include "DisplayListViewBigIconWin32.h"
#include "DisplayTextView.h"
#include "DisplayBitmapView.h"
#include "DisplayWebBrowserView.h"
#include "DisplayMediaView.h"
#include "DisplayPDFView.h"
#include "DisplayNiseOLE.h"
#include "RNotSupportException.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DisplayFactory::DisplayFactory()
{

}

DisplayFactory::~DisplayFactory()
{

}

//Display クラスの作成.
//inDisplayType	作成するディスプレイの種類
//IsLeft		左側に配置?
//inRfileType	ディスプレイで表示する RFILE の種類
//サポートしていない inDisplayType を表示すると RNotSupportException が投げられる。
Display* DisplayFactory::New(__DISPLAYTYPE inDisplayType, Window* inWindow , __RFILETYPE inRfileType)
{
	Display* p = NULL;
	try
	{
		switch(inDisplayType)
		{
		case __DISPLAYTYPE_DEBUG:
			p = new DisplayDebugPrintf(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_BLANK:
			p = new DisplayBlank(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_LISTVIEW_REPORT:
			p = new DisplayWin32(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_LISTVIEW_BIGICON:
			p = new DisplayListViewBigIconWin32(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_TEXT:
			p = new DisplayTextView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_BITMAP:
			p = new DisplayBitmapView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_WEBBROWSER:
			p = new DisplayWebBrowserView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_MEDIA:
			p = new DisplayMediaView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_PDF:
			p = new DisplayPDFView(inWindow,inRfileType);
			return p;
		case __DISPLAYTYPE_PREVIEW_NISEOLE:
			p = new DisplayNiseOLE(inWindow,inRfileType);
			return p;
		}
	}
	catch(RDoNotInitaliseException e)
	{
		if (p) p->Dec();
		throw ;
	}

	throw RNotSupportException("DisplayFactory::指定されたディスプレイ %d はサポートしてません",inDisplayType);
	return NULL;
}

