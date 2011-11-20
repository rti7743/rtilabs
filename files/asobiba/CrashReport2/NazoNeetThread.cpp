
#include "stdafx.h"
#include "NazoNeetThread.h"


unsigned int NazoNeetThread::Run()
{
	//ヒープの取得デモをやりたいので適当に。
	vector<char> v;
	v.reserve(1024);
	memset(&v[0] , 1024 ,72);	//魔法の数字 72. ちーちゃんをいじめるなと何度いえば(ry

	int a = 0;
	//ニートの一日
	//ニートの朝は早い、そしてまた寝る.
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//どうしておじちゃんたち働かないの？
	a = 1;
	//ニートの目覚め
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//2chのチェックしてダウソしておなぬしていたら働く時間なんかなくね？ その理屈はおかしい。
	a = 2;
	//ニートの昼
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//世間の目が厳しい
	a = 3;
	//ニートのおやつ
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//働いたら負けかなと思った。
	a = 4;
	//ニートの夕方
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//親「働け！」 「うるせーババァ」 ←家族の団欒 イイハナシダー (;_;
	a = 5;
	//ニートの夜
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//でも、僕は働くのが嫌いだ。そうか、ぼくは働かなくいいんだ。ニートでいてもいいんだ。おめでとう。
	a = 6;
	//ニートの深夜
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//父にありがとう。母さようなら。そしてすべてのニートにおめでとう。
	a = 7;
	//ニートの早朝
	this->Sleep(1000000);
	a = 8;
	
	//今日も一日がんばったお
	return 0;
}
