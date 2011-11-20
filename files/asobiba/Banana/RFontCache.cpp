// RFontCache.cpp: RFontCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RFontCache.h"
#include "RGraphics.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RFontCache::RFontCache()
{
	Cache = NULL;
}

RFontCache::~RFontCache()
{
	if ( Cache ) delete [] Cache;
}

void RFontCache::Create(int inCacheSize , int inLineMax ,int inFontSize ,const string & FontName ,  bool Bold , bool Italic , bool Underline , bool Strike ,int Charset)
{
	NewFont.Create(inFontSize , FontName , Bold , Italic , Underline , Strike , Charset);

	//そのフォントで描画した場合、どれくらいの大きさになるかを求めてみる。
	{
		HDC dc = GetDC(NULL);
		SelectObject( dc , this->NewFont.getFont() );
		string testString = "メイぽんです。メイは処女でございます。";

		SIZE size;

	#ifndef _WIN32_WCE
		GetTextExtentPoint32(dc  , testString.c_str() , testString.size() , &size );
	#else
		MY_USES_CONVERSION;
		unsigned short * wstr = _A2W(testString.c_str());
		int wsize = lstrlen(wstr);
		GetTextExtentPoint32(dc  , wstr , wsize , &size );
	#endif

		ReleaseDC(NULL,dc);

		this->Width = size.cx / testString.size();
		this->Height = size.cy;
	}

	//モノクロビットマップの作成.
	Canvas.Create( this->Width * inLineMax,this->Height * inCacheSize, 16);
	SelectObject( Canvas.getDC() , this->NewFont.getFont() );
	SetTextColor(Canvas.getDC(),RGB(0,0,0) );	//白
	SetBkColor(Canvas.getDC(),RGB(255,255,255));	//黒
	SetBkMode(Canvas.getDC(),OPAQUE);

	this->CacheSize = inCacheSize;
	this->Cache = new FontCacheData[inCacheSize + 1];
	memset(this->Cache , 0 , sizeof(FontCacheData) * inCacheSize);

	TRACE("FONT CACHE サイズを %d 確保しました\n" , inCacheSize);
	TRACE("FONT CACHE 仮想画面は %d,%d 確保しました\n" , this->Width , this->Height);

}

void RFontCache::TextOut(RDIBBitmap* ioTargetImg ,int inX,int inY, const string & inName)
{
	//ハッシュ値の計算.
	unsigned long hash = makeHache(inName);

	int avg = 0;
	int noEntry = -1;
	for(int i = 0 ; i < this->CacheSize ; i ++)
	{
		FontCacheData * p = &this->Cache[i];
		if ( p->Hash == hash )
		{
			p->Hit ++;

			//キャッシュにヒット
//			::BitBlt(ioTargetImg->getDC() , inX , inY , p->Size.cx , p->Size.cy , Canvas.getDC() , 0 , this->Height * i , SRCAND);
			RGraphics::AndCopy( &Canvas , 0 , this->Height * i ,  p->Size.cx , p->Size.cy , ioTargetImg , inX , inY );
			return ;
		}
		else if(p->Hit == 0)
		{
			//キャッシュの空きをみっけ.
			noEntry = i;
		}
		avg += p->Hit;
	}


	//さて、キャッシュにヒットしなかったわけだが.
	TRACE("文字列 %s はキャッシュにありません\n" , inName.c_str() );

	if (noEntry == -1)
	{
		//キャッシュが満杯なわけだが
		TRACE("FONT CACHE はおなかいっぱいです。\n");

		avg = avg / this->CacheSize / 2;

		//ヒット数が平均以下のやつをすべて解放する.
		for(int i = 0 ; i < this->CacheSize ; i ++)
		{
			FontCacheData * p = &this->Cache[i];
			if (p->Hit <= avg)
			{
				TRACE("平均以下の %d (%d/%d)を落第させました\n" , i , p->Hit , avg);

				p->Hash = 0;
				p->Hit = 0;
				noEntry = i;	//常に上書きし、最後に書き込んだやつを今回は利用する.

				//消し消し
				::BitBlt(Canvas.getDC() , 0 , this->Height * i , Canvas.getWidth() , this->Height , NULL , 0 , 0 , WHITENESS);
			}
		}
		//もし誰もけり落とせない場合は、すべてｼﾞｪﾉｻｲﾄﾞ
		if (noEntry == -1)
		{
			memset(this->Cache , 0 , sizeof(FontCacheData) * this->CacheSize);
			::BitBlt(Canvas.getDC() , 0 , 0 , Canvas.getWidth() , Canvas.getHeight() , NULL , 0 , 0 , WHITENESS);
			noEntry = 0;
		}
	}

	//で、新規作成するわけだが.
	TRACE("新規は %d に割り当てました\n" , noEntry);
	{
		FontCacheData * p = &this->Cache[noEntry];
		RECT rc ;
		rc.top = this->Height * noEntry;
		rc.left = 0;
		rc.bottom = this->Height;
		rc.right = Canvas.getWidth();

	#ifndef _WIN32_WCE
		GetTextExtentPoint32( Canvas.getDC() , inName.c_str() , inName.size() , &p->Size );
		DrawText( Canvas.getDC()  , inName.c_str() , inName.size() , &rc , DT_NOCLIP);
	#else
		MY_USES_CONVERSION;
		unsigned short * wstr = _A2W(inName.c_str());
		int wsize = lstrlen(wstr);
		GetTextExtentPoint32( Canvas.getDC() , wstr , wsize , &p->Size );
		DrawText( Canvas.getDC()  , wstr , wsize , &rc , DT_NOCLIP);
	#endif

		p->Hash = hash;
		p->Hit = 1;

		//で、結果を転送と、、あーながかった。
//		::BitBlt(ioTargetImg->getDC() , inX , inY , p->Size.cx , p->Size.cy , Canvas.getDC() , 0 , rc.top , SRCAND);
		RGraphics::AndCopy( &Canvas ,  0 , rc.top,  p->Size.cx , p->Size.cy , ioTargetImg , inX , inY );
	}

}

//文字列が一致するかどうかはハッシュで判断します。
// ?/40億 で重複します(w まぁ、いいんぢゃない.
unsigned long RFontCache::makeHache(const string &inName) const
{
	unsigned long ret = 0;

	for( const char * p = inName.c_str() ; *p ; p++)
		ret += (unsigned char) *p;

	return ret;
}
