// YDATReader.cpp: YDATReader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "YDATReader.h"
#include "RConv.h"

//URLに使える文字.
#define isurl(x) (isalnum(x) || x=='.' || x==':' ||  x=='@' || x == '/' || x == '-' || x == '_' || x == '#')

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YDATReader::YDATReader()
{

}

YDATReader::~YDATReader()
{

}

const char* YDATReader::makeHTML(RStringMap* outSmap ,int inNo ,const char* inData )
{
	const char * name  = inData;
	const char * mail  = next(name);
	const char * date  = next(mail);
	const char * text  = next(date);
	const char * title = next(text);

	if (name==NULL || mail==NULL || date==NULL || text==NULL || title==NULL)
	{//こわれてます。
		(*outSmap)["YKakikoNo"] = num2str(inNo) ;
		(*outSmap)["YKakikoName"] = "★YoujyoMaster" ;
		(*outSmap)["YKakikoTime"] = " ID: YoujyoMaster" ;
		(*outSmap)["YkakikoText"] = "ここ壊れてます。。。" ;

		return NULL;
	}
	else
	{//通常
		if (date - mail- 2 == 0)
		{//age
			(*outSmap)["YKakikoNo"] = num2str(inNo) ;
			(*outSmap)["YKakikoName"] = string(name , mail - name - 2);
			(*outSmap)["YKakikoTime"] = string(date , text - date - 2) ;
			(*outSmap)["YkakikoText"] = makeLink(string(text , title - text - 2)) ;
		}
		else
		{//sage
			(*outSmap)["YKakikoNo"] = num2str(inNo) ;
			(*outSmap)["YKakikoName"] = "<A href=\"mailto:" + string(mail , date - mail - 2) + "\">" + string(name , mail - name - 2) + "</A>";
			(*outSmap)["YKakikoTime"] = string(date , text - date - 2) ;
			(*outSmap)["YkakikoText"] = makeLink(string(text , title - text - 2)) ;
		}
	}
	return nextnr(title);
}



/**
 * MaskKakikomi:			書き込みのマスク
 *
 * @param inData			一書き込みの dat
 * @param inMaskSize		マスクの数
 * @param inMask			マスクデータ
 * @return string			マスク処理された dat に書き戻せるデータ
 */
string YDATReader::MaskKakikomi(const char* inData , int inMaskSize   ,const string *inMask)
{
	const char * name  = inData;
	const char * mail  = YDATReader::next(name);
	const char * date  = YDATReader::next(mail);
	const char * text  = YDATReader::next(date);
	const char * title = YDATReader::next(text);

	if (name==NULL || mail==NULL || date==NULL || text==NULL || title==NULL)
	{//こわれてます。
		return "<><><><>\n";
	}
	else
	{//通常
		return 
			YDATReader::MaskOneRecord( string(name , mail - name - 2) , inMaskSize , inMask) + "<>" +
			YDATReader::MaskOneRecord( string(mail , date - mail - 2) , inMaskSize , inMask) + "<>" +
			string(date , text - date - 2) + "<>" +
			YDATReader::MaskOneRecord( string(text , title - text - 2) , inMaskSize , inMask) + "<>\n";
	}
}

/**
 * MaskOneRecord:			一項目ごとのマスク
 *
 * @param inData			一書き込み項目
 * @param inMaskSize		マスクの数
 * @param inMask			マスクデータ
 * @return string			マスク処理されたデータ
 */
string YDATReader::MaskOneRecord(const string& inData , int inMaskSize   ,const string *inMask)
{
	string rea = inData ;
	for(int i = 0; i < inMaskSize ; i++)
	{
		rea = RConv::Replace( rea , inMask[i] , "!(削除依頼により削除)!" );
	}
	return rea;
}


//ザブジェクとのロード
string YDATReader::Subject(const char* inDat,int len)
{
	const char * title = next(next(next(next(inDat))));
	if (title == NULL) return "";

	//終端サーチ.
	const char* end = inDat + len;
	const char* str = nextnr(title , end);

	return  string(title , str - title );
}



