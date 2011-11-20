// RGISSmallDouro.cpp: RGISSmallDouro クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallDouro.h"
#include "RStdioMemory.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallDouro::RGISSmallDouro()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallDouro::~RGISSmallDouro()
{
}



void RGISSmallDouro::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}

//線/点の描画
void RGISSmallDouro::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallDouroDataOptimizMainData mainD;
	SmallDouroDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	ParticularLevel level = inDraw->getLevel();
	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.FI) , selectColor(mainD.SB) );
			break;
		case LEVEL_NOT_KOUKYOU:
		case LEVEL_NOT_LOWLEVEL:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.FI) , selectColor(mainD.SB) );
			break;
		case LEVEL_NOT_MIDDLELEVEL:
			//一般道と高速道路
			if (0x15 == mainD.SB || 0x16 == mainD.SB)
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.FI) , selectColor(mainD.SB) );
			break;
		}
/*
		int node = p->Node.getCount();
		for (int n = 0 ; n < node ; n ++)
		{
			inDraw->Point( p->CV.getPoint( n ) , 2 , RGB(0,255,0) );
		}
*/
	}
}

//文字列の描画
void RGISSmallDouro::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	SmallDouroDataOptimizMainData mainD;
	SmallDouroDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RGISSmallDupKill dupKill;
	RStdioMemory memory;
	memory.Open(Data );

	ParticularLevel level = inDraw->getLevel();
	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->TextOut(inBlockStart, d.CV ,d.CVCount, inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_KOUKYOU:
		case LEVEL_NOT_LOWLEVEL:
//			inDraw->TextOut(inBlockStart, d.CV ,d.CVCount, inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_MIDDLELEVEL:
			//一般道と高速道路
//			if (0x15 == mainD.SB || 0x16 == mainD.SB)
//				inDraw->TextOut(inBlockStart, d.CV ,d.CVCount, inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		}
	}
}

