// RCacheFS.cpp: RCacheFS クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RCacheFS.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RCacheFS::RCacheFS()
{

}

RCacheFS::~RCacheFS()
{

}

RCacheFS* RCacheFS::getInstance()
{
	static RCacheFS cfs;
	return &cfs;
}


//キャッシュの検索.
//見つからなかったら NULL
Rfiles* RCacheFS::Search(__RFILETYPE inRfileType , string inPath)
{
	for(RFILESCACHE::iterator it = Cache.begin(); it != Cache.end() ; ++it)
	{
		if ( ((*it)->getRfileType() == inRfileType) && ((*it)->getPath()) == inPath )
		{
			(*it)->Inc();
			return *it;
		}
	}
	return NULL;
}

//キャッシュの格納.
void RCacheFS::Append(Rfiles* inRfiles)
{
	ASSERT(inRfiles != NULL);
	Cache.push_back(inRfiles);
}

//キャッシュのクリア.
void RCacheFS::Remove(Rfiles* ioRfiles)
{
	for(RFILESCACHE::iterator it = Cache.begin() ; it != Cache.end() ; ++it)
	{
		if ( *it == ioRfiles) 
		{
			if ( ioRfiles->Dec() )
			{
				Cache.erase(it);
			}
			return;
		}
	}
}

