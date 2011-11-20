
#include "stdafx.h"
#include "NazoNeetWorker.h"
#include "RDiskUtil.h"
#include "RMiniDump.h"
#include <conio.h>
#include <ctype.h>

//ニートの数
const int NEET_THREAD_COUNT = 10;


//お前らメイン関数ですよ!
int main()
{
	puts("All your thread are belong to us!.");
	puts("");
	puts("");
	puts("初期化なう...");
	
	//カレントディレクトリを保存しておく.
	string currentDirectory = RDiskUtil::getCurrentDirectory();
	
	
	/*
		スレッド君、どこ行ってもうたんや・・・
		
		このスレッド君はまったく仕事をしません。
		一応動作するスレッドなのですが、一度実行したらいつまでたってもメインスレッドに帰ってきません。
		リソース資源の無駄とお客さんから大目玉を食らうし、全く困ったもんなんです。 
		
		そんなスレッド君なのですが、サンプルプログラムとしてらピカイチってことに最近気付きました。
		根気よく使っていれば、長所が見つかるもんです。
	*/
	//ニート増産中
	NazoNeetThread neetThread[NEET_THREAD_COUNT];

	puts("MinuDump がスレッドの仲間で追跡してくれるか調べるサンプルプログラムだお");
	puts("q または、スペースでプログラムが終了.");
	puts("d で minidump(ヒープを除く)を取得します.");
	puts("b で minidump(取れるものすべて)を取得します.");
	puts("");
	puts("");
	printf("コマンド>");

	int i = 0;
	//スレッドスタート
	for(i = 0 ; i < NEET_THREAD_COUNT ; i ++)
	{
		//のワの＜ 働け！愚民ども
		neetThread[i].Start();
	}

	//入力待ちだお
	while( 1 )
	{
		char p = _getch();
		if ( p == 'q') break;				//おしまい
		if ( p == ' ') break;				//これでもおしまい
		if ( p == 'd') 						//minidump(ヒープを除く)
		{
			string dumpfilename = currentDirectory + "__MiniDumpSmall.dmp";
			printf("ダンプを取得中だお。。。\r\n");
			fflush(stdout);
			RMiniDump::MiniDump(dumpfilename,MINIDUMP_TYPE(MiniDumpNormal | MiniDumpWithHandleData));
			printf("MiniDump(ピープを除く)を取得しますた。 %s\r\n" , dumpfilename.c_str() );
		}
		if ( p == 'b') 						//minidump(取れるものすべて)
		{
			string dumpfilename = currentDirectory + "__MiniDumpFull.dmp";
			printf("ダンプ取得中なう\r\n");
			fflush(stdout);
			RMiniDump::MiniDump(dumpfilename,MINIDUMP_TYPE(MiniDumpWithFullMemory | MiniDumpWithHandleData));
			printf("MiniDump(取れるものすべて)を取得しますた。 %s\r\n" , dumpfilename.c_str() );
		}
	}
	puts("");
	puts("(　ﾟдﾟ)ﾎﾟｶｰﾝ");
	puts("終了中です...");
	fflush(stdout);

	//スレッドに終了命令を出す
	for(i = 0 ; i < NEET_THREAD_COUNT ; i ++)
	{
		//死んでください太子!
		neetThread[i].HoldUp();
	}
	//スレッドが完全に終了するまで待機
	for(i = 0 ; i < NEET_THREAD_COUNT ; i ++)
	{
		//ニートが死ぬまで殴るのをやめない！
		neetThread[i].Join();
	}

	//その後二人は幸せに暮らしましたとさ。めでしたしめでたし。
	//おしまい. ただしイケメンに限る。
	return 0;
}
