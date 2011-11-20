// SolvablePath.h: SolvablePath クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEPATH_H__416A6339_2481_4E13_810C_FC5B05787302__INCLUDED_)
#define AFX_SOLVABLEPATH_H__416A6339_2481_4E13_810C_FC5B05787302__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SolvablePath;


#include "comm.h"
#include "PathInterface.h"

//パス管理.
class SolvablePath  : public PathInterface
{

public:
	SolvablePath(SolvablePath* inNext = NULL)
	{
		this->Next = inNext;
	}
	virtual ~SolvablePath()
	{
		delete Next;
	}

	//解決してください
	SolvablePath* PleaseSolve(string inPath)
	{
		//自分でできる? できたら俺凄いぢゃん　ってことで自分を帰す。
		if ( this->DoSolvable(inPath) ) return this;
		//次の人がいるならその人が解決してくれるよきっと。chain of Responsibility
		if ( this->Next ) return this->Next->PleaseSolve(inPath);
		//なんでぇい、解決できないやん。 もうだめぽ。
		return NULL;
	}
protected:
	//そのパスを解決してみる
	//解決できた場合は解決した結果のパスは メンバに setPath でセットすること.
	//www.yahoo.co.jp ---> http://www.yahoo.co.jp/ などとなる
	//解決できた場合は true 解決できなかった場合は false
	virtual bool DoSolvable(string inPath)  = 0;

private:
	//次に解決を試すやつ. chain of Responsibility
	SolvablePath*	Next;
};

#endif // !defined(AFX_SOLVABLEPATH_H__416A6339_2481_4E13_810C_FC5B05787302__INCLUDED_)
