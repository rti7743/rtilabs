// RGISSmallPathFinder.h: RGISSmallPathFinder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLPATHFINDER_H__20EB24EB_2DDE_4ED4_806F_5BC79E38E1A5__INCLUDED_)
#define AFX_RGISSMALLPATHFINDER_H__20EB24EB_2DDE_4ED4_806F_5BC79E38E1A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallIntelligentCache.h"
/*
const int LookAheadDependMax	=	50;
const int RouteTestAreaMax		=	80000;		//960000Bytes -> 約 937.5k

class RGISSmallPathFinder  
{
	enum SearchResult
	{
		OK ,
		NotEnoughMemory ,
		Goal ,
	};

	struct RouteTestData
	{
		unsigned long	Block;						//ブロック番号
		unsigned long	DNumber;					//道路番号
		bool	Active;								//有効な検索ポイントであるか?
		bool	Up;
		unsigned char		nodePoint;				//ノードの場所
		int		Befor;								//一つ前の RouteTestData の場所
	};

	typedef vector<UPOINT>	RootRode;

	//先行宣言.
	class RGISSmallAccumulationCahe;

	//経路は道路以外作成しないので道路だけパースで切ればよい.
	class RGISSmallParentDouroOnly : public RGISSmallParentImp
	{
	private:
		//このクラスの作成、破棄は、 RGISSmallAccumulationCahe 以外は行っては成らない!
		friend class RGISSmallAccumulationCahe;
		RGISSmallParentDouroOnly()
		{
			UseCount = 1;
		}
		~RGISSmallParentDouroOnly()
		{
		}

		//参照回数を変更する命令は RGISSmallAccumulationCahe 以外は行っては成らない!
		void Use()
		{
			UseCount ++;
		}
		bool UnUse()
		{
			UseCount --;
			return (UseCount <= 0) ;
		}
	public:
		//シンボルの解決
		virtual void Parse(unsigned long inBlock ,unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,char* ioBuffer) throw(RException)
		{
			ASSERT(Block == 0 || Block == inBlock);
			Block = inBlock;

			if (inSimbol == 'D')
			{
				Douro.Create(inBlock , inDataSize ,inDataCount ,ioBuffer);
			}
		}

		RGISSmallDouroNode* getDouro() 
		{
			return &Douro;
		}

		//こいつか所属するグループを求める.
		unsigned long getBlock() const
		{
			return Block;
		}

	private:
		int					UseCount;
		RGISSmallDouroNode	Douro;
		unsigned long		Block;
	};

	//こちらが消せというまで消さないことを保証する積み立て式のキャッシュ.
	class RGISSmallAccumulationCahe
	{
	public:
		typedef vector<RGISSmallParentDouroOnly*>	DouroCache;

		RGISSmallAccumulationCahe()
		{
		}
		virtual ~RGISSmallAccumulationCahe()
		{
			DouroCache::iterator i = Cache.begin();
			DouroCache::iterator e = Cache.end();

			for( ; i != e ; i++)
			{
				delete (*i);
			}
			Cache.clear();
		}

		//ファイルの読み込みは RGISSmallIntelligentCache に一任してあるため.
		void Create(RGISSmallIntelligentCache* ioCache)
		{
			ParentCache = ioCache;
		}

		//読み込み
		RGISSmallParentDouroOnly* Load(unsigned long inBlock)
		{
			//キャッシュにある?
			DouroCache::iterator i = Cache.begin();
			DouroCache::iterator e = Cache.end();
			for( ; i != e ; i++)
			{
				if ( (*i)->getBlock() == inBlock )
				{
					(*i)->Use();
					return *i;
				}
			}

			//ないから作成.
			RGISSmallParentDouroOnly* ret = new RGISSmallParentDouroOnly;
			ParentCache->LoadEx(inBlock , ret);

			Cache.push_back(ret);
			return ret;
		}

		//Loadで確保した道路が、不要に成ったら呼ぶこと
		void UnLoad(RGISSmallParentDouroOnly* ioDouro)
		{
			if ( ioDouro->UnUse() )
			{
				DouroCache::iterator i = find(Cache.begin() , Cache.end() , ioDouro);
				if (i == Cache.end())
				{
					ASSERT(0);
					return ;
				}
				Cache.erase(i);
				delete *i;
			}
		}
	private:
		RGISSmallIntelligentCache*	ParentCache;

		DouroCache					Cache;
	};


public:
	RGISSmallPathFinder();
	virtual ~RGISSmallPathFinder();

	void Find(const UPOINT * inStart , const UPOINT * inGoal , RGISSmallIntelligentCache* ioCache) throw(RException) ;
private:
	void FindNode(const UPOINT * inPoint);
	UPOINT RGISSmallPathFinder::FindRoute(const UPOINT * inGloalNodeXY);

private:
	RGISSmallAccumulationCahe	Cache;

	int					UseCount;
	RouteTestData	*	Data;	//道路探索用のワークエリア	
								//スタック上に確保するとメモリが足りなくなるといけないため new で確保します.
};
*/
#endif // !defined(AFX_RGISSMALLPATHFINDER_H__20EB24EB_2DDE_4ED4_806F_5BC79E38E1A5__INCLUDED_)
