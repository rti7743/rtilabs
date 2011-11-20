// RFont.cpp: RFont クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RFont.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RFont::RFont()
{
	this->NewFont = NULL;
}

RFont::~RFont()
{
	if ( this->NewFont ) DeleteObject(this->NewFont);
}

void RFont::Create(int inFontSize ,const string & FontName ,  bool Bold , bool Italic , bool Underline , bool Strike ,int Charset)
{
	if ( this->NewFont ) DeleteObject(this->NewFont);

	LOGFONT		fontInfo = {0};
#ifndef _WIN32_WCE
	strcpy(fontInfo.lfFaceName , FontName.c_str() );
#else
	MY_USES_CONVERSION;
	wcscpy(fontInfo.lfFaceName , _A2W(FontName.c_str()) );
#endif
    fontInfo.lfHeight=inFontSize;
	fontInfo.lfWeight=(Bold==false)*FW_NORMAL+(Bold==true)*FW_BOLD;
	fontInfo.lfEscapement=0;
	fontInfo.lfWidth=0;
	fontInfo.lfItalic=Italic;
	fontInfo.lfUnderline=Underline;
    fontInfo.lfStrikeOut = Strike;
	fontInfo.lfCharSet=Charset;

	//フォントの作成.
	this->NewFont = CreateFontIndirect(&fontInfo);
}
