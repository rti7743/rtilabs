// YMain.cpp: YMain クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YMain.h"
#include "RStreamUtil.h"
#include "RStreamSocket.h"
#include "RHTTPHeader.h"
#include "YRequestPath.h"
#include "RHTTPPOSTParser.h"
#include "RNoComplateException.h"
#include "YThreadWithTls.h"
#include "RSocketForWinSock2.h"
#include "RSocketAsyncTransmitFile.h"
#include "YTransmitFile.h"
#include "YYoujyoLogger.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YMain::YMain()
{
	try
	{
		Mmap.Create(THREAD_WORK_AREA);
	}
	catch(RException e)
	{
		printf("メモリマップ確保に失敗 早期に終了させてください: %s\n",e.getMessage() );
	}
}

YMain::~YMain()
{
}

//これが実行されるのですよー
unsigned int YMain::Run()
{
	while( Execute() )
		;

	return 0;
}

//停止を命じる
//処理はすぐに帰ってきます。
//停止フラグを立てます。
void	YMain::HoldUp()
{
	YConnectNotify::getInstance()->PostShutdownNotify();
	RThread::HoldUp();
}



//メイン部分
bool YMain::Execute()
{
	YConnectPack* yConnectPack;
	YConnectNotify::getInstance()->WaitAcceptNotify(&yConnectPack );

//	printf("start %x\n" ,this);

	//終了命令だったらあぼーん.
	if (yConnectPack == NULL)
	{
		return false;
	}

	RSocketForWinSock2 socket(yConnectPack->AcceptSocket);
//B
	try
	{
		char* buffer = (char*) Mmap.getMemory();

		//改行コードが連続しているところを発見するクラス.
		StreamCheckerForSecutiveReturnCode secutiveReturnCode;
		//ストリームから読み込むユーティリティ
		RStreamUtil	rsu;
		//ソケットをストリームとして扱うためのクラス.
		RStreamSocket rss(&socket);
		//ストリームに mount します。
		rsu.Mount(&rss);

		//連続する改行コードまで読み込み.
		int readByte = rsu.ReadBuffer(buffer , THREAD_WORK_AREA , &secutiveReturnCode);

//C

		//ヘッダと本文を分ける区切りの取得.
		const int separater = secutiveReturnCode.getSeparater();

		//リクエスト解析.
		RHTTPHeader HeaderParser;
		HeaderParser.ReadBuffer(buffer , separater);
		HeaderParser.setRemoteHost(yConnectPack->RemoteIP);



//D

		int conectLength = 0;
		//長さ指定があれば、その長さ文読み込む.
		if ( HeaderParser.ContentLength.IsEnable() )
		{
			/*ChangeLog.txt:2003/01/25	ヘッダの長さを加えていなかったので修正 >70 ｻﾝｸｽｺ
			*/
			conectLength = HeaderParser.ContentLength.getValue();
			const int conectStart = secutiveReturnCode.getConectStart();
			//実は既に読み込んでいないかチェック.
			if (readByte < conectLength+conectStart)
			{
				StreamCheckerForLength scfl( conectLength+conectStart-readByte );
				readByte += rsu.ReadBuffer(buffer+readByte , THREAD_WORK_AREA - readByte ,&scfl);
			}
		}
//E
		try
		{
			YRequestPath	RequestPath;
			if ( ! RequestPath.PathParse( HeaderParser.getPath() ) )
			{//2ch形式以外のアクセス、 通常のファイル転送とみなす.
				YTransmitFile	tf;
				tf.TransmitFile(buffer ,&HeaderParser,&socket);
			}
			else
			{
				//2ch形式のアクセス.

				if (HeaderParser.getMethod() == "GET")
				{//GET メソッド
					YBBSManager::getInstance()->Read(buffer , &HeaderParser , &RequestPath, &rsu);
				}
				else if (HeaderParser.getMethod() == "POST")
				{//POST メソッド
					//POSTされた内容のチェック.
					RHTTPPOSTParser rhpp;
					//コンテンツ開始位置
					const int conectStart = secutiveReturnCode.getConectStart();
					//コンテンツの長さ(実際に読み込んだ量 - コンテンツ開始位置)
					int tureContentSize = readByte-conectStart;
					//Conetent-Length が指定されていれば それにする.
					if (conectLength < tureContentSize)	tureContentSize = conectLength;

					//コンテンツパース
					rhpp.Parse(buffer+conectStart , tureContentSize );

//					rhpp.Dump();

					//submit=%8F%91%82%AB%8D%9E%82%DE&FROM=&mail=sage&MESSAGE=%8D%A1%93%FA%82%CD%8E%9E%8A%D4%82%B8%82%EA%82%E9%82%CC%82%A9&bbs=choanitoku&key=1038415253&time=1039048481
					if (RequestPath.getRequestType() != REQUESTTYPE_BBSCGI)
					{//エラー
						throw RNoComplateException("不正な CGI に対して POSTが実行されました");
					}
					YBBSManager::getInstance()->Write(buffer ,&HeaderParser, rhpp.PostData , &rsu);
				}
				else
				{//それ以外
					rsu.WriteBuffer("HTTP/1.1 501 Not Implemented\n\n");
				}
			}
		}
		catch(RException e)
		{
			rsu.WriteBuffer("HTTP/1.1 403 Forbidden\n\n");
			throw ;
		}
		socket.Close();
		//非同期領域をあぼーん
		delete yConnectPack;

//		string* logString = new string(RDateConv::UnixTime2chTime(time(NULL))  + " " +  HeaderParser.RemoteHost.getIP() + " " + HeaderParser.getMethod() + " " + HeaderParser.getPath() + "\r\n");
//		YLogNotify::getInstance()->Post( logString  );
	}
	catch(RException e)
	{
		socket.Close();
		//非同期領域をあぼーん
		delete yConnectPack;
//		printf("零回 %s\n" , e.getMessage() );
	}
//	::Sleep(1000);
//	printf("end %x\n" ,this);
	return true;
}

