// RGISSmallPathFinder.cpp: RGISSmallPathFinder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallPathFinder.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
/*

RGISSmallPathFinder::RGISSmallPathFinder()
{
	this->Data = new RouteTestData[RouteTestAreaMax];
	memset(this->Data , 0 , sizeof(RouteTestData) * RouteTestAreaMax);
}

RGISSmallPathFinder::~RGISSmallPathFinder()
{
	delete	[]	this->Data;
}

void RGISSmallPathFinder::Find(const UPOINT * inStart , const UPOINT * inGoal , RGISSmallIntelligentCache* ioCache) throw(RException) 
{
	this->Cache.Create(ioCache);

	int startBlock = RGISSmallType::selectBlock(inPoint);
	int endBlock = RGISSmallType::selectBlock(inGoal);

	const RGISSmallParentDouroOnly* gis ;

	//最寄のノードを検索.
	this->Cache->Load( startBlock , gis);
	this->UseCount = 0;
	UPOINT nodeStart = FindNode(gis, inStart);

	//最寄のノードを検索.
	gis = this->Cache->Load( startBlock );
	UPOINT nodeGoal = FindNode(inGoal);

}


UPOINT RGISSmallPathFinder::FindNode(const RGISSmallParent* inGis , const UPOINT * inPoint)
{
	const RGISSmallDouro* douro = inGis->getDouro();


	int dnumber ;
	unsigned char cvnumber;
	UPOINT*	mostNear = NULL;
	int		distance = MAX_INT;

	for(int i = 0 ; i < douro->getCount() ; i++ )
	{
		const SmallDouroData* data = douro->getPoint(i);
		RGISSmallType* cv = data->CV;

		for(int n = 0; n < cv->getCount ; n++ )
		{
			const UPOINT * p = cv->getPoint(n);
			int diff = Distance(p->x > inStart->x ? p->x - inStart->x : inStart->x - p->x,p->y > inPoint->y ? p->y - inPoint->y : inPoint->y - p->y);

			if ( distance > diff )
			{
				distance = diff;
				mostNear = p;
				dnumber = i;
				cvnumber = n;
			}
		}
	}

	ASSERT(mostNear != NULL);
	ASSERT(UseCount == 0);

	{
		//ここから始まります。

		//はじまった cv の戦後のノードを記録します。
		const SmallDouroData* data = douro->getPoint(dnumber);
		RGISSmallType* cv = data->CV;
		RGISSmallNode* node = data->Node;

		this->UseCount = 0;

		int n;
		for(n = cvnumber ; n < cv->getCount() ; n++ )
		{
			if (node->Find(n) != node->getCount() ) break;
		} 
		if (n != node->getCount() )
		{
			RouteTestData* data = this->Data[UseCount++] ;
			data->DNumber = dnumber;
			data->xy = *(cv->getPoint(n));
			data->Active = true;
		}

		for(n = cvnumber - 1; n >= 0 ; n-- )
		{
			if (node->Find(n) != node->getCount() ) break;
		} 
		if (n < 0 )
		{
			RouteTestData* data = this->Data[UseCount++] ;
			data->DNumber = dnumber;
			data->xy = *(cv->getPoint(n));
			data->Active = true;
		}
	}
}

UPOINT RGISSmallPathFinder::FindRoute(const UPOINT * inGloalNodeXY , int GloalCount)
{
	RGISSmallParentDouroOnly*  gis;

	while(1)
	{
		//道路検索....
		//もし、 先読み回数が LookAheadDependMax になるか、
		//途中でワークメモリが足りなくなった RIORouteTestAreaMaxException がでれば探査を終わります。
		for(int i = 0; i < LookAheadDependMax ; i++)
		{
			if ( FindRouteUnit() != OK) break;
		}

		//次の世代の評価.
		AppraiseNextGenesion():
	}
}

//次の世代の評価.
void RGISSmallPathFinder::AppraiseNextGenesion()
{
	//探索した結果を評価して、
	//ゴールまで 4番まで取得します.
	struct Ranking
	{
		int		no;					//場所
		int		distance;			//ゴールまでの距離
	};
	const int SurvivalProspectCount = 4;
	Ranking rank[SurvivalProspectCount];
	for(int i = 0; ; i < SurvivalProspectCount ; i++)
	{
		rank[i].no = 0;
		rank[i].distance = INT_MAX;
	}

	int i;
	for(i = 0; i < this->UseCount ; i++)
	{
		RouteTestData* rdata = &Data[i];
		//袋小路につまったやつは没.
		if (! rdata->Active )	continue;

		//街頭ブロックの道路データの読み込み.
		if ( ! this->Cache->Load( rdata->Block , gis) )
		{
			//読み込み失敗.
			throw RException(EXCEPTIONTRACE + "キャッシュに記録されていたデータが読み込みません!");
		}
		//このノードの点を求めます.
		RGISSmallDouroNode* douro = gis->getDouro();
		SmallDouroNodeData * ddata = douro->getPoint( rdata->DNumber );
		const RGISSmallNode* node = &ddata->Node;

		//道を進みます。
		//交差点があれば、検索データに追加していきます。
		//この道の終端まで行ったら、おしまいです。
		int nodePoint = rdata->nodePoint;
		//この交差点の位置を取得します.
		UPOINT * p = ddata->CV.getPoint( node->getPoint(nodePoint) );

		int diff = Distance(p->x > inGloalNodeXY->x ? p->x - inGloalNodeXY->x : inGloalNodeXY->x - p->x,p->y > inGloalNodeXY->y ? p->y - inGloalNodeXY->y : inGloalNodeXY->y - p->y);

		for(int n = 0; n < SurvivalProspectCount ; n++)
		{
			if ( rank[n].distance <= diff) continue;

			//こいつをここに挿入する.
			memmove(&rank[n + 1] , &rank[n] , sizeof(Ranking) * ( SurvivalProspectCount - (n + 1)) );
			rank[n].no = i;
			rank[n].distance = diff;
		}
	}
	//で、こいつらを除き全員あぼーん.

	//まずバックアップに退避して.
	RouteTestData backup[SurvivalProspectCount];
	for(i = 0 ; i < SurvivalProspectCount ; i++)
	{
		backup[i] = Data[ rank[i].no ];
	}
	//初期化.
	Clear();
	//それを先頭に詰めて書き直す.
	for(i = 0 ; i < SurvivalProspectCount ; i++)
	{
		Data[i] = backup[i];
		Data[i].Befor = -1;	//先方のデータをクリア.
	}
	this->UseCount = SurvivalProspectCount;
}

//初期化.
void RGISSmallPathFinder::Clear()
{
	memset(this->Data , 0 , sizeof(RouteTestData) * RouteTestAreaMax);
}

SearchResult RGISSmallPathFinder::FindRouteUnit(const UPOINT * inGloalNodeXY , int GloalCount)
{
	RGISSmallParentDouroOnly*  gis;
	int count = this->UserCount;
	for(int i = 0; i < count ; i++)
	{
		RouteTestData* rdata = &Data[i]
		//袋小路につまったやつは没.
		if (! rdata->Active )	continue;

		//街頭ブロックの道路データの読み込み.
		if ( ! this->Cache->Load( rdata->Block , gis) )
		{
			//読み込み失敗.
			throw RException(EXCEPTIONTRACE + "キャッシュに記録されていたデータが読み込みません!");
		}
		//このノードの点を求めます.
		RGISSmallDouroNode* douro = gis->getDouro();
		SmallDouroNodeData * ddata = douro->getPoint( rdata->DNumber );
		const RGISSmallNode* node = &ddata->Node;

		//道を進みます。
		//交差点があれば、検索データに追加していきます。
		//この道の終端まで行ったら、おしまいです。
		int nodePoint = rdata->nodePoint;
		//この交差点の位置を取得します.
		UPOINT * p = ddata->CV.getPoint( node->getPoint(nodePoint) );

		//実はゴールだったりします?
		if ( isGoal(p , inGloalNodeXY , GloalCount) )
		{
			return Goal;
		}

		//この交差点に所属しているノードを新たに検索データに追加します。
		//つまり、この交差点を曲がった場合はどうなるかを処理するわけです。
		if ( ! OtherCross(douro , rdata　, p ) )
		{
			return NotEnoughMemory;
		}

		//カウンターアップ / ダウン.
		if (rdata->Up)
		{
			nodePoint ++;
			//終端まで行ったのでおしまい.
			if (nodePoint >= node->getCount() ) rdata->Active = false;
		}
		else
		{
			nodePoint --;
			//終端まで行ったのでおしまい.
			if (nodePoint < 0 ) rdata->Active = false;
		}
	}
}

bool RGISSmallPathFinder::OtherCross(RGISSmallDouroNode* ioDouro ,const RouteTestData* inRdata,const UPOINT * inPoint)
{
	//その点を共有しているノードを見つけます.
	const RGISSmallType* cv;
	const RGISSmallNode* node;
	for(int n = 0 ; n < ioDouro->getCount() ; n++)
	{
		SmallDouroNodeData * ddata = ioDouro->getPoint(n);

		//この道路を使っている人がいないかチェック.
		if ( ddata->AlreadyUse ) continue;

		//使用中フラグを立てる.
		ddata->AlreadyUse = true;

		cv = &ddata->CV;
		node = &ddata->Node;
		for(int c = 0 ; c < cv->getCount() ; c ++)
		{
			UPOINT * p = cv->getPoint(c);
			if ( !(inPoint->x == p->x && inPoint->y == p->y) ) continue;

			//共有しているかもしれないので、ノードでそのノードを外部に公開しているかどうか調べます.
			int nodePoint = node->Find(c)
			if ( ! nodePoint == node->getCount() ) continue;

			//交差点です.
			//up = false		up = true
			//cv1   cv2  cv3  cv4
			// ------------------- ターゲット
			//           |
			//           |
			//           |元の道路
			//           |

			//終端以外は、右に増える
			if (c + 1 != cv->getCount())
			{
				RouteTestData* data = this->Data[UseCount++] ;
				data->Block = inRdata->Block;
				data->DNumber = inRdata->DNumber;
				data->nodePoint nodePoint;
				data->Active = true;
				data->Up = true;
				data->Befor = inRdata->Befor;
			}
			//先頭以外は、左に減る
			if (c == 0)
			{
				RouteTestData* data = this->Data[UseCount++] ;
				data->Block = inRdata->Block;
				data->DNumber = inRdata->DNumber;
				data->nodePoint nodePoint;
				data->Active = true;
				data->Up = false;
				data->Befor = inRdata->Befor;
			}
			//検索用のメモリをすべて使い切った?.
			if (UseCount >= RouteTestAreaMax)
			{
				return false;
			}
		}
	}
	return true;
}

//距離を求める.
int RGISSmallPathFinder::Distance(unsigned int inA ,unsigned  int inB) const
{
	return sqrt(inA * inA + inB * inB);
}
*/