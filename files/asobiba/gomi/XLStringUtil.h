// XLStringUtil.h: XLStringUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_)
#define AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XLStringUtil  
{
public:
	XLStringUtil();
	virtual ~XLStringUtil();

	static string UrlDecode(const string & inUrl);

	//左右の空白の除去
	static string chop(const string & inString);

	//置換
	static string Replace(const string &inTarget ,const string &inOld ,const string &inNew);

	//タグをエスケープ 基本的に PHP の htmlspecialchars と同じ.
	//http://search.net-newbie.com/php/function.htmlspecialchars.html
	static string htmlspecialchars(const string &inStr);

	//\nを<br>に 基本的に PHP の nl2br と同じ.
	static string nl2br(const string &inStr);

	//inTarget の inStart ～ inEnd までを取得
	static string Cut(const string &inTarget , const string & inStart , const string &inEnd , string * outNext = NULL);

	static void test();

};

#endif // !defined(AFX_XLSTRINGUTIL_H__B1B80C81_45F8_4E25_9AD6_FA9AC57294F5__INCLUDED_)
