// REasyAI.h: REasyAI クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REASYAI_H__5B25E038_1A76_46F8_B36A_6404565F98B1__INCLUDED_)
#define AFX_REASYAI_H__5B25E038_1A76_46F8_B36A_6404565F98B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

template<class _T , class _RATINGFUNC , int _USER_TEACH_DEFAULT_RATE = 2>
class REasyAI  
{
public:
	REasyAI()
	{
	}
	virtual ~REasyAI()
	{
	}

	//パターンに一番近いと思われる関連付けられた名前の取得.
	string Place(const _T & inPattern)
	{
		string name = "";
		int maxRate = INT_MIN;
		for( THE_MEMORY::iterator i = Memory.begin() ; i != Memory.end() ; ++i)
		{
			int rate = (*i).getRate(inPattern);
			if ( maxRate < rate)
			{
				name = (*i).getName();
				maxRate = rate;
			}
		}
		return name;
	}

	//正解を教える.
	//inPattern		正解のパターン
	//inRight		正解
	//inError		間違って識別してしまったもの
	void Teach(const _T & inPattern , string inRight , string inError = "")
	{
		if (! inError.empty() )
		{
			//間違って認識したやつらの点数を下げる.
			for( THE_MEMORY::iterator i = Memory.begin() ; i != Memory.end() ; ++i)
			{
				if ((*i).getName() == inError)
				{
					(*i).Down();
				}
			}
		}
		bool BetterData = false;
		//点数が90点以上のデータが存在したらそいつを上げ、正解は加えない
		//点数が75点以上のデータが存在したらそいつを上げる、
		//点数が30点以下のデータでれば点数をさらに下げる.
		for( THE_MEMORY::iterator i = Memory.begin() ; i != Memory.end() ; ++i)
		{
			//名前が同じでないと調べても無意味...
			if ((*i).getName() == inRight)
			{
				int rate = (*i).getRate(inPattern);
				if ( rate >= 90)		//ちよちゃん　（；´Д`）ﾊｧﾊｧ.
				{
					BetterData = true;
					(*i).Up();
				}
				else if (rate <= 75)	//すばらしい
				{
					(*i).Up();
				}
				else if (rate <= 30)	//赤点やー. ボンクラーズ.
				{
					(*i).Down();
				}
			}
		}

		//最悪値のやつらを抹消.
		for( i = Memory.begin() ; i != Memory.end() ; )
		{
			if ( (*i).IsWarstData() )
			{
				THE_MEMORY::iterator  del = i;
				++i;
				Memory.erase(del);
			}
			else		++i;
		}

		//90点以上のデータがなかったら正解を加える.
		if (BetterData)
		{	//正解を加える.
			Memory.push_back( LimitMemory(inRight,inPattern) );
		}
	}
private:
	class LimitMemory
	{
	public:
		LimitMemory(const string inName ,const _T & inPattern)
		{
			Name = inName;
			Pattern = inPattern;
			RateExperience = _USER_TEACH_DEFAULT_RATE;
		}
		//エラーだったら点数を下げる.
		void Down()
		{
			RateExperience--;
		}
		//正解したので点数を上げる.
		void Up()
		{
			RateExperience--;
		}
		//最悪値を更新してます?
		bool IsWarstData()
		{
			return RateExperience < 0;
		}
		//点数付け 100点万点.
		int getRate(const _T & inPattern)
		{
			return _RATINGFUNC::Rating(inPattern , Pattern);
		}
		string getName()
		{
			return Name;
		}
	private:

		//パターン名
		string		Name;
		//データ
		_T			Pattern;
		//今までの一致関係	//マイナスになると消滅.
		int			RateExperience;
	};
private:
	typedef deque<LimitMemory>	THE_MEMORY;
	THE_MEMORY					Memory;
};

#endif // !defined(AFX_REASYAI_H__5B25E038_1A76_46F8_B36A_6404565F98B1__INCLUDED_)
