// RHTTPHeader.cpp: RHTTPHeader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RHTTPHeader.h"
#include "RDateConv.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RHTTPHeader::RHTTPHeader()
{
	
}

RHTTPHeader::~RHTTPHeader()
{

}

/**
 * ReadBuffer:				バッファから読み込みヘッダとして解析します。
 *
 * @param ioHeaderBuffer	解析するバッファ(ヘッダは破壊されるので注意!)
 * @param inHeaderSize		バッファサイズ
 * @return void 
 */
void RHTTPHeader::ReadBuffer(char * ioHeaderBuffer , int inHeaderSize)
{
	bool first = true;
	int lineStart = 0;
	for(int i = 0; i < inHeaderSize ; i ++)
	{
		if ( ioHeaderBuffer[i] == '\r' && ioHeaderBuffer[i+1] == '\n')
		{
			//念のためチェックしておく.
			if (lineStart < 0 || (i-lineStart) < 0) return ;

			//改行を埋め込む.
			ioHeaderBuffer[i] = '\0';
			//解析開始.
			if (first)	FirstScan(ioHeaderBuffer + lineStart);
			else		CommandScan(ioHeaderBuffer + lineStart);

			first = false;
			i++;
			lineStart = i + 1;
		}
		else if ( ioHeaderBuffer[i] == '\r' || ioHeaderBuffer[i] == '\n')
		{
			//念のためチェックしておく.
			if (lineStart < 0 || (i-lineStart) < 0) return ;

			//改行を埋め込む.
			ioHeaderBuffer[i] = '\0';
			//解析開始.
			if (first)	FirstScan(ioHeaderBuffer + lineStart);
			else		CommandScan(ioHeaderBuffer + lineStart);

			first = false;
			lineStart = i + 1;
		}
	}
	//念のためチェックしておく.
	if (lineStart < 0 || (i-lineStart) < 0) return ;

	ioHeaderBuffer[i] = '\0';
	if (first)	FirstScan(ioHeaderBuffer + lineStart);
	else		CommandScan(ioHeaderBuffer + lineStart);
}

/**
 * FirstScan:				最初の一行をスキャン
 *
 * @param isFirst			一番最初のヘッダ?
 * @param inLine			一行
 * @return void 
 */
void RHTTPHeader::FirstScan(const char* inLine)
{
	const char *p = inLine;
	const char *s = inLine ;

	//先頭から空白までがメソッド
	for( ; *p != '\0' && isgraph(*p)  ; p++)	;
	if (*p == '\0') return ;
	Method = string(s , 0 , p - s);

	//そこから認識できる文字まで読み飛ばし.
	for( ; *p != '\0' && !isgraph(*p)  ; p++)	;
	if (*p == '\0') return ;

	//空白までがパス.
	s = p;
	for( ; *p != '\0' && isgraph(*p)  ; p++)	;
	if (*p == '\0') return ;
	Path = string(s , 0 , p - s);
}

/**
 * CommandScan:				最初の一行以外をスキャン
 *
 * @param inLine			一行
 * @return void 
 */

#define HTTP_CONTENT_LENGTH		"Content-Length:"
#define HTTP_RANGE				"Range:"
#define HTTP_IF_MODIFIED_SINCE	"If-Modified-Since:"

void RHTTPHeader::CommandScan(const char* inLine)
{
	//なんか: 値 形式.
	switch(inLine[0])
	{
	case 'C':
		if ( (strncmp(inLine , HTTP_CONTENT_LENGTH , sizeof(HTTP_CONTENT_LENGTH)-1)==0) &&
			ContentLength.Decode(inLine + sizeof(HTTP_CONTENT_LENGTH)-1 ) )	return ;
		break;
	case 'R':
		if ( (strncmp(inLine , HTTP_RANGE , sizeof(HTTP_RANGE)-1)==0) &&
			Range.Decode(inLine + sizeof(HTTP_RANGE)-1 ) )	return ;
		break;
	case 'I':
		if ( (strncmp(inLine , HTTP_IF_MODIFIED_SINCE , sizeof(HTTP_IF_MODIFIED_SINCE)-1)==0) &&
			IfModifiedSince.Decode(inLine + sizeof(HTTP_IF_MODIFIED_SINCE)-1 ) )	return ;
		break;
	}
}

