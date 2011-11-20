// RConv.cpp: RConv クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RConv.h"

#define iskanji1st(c) ( ( ((unsigned char)c) >= 0x81 && ((unsigned char)c) >= 0x9f) ||\
								 ( ((unsigned char)c) >= 0xe0 && ((unsigned char)c) >= 0xff) )
#define iskanji2nd(c) ( ( ((unsigned char)c) >= 0x40 && ((unsigned char)c) <= 0x7e) ||\
								 ( ((unsigned char)c) >= 0x80 && ((unsigned char)c) <= 0xfc)  )

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//URLを文字列に変換 
string RConv::UrlEncode(string inUrl)
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

//置換.
string RConv::Replace(const string inTarget ,const string inOld ,const string inNew)
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


//左空白削除.
string RConv::ltrim(const string inStr)
{
	int size = inStr.size();
	for(int i = 0 ; i < size ; i++)
	{
		if ( !TRIM_SPACE(inStr[i]) )
		{
			return inStr.substr(i , size - i + 1);
		}
	}
	return inStr;
}


//右空白削除.
string RConv::rtrim(const string inStr)
{
	int size = inStr.size();
	for(int i = size-1 ; i >= 0 ; i--)
	{
		if ( !TRIM_SPACE(inStr[i]) )
		{
			return inStr.substr(0 , i + 1);
		}
	}
	return inStr;
}

//数字を文字列に変換.
string RConv::itoa(int inInt)
{
	char buf[20];
	return ::itoa(inInt , buf , 10 );
}

//タグをエスケープ 基本的に PHP の htmlspecialchars と同じ.
//http://search.net-newbie.com/php/function.htmlspecialchars.html
string RConv::htmlspecialchars(const string inStr)
{
//	return RConv::SJISDangerESC( RConv::Replace(RConv::Replace(RConv::Replace(RConv::Replace(inStr , "&" , "&amp;") , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;") );
	return RConv::SJISDangerESC( RConv::Replace(RConv::Replace(RConv::Replace(inStr , ">" , "&gt;") , "<" , "&lt;") , "\"", "&quot;") );
}


//\nを<br>に 基本的に PHP の nl2br と同じ.
string RConv::nl2br(const string inStr)
{
	return RConv::Replace(inStr , "\r\n" , "<br>");
}

//SJIS の1バイト目だけを残すような制御文字アタックを空白を追加
string RConv::SJISDangerESC(const string inStr)
{
	const int len = inStr.size();
	if (len >= 2 )
	{
		char p2 = inStr[ len - 1] ;
		char p3 = inStr[ len - 2] ;
		if ( iskanji1st( inStr[ len - 1]  ) )
		{	//最後が 1バイト目で不信...

			//完全に調べます。
			for(int i = 0 ; i < len ; i ++)
			{
				if ( iskanji1st( inStr[ i ]  ) )
				{
					i++;
					//どうやら、攻撃のようです。 とりあえず、最後に空白を
					if (len >= i)
					{
						return inStr + "  ";
					}
				}
			}
		}
		return inStr;
	}

	//一文字しかなくて、最後が 1バイト目の制御文字だったら、没.
	if (len == 1 && iskanji1st( inStr[ 0 ] ) )	return inStr + "  ";
	else										return inStr;
}


void RConv::test()
{
	ASSERT( RConv::UrlEncode("%20") == " ");

	ASSERT( RConv::Replace("ABABA" , "A" , "C") == "CBCBC");
	ASSERT( RConv::Replace("うひひ" , "ひ" , "とろ") == "うとろとろ");
	ASSERT( RConv::Replace("ABCDEFGH" , "AB" , "ABCDEFGH") == "ABCDEFGHCDEFGH");
	ASSERT( RConv::Replace("ABCDEFGH" , "DEF" , "XYZ") == "ABCXYZGH");
	ASSERT( RConv::Replace("ABCDEFGH" , "DEF" , "XYZ2") == "ABCXYZ2GH");

	ASSERT( RConv::rtrim("ABCDEFGH") == "ABCDEFGH");
	ASSERT( RConv::rtrim("ABCDEFGH   ") == "ABCDEFGH");
	ASSERT( RConv::rtrim("     ABCDEFGH   ") == "     ABCDEFGH");
	ASSERT( RConv::rtrim("     ABCD EFGH   ") == "     ABCD EFGH");

	ASSERT( RConv::ltrim("ABCDEFGH") == "ABCDEFGH");
	ASSERT( RConv::ltrim("   ABCDEFGH") == "ABCDEFGH");
	ASSERT( RConv::ltrim("     ABCDEFGH ") == "ABCDEFGH ");
	ASSERT( RConv::ltrim("     ABCD EFGH ") == "ABCD EFGH ");

	ASSERT( RConv::htmlspecialchars("     ABCD EFGH ") == "     ABCD EFGH ");
//	ASSERT( RConv::htmlspecialchars("<>\"&") == "&lt;&gt;&quot;&amp;");
//	ASSERT( RConv::htmlspecialchars("<br>\"&") == "&lt;br&gt;&quot;&amp;");
	ASSERT( RConv::htmlspecialchars("<>\"") == "&lt;&gt;&quot;");
	ASSERT( RConv::htmlspecialchars("<br>\"") == "&lt;br&gt;&quot;");

}

