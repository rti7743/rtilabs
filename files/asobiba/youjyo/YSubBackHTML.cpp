// YSubBackHTML.cpp: YSubBackHTML クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YSubBackHTML.h"
#include "YHttpdConf.h"

//-----------debug use.
#include "RDebug.h"
#include "YDATWritter.h"
#include "YDATCache1.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YSubBackHTML::YSubBackHTML()
{

}

YSubBackHTML::~YSubBackHTML()
{

}


void YSubBackHTML::Load(const string inBoardName ,const YSettingTXT*	inSettingTXT)
{
	try
	{
		Template.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\subback.tpl");
	}
	catch(RException e)
	{
		printf("板(%s)subback.tplテンプレート読み込み中に例外を感知しました。 早期に終了させてください。\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}

	//一覧
	this->ThreadIndex	=	this->Template.Select("YThreadIndex");

	this->BoardName = inBoardName;
	this->SettingTXT = inSettingTXT;
}

/**
 * Write:					subback.html の作成.
 *
 * @param inSubject			subject.txt
 * @return void 
 */
void YSubBackHTML::Write(YSubject* inSubject)
{
	RAutoWriteSpinLock al(&Lock);

	RStringMap smap;
	smap["YItaFullName"] = SettingTXT->getBBS_TITLE();
	smap["YItaName"] = this->BoardName;
	smap["YItaReadCGI"] = string("http://") + YHttpdConf::getInstance()->getServerName() + "/test/read.cgi/" + BoardName + "/" ;
	smap["YThreadIndex"] = inSubject->DrawThreadIndex(ThreadIndex , "" , MAX_SUBJECT);

	this->SubBackHTML = this->Template.Replace(&smap);
}

/**
 * Show:				表示			YTextFileより継承
 *
 * @param outRPack		これに格納
 * @return void 
 */
void YSubBackHTML::Show(RPack * outRPack) const
{
	RAutoReadSpinLock al(&Lock);

	outRPack->Write( SubBackHTML.c_str()  , SubBackHTML.size() );
}


void YSubBackHTML::test()
{
		YSubBackHTML subBack;
		YDATCache Cache;
		YSubject Subject;
		string board;
		YSettingTXT setting;

		board = "template4";

		string path = YHttpdConf::getInstance()->getDocumentRoot() + "/" + board ;

		setting.Load(board);
		subBack.Load(board,&setting);
		Cache.setBoardName(board);
//		Subject.Load(board);

		//最初に何もないので、スレッドを作ってみます。 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "1test thread." );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1000 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);
			Cache.Release(dat);

			subBack.Write(&Subject);
			RDebug::MemoryToFile(path + "/test.html" , subBack.SubBackHTML );
			RDebug::FileVerify(path + "/test.html" , path + "/test1.html");
		}
		//つづいて作ってみる 2 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "2test thread." );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1100 );
			dat->Append(&pack);
			dat->setSubject( "2test thread." );
			Subject.Age(dat);
			Cache.Release(dat);

			subBack.Write(&Subject);
			RDebug::MemoryToFile(path + "/test.html" , subBack.SubBackHTML );
			RDebug::FileVerify(path + "/test.html" , path + "/test2.html");
		}
		//つづいて作ってみる 3 2 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "3test thread." );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1200 );
			dat->Append(&pack);
			dat->setSubject( "3test thread." );
			Subject.Age(dat);
			Cache.Release(dat);

			subBack.Write(&Subject);
			RDebug::MemoryToFile(path + "/test.html" , subBack.SubBackHTML );
			RDebug::FileVerify(path + "/test.html" , path + "/test3.html");
		}

}

