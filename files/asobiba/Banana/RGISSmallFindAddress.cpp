// RGISSmallFindAddress.cpp: RGISSmallFindAddress クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallFindAddress.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallFindAddress::RGISSmallFindAddress()
{

}

RGISSmallFindAddress::~RGISSmallFindAddress()
{
	Delete();
}


void RGISSmallFindAddress::Create(unsigned long inCode , const string & inName , unsigned long inBanchi , RGISSmallIntelligentCache* ioCache) throw(RException) 
{
	this->Code = inCode;
	this->Name = inName;
	this->Banchi = inBanchi;
	this->Cache = ioCache;
}

void RGISSmallFindAddress::Delete()
{
	FindPool::iterator i = FoundData.begin();
	FindPool::iterator e = FoundData.end();

	for( ; i != e ; i++)
	{
		delete (*i);
	}
	FoundData.clear();
}

//シンボルの解決
void RGISSmallFindAddress::Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) 
{
	ASSERT(inSimbol == getSimbol() );
	if (inSimbol != getSimbol() ) return ;

	unsigned long code;
	unsigned long block;

	RStdioMemory memory;
	memory.Open(inBuffer );
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		memory.fread( &code , sizeof(code) );
		ASSERT(memory.ftell() <= inDataSize);

		memory.fread( &block , sizeof(block) );
//		ASSERT(memory.ftell() <= inDataSize);

		//探している町ですか?
		if (code != this->Code)
		{
			continue;
		}

		//探している町があるようなので、そのブロックを詳しく知りたい。
		const RGISSmallParent* parent = this->Cache->Load(block);
		if (parent == NULL)
		{
			continue;
		}


		//ある程度絞り込めたので、実際に検索命令を出してみよう。
		parent->FindAddress(&FoundData , Name , Banchi);

	}

//	ASSERT(!(memory.ftell() <= inDataSize));
}

