// YReadCGITemplate.cpp: YReadCGITemplate クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YReadCGITemplate.h"
#include "YIndexHTML.h"
#include <math.h>
#include "YDATReader.h"
#include "YDATWritter.h"
#include "RDebug.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YReadCGITemplate::YReadCGITemplate()
{

}

YReadCGITemplate::~YReadCGITemplate()
{

}

void YReadCGITemplate::Load(const string inBoardName,const YSettingTXT*	inSettingTXT)
{
	try
	{
		Template.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\readcgi.tpl");
	}
	catch(RException e)
	{
		printf("板(%s)のreadcgi.tplテンプレート読み込み中に例外を感知しました。 早期に終了させてください。\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}

	//ページ割ループ
	PageJump	=	this->Template.Select("YPageJump");
	//前100件
	IfBefor100	=	this->Template.Select("YIfBefor100");
	//次100件
	IfNext100	=	this->Template.Select("YIfNext100");
	//書き込み表示部分. メインですな。
	Kakiko	=	this->Template.Select("YKakiko");
	//レス数が<!--?V:YResAlert-->を超えています。<!--?V:YResOver-->を超えると表示できなくなるよ。
	IfResWarning	=	this->Template.Select("YIfResWarning");
	//サイズが<!--?V:YSizeAlert-->KBを超えています。<!--?V:YSizeOver-->KBを超えると表示できなくなるよ。
	IfSizeWarning	=	this->Template.Select("YIfSizeWarning");
	//レス数が<!--?V:YResOver-->を超えています。残念ながら全部は表示しません。
	IfResOver	=	this->Template.Select("YIfResOver");

	this->BoardName = inBoardName;
	this->SettingTXT =	inSettingTXT;
}

/**
 * make:					指定された範囲内でスレッドの中身を html で表示する.
 *
 * @param inDAT				使用する DAT.
 * @param inStart			開始位置
 * @param inEnd 
 * @param is1san 
 * @return const string 
 */
const string YReadCGITemplate::make(YDAT* inDAT ,unsigned int inStart ,unsigned int inEnd , bool is1san) const
{
	RStringMap	masterSmap;

	const int res = inDAT->getLastNo();
	const int size = inDAT->getDatSize();
	//警告表示の作成.
	if ( res >= MAX_NUMBER )
	{
		RStringMap	smap;
		smap["YResOver"] = num2str(MAX_NUMBER);
		masterSmap["YIfResOver"] = IfResOver->Replace(&smap);
	}
	else if ( size >= WARNING_DAT_SIZE )
	{
		RStringMap	smap;
		smap["YSizeOver"] = num2str(WARNING_DAT_SIZE);
		masterSmap["YIfResOver"] = IfResOver->Replace(&smap);
	}
	else if ( res >= WARNING_NUMBER )
	{
		RStringMap	smap;
		smap["YResAlert"] = num2str(WARNING_NUMBER);
		smap["YResOver"] = num2str(MAX_NUMBER);
		masterSmap["YIfResWarning"] = IfResWarning->Replace(&smap);
	}
	
	//ページ割の作成.
	{
		const int page = ceil(res / 100) + 1;
		string pagestr;
		pagestr.reserve(page * 100);	//100は適当.
		for(int i = 1 ; i < page ; i++)
		{
			RStringMap	smap;
			smap["YPageFrom"] = num2str(i * 100 + 1);
			smap["YPageTo"] = num2str( i * 100 + 100);
			pagestr += PageJump->Replace(&smap);
		}
		masterSmap["YPageJump"] = pagestr;
	}

	//次 100.
	{
		RStringMap	smap;
		smap["YPageFrom"] = num2str(inEnd + 1);
		smap["YPageTo"] = num2str(inEnd + 101);
		masterSmap["YIfNext100"] = IfNext100->Replace(&smap);
	}
	//前 100.
	if (inStart > 1)
	{
		RStringMap	smap;
		if (inStart <= 101)
		{
			smap["YPageFrom"] = "";
		}
		else
		{
			smap["YPageFrom"] = num2str( inStart - 101 );
		}
		smap["YPageTo"] = num2str(inStart - 1);
		masterSmap["YIfBefor100"] = IfBefor100->Replace(&smap);
	}

	//書き込み文章の表示.
	{
		RPack pack;

		// 0 からはじめるから。
		if (inStart != 0) inStart--;	//unsigned ということを忘れるな.

		string thread;
		if (inEnd < inStart)
			thread.reserve(ONE_RES_MAX * 1 );
		else
			thread.reserve(ONE_RES_MAX * (inEnd-inStart + 2) );
		//1をつける?
		if (is1san)
		{
			if ( inDAT->ReadAt(&pack , 0 , 1 ) )
			{
				RStringMap	smap;
				YDATReader::makeHTML( &smap , 1  ,(const char*) pack.getMemory() );
				thread += Kakiko->Replace(&smap);

				//すでに1を書いたので、スキップ.
				if (inStart == 0)  inStart= 1;
			}
		}

		//それ以降を作成.
		if ( inDAT->ReadAt(&pack , inStart , inEnd ) )
		{
			const char * p = (const char*) pack.getMemory();
			const char * m = p + pack.getSize();
			for(int i = inStart; ; i++)
			{
				if (!p || p >= m) break;

				RStringMap	smap;
				p = YDATReader::makeHTML( &smap , i+1  , p);
				thread += Kakiko->Replace(&smap);
			}
		}
		masterSmap["YKakiko"] = thread;
	}

	/*ChangeLog.txt:2003/01/23	新着レスの表示が 2- に固定されていたのを修正.
	*/
	masterSmap["YThreadResNumber"] = num2str(res);			//書き込み件数.
	masterSmap["YThreadSubject"] = inDAT->getSubject();
	masterSmap["YDATSize"] = num2str( (int)(size / 1024) );	//KB
	masterSmap["YItaName"] = BoardName;
	masterSmap["YThreadNumber"] = num2str( inDAT->getDatNumber() );
	masterSmap["YServerInfo"] = YHttpdConf::getInstance()->getVersion();

	return Template.Replace(&masterSmap);
}


void YReadCGITemplate::test()
{
	{
		YReadCGITemplate read;
		YDATCache Cache;
		YSubject Subject;
		string board;
		YSettingTXT setting;

		board = "template3";
		string path = YHttpdConf::getInstance()->getDocumentRoot() + "/" + board ;

		setting.Load(board);
		read.Load(board,&setting);
		Cache.setBoardName(board);

		//700ぐらい書き込んでみる.
		//話はそれからだ。
		{
			{
				const string msg = YDATWritter::make("ななしさん" , "age" , "test!" , "1test thread." ,"");
				RPackSuper pack;
				pack.Write(msg.c_str() , msg.size() );
				YDAT* dat = Cache.getNewDAT( 1000 );
				dat->Append(&pack);
				dat->setSubject( "1test thread." );
				Subject.Age(dat);
				Cache.Release(dat);
			}
			for(int i = 1 ; i < 700 ; i++)
			{
				const string msg = YDATWritter::make("ななしさん" , "age" , "test!" + num2str(i+1) ,"");
				RPackSuper pack;
				pack.Write(msg.c_str() , msg.size() );
				YDAT* dat = Cache.getDAT( 1000 );
				dat->Append(&pack);
				Subject.Age(dat);
				Cache.Release(dat);
			}
			YDAT* dat = Cache.getDAT( 1000 );
			// 逆転入力.
			{ 
				string ret = read.make( dat , 100 , 0 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test8.html");
			}
			// 0 - 0 を表示してみる. 1だけ表示.
			{ 
				string ret = read.make( dat , 0 , 0 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test7.html");
			}
			// 0 - 200 を表示してみる.
			{
				string ret = read.make( dat , 0 , 200 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test6.html");
			}
			// 101 - 200 を表示してみる.
			{
				string ret = read.make( dat , 101 , 200 , true);
				RDebug::MemoryToFile(path + "/test.html" , ret);
				RDebug::FileVerify(path + "/test.html" , path + "/test5.html");
			}
			Cache.Release(dat);
		}
		//ありえない範囲の指定. だけど、1を表示するフラグかOK. 1だけ表示される.
		{
			const string msg = YDATWritter::make("ななしさん" , "age" , "test!" , "1test thread.","" );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1100 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 100 , 1001 , true);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test4.html");
			Cache.Release(dat);
		}
		//ありえない範囲の指定. 何も表示されない.
		{
			const string msg = YDATWritter::make("ななしさん" , "age" , "test!" , "1test thread." ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1200 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 100 , 1001 , false);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test3.html");
			Cache.Release(dat);
		}
		//1だけなのに、 1 を表示するフラグをおろす. やっぱり1だけが表示されるが正解.
		{
			const string msg = YDATWritter::make("ななしさん" , "age" , "test!" , "1test thread." ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1300 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 1 , 1 , false);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test2.html");
			Cache.Release(dat);
		}
		//1だけなのに、 1 を表示するフラグをたてて、 1 を表示するようにする. 1だけが表示されるが正解.
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "1test thread." ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1400 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			string ret = read.make( dat , 1 , 1 , true);
			RDebug::MemoryToFile(path + "/test.html" , ret);
			RDebug::FileVerify(path + "/test.html" , path + "/test1.html");
			Cache.Release(dat);
		}
	}

	puts("YReadCGITemplate::test() OK");
}