void RHTTPHeader::test()
{
	{
		RHTTPHeader	rfh;
		char head[] = "GET /weekly/dat/1039930506.dat HTTP/1.0\r\nHost: live3.2ch.net\r\nRange: bytes=104547-\r\nPragma: no-cache\r\nIf-Modified-Since: Sun, 15 Dec 2002 08:44:45 GMT\r\nIf-None-Match: W/\"36f0f9-19864-3dfc40fd\"\r\nUser-Agent: Monazilla/1.00 kage/0.99.1.1017 (1000)\r\nConnection: close\r\nPragma: no-cache\r\n\r\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "GET" );
		ASSERT( rfh.getPath() == "/weekly/dat/1039930506.dat" );
		ASSERT( rfh.Range.getValue().getStart() == 104547 );
		ASSERT( rfh.Range.getValue().getEnd() == INT_MAX );
		ASSERT( rfh.IfModifiedSince.getValue().getDate() == RDateConv::HttpTimeToUnixTime("Sun, 15 Dec 2002 08:44:45 GMT") );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "POST / HTTP/1.0\nContent-Length: 123456\nRange: bytes=-117815\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "POST" );
		ASSERT( rfh.getPath() == "/" );
		ASSERT( rfh.Range.getValue().getStart() == 0 );
		ASSERT( rfh.Range.getValue().getEnd() == 117815 );
		ASSERT( rfh.ContentLength.getValue() == 123456 );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "POST / HTTP/1.0\nContent-Length: 123456\nRange: bytes=100-117815\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "POST" );
		ASSERT( rfh.getPath() == "/" );
		ASSERT( rfh.Range.getValue().getStart() == 100 );
		ASSERT( rfh.Range.getValue().getEnd() == 117815 );
		ASSERT( rfh.ContentLength.getValue() == 123456 );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "POST /test/bbs.cgi HTTP/1.0\r\nReferer: http://192.168.1.180/test/read.cgi/rti/1039732201/\r\nHost: 192.168.1.180\r\nUser-Agent: Monazilla/1.00 Live2ch/1.00beta64\r\nAccept: text/plain\r\nAccept-charset: shift_jis\r\nAccept-Encoding:\r\nContent-Length: 99\r\nCookie: NAME=aaa; Mail=aa; SPID=\r\n\r\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "POST" );
		ASSERT( rfh.getPath() == "/test/bbs.cgi" );
		ASSERT( rfh.ContentLength.getValue() == 99 );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "GET / HTTP/1.0\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "GET" );
		ASSERT( rfh.getPath() == "/" );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "GET /index.html HTTP/1.0\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "GET" );
		ASSERT( rfh.getPath() == "/index.html" );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "" );
		ASSERT( rfh.getPath() == "" );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "" );
		ASSERT( rfh.getPath() == "" );
	}
	{
		RHTTPHeader	rfh;
		char head[] = " ";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "" );
		ASSERT( rfh.getPath() == "" );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "GET/index.htmlHTTP1.0\nGRT\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "" );
		ASSERT( rfh.getPath() == "" );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "GET  HTTP/1.0\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "GET" );
		ASSERT( rfh.getPath() == "" );
	}
	{
		RHTTPHeader	rfh;
		char head[] = "POST / HTTP/1.0\nContent-Length: 123456\n";
		rfh.ReadBuffer( head , strlen(head) );
		ASSERT( rfh.getMethod() == "POST" );
		ASSERT( rfh.getPath() == "/" );
		ASSERT( rfh.ContentLength.getValue() == 123456 );
	}
}


