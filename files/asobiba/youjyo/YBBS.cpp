// YBBS.cpp: YBBS クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YBBS.h"
#include "RHTTPResponse.h"
#include "RPackSuper.h"
#include "YThreadWithTls.h"
#include "RConv.h"
#include "YBBSManager.h"
#include "YFilter.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YBBS::YBBS(string inName)
{
	this->BoardName = inName;
	//板の設定の読み込み
	SettingTXT.Load(inName);
	//DATキャッシュ
	Cache.setBoardName(inName);
	//subject.txt
	Subject.Load(inName);
	//ローカルルールの読み込み.
	LocalRule.Load(inName);
	//index.html
	IndexHTML.Setting( inName , &SettingTXT );
	IndexHTML.Load( &Cache , &Subject , &LocalRule);
	//read.cgi
	ReadCGI.Load(inName , &SettingTXT);
	//subback.html
	SubBackHTML.Load(inName , &SettingTXT);
	SubBackHTML.Write(&Subject );
	//新規すれ
	BuildThread.Load(inName , &SettingTXT);
	//書き込み関係.
	WriteHTML.Load(inName , &SettingTXT);

	DeleteWait.Load(inName);

}

YBBS::~YBBS()
{
	Subject.Save(this->BoardName);
	DeleteWait.Save(this->BoardName);
}
//REQUESTTYPE_DAT
void YBBS::RequestTypeDat(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,  const YRequestPath* inRequestPath, RStreamUtil * outBuffer)	throw(RIOCannotOpenException)
{
	//G
	YDAT* dat = Cache.getDAT(inRequestPath->getDatNumber() );
	time_t	lastModified = dat->getLastModified();

	//If ModifiedSince の場合、更新されていないかチェック.
	if ( inRHTTPHeader->IfModifiedSince.getValue().IsEnabled() &&
		 inRHTTPHeader->IfModifiedSince.getValue().getDate() >= lastModified	)
	{
		//キャッシュにもう使用しないことを伝えます
		Cache.Release(dat);

		int len = 
			sprintf( outShareBuffer ,	"HTTP/1.1 304 Not Modified\r\n"
										"Date: %s\r\n"
										"Server: Youjyo\r\n"
										"Connection: close\r\n"
										"Content-Length: 0\r\n"
										"\r\n"
									,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								);
		
		//ヘッダの転送
		outBuffer->WriteBuffer( outShareBuffer , len ); 
		return ;
	}

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
		int filesize = dat->getDatSize();

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
								"Content-Type: text/plain\r\n"
								"Content-Range: bytes %d-%d/%d\r\n"
								"\r\n"
							,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
							,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
							,	lenth
							,	rangeStart , rangeEnd , filesize
							);
			//ヘッダの転送
			outBuffer->WriteBuffer( outShareBuffer , len ); 

			RPackSuper pack;
			pack.Mount(outShareBuffer , TLS_POOL_MAX);
			dat->ReadByte( &pack , rangeStart , rangeStart + lenth);
			//キャッシュにもう使用しないことを伝えます
			Cache.Release(dat);

			//本体の転送.
			outBuffer->WriteBuffer((const char*) pack.getMemory() , pack.getSize() ); 
			return ;
		}
	}
	//通常転送.
	{
		int len = 
			sprintf( outShareBuffer ,	"HTTP/1.1 200 OK\r\n"
										"Date: %s\r\n"
										"Server: Youjyo\r\n"
										"Last-Modified: %s\r\n"
										"Accept-Ranges: bytes\r\n"
										"Content-Length: %d\r\n"
										"Connection: close\r\n"
										"Content-Type: text/plain\r\n"
										"\r\n"
									,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
									,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
									,	dat->getDatSize()
									);
		//ヘッダの転送
		outBuffer->WriteBuffer( outShareBuffer , len ); 

		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);
		dat->ReadByteGoEND( &pack , 0);

		//キャッシュにもう使用しないことを伝えます
		Cache.Release(dat);

		//本体の転送.
		outBuffer->WriteBuffer((const char*) pack.getMemory() , pack.getSize() ); 
	}
}


