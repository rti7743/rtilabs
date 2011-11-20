// YDATCache1.cpp: YDATCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YDATCache1.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YDATCache::YDATCache()
{
	BoardName = "";
	UnHit = 0;
}

YDATCache::~YDATCache()
{
	//すべて保存し、解放する.
	for(DATCACHE::iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		try
		{
			ydat->Save(this->BoardName);
		}
		catch(RException e)
		{
			printf("(%s:%d)のキャッシュをフラッシュしているときに例外を感知:%s\n",
				this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
		}
		delete ydat;
	}
}

YDAT* YDATCache::getDAT(DATNUMBER inDATNumber) throw(RIOCannotOpenException)
{
	ASSERT(inDATNumber != 0);

	YDATCap* ydat;
	{
		RAutoReadSpinLock al(&Lock);
		DATCACHE::iterator i = this->Cache.find(inDATNumber);
		if ( i != this->Cache.end() )
		{
			ydat = (*i).second;
			ydat->Use();
			return ydat;
		}
	}
	{
		RAutoWriteSpinLock al(&Lock);

		try
		{
			ydat = new YDATCap( inDATNumber);
			ydat->Load(this->BoardName);
		}
		catch(RException e)
		{	//読み込みに失敗.
			delete ydat;
			throw;
		}
		Cache[inDATNumber] = ydat;

		ydat->Use();

		this->UnHit ++;

		return ydat;
	}
}


YDAT* YDATCache::getNewDAT(DATNUMBER inDATNumber) throw (RAlreadyException)
{
	RAutoWriteSpinLock al(&Lock);

//	if ( Cache.end() != Cache.find(inDATNumber) ) throw RAlreadyException("すでに DATは存在しています");

	YDATCap* ydat = new YDATCap(inDATNumber);
	ydat->Use();
	Cache[inDATNumber] = ydat;

	return ydat;
}

void YDATCache::Release(YDAT* inDAT)
{
	RAutoWriteSpinLock al(&Lock);
	//危険だけどまぁいいんぢゃない?

	//参照カウントの破棄.
	((YDATCap*)inDAT)->UnUse();
}

/**
 * Collecter:		ガページコレクト
 *
 * @return void 
 */
void YDATCache::Collecter()
{
	RAutoWriteSpinLock al(&Lock);

	/*ChangeLog.txt:2003/01/23	ガページコレクトする時間の指定を間違っていたので修正 60 * 1 = 1分 のところを 1000 * 1 = 1分 としていた(w
	*/
	//安全圏?
	if ( Cache.size() < 20 )
	{
		/*ChangeLog.txt:2003/01/23	クラッシュによりデータを失うのが怖いので 30分アクセスされていないファイルは自動的にディスクに書き戻すように修正
		*/
		//ガページコレクトするほどデータはたまっていません。
		//しかし、データ保護のため
		//30分アクセスしていないデータはディスクに自動的に書き戻します。 キャッシュの開放はしません。
		Saver( 60 * 30);
	}
	else if ( Cache.size() < 30 )
	{
		//30分が経過しているデータの削除
		DeleteWhere( 60*30);
	}
	else if ( Cache.size() < 40 )
	{
		//20分が経過しているデータの削除
		DeleteWhere(60*20);
	}
	else if ( Cache.size() < 50 )
	{
		//5分が経過しているデータの削除
		DeleteWhere( 60*5);
	}
	else
	{
		//0分が経過しているデータの削除
		DeleteWhere(0);
	}
}

/**
 * DeleteWhere:			削除
 *
 * @param inPoolTime	保持する期間
 * @return void 
 */
void YDATCache::DeleteWhere(int inPoolTime)
{
	DWORD now = time(NULL);
	for(DATCACHE::iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		if ( (!ydat->isUse()) && ydat->DifferenceTime(now) >= inPoolTime)
		{
			try
			{
				printf("YoujyoGC::(%s:%d)のGC発動!\n",this->BoardName.c_str() , ydat->getDatNumber() );

				ydat->Save( this->BoardName );
				delete ydat;	//あぼーん
				ydat = NULL;
				i = Cache.erase(i);
			}
			catch(RException e)
			{
				if (ydat)
				{
					printf("YoujyoGC::(%s:%d)のGC処理している最中に例外を感知:%s\n",
						this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
				}
				else
				{
					printf("YoujyoGC::(%s:不明)のGC処理している最中に例外を感知:%s\n",
						this->BoardName.c_str() , e.getMessage() );
				}
			}
		}
	}
}

//ずっとメモリ上にしか存在している DAT は危ないのでディスクに書き込む.
void YDATCache::Saver(int inPoolTime)
{
	DWORD now = time(NULL);
	for(DATCACHE::iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		if ( (!ydat->isUse()) && ydat->DifferenceOnMemoryTime(now) >= inPoolTime)
		{
			try
			{
				printf("YoujyoGC::(%s:%d)はしばらくアクセスされていないのでフラッシュします!\n",this->BoardName.c_str() , ydat->getDatNumber() );
				ydat->Save(this->BoardName);
				ydat->WriteBack();
			}
			catch(RException e)
			{
				printf("(%s:%d)のキャッシュをフラッシュしているときに例外を感知:%s\n",
					this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
			}
		}
	}
}


//すべてのキャッシュをディスクにフラッシュする.
void YDATCache::Flush() const
{
	RAutoWriteSpinLock al(&Lock);

	for(DATCACHE::const_iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		try
		{
			ydat->Save(this->BoardName);
		}
		catch(RException e)
		{
			printf("(%s:%d)のキャッシュをフラッシュしているときに例外を感知:%s\n",
				this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
		}
	}
}


//使用率を出力
void YDATCache::Dump() const
{
	RAutoWriteSpinLock al(&Lock);

	puts("========================== Cache Dump ============================");
	printf("保持キャッシュ %d\r\n" , Cache.size() );
	printf("非ヒット数 %d\r\n" , this->UnHit );
	puts("========================== ========= ============================");
}

