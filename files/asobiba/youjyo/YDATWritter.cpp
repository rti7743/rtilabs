// YDATWritter.cpp: YDATWritter クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "YDATWritter.h"
#include "RDateConv.h"


//>> のレスポンスで使える文字
#define isrespons(x) (isdigit(x) || x=='-')

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YDATWritter::YDATWritter()
{

}

YDATWritter::~YDATWritter()
{

}

string YDATWritter::make(const string & inName ,const string & inMail ,const string & inText,const string & inAppend)
{
	//ﾅｰﾅｰｼｰﾆｰｲｰﾔｰ♪<>sage<>2002/11/14 00:55<>ｷﾀ━━━━━━━━━ξ´ー`)━━━━━━━━━!<>
	return inName + "<>" + inMail + "<>" + RDateConv::UnixTime2chTime(time(NULL)) + " " + inAppend + "<>" + inText + "<>\n";
}
string YDATWritter::make(const string & inName ,const string & inMail ,const string & inText , const string & inSubject,const string & inAppend)
{
	//ﾅｰﾅｰｼｰﾆｰｲｰﾔｰ♪<>sage<>2002/11/14 00:55<>ｷﾀ━━━━━━━━━ξ´ー`)━━━━━━━━━!<>
	return inName + "<>" + inMail + "<>" + RDateConv::UnixTime2chTime(time(NULL)) + " " + inAppend + "<>" + inText + "<>" + inSubject + "\n";
}

string YDATWritter::makeAborn(const string & inAbornName)
{
	return inAbornName + "<>" + inAbornName + "<>" + inAbornName + "<>" + inAbornName + "<>\n";
}

//システム特権で書き込み
string YDATWritter::makeSystem(const string & inName ,const string & inMail ,const string & inText)
{
	//システム特権で書き込む場合.
	return inName + "<>" + inMail + "<>" + RDateConv::UnixTime2chTime(time(NULL)) + "<>" + inText + "<>\n";
}

// >> へのレスをリンクにする.
const string YDATWritter::AppendTagHtml(const string & inBaee , const string & inTarget)
{
	if (inBaee.empty() ) return inTarget;

	string ret;
	ret.reserve( inTarget.size() );	//先読み.

	int full = inTarget.size();
	int oldpos = 0;
	int pos = 0;

	//>> を探します。
	while( (pos = inTarget.find( "&gt;&gt;" , oldpos)) != -1 )
	{
		ret += inTarget.substr(oldpos,pos - oldpos);

		pos += sizeof("&gt;&gt;") - 1;
		oldpos = pos;
		for( ; isrespons(inTarget[pos]) ; ++pos)	;

		string res = inTarget.substr(oldpos,pos-oldpos );
		if ( res.empty() )
		{	//>> しかなかった場合.
			ret += "&gt;&gt;";
		}
		else
		{
			ret += "<a href=\"";
			ret += inBaee + res;
			ret += "\" target=\"_blank\">&gt;&gt;";
			ret += res;
			ret += "</a>";
		}
		oldpos = pos;
	}
	ret += inTarget.substr(oldpos , full - oldpos);
	return ret;
}


void YDATWritter::test()
{
	string r;
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","&gt;&gt;10-100") ) == "<a href=\"a/10-100\" target=\"_blank\">&gt;&gt;10-100</a>");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","&gt;&gt;") ) == "&gt;&gt;");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","吉野家へ&gt;&gt;10-100むひ") ) == "吉野家へ<a href=\"a/10-100\" target=\"_blank\">&gt;&gt;10-100</a>むひ");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","吉野家へ&gt;&gt;10-あ100むひ") ) == "吉野家へ<a href=\"a/10-\" target=\"_blank\">&gt;&gt;10-</a>あ100むひ");
	ASSERT( ( r = YDATWritter::AppendTagHtml("a/","吉野家へ&gt;&gt;10->>100むひ") ) == "吉野家へ<a href=\"a/10-\" target=\"_blank\">&gt;&gt;10-</a>>>100むひ");
}
