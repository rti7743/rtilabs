// YDATCache1.h: YDATCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDATCACHE1_H__00131CC4_CE1A_444F_AB27_A9F915767561__INCLUDED_)
#define AFX_YDATCACHE1_H__00131CC4_CE1A_444F_AB27_A9F915767561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YDAT.h"
#include "RSpinLock.h"
#include "RAlreadyException.h"

class YDATCap : public YDAT
{
public:
	YDATCap(DATNUMBER	inDatNumber) : YDAT(inDatNumber)
	{
		Count = 0;
		LastAccess = time(NULL);
		OnMemoryTime = LastAccess;
	}
	void Use()
	{
		Count ++;
		LastAccess = time(NULL);
	}
	void UnUse()
	{
		Count --;
	}

	bool isUse()
	{
		return (Count != 0);
	}

	//ディスクに書き戻した
	void WriteBack()
	{
		OnMemoryTime = time(NULL);
	}

	//現在時刻との差を求める
	DWORD	DifferenceTime(DWORD inNow)
	{
		return inNow - LastAccess;
	}
	//現在時刻との差を求める
	DWORD	DifferenceOnMemoryTime(DWORD inNow)
	{
		return inNow - OnMemoryTime;
	}
private:
	int		Count;
	//最終アクセス時間
	DWORD	LastAccess;
	//メモリ上にしか存在していない時間
	DWORD	OnMemoryTime;
};

class YDATCache  
{
public:
	YDATCache();
	virtual ~YDATCache();

	//DATの取得.
	YDAT* getDAT(DATNUMBER inDATNumber) throw(RIOCannotOpenException);
	//新規すれ
	YDAT* getNewDAT(DATNUMBER inDATNumber) throw (RAlreadyException);
	//DAT使用を止める
	void Release(YDAT* inDAT);
	/**
	 * Collecter:		ガページコレクト
	 *
	 * @return void 
	 */
	void YDATCache::Collecter();

	void setBoardName(const string inBoardName)
	{
		this->BoardName = inBoardName;
	}
	/////////////////管理者用命令
	//すべてのキャッシュをディスクにフラッシュする.
	void Flush() const;
	//使用率を出力
	void Dump() const;


private:
	/**
	 * DeleteWhere:			削除
	 *
	 * @param inPoolTime	保持する期間
	 * @return void 
	 */
	void YDATCache::DeleteWhere(int inPoolTime);
	//ずっとメモリ上にしか存在している DAT は危ないのでディスクに書き込む.
	void YDATCache::Saver(int inPoolTime);

private:
	string		BoardName;
	typedef map<DATNUMBER , YDATCap*>	DATCACHE;
	DATCACHE	Cache;
	mutable RSpinLock	Lock;

	int			UnHit ;
};

#endif // !defined(AFX_YDATCACHE1_H__00131CC4_CE1A_444F_AB27_A9F915767561__INCLUDED_)
