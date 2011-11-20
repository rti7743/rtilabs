// RGISSmallNavigation.cpp: RGISSmallNavigation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
/*
#include "stdafx.h"
#include "RGISSmallNavigation.h"
#include "RGISSmallDouro.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallNavigation::RGISSmallNavigation()
{

}

RGISSmallNavigation::~RGISSmallNavigation()
{

}

void RGISSmallNavigation::Create(const RGISSmallIntelligentCache* inCache)
{
	Cache = inCache;
	DetourMax = inDetourMax;
}

void RGISSmallNavigation::Find(const SmallPathfinderOneData * inStart , const SmallPathfinderOneData * inEnd)
{
	SmallPathfinderSave* saveData = new SmallPathfinderSave;
	SmallPathfinderOneData* saveOneData = new SmallPathfinderOneData;

	*saveOneData = *inStart;
	saveData->Distance = 0;
	saveData->Data.push_back(saveOneData);
	Save.push_back(saveData);

	newVector<SmallPathfinderSave>::iterator i = Save.begin();
	newVector<SmallPathfinderSave>::iterator e = Save.end();
	for( ; i != e ; i ++)
	{
		SmallPathfinderOneData* one = (*i).saveData[ (*i).saveData.size() - 1 ];
		const RGISSmallIntelligentCache* cache = Load(one->MapNumber);
		ASSERT ( one->DouroNumber < cache->DataCount );
		ASSERT ( one->DouroPos < cache->Data[one->DouroNumber].NodeCount );

		//今いるノードデータを求めます.
		SmallPathfinderData::SmallPathfinderDataOne::_Node * node = cache->Data[one->DouroNumber].Node[one->DouroPos];


		Save.push_back();



	}
}

*/
