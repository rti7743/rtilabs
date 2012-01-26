// XLStringUtil.cpp: XLStringUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "XLStringUtil.h"

USE_TEST(XLStringUtil)


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XLStringUtil::XLStringUtil()
{

}

XLStringUtil::~XLStringUtil()
{

}

#define TRIM_SPACE(x)	((x) == ' ' || (x) == '\t' || (x) == '\r' || (x) == '\n')

string XLStringUtil::UrlDecode(const string & inUrl)
{
	static const char xc[] = "0123456789ABCDEF";

	const char* url = inUrl.c_str();
	char* orinal = new char[ inUrl.size() + 1];
	char* nomal = orinal;


	for( ; *url ; url++ , nomal++)
	{
		if (*url != '%')
		{
			if (*url == '+')	*nomal = ' ';
			else				*nomal = *url;
		}
		else
		{
			//% だけで終わっている文字列の排除
			if ( *(url+1) == 0 ){	*nomal = '%';	continue;	}

			//%Z とかのわけわかめの排除
			char* first = strchr(xc,*(url+1));
			if ( first == NULL ) {	*nomal = '%';	continue;	}

			//%A だけで終わっている文字列の排除
			if ( *(url+2) == 0 ){	*nomal = '%';	continue;	}

			//%AZ とかのわけわかめの排除
			char* second = strchr(xc,*(url+2));
			if ( second == NULL ) {	*nomal = '%';	continue;	}

			*nomal = 16 * (first - xc) + (second - xc);
			url ++;
			url ++;
		}
	}
	*nomal = '\0';
	//文字列に入れて、バッファをクリア.
	string nomalString = orinal;
	delete [] orinal;

	return nomalString;
}


//左右の空白の除去
string XLStringUtil::chop(const string & inString)
{
	const char * p = inString.c_str();
	int size = inString.size();
	int l;
	for(l = 0 ; l < size ; l++)
	{
		if ( !TRIM_SPACE(p[l]) )
		{
			break;
		}
	}
	if (l == size)
	{
		return "";	//すべて空白
	}

	int r;
	for(r = size-1 ; r >= 0 ; r--)
	{
		if ( !TRIM_SPACE(p[r]) )
		{
			break;
		}
	}
	ASSERT (l  <= r);

	return inString.substr(l , r - l + 1);
}

//置換
string XLStringUtil::Replace(const string &inTarget ,const string &inOld ,const string &inNew)
{
	string ret;
	ret.reserve( inTarget.size() );	//先読み.

	int full = inTarget.size();
	int oldpos = 0;
	int pos = 0;
	//置換される文字列の長さ
	int replaceTargetSize = inOld.size();

	while( (pos = inTarget.find( inOld , oldpos)) != -1 )
	{
		ret += inTarget.substr(oldpos , pos - oldpos);
		ret += inNew;

		oldpos = pos + replaceTargetSize ;
	}
	ret += inTarget.substr(oldpos , full - oldpos);
	return ret;

}

//タグをエスケープ 基本的に PHP の htmlspecialchars と同じ.
//http://search.net-newbie.com/php/function.htmlspecialchars.html
string XLStringUtil::htmlspecialchars(const string &inStr)
{
	return Replace(Replace(Replace(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;");
}


//\nを<br>に 基本的に PHP の nl2br と同じ.
string XLStringUtil::nl2br(const string &inStr)
{
	return Replace(inStr , "\r\n" , "<br>");
}

//inTarget の inStart ～ inEnd までを取得
string XLStringUtil::Cut(const string &inTarget , const string & inStart , const string &inEnd , string * outNext )
{
	const char * p = NULL;
	if ( !inStart.empty() )
	{
		p = strstr(inTarget.c_str() , inStart.c_str() );
		if (p == NULL ) return "";
		p += inStart.size();
	}
	else
	{
		p = inTarget.c_str();
	}

	const char * p2 = NULL;
	if ( ! inEnd.empty() )
	{
		p2 = strstr(p , inEnd.c_str() );
		if (p2 == NULL ) return "";
	}
	else
	{
		p2 = p + strlen(p);
	}

	string ret = string( p , 0 , p2 - p );
	if (outNext)
	{
		*outNext = string(p2 + inEnd.size());
	}

	return ret;
}


void XLStringUtil::test()
{
	{
		string r;
		ASSERT( (r = chop("abc") ) == "abc");
		ASSERT( (r = chop(" abc") ) == "abc");
		ASSERT( (r = chop(" abc  ") ) == "abc");
		ASSERT( (r = chop(" abc \r\n") ) == "abc");
		ASSERT( (r = chop("			abc \r\n") ) == "abc");
	}
}
