// RFontCache.h: RFontCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFONTCACHE_H__216376D9_C454_463E_9161_29D73E42745E__INCLUDED_)
#define AFX_RFONTCACHE_H__216376D9_C454_463E_9161_29D73E42745E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDIBBitmap.h"
#include "RFont.h"
struct FontCacheData
{
	unsigned long	Hash;
	SIZE			Size;
	int				Hit;
};

class RFontCache  
{
public:
	RFontCache();
	virtual ~RFontCache();

	void Create(int inCacheSize , int inLineMax ,int inFontSize ,const string & FontName ,  bool Bold , bool Italic , bool Underline , bool Strike ,int Charset);

	void TextOut(RDIBBitmap* ioTargetImg ,int inX,int inY, const string & inName);
private:
	unsigned long makeHache(const string &inName) const;
private:
	RDIBBitmap	Canvas;
	RFont		NewFont;

	int				CacheSize;
	FontCacheData*	Cache;

	int				Width;
	int				Height;
};

#endif // !defined(AFX_RFONTCACHE_H__216376D9_C454_463E_9161_29D73E42745E__INCLUDED_)
