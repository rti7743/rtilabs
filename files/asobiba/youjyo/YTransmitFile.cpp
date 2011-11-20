// YTransmitFile.cpp: YTransmitFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YTransmitFile.h"
#include <sys/stat.h>
#include <sys/utime.h>
#include "RDateConv.h"
#include "RSocketAsyncTransmitFile.h"
#include "YHttpdConf.h"
#include "RMimeType.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YTransmitFile::YTransmitFile()
{

}

YTransmitFile::~YTransmitFile()
{

}

//ファイル名より拡張子を分離
string YTransmitFile::Filename2Ext(const string inFilename) const
{
	const char* str = inFilename.c_str();
	for(int i = inFilename.length() - 1; str[i] != '.' && i >= 0 ; i--)
		;
	if (i < 0 )	return "";

	//拡張子を子文字に
	string ext = (char*)(str + i + 1);
	for(register int l = 0 ; ext[l] != '\0' ;l++) ext[l] = tolower(ext[l]);

	return ext;	// +1 は '.' の分です。
}

void YTransmitFile::TransmitFile(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,RSocketForWinSock2* ioSocket)
{
	
	RSocketAsyncTransmitFile	transmitFile;
	transmitFile.Mount(ioSocket);

	string requestFilename = inRHTTPHeader->getPath();
	//危険なパスの排除.
	if ( strstr( requestFilename.c_str() , ".." ) != NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "リクエストファイル名に危険な文字を含んでいるのでキャンセルされました");
	}
	//index.html の保管.
	if ( requestFilename[requestFilename.size() - 1] == '/')  requestFilename += "index.html";

	//ファイル名を完璧にする.  
	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + requestFilename;

	//最終更新時刻を求める.
	struct _stat statbuf;
	if ( _stat( filename.c_str() ,  &statbuf) == -1)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "stat の取得に失敗" + filename);
	}

	const time_t lastModified = statbuf.st_mtime;
	const int filesize = statbuf.st_size;

	//If ModifiedSince の場合、更新されていないかチェック.
	if ( inRHTTPHeader->IfModifiedSince.getValue().IsEnabled() &&
		 inRHTTPHeader->IfModifiedSince.getValue().getDate() >= lastModified	)
	{
		int len = sprintf( outShareBuffer ,	
									"HTTP/1.0 304 Not Modified\r\n"
									"Date: %s\r\n"
									"Server: Youjyo\r\n"
									"Connection: close\r\n"
									"Content-Length: 0\r\n"
									"\r\n"
								,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								);


		TRANSMIT_FILE_BUFFERS tfb = {0};
		tfb.Head = outShareBuffer;
		tfb.HeadLength = len;

		transmitFile.TransmitWithout( &tfb );
		transmitFile.WaitForComplate();
		return ;
	}
	//mime の取得
	const string mime = RMimeType::getInstance()->getMimeType( Filename2Ext( requestFilename ) );
	//Rangeが有効だったら差分取得をします。
	if ( inRHTTPHeader->Range.getValue().IsEnabled() )
	{
		/*ChangeLog.txt:2003/01/23	range を行ったときの Content-Range がおかしかったので修正.
			http://pc3.2ch.net/test/read.cgi/tech/1043275721/22
			Range指定でGETする時に返されるContent-Rangeがおかしいﾖｶﾝ

			本来なら
			Content-Length: 400001
			Content-Range: bytes 0-400000/400001　となるところが

			Content-Length: 400001
			Content-Range: bytes 0-400001/400002　になってます			
		*/
		int rangeStart = inRHTTPHeader->Range.getValue().getStart();
		int rangeEnd = inRHTTPHeader->Range.getValue().getEnd();

		if (rangeEnd >= rangeStart && rangeEnd >= 0 && rangeStart >= 0)
		{
			//ファイルよりでかい転送は認めない.
			if (rangeEnd >= filesize) rangeEnd = filesize - 1;
			//実転送バイト
			int lenth = (rangeEnd+1) - rangeStart;

			int len = 
					sprintf( outShareBuffer ,	
								"HTTP/1.1 206 Partial Content\r\n"
								"Date: %s\r\n"
								"Server: Youjyo\r\n"
								"Last-Modified: %s\r\n"
								"Accept-Ranges: bytes\r\n"
								"Content-Length: %d\r\n"
								"Connection: close\r\n"
								"Content-Type: %s\r\n"
								"Content-Range: bytes %d-%d/%d\r\n"
								"\r\n"
							,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
							,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
							,	lenth
							,	mime.c_str()
							,	rangeStart , rangeEnd , filesize
							);
			TRANSMIT_FILE_BUFFERS tfb = {0};
			tfb.Head = outShareBuffer;
			tfb.HeadLength = len;

			transmitFile.Transmit(filename.c_str() ,rangeStart , rangeStart+lenth, &tfb );
			transmitFile.WaitForComplate();
			return ;
		}
	}
	//通常転送.
	{
		int len = sprintf( outShareBuffer ,	
									"HTTP/1.1 200 OK\r\n"
									"Date: %s\r\n"
									"Server: Youjyo\r\n"
									"Last-Modified: %s\r\n"
									"Accept-Ranges: bytes\r\n"
									"Content-Length: %d\r\n"
									"Connection: close\r\n"
									"Content-Type: %s\r\n"
									"\r\n"
								,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
								,	filesize
								,	mime.c_str()
								);

		TRANSMIT_FILE_BUFFERS tfb = {0};
		tfb.Head = outShareBuffer;
		tfb.HeadLength = len;

		transmitFile.Transmit(filename.c_str() ,0, filesize , &tfb );
		transmitFile.WaitForComplate();
	}
}