/*
//ナビゲーションをする際の情報の収集.
void RGISSmallDouro::MakPathFinder(const LPOINT *inBlockStart , SmallPathfinderData * outPathFinder) const
{
	SmallDouroDataOptimizMainData mainD;
	SmallDouroDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	outPathFinder->DataCount = this->DataCount;
	outPathFinder->Data = new SmallPathfinderData::SmallPathfinderDataOne[this->DataCount];
	//名前が長くなるのでポインタにして短くする.
	SmallPathfinderData::SmallPathfinderDataOne* one = outPathFinder->Data ;

	int n ;
	SmallPathfinderData::SmallPathfinderDataOne::_Node::_Connect connect[255]; 

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}
		one->SB = mainD.SB;
		one->FI = mainD.FI;

		//CVの作成.
		one->CVCount = d.CVCount;
		one->CV = new SPOINT[d.CVCount];

		RStdioMemory cvMemory;
		cvMemory.Open(d.CV );
		one->CV[0].x = cvMemory.readGetUS();
		one->CV[0].y = cvMemory.readGetUS();
		for(n = 1 ; n < d.CVCount ; n++)
		{
			one->CV[n].x = one->CV[n - 1].x + cvMemory.readGetC();
			one->CV[n].y = one->CV[n - 1].y + cvMemory.readGetC();
		}

		//Nodeの作成.
		one->NodeCount = d.NodeCount;
		one->Node = new SmallPathfinderData::SmallPathfinderDataOne::_Node[d.NodeCount];

		RStdioMemory nodeMemory;
		nodeMemory.Open(d.Node );
		for(n = 0 ; n < d.NodeCount ; n++)
		{
			SmallPathfinderData::SmallPathfinderDataOne::_Node * node = &one->Node[n];
			node->Pos = nodeMemory.readGetC();
			//接続情報の取得.
			int connectCount = FindConnect(connect , &one->CV[node->Pos] , i);
			node->ConnectCount = connectCount;
			node->Connect = new SmallPathfinderData::SmallPathfinderDataOne::_Node::_Connect[connectCount];

			memcpy(node->Connect , connect , sizeof(SmallPathfinderData::SmallPathfinderDataOne::_Node::_Connect)* connectCount);
		}
	}
}

//接続情報の取得.
int RGISSmallDouro::FindConnect(SmallPathfinderData::SmallPathfinderDataOne::_Node::_Connect * outConnect , SPOINT * inPos , int inMyDouroNumber) const
{
	SmallDouroDataOptimizMainData mainD;
	SmallDouroDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	SPOINT pos[256];
	unsigned char oneNode;

	int nodeCount = 0;
	int n;

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		//自分自身は無視.
		if (i == inMyDouroNumber) continue;

		//CVの解析.
		RStdioMemory cvMemory;
		cvMemory.Open(d.CV );
		pos[0].x = cvMemory.readGetUS();
		pos[0].y = cvMemory.readGetUS();
		for(n = 1 ; n < d.CVCount ; n++)
		{
			pos[n].x = pos[n - 1].x + cvMemory.readGetC();
			pos[n].y = pos[n - 1].y + cvMemory.readGetC();

			ASSERT(n <= 255);
		}

		//Nodeの解析.
		RStdioMemory nodeMemory;
		nodeMemory.Open(d.Node );
		for(n = 0 ; n < d.NodeCount ; n++)
		{
			oneNode = nodeMemory.readGetC();
			if (pos[oneNode].x == inPos->x &&  pos[oneNode].y == inPos->y)
			{
				outConnect[nodeCount].AtherMap = 0;
				outConnect[nodeCount].Number = i;
				outConnect[nodeCount].Pos = n;
				nodeCount++;

				ASSERT(nodeCount <= 255);
			}
		}
	}
	return nodeCount;
}
*/
COLORREF RGISSmallDouro::selectColor(unsigned char inShubetsu) const
{
	/*
	if (0x13 == inShubetsu)				return COLOR_3;		//庭園路
	if (0x14 == inShubetsu)				return COLOR_3;		//石段
	if (0x15 == inShubetsu)				return COLOR_3;		//一般道
	if (0x16 == inShubetsu)				return COLOR_4;		//高速道路

	ASSERT(0);
	return 0;
	*/

	return BBananaConfig::getInstance()->getColor(inShubetsu - 0x13 + COLOR_TEIENRO);
}

COLORREF RGISSmallDouro::selectSize(unsigned char inShubetsu) const
{
	if (0x17 == inShubetsu)				return 1;		//1.5m未満
	if (0x18 == inShubetsu)				return 2;		//1.5m以上3.0m未満
	if (0x19 == inShubetsu)				return 3;		//3.0m以上5.5m未満
	if (0x1a == inShubetsu)				return 4;		//5.5m以上13.0m未満
	if (0x1b == inShubetsu)				return 5;		//13.0m以上
	if (0x1e == inShubetsu)				return 6;		//真幅道路等

	ASSERT(0);
	return 0;
}

//データの読み込み.
void RGISSmallDouro::readDataMain(RStdioMemory * ioFile, SmallDouroDataOptimizMainData * outData) const
{
	outData->SB = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->FI = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//名前を求める.
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}

//データの読み込み.
void RGISSmallDouro::readDataUniq(RStdioMemory * ioFile, SmallDouroDataOptimizUniqData * outData) const
{
	//ノード(交差点)数の取得
	outData->NodeCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//ノードデータの読み飛ばし
	outData->Node = ioFile->ptr();
	ioFile->fseek(  outData->NodeCount *  sizeof(unsigned char) , SEEK_CUR );
	ASSERT(ioFile->ftell() <= DataSize);

	//CVデータ(点)の取得
	outData->CVCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//点データの読み飛ばし
	outData->CV = ioFile->ptr();
	ioFile->fseek(  sizeof(unsigned short) * 2, SEEK_CUR );
	ioFile->fseek(  (outData->CVCount - 1) *  sizeof(unsigned char) * 2, SEEK_CUR );
	ASSERT(ioFile->ftell() <= DataSize);
}
