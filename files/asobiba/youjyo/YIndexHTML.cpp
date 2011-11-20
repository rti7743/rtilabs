// YIndexHTML.cpp: YIndexHTML クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YIndexHTML.h"
#include "YDATWritter.h"
#include "YDATReader.h"
#include "RDebug.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YIndexHTML::YIndexHTML()
{
}

YIndexHTML::~YIndexHTML()
{
}

void YIndexHTML::Setting(const string inBoardName  ,const YSettingTXT*	inSettingTXT)
{
	try
	{
		AllTemplate.Load(YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\index.tpl");
	}
	catch(RException e)
	{
		printf("板(%s)のindex.tplテンプレート読み込み中に例外を感知しました。 早期に終了させてください。\r\n%s\r\n" ,inBoardName.c_str() , e.getMessage() );
	}

	//スレッド一覧
	ThreadIndex = AllTemplate.Select("YThreadIndex");
	//スレッド単位
	ThreadsTemplate = AllTemplate.Select("YThreads");
	//高速化を図るためにスレッドで固定のところの部分を切り出したヤツ
	SubThreadTemplate = (ThreadsTemplate == NULL) ? NULL : ThreadsTemplate->Select("YSubThread");
	//一書き込み
	KakikoTemplate = (SubThreadTemplate == NULL) ? NULL : SubThreadTemplate->Select("YKakiko");

	ASSERT(ThreadIndex);
	ASSERT(ThreadsTemplate);
	ASSERT(SubThreadTemplate);
	ASSERT(KakikoTemplate);

	this->BoardName = inBoardName;
	this->SettingTXT =	inSettingTXT;
}


/**
 * Load:			Subject から先頭の TOP_DAT を取得し index.html を作成します.
 *
 * @param ioCache	キャッシュ
 * @param inSubject	サブジェクト
 * @return void 
 */
void YIndexHTML::Load(YDATCache* ioCache , YSubject* inSubject, YLocalRule* inLocalRule)throw(RIOCannotOpenException)
{
	RAutoWriteSpinLock al(&Lock);

	const string byReadcgi = "../test/read.cgi/" + BoardName + "/" ;

	string threads;
	threads.reserve(ONE_RES_MAX * ALL_PAGE_RES);
	//メインコンテンツの作成.
	{
		DATNUMBER numbers[TOP_DAT];
		//取得.
		int datCount = inSubject->getNumbers(numbers , TOP_DAT);
		for(int i = 0; i < datCount ; i++)
		{
			DATNUMBER datnumer = numbers[i] ; 
			YDAT * dat = ioCache->getDAT( datnumer );
			const int end = dat->getLastNo();
			const int start = end > ONE_DAT_TAIL ? (end-ONE_DAT_TAIL) : (1) ;

			string kakiko;
			kakiko.reserve(ONE_RES_MAX * (ONE_DAT_TAIL + 1));
			//1を作る.
			{
				RPack pack;
				if ( dat->ReadAt(&pack , 0 , 1) )
				{
					RStringMap	smap;
					YDATReader::makeHTML( &smap , 1  ,(const char*) pack.getMemory() );
					kakiko += KakikoTemplate->Replace(&smap);
				}
			}

			//それ以降を作成.
			RPack pack;
			if ( dat->ReadAt(&pack , start , end ) )
			{
				const char * p = (const char*) pack.getMemory();
				const char * m = p + pack.getSize();
				for(int i = start; ; i++)
				{
					if (!p || p >= m) break;

					RStringMap	smap;
					p = YDATReader::makeHTML( &smap , i+1  , p);
					kakiko += KakikoTemplate->Replace(&smap);
				}
			}
			const string readcgi = byReadcgi + num2str( datnumer ) + "/";

			// Subthread の構築.
			RStringMap	smap;
			smap["YThreadResNumber"] = num2str( dat->getLastNo() );
			smap["YThreadSubject"] = dat->getSubject();
			smap["YItaName"] = BoardName;
			smap["YThreadNumber"] = num2str( datnumer );
			smap["YThreadALL"] = readcgi;
			smap["YThreadL50"] = readcgi + "l50";
			smap["YThreadL1-100"] =  readcgi + "-100";
			smap["YKakiko"] = kakiko;
			string subthread = SubThreadTemplate->Replace(&smap) ;

			// SubThread をすかさずキャッシュに。
			Cache.setCache(i , subthread);

			//Threads を作成。
			int index = i + 1;
			smap["YThreadIndexNo"] = num2str(index);
			smap["YThreadLocalJumpBack"] = "#" + num2str( (index-1) <= 0 ? datCount : index-1 );
			smap["YThreadLocalJumpNext"] = "#" + num2str( (index+1) > datCount ? 1 : index+1 );
			smap["YSubThread"] = subthread;
			
			threads += ThreadsTemplate->Replace(&smap) ;
		}
	}

	//画面をつくるのですよー.
	RStringMap	smap;

	smap["YItaFullName"] = SettingTXT->getBBS_TITLE();
	smap["YItaName"] = BoardName;
	smap["YThreads"] = threads;
	smap["YThreadIndex"] = inSubject->DrawThreadIndex(ThreadIndex , byReadcgi , SHOW_INDEX_MAX);
	smap["YServerInfo"] = YHttpdConf::getInstance()->getVersion();
	smap["YItaLocalRule"] = inLocalRule->getLocalRule();

	this->IndexHTML = AllTemplate.Replace(&smap);
}


void YIndexHTML::Write(YDAT* inDAT ,YSubject* inSubject, YLocalRule* inLocalRule, int  oldPos , bool isAge )
{
	RAutoWriteSpinLock al(&Lock);

	if (! isAge )
	{
		// sage で top 10 に入っていなければ index.html に含めなくても良いので return .
		//ただし、このシナリオはほとんど期待できない。
		//だって実況だと一つのすれだけ age り、ほかのスレが age らないので 結果として
		//実況すれは表示されっぱなしになるのだ。
		if ( oldPos >= TOP_DAT ) return ;
	}

	DATNUMBER datnumer = inDAT->getDatNumber();

	const int end = inDAT->getLastNo();
	const int start = end > ONE_DAT_TAIL ? (end-ONE_DAT_TAIL) : (1) ;

	//自分の DAT の作成.
	string kakiko;
	kakiko.reserve(ONE_RES_MAX * (ONE_DAT_TAIL + 1));
	//1を作る.
	{
		RPack pack;
		if ( inDAT->ReadAt(&pack , 0 , 1) )
		{
			RStringMap	smap;
			YDATReader::makeHTML( &smap , 1  ,(const char*) pack.getMemory() );
			kakiko += KakikoTemplate->Replace(&smap);
		}
	}
	//それ以降を作成.
	RPack pack;
	if ( inDAT->ReadAt(&pack , start , end ) )
	{
		const char * p = (const char*) pack.getMemory();
		const char * m = p + pack.getSize();
		for(int i = start; ; i++)
		{
			if (!p || p >= m) break;

			RStringMap	smap;
			p = YDATReader::makeHTML( &smap , i+1  , p);
			kakiko += KakikoTemplate->Replace(&smap);
		}
	}

	const string readcgi = "../test/read.cgi/" + BoardName + "/" + num2str( datnumer ) + "/";

	// Subthread の構築.
	RStringMap	smap;
	smap["YThreadResNumber"] = num2str( inDAT->getLastNo() );
	smap["YThreadSubject"] = inDAT->getSubject();
	smap["YItaName"] = BoardName;
	smap["YThreadNumber"] = num2str( datnumer );
	smap["YThreadALL"] = readcgi;
	smap["YThreadL50"] = readcgi + "l50";
	smap["YThreadL1-100"] =  readcgi + "-100";
	smap["YKakiko"] = kakiko;

	kakiko = SubThreadTemplate->Replace(&smap);

//	Cache.Dump();

	if ( isAge )
	{
		//新規 age
		if (oldPos >= TOP_DAT)	Cache.NewAge(kakiko);
		//既存の DAT の age . (ふつーで一番考えられるシナリオ)
		else					Cache.Age(kakiko,oldPos);
	}
	else
	{
		//sage 実況では期待できそう。
		Cache.Sage(kakiko,oldPos);
	}

//	Cache.Dump();

	this->LoadWithCache(inSubject, inLocalRule);

//	Cache.Dump();

	Update();
}

void YIndexHTMLAlloc::Dump()
{
	{
		printf("===================== YIndexHTMLAlloc::Dump() ===========================\r\n");
		for(int i = 0; i < TOP_DAT ; i++)
		{
			printf("Cache[%d] as %x --> \r\n" ,i , &Cache[i]  );
		}
	}
	{
		for(int i = 0; i < TOP_DAT ; i++)
		{
			printf("CacheIndex[%d] as %x --> \r\n" ,i , CacheIndex[i]  );
		}
		printf("===================== --------------------- ===========================\r\n");
	}
	//自己チェック
	//同じところをさしているポインタはない.
	{
		for(int i = 0; i < TOP_DAT ; i++)
		{
			for(int n = i+1; n < TOP_DAT ; n++)
			{
				if (CacheIndex[i] == CacheIndex[n])
				{
					printf( "同じところを指しています. ");
					ASSERT(0);
				}
			}
		}
	}
}



/**
 * LoadWithCache:					Subject から先頭の TOP_DAT を取得し index.html を作成します.
 *
 * @param ioCache	キャッシュ
 * @param inSubject	サブジェクト
 * @return void 
 */
void YIndexHTML::LoadWithCache(YSubject* inSubject, YLocalRule* inLocalRule)
{
	const string byReadcgi = "../test/read.cgi/" + BoardName + "/" ;
	string threads;
	threads.reserve(ONE_RES_MAX * ALL_PAGE_RES);
	//メインコンテンツの作成.
	{
		RStringMap	smap;
		DATNUMBER numbers[TOP_DAT];
		//取得.
		int datCount = inSubject->getNumbers(numbers , TOP_DAT);
		for(int i = 0; i < datCount ; i++)
		{
			DATNUMBER datnumer = numbers[i]; 
			int index = i + 1;

			smap["YThreadIndexNo"] = num2str(index);
			smap["YThreadLocalJumpBack"] = "#" + num2str( (index-1) <= 0 ? datCount : index-1 );
			smap["YThreadLocalJumpNext"] = "#" + num2str( (index+1) > datCount ? 1 : index+1 );
			smap["YSubThread"] = Cache.getCache(i);
			string r = Cache.getCache(i);
			threads += ThreadsTemplate->Replace(&smap) ;
		}
	}

	RStringMap	smap;

	smap["YItaFullName"] = SettingTXT->getBBS_TITLE();
	smap["YItaName"] = BoardName;
	smap["YThreads"] = threads;
	smap["YThreadIndex"] = inSubject->DrawThreadIndex(ThreadIndex , byReadcgi , SHOW_INDEX_MAX);
	smap["YServerInfo"] = YHttpdConf::getInstance()->getVersion();
	smap["YItaLocalRule"] = inLocalRule->getLocalRule();

	this->IndexHTML = AllTemplate.Replace(&smap);
}


void YIndexHTML::Show(RPack * outRPack) const
{
	RAutoReadSpinLock al(&Lock);

	outRPack->Write( IndexHTML.c_str()  , IndexHTML.size() );
}

void YIndexHTML::test()
{
	{
		YIndexHTML IndexHTML;
		YDATCache Cache;
		YSubject Subject;
		string board;

		board = "template2";
		string path = YHttpdConf::getInstance()->getDocumentRoot() + "/" + board ;

		Cache.setBoardName(board);
//		Subject.Load(board);

		YSettingTXT setting;
		YLocalRule	localRule;

		setting.Load(board);
		IndexHTML.Setting( board ,&setting );
		IndexHTML.Load(&Cache,&Subject,&localRule);

		//最初に何もないので、スレッドを作ってみます。 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "1test thread." );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1000 );
			dat->Append(&pack);
			dat->setSubject( "1test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject ,&localRule, INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test1.html");
			Cache.Release(dat);
		}
		//もう一つスレッドを作ります。 2 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" , "2test thread." );
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1100 );
			dat->Append(&pack);
			dat->setSubject( "2test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule , INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test2.html");
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "3test thread." ); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1200 );
			dat->Append(&pack);
			dat->setSubject( "3test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test3.html");
			Cache.Release(dat);
		}
		//2番のスレッドを age します。 2 3 1
		{
			const string msg = YDATWritter::make("" , "age" , "test!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test4.html");
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "4test thread." ,""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1400 );
			dat->Append(&pack);
			dat->setSubject( "4test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test5.html");
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "5test thread." ,""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1500 );
			dat->Append(&pack);
			dat->setSubject( "5test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "6test thread.",""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1600 );
			dat->Append(&pack);
			dat->setSubject( "6test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "7test thread." ,""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1700 );
			dat->Append(&pack);
			dat->setSubject( "7test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}

		//さらにスレッドを作ります。8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "8test thread." ,""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1800 );
			dat->Append(&pack);
			dat->setSubject( "8test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。9 8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "9test thread." ,""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 1900 );
			dat->Append(&pack);
			dat->setSubject( "9test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。10 9 8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "10test thread." ,""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 2000 );
			dat->Append(&pack);
			dat->setSubject( "10test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			Cache.Release(dat);
		}
		//さらにスレッドを作ります。11 10 9 8 7 6 5 4 3 2 1
		{
			const string msg = YDATWritter::make("" , "sage" , "test!" , "11test thread." ,""); //スレッド作成は age になる.
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getNewDAT( 2100 );
			dat->Append(&pack);
			dat->setSubject( "11test thread." );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  INT_MAX ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test6.html");
			Cache.Release(dat);
		}
		//2番のスレッドを age します。 2 11 10 9 8 7 6 5 4 3 1
		{
			const string msg = YDATWritter::make("ななしさん" , "age" , "age!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test7.html");
			Cache.Release(dat);
		}
		//1番のスレッドを new age します。 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("ななしさんMX" , "age" , "age!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1000 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1000 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test8.html");
			Cache.Release(dat);
		}
		//2番のスレッドを sage します。 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("ななしさん" , "sage" , "sage!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  false);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test9.html");
			Cache.Release(dat);
		}
		//3番のスレッドを sage(画面外 sage) します。 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("ななしさん" , "sage" , "sage!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1200 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1200 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  false);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test10.html");
			Cache.Release(dat);
		}
		//1番のスレッドを age します。 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("ななしさん" , "age" , "age!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1000 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1000 );
			Subject.Age(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test11.html");
			Cache.Release(dat);
		}
		//1番のスレッドを sage します。 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("ななしさん" , "sage" , "sage!" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1000 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1000 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  true);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test12.html");
			Cache.Release(dat);
		}
		//2番のスレッドを sage します。 1 2 11 10 9 8 7 6 5 4 3
		{
			const string msg = YDATWritter::make("ななしさん" , "sage" , "sageですわ" ,"");
			RPackSuper pack;
			pack.Write(msg.c_str() , msg.size() );
			YDAT* dat = Cache.getDAT( 1100 );
			dat->Append(&pack);

			//どこの位置にあるか検索.
			int oldpos = Subject.WhatPosition( 1100 );
			Subject.Sage(dat);

			IndexHTML.Write( dat , &Subject , &localRule ,  oldpos ,  false);
			RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );
			RDebug::FileVerify(path + "/index.html" , path + "/test13.html");
			Cache.Release(dat);
		}
	}

	{
		YIndexHTML IndexHTML;
		YDATCache Cache;
		YSubject Subject;
		string board;

		board = "template";
		string path = YHttpdConf::getInstance()->getDocumentRoot() + "/" + board ;

		Cache.setBoardName(board);
		Subject.Load(board);

		YSettingTXT setting;
		YLocalRule	localRule;

		setting.Load(board);
		IndexHTML.Setting( board ,&setting );
		IndexHTML.Load(&Cache,&Subject ,  &localRule );
		RDebug::MemoryToFile(path + "/index.html" , IndexHTML.IndexHTML );

		//作成したファイルがあらかじめテストで作ったファイルと同じであるかどうかチェック.
		RDebug::FileVerify(path + "/index.html" , path + "/indexcheck.html");
	}

	puts("YIndexHTML::test() OK");
}