//ファイル転送.
void YBBS::TextFileTransmit(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,YTextFile* inUseFile,  RStreamUtil * outBuffer)
{
	RPackSuper pack;
	pack.Mount(outShareBuffer , TLS_POOL_MAX);

	time_t	lastModified = inUseFile->getLastModified();

	//If ModifiedSince の場合、更新されていないかチェック.
	if ( inRHTTPHeader->IfModifiedSince.getValue().IsEnabled() &&
		 inRHTTPHeader->IfModifiedSince.getValue().getDate() >= lastModified	)
	{
		int len = 
			sprintf( outShareBuffer ,	"HTTP/1.1 304 Not Modified\r\n"
										"Date: %s\r\n"
										"Server: Youjyo\r\n"
										"Connection: close\r\n"
										"Content-Length: 0\r\n"
										"\r\n"
									,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								);
		//ヘッダの転送
		outBuffer->WriteBuffer( outShareBuffer , len ); 
		return ;
	}

	inUseFile->Show(&pack);

	string ret;
	ret.reserve(1024);
	ret += RHTTPResponse::OK();
	ret += RHTTPResponse::ServerName();
	ret += "Last-Modified: " + RDateConv::UnixTimeToHttpTime( lastModified ) + "\r\n";
	ret += "Accept-Ranges: bytes\r\n";
	ret += "Content-Length: "+ num2str( pack.getSize() ) + "\r\n";
	ret += "Connection: close\r\n";
	ret += "Content-Type: text/html\r\n";
	ret += RHTTPResponse::ResponseEnd();
	outBuffer->WriteBuffer( ret.c_str() , ret.size() ); 

	outBuffer->WriteBuffer( (const char*)pack.getMemory() , pack.getSize() ); 
}






//read.cgi コンパチな表示を行います。 DATの解放はこの中で行います(高速化のため).
void YBBS::RequestTypeCGI(char* outShareBuffer,YDAT * inDat , int inStart, int inEnd , bool is1san , RStreamUtil * outBuffer)	throw(RIOCannotOpenException)
{
	const string content = ReadCGI.make(inDat ,  inStart ,inEnd  , is1san );

	//キャッシュにもう使用しないことを伝えます
	Cache.Release(inDat);

	//書き込み成功を返す
	this->TranceString(outShareBuffer, content ,outBuffer);
}




