// RGISSmallIntelligentCache.cpp: RGISSmallIntelligentCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallIntelligentCache.h"
#include "RGISSmallGlobalToBlock.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallIntelligentCache::RGISSmallIntelligentCache()
{
	for(int i = 0 ; i < GIS_SMALL_CACHE_MAX ; i ++)
	{
		this->Cache[i] = NULL;
	}
}

RGISSmallIntelligentCache::~RGISSmallIntelligentCache()
{
	Destroy();
}

//作成
void RGISSmallIntelligentCache::Create(const string & inFilename , const string & inUserDataPath) throw(RException)
{
	File.Create(inFilename , inUserDataPath);
}

//キャッシュの破棄.
void RGISSmallIntelligentCache::Destroy()
{
	for(int i = 0 ; i < GIS_SMALL_CACHE_MAX ; i ++)
	{
		delete this->Cache[i];
		this->Cache[i] = NULL;
	}
}

void RGISSmallIntelligentCache::New(RDrawObject  * ioDraw  ,const RGISSmallDrawEffect* inEffect)
{
	File.KeepHanlde();

	RGISSmallDraw draw(ioDraw , inEffect);

	//ヒット位置を計算しやすいように、 ディスプレイ座標をブロック座標に変換.
	LRECT hitBox = ConvertHitBox( inEffect->getGlobalDrawDisplay() );

	//消してもいいキャッシュ.
	bool deletbleCache[GIS_SMALL_CACHE_MAX];

	//キャッシュを調べて、現在表示しなければいけないエリアだったら表示し、
	//表示しなくてもよいエリアだったら、キャッシュから消すようにフラグを立てます.
	int i;
	for(i = 0 ; i < GIS_SMALL_CACHE_MAX ; i++)
	{
		if ( Cache[i] == NULL )
		{
			//NULLなので消してもよい.
			deletbleCache[i] = true;
		}
		else if ( isHit( &hitBox, Cache[i]->getBlock() )  )
		{
			//現在表示しなければいけないエリアなので消してはだめ.
			deletbleCache[i] = false;
			
			Cache[i]->Draw(&draw , BlockToDisplayRect(ioDraw , Cache[i]->getBlock(),  inEffect )  );

			TRACE("DrawCache:%d\n",Cache[i]->getBlock() );

		}
		else
		{
			//現在表示しなくてもいい場所なので消してよい.
			deletbleCache[i] = true;
		}
	}


	for(int y = hitBox.top; y < hitBox.bottom ; y++)
	{
		for(int x = hitBox.left; x < hitBox.right ; x++)
		{
			unsigned long key = y * NEXT_LINE_BLOCK + x;
			//表示しなきゃだめだね、、キャッシュにはいってます?
			if (! findCache( key ) )
			{
				//はいっていないのか、ぢゃあ、追加.
				RGISSmallParent * gis = new RGISSmallParent;

				bool ret = File.Load( key , gis);
				if (ret)
				{
					gis->Draw(&draw , BlockToDisplayRect(ioDraw , key,inEffect )  );

					TRACE("Draw!:%d\n",key );
				}
				else
				{
					delete gis;
					TRACE("ロードに失敗しました:%d\n",key );

					continue;
				}
				//で、空いている場所に追加するよ.
				for(int n = 0 ; n < GIS_SMALL_CACHE_MAX ; n++ )
				{
					if ( deletbleCache[n] ) break;
				}
				if (n == GIS_SMALL_CACHE_MAX)
				{
					//キャッシュが満杯だよ。。。
					TRACE("キャッシュがいっぱいになりました!!");
					//仕方ないので、先頭のところを奪う.
					n = 0;
				}
				delete Cache[n];
				Cache[n] = gis;
			}
		}
	}
}

//指定したブロックを読み込む
const RGISSmallParent* RGISSmallIntelligentCache::Load(unsigned long inBlock)
{
	return LoadPrivate(inBlock);
}