//次の <> へ
const char* YDATReader::next(const char* str)
{
	if (str == NULL) return NULL;

	const char*  p = strstr(str , "<>");
	if ( p == NULL) return NULL;

	return p + 2;
}
//次の \n へ
const char* YDATReader::nextnr(const char* str)
{
	if (str == NULL) return NULL;

	const char*  p = strchr(str , '\n');
	if ( p == NULL) return NULL;

	return p + 1;
}
//次の \n へ
const char* YDATReader::nextnr(const char* str,const char* end)
{
	if (str == NULL) return NULL;

	for ( ; *str != '\n' && str < end ; str++)	;

	return str;
}

void YDATReader::test()
{
	{
		string http;
		ASSERT( (http = makeLink("unkonageman"))  == "unkonageman");
		ASSERT( (http = makeLink("http://www.yahoo.co.jp/"))  == "<a href=\"http://www.yahoo.co.jp/\">http://www.yahoo.co.jp/</a>");
		ASSERT( (http = makeLink("吉野家http://www.yahoo.co.jp/吉野家"))  == "吉野家<a href=\"http://www.yahoo.co.jp/\">http://www.yahoo.co.jp/</a>吉野家");
		ASSERT( (http = makeLink("http://www.catnet.ne.jp/kouno/c_faq/c1.html#32"))  == "<a href=\"http://www.catnet.ne.jp/kouno/c_faq/c1.html#32\">http://www.catnet.ne.jp/kouno/c_faq/c1.html#32</a>");
		
	}
	{
		string ret;

		RStringMap smap;
		makeHTML(&smap,1 , "<><>2003/1/3 14:19<>&lt;<br>&gt;<br>&lt;&gt;<br>&lt;&gt;<>\n");
		ASSERT((ret = smap["YKakikoNo"]) == "1");
		ASSERT((ret = smap["YKakikoName"]) == "");
		ASSERT((ret = smap["YKakikoTime"]) == "2003/1/3 14:19");
		ASSERT((ret = smap["YkakikoText"]) == "&lt;<br>&gt;<br>&lt;&gt;<br>&lt;&gt;");
	}
	{
		string ret;

		RStringMap smap;
		makeHTML(&smap,1 , "nazo<><>2002/12/1 1:39<>www.nazo.to<>\n");
		ASSERT((ret = smap["YKakikoNo"]) == "1");
		ASSERT((ret = smap["YKakikoName"]) == "nazo");
		ASSERT((ret = smap["YKakikoTime"]) == "2002/12/1 1:39");
		ASSERT((ret = smap["YkakikoText"]) == "www.nazo.to");
	}
	{
		string ret;

		RStringMap smap;
		makeHTML(&smap,1 , "nazo<>sage<>2002/12/1 1:39<>www.nazo.to<>\n");
		ASSERT((ret = smap["YKakikoNo"]) == "1");
		ASSERT((ret = smap["YKakikoName"]) == "<A href=\"mailto:sage\">nazo</A>");
		ASSERT((ret = smap["YKakikoTime"]) == "2002/12/1 1:39");
		ASSERT((ret = smap["YkakikoText"]) == "www.nazo.to");
	}
	//書き込みのマスク
	{
		string ret;
		string mask[3];
		mask[0] = "www";
		mask[1] = "nazo";
		mask[2] = "to";
		ret = MaskKakikomi("nazo<>sage<>2002/12/1 1:39<>www.nazo.to<>\n" , 3 , mask);
		ASSERT( ret == "!(削除依頼により削除)!<>sage<>2002/12/1 1:39<>!(削除依頼により削除)!.!(削除依頼により削除)!.!(削除依頼により削除)!<>\n" );
	}
}


// htmlらしいものがあった場合リンクとします
string YDATReader::makeLink(const string &inTarget)
{
	string ret;
	ret.reserve( inTarget.size() );	//先読み.

	int full = inTarget.size();
	int oldpos = 0;
	int pos = 0;

	//http://を探します。
	while( (pos = inTarget.find( "http://" , oldpos)) != -1 )
	{
		ret += inTarget.substr(oldpos,pos - oldpos);

		oldpos = pos;
		pos += sizeof("http://");
		for( ; isurl(inTarget[pos]) ; ++pos)	;

		string http = inTarget.substr(oldpos,pos-oldpos);
		ret += "<a href=\"";
		ret += http;
		ret += "\">";
		ret += http;
		ret += "</a>";
		oldpos = pos;
	}
	ret += inTarget.substr(oldpos , full - oldpos);
	return ret;
}