void YBBS::Read(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , const YRequestPath* inRequestPath, RStreamUtil * outBuffer )		throw(RIOCannotOpenException,ROutOfRangeException)
{
	//リクエストに従い処理をするデス.
	//virtual 使ったほうがオブジェクト思考的にはいいかもしれませんが、速度出ません。
	//幹線の部分はこーやるのが一番のよーなきがします。
	//enum だし、 jump table をコンパイラが作ってくれることを期待しています。
	switch( inRequestPath->getRequestType() )
	{
	case REQUESTTYPE_DAT:
		RequestTypeDat(outShareBuffer , inRHTTPHeader , inRequestPath , outBuffer);
		break;
	case REQUESTTYPE_INDEX:
		TextFileTransmit(outShareBuffer , inRHTTPHeader ,  &IndexHTML , outBuffer);
		break;
	case REQUESTTYPE_SUBJECT:
		TextFileTransmit(outShareBuffer , inRHTTPHeader ,  &Subject , outBuffer);
		break;
	case REQUESTTYPE_SUBBACK:
		TextFileTransmit(outShareBuffer , inRHTTPHeader ,  &SubBackHTML , outBuffer);
		break;
	case REQUESTTYPE_CGIALL:
		//REQUESTTYPE_CGIALL	/test/read.cgi/php/1008344964/
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat ,  0 ,dat->getLastNo() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGILAST:
		//REQUESTTYPE_CGILAST	/test/read.cgi/php/1008344964/l50
		{
			YDAT* dat = Cache.getDAT (inRequestPath->getDatNumber() );

			unsigned int start = dat->getLastNo() ;
			if (start >= inRequestPath->getwParam() )	start -= inRequestPath->getwParam();
			else										start = 0;

			RequestTypeCGI( outShareBuffer , dat ,  start ,dat->getLastNo() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGIONE:
		//REQUESTTYPE_CGIONE	/test/read.cgi/php/1000797355/231
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			const int one = inRequestPath->getwParam();

			RequestTypeCGI( outShareBuffer , dat , one,one , false , outBuffer);
		}
		break;
	case REQUESTTYPE_CGIFORM:
		//REQUESTTYPE_CGIFORM	/test/read.cgi/php/1000797355/201-
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat , inRequestPath->getwParam() , dat->getLastNo() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGIFORMTO:
		//REQUESTTYPE_CGIFORMTO	/test/read.cgi/php/1000797355/201-300
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat ,
				inRequestPath->getwParam() , inRequestPath->getlParam() , true , outBuffer);
		}
		break;
	case REQUESTTYPE_CGITO:
		//REQUESTTYPE_CGITO	/test/read.cgi/php/1000797355/-300
		{
			YDAT* dat = Cache.getDAT( inRequestPath->getDatNumber() );
			RequestTypeCGI( outShareBuffer , dat ,	0 , inRequestPath->getwParam() , true , outBuffer);
		}
		break;
	default:
		throw ROutOfRangeException(EXCEPTIONTRACE + "リクエストは範囲外です");
	}
}

void YBBS::Write(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) 
{
	//削除依頼に対する異議ありはこっちでやれ.
	if (SettingTXT.getIS_SAKUBBS() && !ioPost["igiari"].empty() ) 
	{
		Igiari(outShareBuffer , inRHTTPHeader,ioPost , outBuffer);
		return ;
	}

	try
	{
		//アクセス禁止のチェック.
		AccessCheck(inRHTTPHeader);
		//書き囲み内容のチェック.
		WriteCheck(ioPost);

		DATNUMBER datnumber = atol( ioPost["key"].c_str() );

		//実際に書き込みます。
		const string msg = YDATWritter::make(
			CheckName( inRHTTPHeader , RConv::htmlspecialchars(ioPost["FROM"]) ),
			RConv::htmlspecialchars(ioPost["mail"]) ,
			YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]),
			MakeAppendData(inRHTTPHeader,ioPost)
			);
		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);

		pack.Write(msg.c_str() , msg.size() );
		YDAT* dat = Cache.getDAT(datnumber);

		dat->Append(&pack);

		//どこの位置にあるか検索.
		int oldpos = Subject.WhatPosition( datnumber );

		if (ioPost["mail"] != "sage")
		{
			//subject.txt の更新
			Subject.Age(dat);
			//index.htmlの更新
			IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  true);
			//subject.txt の更新.
			SubBackHTML.Write(&Subject);
		}
		else
		{
			/*ChangeLog.txt:2003/01/26	sageで subject.txt , subback.html を更新していなかったのを修正.
			*/
			//subject.txt の更新
			Subject.Sage(dat);
			/*ChangeLog.txt:2003/01/29	sageで age の処理をしていたのを修正.
			*/
			//index.htmlの更新
			IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  false);
			//subject.txt の更新.
			SubBackHTML.Write(&Subject);
		}

		//キャッシュにもう使用しないことを伝えます
		Cache.Release(dat);

		//5000を超えていたら警告を書き込む
		if (dat->getLastNo() >= MAX_NUMBER)
		{
			POST	post;
			post["bbs"]  = this->BoardName;
			post["FROM"]  = "YoujyoMaster★";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"このスレッドは" + num2str(MAX_NUMBER) + "を超えました。\r\n"
						"もう書けないので、新しいスレッドを立ててくださいです。。。";
			post["key"]  = num2str(datnumber);
			post["submit"] = "書き込む";

			this->SystemWrite(outShareBuffer , post);
		}

		//書き込み成功を返す
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//書き込みエラー.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP() ) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" ,ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