//指定したブロックを読み込む 内部向け
RGISSmallParent* RGISSmallIntelligentCache::LoadPrivate(unsigned long inBlock)
{
	//消してもいいキャッシュ.
	bool deletbleCache[GIS_SMALL_CACHE_MAX];

	//キャッシュを調べて、現在表示しなければいけないエリアだったら表示し、
	//表示しなくてもよいエリアだったら、キャッシュから消すようにフラグを立てます.
	int i;
	for(i = 0 ; i < GIS_SMALL_CACHE_MAX ; i++)
	{
		if ( Cache[i] == NULL )
		{
			//NULLなので消してもよい.
			deletbleCache[i] = true;
		}
		else if ( Cache[i]->getBlock() == inBlock )
		{
			//現在表示しなければいけないエリアなので消してはだめ.
			deletbleCache[i] = false;

			TRACE("DrawCache:%d\n",Cache[i]->getBlock() );
			return Cache[i];
		}
		else
		{
			//現在表示しなくてもいい場所なので消してよい.
			deletbleCache[i] = true;
		}
	}

	//はいっていないのか、ぢゃあ、追加.
	RGISSmallParent * gis = new RGISSmallParent;

	bool ret = File.Load( inBlock , gis);
	if (ret)
	{
		TRACE("Draw!:%d\n",inBlock );
	}
	else
	{
		delete gis;
		TRACE("ロードに失敗しました:%d\n",inBlock );

		return NULL;
	}
	//で、空いている場所に追加するよ.
	for(int n = 0 ; n < GIS_SMALL_CACHE_MAX ; n++ )
	{
		if ( deletbleCache[n] ) break;
	}
	if (n == GIS_SMALL_CACHE_MAX)
	{
		//キャッシュが満杯だよ。。。
		TRACE("キャッシュがいっぱいになりました!!");
		//仕方ないので、先頭のところを奪う.
		n = 0;
	}
	delete Cache[n];
	Cache[n] = gis;

	return gis;
}

//指定されたブロックをキャッシュとは関係なく読み込み(キャッシュを参照することはしない)
bool RGISSmallIntelligentCache::LoadEx(unsigned long inBlock , RGISSmallParentImp* ioParentImp)
{
	bool ret = File.Load( inBlock , ioParentImp);
	if (ret)
	{
		TRACE("LoadEx!:%d\n",inBlock );
	}
	return ret;
}




//キャッシュに指定したキーがあるかどうか検索.
bool RGISSmallIntelligentCache::findCache(unsigned long inKey)
{
	for(int i = 0 ; i < GIS_SMALL_CACHE_MAX ; i++ )
	{
		if ( Cache[i] != NULL)
		{
			if ( Cache[i]->getBlock() == inKey )
			{
				return true;	//みっけ.
			}
		}
	}
	//キャッシュにはないです.
	return false;
}



//この範囲に含まれますか?
bool RGISSmallIntelligentCache::isHit(const LRECT * inHitBox , unsigned long inBlock) const
{
	LPOINT xy;
	xy.y = inBlock / NEXT_LINE_BLOCK;
	xy.x = inBlock - (xy.y * NEXT_LINE_BLOCK);

	return (inHitBox->top		<= xy.y && inHitBox->bottom		> xy.y &&
			inHitBox->left		<= xy.x && inHitBox->right		> xy.x);
}

//Hit検索に使いやすいように、あらかじめ計算しておきます。
LRECT RGISSmallIntelligentCache::ConvertHitBox(const LRECT * inDisplay) const
{
	RGISSmallGlobalToBlock g2b;
	g2b.Create(inDisplay->left	, inDisplay->top);
	unsigned long a	= g2b.getBlock();

	g2b.Create(inDisplay->right	, inDisplay->bottom);
	unsigned long b	= g2b.getBlock();

	LRECT ret;

	ret.top		= (a / NEXT_LINE_BLOCK);
	ret.left	= a - (ret.top * NEXT_LINE_BLOCK);
	ret.bottom	= (b / NEXT_LINE_BLOCK) ;
	ret.right	= b - (ret.bottom * NEXT_LINE_BLOCK);

	ret.bottom	++;
	ret.right	++;

	return ret;
}

