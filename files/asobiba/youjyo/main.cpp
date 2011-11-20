#include "comm.h"
#include "YServer.h"
#include "YYoujyoKiller.h"
#include "YYoujyoGC.h"
#include "YYoujyoLogger.h"

#include <conio.h>
#include <ctype.h>



//テスト
#include "RToken.h"
#include "YRequestPath.h"
#include "RCollect.h"
#include "YDAT.h"
#include "RConv.h"
#include "YDATWritter.h"
#include "YDATReader.h"
#include "RHTTPHeaderAttribute.h"
#include "RHTTPHeader.h"
#include "RHTTPPOSTParser.h"
#include "YBBSManager.h"
#include "YSubject.h"
#include "RAlphabetHash.h"
#include "RDateConv.h"
#include "RHtmlTemplate.h"
#include "YIndexHTML.h"
#include "YReadCGITemplate.h"
#include "YSubBackHTML.h"
#include "RConfigFile.h"
#include "RMimeType.h"
#include "YDeleteWait.h"
#include "RIPFilter.h"
#include "YFilter.h"
#include "BlowFish.h"
#include "RFileDB.h"

/*
readme.html:動作環境 
OS 　 
	　 Windows2000以降
	WindowsXPでの動作は確認していません(おいらはXPもってないよーん)
 
ハード 　 
	　 高速なLANカードと高速なメモリ(256MB以上必須)
	いまどき低スペックのマシンでサーバたてているやつなんて(ﾌﾟ
*/
/*
readme.html:ライセンス 

バイナリライセンス 　 
	　 ょぅι゛ょ宣言に抵触しなければ、ご自由に。
	ただし、雑誌などに転載、紹介する場合は、
	FAQにあるとおり、ょぅι゛ょ宣言を紹介するスペースと同じ広さの紙面に印字する必要があります。
 
ソースコードライセンス 　 
	　 ょぅι゛ょのソースを使っていると明記していただければ、ソースコードは自由に使ってもらってかまいません。
	ただし、一部に apache2 のソースコードを使っていますので、その部分は apache のソースコードのライセンスにしたがって使用してください。

	apache2のソースコードを使っている部分は、 RDateConv.cpp の部分です。
	httpでの日付を文字列から unixtime に戻すためのルーチンで apache2 よりも高速なルーチンを書く自信がなかったために
	コ゜ピペさせていただきました。

	その他のソースコードについては、ょぅι゛ょ宣言に抵触しなければ、ご自由に使用してください。
	ただし、ょぅι゛ょサーバのソースコードを含むとマニュアル等に記述する必要があります。 

掲載等のライセンス 　 
	　 無許可でご自由に。
	ただし、その時点で最新のょぅι゛ょ宣言を紹介するスペースと同じ広さの紙面に印字する必要があります。

	つまりょぅι゛ょを1へージ扱う雑誌は、そのスペースと同じスペースつまり、1ページまるごとょぅι゛ょ宣言を
	印刷しなくてはなりません。
 
著作権 　 
	　 著作権は rti になります。
	こんなソフトウェアですが、日本国の法律で多分保護されています。 
	もうだめぽのおじさんにおこられないように、がんがって使用してください。 

最新のライセンス 　 
	　 最新のライセンスは常にソースコードの中に含まれています。
	もし、ここにかいてあることと、ソースコードの中に書いてあるライセンスが矛盾した場合、ソースコードの方が優先されます。
	場所は main.cpp の ライセンスの項目をご覧ください。 
	つまりココです!!
その他 　 
	　 何かありましたら、要望板に書いてください 

*/
/*
readme.html:FAQ

ょぅι゛ょサーバについて 　 
	　 ょぅι゛ょサーバは read.cgi および、 bbs.cgi 機能を組み込んだ、高速web server です。
	掲示板を維持するのに必要なファイルはほとんど on memory で保持しているために高速に動作します。 
	実況サーバで 風呂キタ━━━━(゜∀゜)━━━━!!!!! とか書き込んでも「応答がありません」 とかで、
	しょぼーん　な時がありませんか?
	そんな実況大好きな人達にお送りする、高速な 2chコンパチなサーバです。

	KeepAlive , E-TAG , などはサポートしていません。
	LastModifyed , Range についてはサポートしています。
	GET , POST メソッドしかサポートしていません。 POST で Bounder はサポートしていません。
 
名前はなんと呼べばよいのでしょうか? 　 
	　 ょぅι゛ょは日本語では発音できない文字を含んでいます。
	これは、 唯一神である ＹＨＷＨ が、あまりに神聖な名前のため発音できないように母音を抜いたように
	あまりに偉大で神聖なために、ょぅι゛ょと表記します。 
	ただ、このままでは、実生活で支障をきたすために、
	一番近い日本語である ようじょ を割り当てることにし、 Youjyo と表記します。
	これは、あくまで発音上の処理であり、ほかに意味はありません。 ないんだってば。
 
私の使っている 2chブラウザから貴様のサーバはみえんぞｺﾞﾗｧ 　 
	　 一応、かちゅーしゃ と、 Live2ch では動作していることを確認しています。
	それ以外で動作しないものがありましたら、適当にスレたてて質問してください。 
	なお、推奨ブラウザは 当然 Live2ch となります。
	リロード時間を最小にしてご使用ください。
 
Windows9xでは動作しません 　 
	動作しません。
	NTからしか実装されていない API を呼び出しているからです。
	サーバの性質上 Windows 2000以降をお勧めします。 

メモリをアホのように食うんですが 　 
	　 ひたすら on memory で動作するので、 かなりのメモリを消費します。
	また、掲示板の数によってどんどん消費メモリは増えていきます。 
	が、現在メモリなんて10年前から見れば、タダみたいな価格で買えるのでじゃんじゃんつみましょう。
	開発目標としては 200MB いないで動作すればいいと思っていますし(w
	メモリは 512MB 以上つめ!!!
 
終了に時間がかかるのですが 　 
	　 スレッドプールがあるため、それらを終了しているからです。
	(　ﾟдﾟ)ﾎﾟｶｰﾝと表示された後、しばらくお待ちください。 

落ちるんですけど 　 
	　 まだα版なんで許してください(w
	バグレポしてもらえると、うれしいです。 

サーバの癖に windows アプリですか(ﾌﾟ 　 
	　 うるせー、 linux? あんな開発環境が糞で、スレッドの挙動がぁゃιぃOSでプログラムができるかってんでー。 
	VC最高、マンセー。 Emacs , vi を使っているやつは人生の敗北者。
	Emacs信者は Emacsで VC と*完全に同じ*機能を整えてから反論してください。 話しはそれからだ。
	(-> で メソッド一覧、 Shift + SPACE で補完、 F1 でヘルプ、 VCコンパチなマクロ)

	どうせ、 Linux用に作るとしたら linux kernel に組み込みたいですね(w
 
今後の野望 　 
	　 更なる高速化、バグ潰し、目指せWebServerシェアNo1、世界制覇、世界制服 



	目指す理想の世界 
	マイク やぁ、ボブ、 自宅でサーバを開始したっんだって? 
	ボブ そうさ、I×Sを使って簡単にWebServerを設定したんだ。 
	マイク いまどき、I×Sなんて。
	これからは、 ょぅι゛ょさ。  
	ボブ ょぅι゛ょ? なんだいそりゃ? 
	マイク それが、ジャパニーズ発祥のすごくイカレタ^h^h^h^h WebServer らしいよ。 
	ボブ うわっ、これすごいね。それに掲示板もディフォルトで入っているなんて便利だね。 
	マイク そうだよ、これからはょぅι゛ょだよ。 
	ボブ うん、これからはょぅι゛ょの時代だね 

	名前はすべて空想上のものです。実在のいかなるものでもありません。

	まじめに書くと、
	2.0 で インチキ akamai 方式による負荷分散
	3.0 で P2P による負荷分散をねらってまうす。
 
掲載について 　 
	　 無許可でご自由に。
	ただし、その時点で最新のょぅι゛ょ宣言を紹介するスペースと同じ広さの紙面に印字する必要があります。

	つまりょぅι゛ょを1へージ扱う雑誌は、そのスペースと同じスペースつまり、1ページまるごとょぅι゛ょ宣言を
	印刷しなくてはなりません。 
	ソースコードライセンス 　 
	　 ょぅι゛ょのソースを使っていると明記していただければ、ソースコードは自由に使ってもらってかまいません。
	ただし、一部に apache2 のソースコードを使っていますので、その部分は apache のソースコードのライセンスにしたがって使用してください。

	apache2のソースコードを使っている部分は、 RDateConv.cpp の部分です。
	httpでの日付を文字列から unixtime に戻すためのルーチンで apache2 よりも高速なルーチンを書く自信がなかったために
	コ゜ピペさせていただきました。

	その他のソースコードについては、ょぅι゛ょ宣言に抵触しなければ、ご自由に使用してください。
	ただし、ょぅι゛ょサーバのソースコードを含むとマニュアル等に記述する必要があります。  
*/
void main()
{
	puts("startup.....");

	try
	{
		//サーバの設定の読み込み.
		YHttpdConf::getInstance()->Load("httpd.conf");
		//Mimeタイプの読み込み.
		RMimeType::getInstance()->Load("mime.types");
		//フィルターするIP
		YFilter::getInstance()->Load();
		//BBSを登録.
		YBBSManager::getInstance()->ReloadBBS();
	}
	catch(RException e)
	{
		printf("設定ファイルを読み込み中にエラーが発生しました:%s\r\n",e.getMessage() );
		abort();
	}
	puts("OK... 設定ファイルを読み込み終わりました。");

#ifndef _NO_TEST_MODE
	//テストを行います。
	YDeleteWait::test();
	RDateConv::test();
	RToken::test();
	YRequestPath::test();
	YDAT::test();
	RConv::test();
	YDATWritter::test();
	YDATReader::test();
	RHTTPHeaderAttribute<string>::test();
	RHTTPHeader::test();
	RHTTPPOSTParser::test();
//	YSubject::test();
	RAlphabetHash::test();
	RHtmlTemplate::test();
	YIndexHTML::test();
	YReadCGITemplate::test();
	YSubBackHTML::test();
	RConfigFile::test();
	RMimeType::test();
	RMmap::test();
	RIPFilterData::test();
	RIPFilter::test();
	RBlowFish::test();
	RFileDB::test();

	RCollect<string ,int > rc;
	rc["10"] = new int;
#endif

	puts("winsock を初期化します...");

	WSADATA wsaData;
	WSAStartup(2 , &wsaData);
	printf("winsocバージョン = %d.%d\n記述 = %s\n状態 = %s\n" ,
		(BYTE)wsaData.wHighVersion , wsaData.wHighVersion >> 8 ,
		wsaData.szDescription ,	wsaData.szSystemStatus
	);
	puts("winsock を初期化に成功しました.");

	YBBSManager::getInstance();
	{
		YServer			yServer;
		YYoujyoKiller	youjyoKiller;
		YYoujyoGC		youjyoGC;
		YYoujyoLogger	youjyoLogger;

		yServer.Start();
		youjyoLogger.Start();
		youjyoKiller.Start();
		youjyoGC.Start();

		puts("ょぅι゛ょキタ━━━━(゜∀゜)━━━━!!!!!");
		puts("終了は q / SPACE , メモリダンプは d , 全データのフラッシュは f だよ、おにぃちゃま★");

		while( 1 )
		{
			char p = _getch();
			if ( p == 'q') break;				//おしまい
			if ( p == ' ') break;				//これでもおしまい
			if ( p == 'd') //dump
			{
				YBBSManager::getInstance()->Dump();
			}
			if ( p == 'f') //flush
			{
				YBBSManager::getInstance()->Flush();
			}
		}
		puts("(　ﾟдﾟ)ﾎﾟｶｰﾝ");

		puts("サーバを停止しています");
		yServer.Shutdown();
		puts("YoujyoLoggerを停止しています");
		youjyoLogger.Shutdown();
		puts("YoujyoKillerを停止しています");
		youjyoKiller.Shutdown();
		puts("YoujyoGCを停止しています");
		youjyoGC.Shutdown();
		Sleep(2);						//おまじない
		yServer.Join();
		puts("サーバの停止を完了しました");
		youjyoLogger.Join();
		puts("YoujyoLoggerの停止を完了しました"); 
		youjyoKiller.Join();
		puts("YoujyoKillerの停止を完了しました");
		youjyoGC.Join();
		puts("YoujyoGCの停止を完了しました");
		Sleep(2);						//おまじない
	}

	WSACleanup();

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

}
