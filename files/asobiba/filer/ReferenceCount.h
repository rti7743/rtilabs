// ReferenceCount.h: ReferenceCount クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REFERENCECOUNT_H__665C2388_60B8_44CA_A8A6_E2F5488DAE4B__INCLUDED_)
#define AFX_REFERENCECOUNT_H__665C2388_60B8_44CA_A8A6_E2F5488DAE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ReferenceCount  ;

#include "comm.h"
#include "RLock.h"

class ReferenceCount  
{
public:
	ReferenceCount()
	{
		this->UseCount = 0;
		//作成.
		Inc();
	}
protected:
	//デストラクタは勝手に呼び出すことはできません
	virtual ~ReferenceCount()
	{
	}
private:
	//コピーコンストラクタは呼べません.
	ReferenceCount(ReferenceCount & inOrignal)
	{
	}
public:
	//このクラスを開始した
	void	Inc()
	{
		Lock.Lock();
		{//ここの間はクリティカルセクションで守られています。
			UseCount++;
		}
		Lock.UnLock();
	}
	//このクラスの使用を止めた
	bool	Dec()
	{
		Lock.Lock();
		{//ここの間はクリティカルセクションで守られています。
			UseCount--;
			if (UseCount <= 0)
			{
				Lock.UnLock();
				delete this;
				return true;
			}
		}
		Lock.UnLock();
		return false;
	}
private:
	//ロック.
	RLock			Lock;
	//参照カウント.
	int		UseCount;
};

#endif // !defined(AFX_REFERENCECOUNT_H__665C2388_60B8_44CA_A8A6_E2F5488DAE4B__INCLUDED_)