void YBBS::New(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) 
{

	//削除依頼の場合はこっちでやれ.
	if (SettingTXT.getIS_SAKUBBS())
	{
		this->NewDelete(outShareBuffer , inRHTTPHeader,ioPost , outBuffer );
		return ;
	}

	try
	{
		//アクセス禁止のチェック.
		AccessCheck(inRHTTPHeader);
		//書き囲み内容のチェック.
		WriteCheck(ioPost);

		DATNUMBER datnumber =  time(NULL);

		//実際のスレたて
		const string subject = RConv::htmlspecialchars(ioPost["subject"]);
		const string msg = YDATWritter::make(
			CheckName( inRHTTPHeader , RConv::htmlspecialchars(ioPost["FROM"]) ),
			RConv::htmlspecialchars(ioPost["mail"]) ,
			YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]),
			subject ,
			MakeAppendData(inRHTTPHeader,ioPost)
			);

		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);
		pack.Write(msg.c_str() , msg.size() );

		YDAT* dat = Cache.getNewDAT( datnumber );
		dat->Append(&pack);
		dat->setSubject( subject );

		//subject.txt の更新.
		Subject.Age(dat);

		//index.htmlの更新
		IndexHTML.Write( dat , &Subject , &LocalRule , INT_MAX ,  true);
		//subject.txt の更新.
		SubBackHTML.Write(&Subject);

		//キャッシュにもう使用しないことを伝えます
		Cache.Release(dat);

		//書き込み成功を返す
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//エラーを返す.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP()) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" , ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

