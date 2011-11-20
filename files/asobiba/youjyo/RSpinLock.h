// RSpinLock.h: RSpinLock クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPINLOCK_H__5CBD6C4C_F745_412B_BC55_BD9514D0E834__INCLUDED_)
#define AFX_RSPINLOCK_H__5CBD6C4C_F745_412B_BC55_BD9514D0E834__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RLock.h"
/*
class RSpinLock  
{
public:
	RSpinLock()
	{
		ReadCount = 0;
	}
	virtual ~RSpinLock()
	{

	}

	void ReadLock()
	{
		printf("%x ReadLock() start\r\n",this);

		//書き込んでいないことを確認.
		this->WriteLockKey.Lock();

		//ロックオブジェクト事態を占有.
		this->SystemLock.Lock();

		//誰も読み込んでいない場合は読み込みロックを書ける.
		if (this->ReadCount == 0)
		{
			ReadLockKey.Lock();
				printf("%x @Read Lock\r\n",this);
		}
				printf("%x %d\n" , this,this->ReadCount);
		this->ReadCount ++;
		
		//ロックオブジェクトの占有を止める
		this->SystemLock.UnLock();
		//当然書き込みではないので書き込みロックは排除する.
		this->WriteLockKey.UnLock();

		printf("%x ReadLock() end\r\n",this);
	}
	void ReadUnLock()
	{
		printf("%x ReadUnLock() start\r\n",this);

		//ロックオブジェクト事態を占有.
		this->SystemLock.Lock();
	
		//自分が最後の書き込みだったら、読み込みロックを解放.
		if (this->ReadCount == 1)
		{
			ReadLockKey.UnLock();
				printf("%x @Read UnLock\r\n",this);
		}
		this->ReadCount --;
				printf("%x %d\n" , this,this->ReadCount);

		//ロックオブジェクトの占有を止める
		this->SystemLock.UnLock();

		printf("%x ReadUnLock() end\r\n",this);
	}

	void WriteLock()
	{
		printf("%x WriteLock() start\r\n",this);

		//誰も読み込んでいないのを確認.
		this->ReadLockKey.Lock();
		//誰も書き込んでいないのを確認
		this->WriteLockKey.Lock();
			printf("%x *Write Lock\r\n",this);
		//読み込みはしないので読み込みロックを解除
		this->ReadLockKey.UnLock();

		printf("%x WriteLock() end\r\n",this);
	}

	void WriteUnLock()
	{
		printf("%x WriteUnLock() start\r\n",this);

		//書き込みの解放.
		this->WriteLockKey.UnLock();
			printf("%x *Write UnLock\r\n",this);

		printf("%x WriteUnLock() end\r\n",this);
	}

private:
	RLock	SystemLock;

	RLock	ReadLockKey;
	RLock	WriteLockKey;
	int		ReadCount;
};


//自動読み込みロック
class RAutoReadSpinLock
{
public:
	RAutoReadSpinLock(RSpinLock * l)
	{
		Lock = l;
		Lock->ReadLock();
	}
	~RAutoReadSpinLock()
	{
		Lock->ReadUnLock();
	}
private:
	RSpinLock*	Lock;
};
//自動書き込みロック
class RAutoWriteSpinLock
{
public:
	RAutoWriteSpinLock(RSpinLock * l)
	{
		Lock = l;
		Lock->WriteLock();
	}
	~RAutoWriteSpinLock()
	{
		Lock->WriteUnLock();
	}
private:
	RSpinLock*	Lock;
};
*/

#define RSpinLock RLock
#define RAutoReadSpinLock RAutoLock
#define RAutoWriteSpinLock RAutoLock

#endif // !defined(AFX_RSPINLOCK_H__5CBD6C4C_F745_412B_BC55_BD9514D0E834__INCLUDED_)