void RGISSmallIntelligentCache::Find(RGISSmallHeaderImp* ioFindHeader)
{
	File.LoadHeader(ioFindHeader);
}

//ブロックからディスプレイ座標に変換.
RECT RGISSmallIntelligentCache::BlockToDisplayRect(const RDrawObject *inDraw ,unsigned long inBlock , const RGISSmallDrawEffect* inEffect) const
{
	//ブロックから絶対座標へ.
	LPOINT xy;
	xy.y = inBlock / NEXT_LINE_BLOCK;
	xy.x = inBlock - (xy.y * NEXT_LINE_BLOCK);

	LRECT rc;
	rc.top = xy.y * 65536;
	rc.left = xy.x * 65536;
	rc.bottom = rc.top + 65536;
	rc.right = rc.left + 65536;

	//表示する点の座標に変換 (スケール変換)
	RECT ret = inEffect->ConvertShowDisplay(&rc);

	if ( ret.top < 0 ) ret.top = 0;
	if ( ret.left < 0 ) ret.left = 0;
	if ( ret.bottom > inDraw->getHeight() ) ret.bottom = inDraw->getHeight();
	if ( ret.right > inDraw->getWidth() ) ret.right = inDraw->getWidth();

	return ret;
}


//ユーザーデータの編集.
void RGISSmallIntelligentCache::UserDataModify(const LPOINT * inPoint , const int * inDeleteNumber , int inDeleteCount ,
											   const char * inData , int inSize,int inCount ) throw(RException)
{
	//ブロックの特定
	RGISSmallGlobalToBlock g2b;
	g2b.Create(inPoint->x , inPoint->y);
	unsigned long block = g2b.getBlock();
	//ブロックのロード.
	RGISSmallParent * parent = LoadPrivate( block );
	if (parent == NULL) throw RNotFoundException("ブロック %d をロードできません" , block);
	//書き込み
	File.UserDataModify(block, parent , inDeleteNumber , inDeleteCount , inData , inSize , inCount );
}


//ユーザーデータの検索.
bool RGISSmallIntelligentCache::UserDataFind(const LRECT * inSearchArea , RGISSmallUserDataRead * outUserData ) throw(RException)
{
	//ブロックの特定
	RGISSmallGlobalToBlock g2b;
	g2b.Create(inSearchArea->left , inSearchArea->top);
	unsigned long block = g2b.getBlock();
	//ブロックのロード.
	const RGISSmallParent * parent = Load( block );
	if (parent == NULL) throw RNotFoundException("ブロック %d をロードできません" , block);
	//検索
	int distance = parent->FindUserData(outUserData , INT_MAX , inSearchArea);

	//ブロックの特定
	g2b.Create(inSearchArea->left , inSearchArea->bottom);
	unsigned long block2 = g2b.getBlock();
	if (block != block2)
	{
		//ブロックのロード.
		parent = Load( block );
		if (parent == NULL) throw RNotFoundException("ブロック %d をロードできません" , block);
		//検索
		distance = parent->FindUserData(outUserData, distance , inSearchArea);
	}

	//ブロックの特定
	g2b.Create(inSearchArea->right , inSearchArea->top);
	block2 = g2b.getBlock();
	if (block != block2)
	{
		//ブロックのロード.
		parent = Load( block );
		if (parent == NULL) throw RNotFoundException("ブロック %d をロードできません" , block);
		//検索
		distance = parent->FindUserData(outUserData, distance , inSearchArea);
	}

	//ブロックの特定
	g2b.Create(inSearchArea->right , inSearchArea->bottom);
	block2 = g2b.getBlock();
	if (block != block2)
	{
		//ブロックのロード.
		parent = Load( block );
		if (parent == NULL) throw RNotFoundException("ブロック %d をロードできません" , block);
		//検索
		distance = parent->FindUserData(outUserData, distance , inSearchArea);
	}

	return (distance != INT_MAX);
}

