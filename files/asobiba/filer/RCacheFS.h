// RCacheFS.h: RCacheFS クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCACHEFS_H__16349AF9_72A8_4AE2_AEAF_F6C4520AE304__INCLUDED_)
#define AFX_RCACHEFS_H__16349AF9_72A8_4AE2_AEAF_F6C4520AE304__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "Rfiles.h"


class RCacheFS  
{
private:
	RCacheFS();
public:
	virtual ~RCacheFS();

	static RCacheFS* getInstance();

	//キャッシュの検索.
	//見つからなかったら NULL
	Rfiles* Search(__RFILETYPE inRfileType , string inPath);

	//キャッシュの格納.
	void Append(Rfiles* inRfiles);

	//キャッシュのクリア.
	void Remove(Rfiles* inRfiles);



private:
	//キャッシュ
	//重くなるようだったら map にしてちょ.
	typedef deque<Rfiles*>	RFILESCACHE;
	RFILESCACHE Cache;
};

#endif // !defined(AFX_RCACHEFS_H__16349AF9_72A8_4AE2_AEAF_F6C4520AE304__INCLUDED_)
