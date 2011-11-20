// YBBS.h: YBBS クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YBBS_H__18D276CD_B0BD_452C_A839_248197EFCD5C__INCLUDED_)
#define AFX_YBBS_H__18D276CD_B0BD_452C_A839_248197EFCD5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YDAT.h"
#include "YDatCache.h"
#include "YDATReader.h"
#include "YDATWritter.h"
#include "RCollect.h"
#include "YRequestPath.h"
#include "RStreamUtil.h"
#include "ROutOfRangeException.h"
#include "RHTTPPOSTParser.h"
#include "RSpinLock.h"
#include "YSubject.h"
#include "YDATCache1.h"
#include "RHTTPHeader.h"
#include "YIndexHTML.h"
#include "YReadCGITemplate.h"
#include "YSubBackHTML.h"
#include "YBuildThread.h"
#include "YTextFile.h"
#include "YSettingTXT.h"
#include "YWriteHTML.h"
#include "YDeleteWait.h"
#include "YLocalRule.h"


#include "RDateConv.h"
class YBBS  
{
public:
	YBBS(string inName);
	virtual ~YBBS();

	//読み込み.
	void Read(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,  const YRequestPath* inRequestPath, RStreamUtil * outBuffer ) throw(RIOCannotOpenException);

	//書き込み.
	void Write(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) ;

	//新規.
	void New(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) ;

	//新規スレ.
	void NewThread(char* outShareBuffer, POST& ioPost, RStreamUtil * outBuffer);

	//あぼーん
	void Aborn(DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete) throw(RIOCannotOpenException,RIOCannotWriteException);

	//削除依頼の提出補助
	void YBBS::NewSakujyoIrai(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , POST& ioPost, RStreamUtil * outBuffer);


	/**
	 * SystemWrite:				システムからのメッセージ
	 *
	 * @param outShareBuffer	TLSバッファ.
	 * @param ioPost			書き込む内容
	 * @return void 
	 */
	void SystemWrite(char* outShareBuffer,POST& ioPost);

	//特定DATのSubjectの取得.
	const string getSubject(DATNUMBER inDatNumber) throw(RNotFoundException,RIOCannotOpenException);

	//異議あり!
	void Igiari(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) ;

	//削除依頼された書き込みのやばい単語をマスクする.
	void MaskReWrite(DATNUMBER inDatNumber ,const string & password ,
		 int inDeleteSize ,const int * inDelete , int inMaskSize   ,const string *inMask ) 
		 throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException);

	/////制御用スレッドによる実行
	//削除人タスク.
	void KillerTask(char* outShareBuffer);
	//DATガページコレクタ
	void GCTask();

	/////////////////管理者用命令
	//情報を出力.
	void Dump();
	///すべての情報をディスクにフラッシュする.
	void Flush();

private:
	//DATの取得.
	YDAT* YBBS::getDAT(DATNUMBER inDATNumber) throw(RIOCannotOpenException);
	//新規すれ
	YDAT* YBBS::getNewDAT(DATNUMBER inDATNumber);

	//read.cgi コンパチな表示を行います。 DATの解放はこの中で行います(高速化のため).
	void RequestTypeCGI(char* outShareBuffer,YDAT * inDat , int inStart, int inEnd , bool is1san , RStreamUtil * outBuffer)	throw(RIOCannotOpenException);
	//ファイル転送.
	void TextFileTransmit(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,YTextFile* inUseFile,  RStreamUtil * outBuffer);

	//REQUESTTYPE_DAT
	void YBBS::RequestTypeDat(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , const YRequestPath* inRequestPath, RStreamUtil * outBuffer)	throw(RIOCannotOpenException);
	//書き込み内容のチェック.
	void YBBS::WriteCheck(POST& ioPost) throw(RException);
	//削除文を作る.
	const string YBBS::makeSakuJyoMessage(POST& ioPost) throw(RException);
	//削除依頼
	void YBBS::NewDelete(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) throw(RException);
	//異議あり!
	void YBBS::Igiari(char* outShareBuffer,POST& ioPost, RStreamUtil * outBuffer ) throw(RException);
	//IP抜いたり、ID表示したり、 mailアドレスの右にあるいろいろな情報を作ります。
	const string YBBS::MakeAppendData(const RHTTPHeader* inRHTTPHeader ,const POST & iopPost) const;
	//名前チェック
	const string YBBS::CheckName(const RHTTPHeader* inRHTTPHeader ,const string & inName) const;
	//アク禁チェック.
	void AccessCheck(const RHTTPHeader* inRHTTPHeader) throw(RException);

	//メッセージを普通に転送する.
	void YBBS::TranceString(char* outShareBuffer,const string &inMessage , RStreamUtil * outBuffer)	throw(RException)
	{
		/*ChangeLog.txt:2003/01/23	Connection: close の最後の \r\nを忘れていて ネスケ で見えなかったのを修正
			http://pc3.2ch.net/test/read.cgi/tech/1043275721/10
		*/
		int len = sprintf( outShareBuffer ,	"HTTP/1.1 200 OK\r\n"
											"Date: %s\r\n"
											"Server: Youjyo\r\n"
											"Accept-Ranges: bytes\r\n"
											"Connection: close\r\n"
											"Content-Type: text/html\r\n"
											"Content-Length: %d\r\n"
											"\r\n"
										,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
										,	inMessage.size()
										);

		outBuffer->WriteBuffer( outShareBuffer , len ); 
		outBuffer->WriteBuffer( inMessage.c_str() , inMessage.size() ); 
	}
	//この板のリンク.
	const string ThisBoard() const
	{
		return "http://" + YHttpdConf::getInstance()->getServerName() + "/test/read.cgi/" + this->BoardName + "/";
	}
	//特定DATへのリンク.
	const string ThisBoardShortCut(DATNUMBER inDatNumber) const
	{
		return "../test/read.cgi/" + this->BoardName + "/" + num2str(inDatNumber) + "/";
	}

	
private:
	//板名
	string		BoardName;

	//DATキャッシュ
	YDATCache	Cache;

	//subject.txt
	YSubject	Subject;

	//index.html
	YIndexHTML	IndexHTML;

	//read.cgi の画面
	YReadCGITemplate	ReadCGI;

	//subback.html
	YSubBackHTML	SubBackHTML;

	//新規スレッド作成
	YBuildThread	BuildThread;

	//板設定ファイル
	YSettingTXT		SettingTXT;

	//書き込みますた html
	YWriteHTML		WriteHTML;

	//削除待ち
	YDeleteWait		DeleteWait;

	//ローカルルール
	YLocalRule		LocalRule;
};

#endif // !defined(AFX_YBBS_H__18D276CD_B0BD_452C_A839_248197EFCD5C__INCLUDED_)
