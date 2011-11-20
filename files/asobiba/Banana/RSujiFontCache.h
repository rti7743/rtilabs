// RSujiFontCache.h: RSujiFontCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSUJIFONTCACHE_H__0CDAD313_B3AC_4FA8_A84F_C0FB46122530__INCLUDED_)
#define AFX_RSUJIFONTCACHE_H__0CDAD313_B3AC_4FA8_A84F_C0FB46122530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDIBBitmap.h"
#include "RFont.h"

class RSujiFontCache  
{
public:
	RSujiFontCache();
	virtual ~RSujiFontCache();

	void Create(int inFontSize ,const string & FontName ,  bool Bold , bool Italic , bool Underline , bool Strike ,int Charset);

	void TextOut(HDC ioTargetDC ,int inX,int inY, unsigned int inNum) ;
private:
	RDIBBitmap	Canvas;
	RFont		NewFont;

	int				Width;
	int				Height;

	int				XPoint[10];
	int				YWidth[10];
};

#endif // !defined(AFX_RSUJIFONTCACHE_H__0CDAD313_B3AC_4FA8_A84F_C0FB46122530__INCLUDED_)