//削除依頼
void YBBS::NewDelete(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) throw(RException)
{
	try
	{
		string mainMessage ;
		//アクセス禁止のチェック.
		AccessCheck(inRHTTPHeader);
		//書き囲み内容のチェック.
		WriteCheck(ioPost);
		//削除文の作成.
		mainMessage =  makeSakuJyoMessage(ioPost);

		//この時間たったら削除します.
		time_t deleteWait = time(NULL) + SettingTXT.getDELETE_WAIT();
		//削除するスレッド.
		DATNUMBER datNumber = atol( ioPost["threadno"].c_str() );
		//削除依頼すれのタイトル.
		string subject ;

		//削除対象のスレッドの タイトル を取得し、これを削除依頼スレの タイトル します。
		//このときタイトルが取得できなかった時、そんなスレッドはないわけで、エラーとします。
		subject = YBBSManager::getInstance()->getSubject(SettingTXT.getSAKUBBS() , datNumber) + "に対する削除依頼";
			
		//新規に立てるスレッド.
		DATNUMBER newThreadNumber = time(NULL);
		//削除スレをたてる.
		{
			const string msg = YDATWritter::make(
				CheckName(inRHTTPHeader, RConv::htmlspecialchars(ioPost["FROM"]) ),
				RConv::htmlspecialchars(ioPost["mail"]) ,
				YDATWritter::MessageESC(ThisBoardShortCut(newThreadNumber) ,mainMessage) ,
				RConv::htmlspecialchars(subject) ,
				MakeAppendData(inRHTTPHeader,ioPost)
				);
			RPackSuper pack;
			pack.Mount(outShareBuffer , TLS_POOL_MAX);
			pack.Write(msg.c_str() , msg.size() );

			YDAT* dat = Cache.getNewDAT( newThreadNumber );
			dat->Append(&pack);
			dat->setSubject( subject );

			//subject.txt の更新.
			Subject.Age(dat);

			//index.htmlの更新
			IndexHTML.Write( dat , &Subject , &LocalRule , INT_MAX ,  true);
			//subject.txt の更新.
			SubBackHTML.Write(&Subject);

			//キャッシュにもう使用しないことを伝えます
			Cache.Release(dat);
		}

		//削除対象スレッドに削除通知を出す.
		{
			POST	post;
			post["bbs"]  = SettingTXT.getSAKUBBS();
			post["FROM"]  = "YoujyoKiller★";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"このスレッドに対して削除依頼がだされたので報告します。\r\n\r\n"
						"削除依頼スレッド " + ThisBoard() + num2str(newThreadNumber) + "/l50\r\n\r\n"
						"このまま異議がなければ、 " + RDateConv::UnixTime2chTime(deleteWait) + " に、該当書き込みは削除されてしまいます。\r\n"
						"異議のある人は上のスレッドに「異議あり」カキコしてください。\r\n"
						"その場合、すべての責任は「異議あり」カキコした人になるので注意してください\r\n\r\n"
						" ============= 削除依頼の詳細 ===================\r\n"
						+ mainMessage;
			post["key"]  = num2str(datNumber);
			post["submit"] = "書き込む";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}

		//削除する書き込みの収集.
		int deleteSize = 0;
		int deletes[DELETE_REQUEST_MAX];
		for(int i = 0; i < DELETE_REQUEST_MAX ; i ++)
		{
			string thread = "t" + num2str(i);

			if ( ! ioPost[thread].empty() )
			{
				deletes[deleteSize++] = atol( ioPost[thread].c_str() ) ;
			}
		}

		//削除依頼対象書き込みのマスキング
		{
			string targetBoard = SettingTXT.getSAKUBBS() ;
			string password = ioPost["pass"] ;

			//マスク単語を収集
			int maskSize = 0;
			string mask[DELETE_REQUEST_MAX];
			for(int i = 0; i < DELETE_REQUEST_MAX ; i ++)
			{
				string thread = "m" + num2str(i);

				if ( ! ioPost[thread].empty() )
				{
					mask[maskSize++] = ioPost[thread];
				}
			}
			
			//マスク発動.
			YBBSManager::getInstance()->MaskReWrite
						(targetBoard , datNumber , password ,deleteSize , deletes ,maskSize ,  mask );
		}

		//削除依頼をシステムに提出.
		{
			DeleteWait.Presentation(newThreadNumber ,datNumber ,  deleteWait , deleteSize , deletes);
		}

		//書き込み成功を返す
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//エラーを返す.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP()) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" , ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

//削除文を作る.
const string YBBS::makeSakuJyoMessage(POST& ioPost) throw(RException)
{
	DATNUMBER datNumber = atol( ioPost["threadno"].c_str() );

	//削除依頼板
	string message;
	message += "対象スレッド http://" + YHttpdConf::getInstance()->getServerName() + "/test/read.cgi/" + SettingTXT.getSAKUBBS() + "/" + ioPost["threadno"] + "/l50\r\n\r\n";

	for(int i = 0; i < DELETE_REQUEST_MAX ; i ++)
	{
		string thread = "t" + num2str(i);
		string reason = "r" + num2str(i);

		if ( ! ioPost[thread].empty() )
		{
			if ( ioPost[reason].empty() )
			{
				throw RException("削除を依頼された " + ioPost[thread] + "に対する理由が入力されていません");
			}
			message += ">>" + ioPost[thread] + " 理由: " + ioPost[reason] + "\r\n";
		}
	}
	message += "\r\n";
	message += "詳細理由\r\n";
	message += ioPost["MESSAGE"];

	return message;
}

//書き込み内容のチェック.
void YBBS::WriteCheck(POST& ioPost) throw(RException)
{
	//名前
	string name = ioPost["FROM"];
	if ( name.empty() )
	{
		//名無しさん禁止の場合エラー.
		if ( SettingTXT.getNANASHI_CHECK() )
		{
			throw RException("名前が入力されていません");
		}
	}
	if ( name.size() >= SettingTXT.getBBS_NAME_COUNT() )
	{
		throw RException("名前が長すぎます");
	}

	//メール
	if ( ioPost["mail"].size() >= SettingTXT.getBBS_MAIL_COUNT() )
	{
		throw RException("メールアドレスが長すぎます");
	}

	//本文.
	if ( ioPost["MESSAGE"].size() >= SettingTXT.getBBS_MESSAGE_COUNT() )
	{
		throw RException("本文が長すぎます");
	}

	//本文がない.
	if ( ioPost["MESSAGE"].size() <= 0 )
	{
		throw RException("内容がないよう。。。");
	}

	//Subject.
	if ( ioPost["subject"].size() >= SettingTXT.getBBS_SUBJECT_COUNT() )
	{
		throw RException("タイトルが長すぎます");
	}
}

//アク禁チェック.
void YBBS::AccessCheck(const RHTTPHeader* inRHTTPHeader) throw(RException)
{
	in_addr ip = inRHTTPHeader->RemoteHost.getRawIP();

	if ( YFilter::getInstance()->IsDenyIP( ip ) )
	{
		throw RException("この IP からの書き込みは禁止されています。 アク禁くらうようなことしませんでした?");
	}
	if ( SettingTXT.getBBS_NOTJP_FILTER() )
	{
		if (! YFilter::getInstance()->IsJapanIP( ip ) )
		{
			throw RException("海外からの書き込みは禁止させていただいています。");
		}
	}
}


//新規スレ.
void YBBS::NewThread(char* outShareBuffer, POST& ioPost, RStreamUtil * outBuffer)
{
	//新スレ(or削除依頼)を立てる画面を出す.
	this->TranceString(outShareBuffer, BuildThread.make(ioPost , &LocalRule) ,outBuffer);
}



//あぼーん
void YBBS::Aborn(DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete) throw(RIOCannotOpenException,RIOCannotWriteException)
{
	const string msg = YDATWritter::makeAborn(SettingTXT.getBBS_DELETE_NAME());
	RPackSuper pack;
	pack.Write(msg.c_str() , msg.size() );

	YDAT* dat;
	try
	{
		dat = Cache.getDAT(inDeleteTarget);

		int maxNo = dat->getLastNo();

		for(int i = 0; i < inDeleteSize ; i++)
		{
			int no = inDelete[i];
			if (no <= maxNo && no > 0 )
			{
				dat->Overwrite(no-1 , &pack);
			}
		}

		//index.htmlの更新
		IndexHTML.Write( dat , &Subject , &LocalRule , INT_MAX ,  true);

		Cache.Release(dat);
	}
	catch(RException e)
	{
		Cache.Release(dat);
		throw;
	}
}


/**
 * SystemWrite:				システムからのメッセージ
 *
 * @param outShareBuffer	TLSバッファ.
 * @param ioPost			書き込む内容
 * @return void 
 */
void YBBS::SystemWrite(char* outShareBuffer,POST& ioPost) 
{
	try
	{
		DATNUMBER datnumber = atol( ioPost["key"].c_str() );
		//システム特権で書き込み.
		const string msg = YDATWritter::makeSystem(
				RConv::htmlspecialchars(ioPost["FROM"]) ,
				RConv::htmlspecialchars(ioPost["mail"]) ,
				YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]) 
				);

		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);

		pack.Write(msg.c_str() , msg.size() );

		YDAT* dat = Cache.getDAT(datnumber);
		dat->AppendAdmin(&pack);		//システム特権で書き込み

		//どこの位置にあるか検索.
		int oldpos = Subject.WhatPosition( datnumber );

		//subject.txt の更新
		Subject.Age(dat);
		//index.htmlの更新
		IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  true);
		//subject.txt の更新.
		SubBackHTML.Write(&Subject);

		//キャッシュにもう使用しないことを伝えます
		Cache.Release(dat);
	}
	catch(RException e)
	{
		printf("例外:システム特権書き込みでエラーが発生しました:%s\r\n" , e.getMessage() );
		ASSERT(0);
	}
}

