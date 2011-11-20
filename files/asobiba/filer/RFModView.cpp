// RFModView.cpp: RFModView クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "fmod_define.h"
#include "RFModView.h"
#include "Rfiles.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RFModView::RFModView()
{
}

RFModView::~RFModView()
{
	
}

void RFModView::Create(HINSTANCE inINstance,HWND isParentWindow) throw()
{
	RNormalWindow::Create(inINstance,isParentWindow,0,
		WS_CHILD | WS_VISIBLE  ,0);
}


//サポートしてます?
bool RFModView::IsSupport(const string inFilename)
{
	//とりあえず、拡張子判定.
	string ext = Rfiles::Filename2Ext(inFilename);
	return  (ext == "MID" || ext == "S3M" || ext == "XM" || ext == "IT" || ext == "RMI" ||
		ext == "SGT" || ext == "WAV" || ext == "MP2" || ext == "MP3" || ext == "OGG" || 
		ext == "WMA" || ext == "ASF" );
}


//タイトルの取得
string RFModView::getTitle() const 
{
	return "";
}

