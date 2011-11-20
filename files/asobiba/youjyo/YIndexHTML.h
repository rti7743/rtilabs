// YIndexHTML.h: YIndexHTML クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YINDEXHTML_H__38A1CEFB_2347_4F39_8335_28F4DF5C86E0__INCLUDED_)
#define AFX_YINDEXHTML_H__38A1CEFB_2347_4F39_8335_28F4DF5C86E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotFoundException.h"
#include "YDATCache1.h"
#include "YSubject.h"
#include "RSpinLock.h"
#include "YTextFile.h"
#include "YSettingTXT.h"
#include "YLocalRule.h"
/*
readme.html:Step3
<B>一行に書き込める長さは?</B>
	一行に書き込める長さは、 100バイトです。
	また、書き込める行数は 40 行までです。
	なお、index.htmlでは、 20 行を超えるものは省略されます。
	これを書き換えるには、 YIndexHTML.h を書き換えてください。

<B>index.html で何件まで表示されますか?</B>
	index.html では 10件まで表示する表示するできます。
	これを書き換えるには、 YIndexHTML.h を書き換えてください。

<B>index.html で menu に表示されるスレッドの数は?</B>
	index.html では 40件までのスレッドを menu に表示します。
	これを書き換えるには、 YIndexHTML.h を書き換えてください。
*/

//書き込みの一行の長さ
const int ONE_LINE_MAX	=	100;
//書き込みの行数
const int ALL_LINE_MAX	=	40;
//index.html で省略されるサイズ.
const int ABBREVIATION_LINE	=	20;
//つまり、位置書き込みの最大サイズは..
const int ONE_RES_MAX	=	(ONE_LINE_MAX * ALL_LINE_MAX);
//DAT の後ろから表示する数
const int ONE_DAT_TAIL	=	10;
//表示する DAT の数.
const int TOP_DAT		=	10;
//つまり、一ページに表示するためにプールしておかなければいけない書き込みの数
const int ALL_PAGE_RES	=	(ONE_DAT_TAIL * TOP_DAT);
//index.html に表示する スレッド一覧.
const int SHOW_INDEX_MAX	=	40;


class YIndexHTMLAlloc
{
public:
	YIndexHTMLAlloc()
	{
		for(int i = 0 ; i < TOP_DAT ; i++)
		{
			Cache[i] = "";	//size() == 0 にする.
			CacheIndex[i] = &Cache[i];
		}
	}
	void setCache(int inNo,const string & inStr)
	{
		ASSERT(inNo >= 0 && inNo < TOP_DAT);
		(*CacheIndex[inNo]) = inStr;
	}
	string getCache(int inNo)
	{
		ASSERT(inNo >= 0 && inNo < TOP_DAT);
		return (*CacheIndex[inNo]);
	}
	inline void NewAge(const string & inStr)
	{
		string* newpoint = CacheIndex[TOP_DAT-1];
		//この DAT を一番上にするために、 1 -> 2 , 2 -> 3 , 3 -> 4 とシフトします。
		{
			for(int i = TOP_DAT-1 ; i > 0 ; i--)
			{
				CacheIndex[i] = CacheIndex[i-1];
			}
		}
		//ageられたデータ.
		CacheIndex[0] = newpoint;
		*newpoint =		inStr;
	}
	inline void Age(const string & inStr,int oldpos)
	{
		ASSERT(oldpos >= 0 && oldpos < TOP_DAT);
		//befor
		//0
		//1
		//2 <- これが age られる
		//3
		//4

		//after
		//2 <- こうなる
		//0
		//1
		//3
		//4

		//2の領域を保護.
		string* newpoint = CacheIndex[oldpos];

		//2 を上げるスペースを作るため 0 , 1 をスライドさせます。
		{
			for(int i = oldpos ; i > 0 ; i--)
			{
				CacheIndex[i] = CacheIndex[i-1];
			}
		}
		//ageられたデータ.
		CacheIndex[0] = newpoint;
		*newpoint =		inStr;
	}
	inline void Sage(const string & inStr,int oldpos)
	{
		ASSERT(oldpos >= 0 && oldpos < TOP_DAT);
		(*CacheIndex[oldpos]) = inStr;
	}

	void Dump();
private:
	//便宜上の index.
	string*	CacheIndex[TOP_DAT];
	//実態.
	string	Cache[TOP_DAT];
};

class YIndexHTML  : public YTextFile
{
public:
	YIndexHTML();
	virtual ~YIndexHTML();

	//初期設定.
	void Setting(const string inBoardName  ,const YSettingTXT*	inSettingTXT);

	//subject.txt より top を読み込み index.html を作成. キャッシュを使わない.
	void Load(YDATCache* ioCache , YSubject* inSubject,YLocalRule* inLocalRule)throw(RIOCannotOpenException);

	//age / sage の書き込み
	void YIndexHTML::Write(YDAT* inDAT ,YSubject* inSubject,YLocalRule* inLocalRule,int  oldPos , bool isAge );


	/**
	 * Show:				表示			 YTextFileより継承
	 *
	 * @param outRPack		これに格納
	 * @return void 
	 */
	virtual void Show(RPack * outRPack) const;

	static void YIndexHTML::test();

private:
	//キャッシュからロードして index.html の作成をする.
	void YIndexHTML::LoadWithCache(YSubject* inSubject,YLocalRule* inLocalRule);

	//index.html の中身 超でかい.
	string						IndexHTML;
	//BBSの名前
	string						BoardName;
	//設定関係
	const YSettingTXT*			SettingTXT;

	//全体
	RHtmlTemplate				AllTemplate ;
	//スレッド一覧
	const RHtmlTemplate*		ThreadIndex ;
	//スレッド単位
	const RHtmlTemplate*		ThreadsTemplate ;
	//高速化を図るためにスレッドで固定のところの部分を切り出したヤツ
	const RHtmlTemplate*		SubThreadTemplate;
	//一書き込み
	const RHtmlTemplate*		KakikoTemplate ;

	//キャッシュ.
	YIndexHTMLAlloc				Cache;

	mutable RSpinLock	Lock;

};

#endif // !defined(AFX_YINDEXHTML_H__38A1CEFB_2347_4F39_8335_28F4DF5C86E0__INCLUDED_)