//特定DATのSubjectの取得.
const string YBBS::getSubject(DATNUMBER inDatNumber) throw(RNotFoundException,RIOCannotOpenException)
{
	//DATの取得.
	YDAT* dat = Cache.getDAT(inDatNumber);

	//Subjectの取得.
	const string subject = dat->getSubject();

	//キャッシュにもう使用しないことを伝えます
	Cache.Release(dat);

	return subject;
}

//削除依頼の提出補助
void YBBS::NewSakujyoIrai(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer)
	throw(RNotFoundException,RIOCannotOpenException)
{
	ioPost["saku"] = "";
	ioPost["submit"] = "新規スレッド作成画面へ";
	ioPost["bbs"] = SettingTXT.getSAKUBBS();

	YBBSManager::getInstance()->Write(outShareBuffer , inRHTTPHeader , ioPost , outBuffer);
}

//異議あり!
void YBBS::Igiari(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) 
{
	DATNUMBER datnumber = atol( ioPost["key"].c_str() );
	DATNUMBER targetdat ;

	try
	{
		//アクセス禁止のチェック.
		AccessCheck(inRHTTPHeader);
		//書き囲み内容のチェック.
		WriteCheck(ioPost);

		if (ioPost["mail"] == "sage")	RException("sage では異議ありできません");

		//まだこの削除依頼は有効?
		targetdat = this->DeleteWait.IsYourRequest(datnumber);
		if ( targetdat == 0  )	throw RException("この削除依頼はすでに執行されているので取り消せません");

		try
		{
			//相手の DAT が存在しているのか調べる.
			(void) YBBSManager::getInstance()->getSubject( SettingTXT.getSAKUBBS() , targetdat);
		}
		catch(RException e)
		{
			throw RException("削除依頼元は存在していません");
		}

		const string msg = YDATWritter::make(
			CheckName( inRHTTPHeader , RConv::htmlspecialchars(ioPost["FROM"]) ),
			RConv::htmlspecialchars(ioPost["mail"]) ,
			"<font color=\"red\" size=+2><B>異議あり!</B></font><br>" + YDATWritter::MessageESC(ThisBoardShortCut(datnumber) , ioPost["MESSAGE"]),
			MakeAppendData(inRHTTPHeader,ioPost)
			);
		RPackSuper pack;
		pack.Mount(outShareBuffer , TLS_POOL_MAX);

		pack.Write(msg.c_str() , msg.size() );

		YDAT* dat = Cache.getDAT(datnumber);
		dat->Append(&pack);

		//どこの位置にあるか検索.
		int oldpos = Subject.WhatPosition( datnumber );

		//subject.txt の更新
		Subject.Age(dat);
		//index.htmlの更新
		IndexHTML.Write( dat , &Subject , &LocalRule , oldpos ,  true);
		//subject.txt の更新.
		SubBackHTML.Write(&Subject);

		//キャッシュにもう使用しないことを伝えます
		Cache.Release(dat);

		//このスレに停止されたことを書き込む
		{
			POST	post;
			post["bbs"]  = ioPost["bbs"];
			post["FROM"]  = "YoujyoKiller★";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"削除依頼の取り消しを受理しました\r\n\r\n"
						"このスレッドの削除依頼は 「異議あり」カキコがされたので削除依頼はキャンセルされました\r\n"
						"削除しなかったすべての責任は「異議あり」カキコした人にありますので、\r\n"
						"この決定に不服の人は「異議あり」カキコした人に対して裁判でもなんでも起こしてください(w\r\n\r\n"
						" ============= 異議ありの内容 ===================\r\n"
						+ ioPost["MESSAGE"];
			post["key"]  = num2str(datnumber);
			post["submit"] = "書き込む";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}
		//削除対象スレッドに削除取り消し通知をだす.
		{
			POST	post;
			post["bbs"]  = SettingTXT.getSAKUBBS();
			post["FROM"]  = "YoujyoKiller★";
			post["mail"]  = "age";
			post["MESSAGE"]  = 		string("") + 
						"このスレッドに対して削除依頼が取り消されたので報告します。\r\n\r\n"
						"削除依頼スレッド " + ThisBoard() + num2str(datnumber) + "/l50\r\n\r\n"
						"上記スレッドにて 「異議あり」カキコがされたので削除依頼はキャンセルされました\r\n"
						"削除しなかったすべての責任は「異議あり」カキコした人にありますので、\r\n"
						"この決定に不服の人は「異議あり」カキコした人に対して裁判でもなんでも起こしてください(w\r\n\r\n"
						" ============= 異議ありの内容 ===================\r\n"
						+ ioPost["MESSAGE"];
			post["key"]  = num2str(targetdat);
			post["submit"] = "書き込む";

			YBBSManager::getInstance()->SystemWrite(outShareBuffer , post);
		}

		//削除依頼の取り消し.
		this->DeleteWait.Cancel(datnumber);

		//書き込み成功を返す
		this->TranceString(outShareBuffer, WriteHTML.makeKakikoSimasuta() ,outBuffer);
	}
	catch(RException e)
	{
		//エラーを返す.
		this->TranceString(outShareBuffer, 
					WriteHTML.makeError( e.getMessage() , RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP()) ,
														  RConv::htmlspecialchars(ioPost["FROM"]) ,
														  RConv::htmlspecialchars(ioPost["mail"]) ,
														  YDATWritter::MessageESC("" , ioPost["MESSAGE"])  ),
					outBuffer);
		return ;
	}
}

