// Owners.h: Owners クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OWNERS_H__B6538D8C_2F8B_4F17_B58B_33D9179143B7__INCLUDED_)
#define AFX_OWNERS_H__B6538D8C_2F8B_4F17_B58B_33D9179143B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Owners;

#include "comm.h"


//ファイルを所持しているオーナーさんたち.
//Compositeパターン.
class Owners  
{
private:
	//さらにオーナーがいる場合.
	Owners*		MoreOwners;
	string		Name;
public:
	//かならず new で確保すること!!
	Owners(string inName , Owners* inMoreOwners = NULL)
	{
		Name = inName;
		MoreOwners = inMoreOwners;
	}
private:
	//new 以外での作成を禁止するための処置.
	virtual ~Owners()
	{
		if ( MoreOwners ) MoreOwners->Delete();
	}
public:
	//new を以外を禁止するために デストラクタを private にしたために
	//外部からdelete できなくなった。　そのために自爆メソッドを用意する
	void		Delete(){	delete this;	}
	//オーナーの名前を返す.
	string		getName(){	return Name;	}
	//次のオーナーさん NULL で終了.
	Owners*		next(){		return MoreOwners;	}

	//テスト
	static void test()
	{
		Owners * p = new Owners("ととろ", new Owners("めい", new Owners("さつき")) );

		Owners * d;
		d = p;

		string debugBuffer;
		while(d)
		{
			debugBuffer += d->getName();
			d = d->next();
		}

		if (debugBuffer != "ととろめいさつき")
		{
			ASSERT(0);
		}

		p->Delete();
	}
};

#endif // !defined(AFX_OWNERS_H__B6538D8C_2F8B_4F17_B58B_33D9179143B7__INCLUDED_)