//削除人タスク
void YBBS::KillerTask(char* outShareBuffer)
{
	//削除依頼板でなければ没.
	if (!SettingTXT.getIS_SAKUBBS())	return ;

	//削除依頼の状態を見て、
	//削除するべきであれば削除する.
	DeleteWait.Check(outShareBuffer , this->BoardName , SettingTXT.getSAKUBBS() );
}

//DATガページコレクタ
void YBBS::GCTask()
{
	Cache.Collecter();
}


//情報を出力.
void YBBS::Dump()
{
	Cache.Dump();
}

///すべての情報をディスクにフラッシュする.
void YBBS::Flush()
{
	Cache.Flush();
	Subject.Save(this->BoardName);
	DeleteWait.Save(this->BoardName);
}

//IP抜いたり、ID表示したり、 mailアドレスの右にあるいろいろな情報を作ります。
const string YBBS::MakeAppendData(const RHTTPHeader* inRHTTPHeader ,const POST & iopPost) const
{
	if ( SettingTXT.getBBS_DISP_IP() )
	{	//IP表示.
		return string(" Host:") + RConv::htmlspecialchars(inRHTTPHeader->RemoteHost.getIP());
	}
	//ID表示は非対応ということで(w
	//アルゴリズムがわからん、 DES なの? だれか教えてちょ.
	return "";
}

//名前チェック
const string YBBS::CheckName(const RHTTPHeader* inRHTTPHeader ,const string & inName) const
{
	//名前がからなら名無しさん.
	if (inName.empty() ) return SettingTXT.getBBS_NONAME_NAME();

	//逆引き遅くなるからやらないので各自 whois で調べてください(w
	const string ip = inRHTTPHeader->RemoteHost.getIP();

	//fusianasanトラップ.
	string name = RConv::Replace(inName , "fusianasan" , ip);
	//★はかけないようにする.
	name = RConv::Replace(name , "★" , "\"★\"");
	
	//キャップとかいろいろはまだ実装してません。
	return name;
}


//削除依頼された書き込みのやばい単語をマスクする.
void YBBS::MaskReWrite(DATNUMBER inDatNumber ,const string & password ,
	 int inDeleteSize ,const int * inDelete , int inMaskSize   ,const string *inMask ) 
	 throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException)
{
}

