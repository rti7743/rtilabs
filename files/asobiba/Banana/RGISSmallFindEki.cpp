// RGISSmallFindEki.cpp: RGISSmallFindEki クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallFindEki.h"
#include "RStdioMemory.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallFindEki::RGISSmallFindEki()
{

}

RGISSmallFindEki::~RGISSmallFindEki()
{
	Delete();
}


void RGISSmallFindEki::Create(const string & inName , RGISSmallIntelligentCache* ioCache) throw(RException) 
{
	if (inName.size() >= NAME_SHORT_CUT_SIZE) 
	{
		memcpy(NameShortCut , inName.c_str() , NAME_SHORT_CUT_SIZE);
		this->NameSize = NAME_SHORT_CUT_SIZE;
	}
	else
	{
		memset(NameShortCut , 0 , NAME_SHORT_CUT_SIZE );
		memcpy(NameShortCut , inName.c_str() , inName.size() );
		this->NameSize = inName.size() ;
	}

	this->Name = inName;
	this->Cache = ioCache;
}

void RGISSmallFindEki::Delete()
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
void RGISSmallFindEki::Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) 
{
	ASSERT(inSimbol == getSimbol() );
	if (inSimbol != getSimbol() ) return ;

	char name[NAME_SHORT_CUT_SIZE];
	unsigned long block;
	unsigned short nameIndex;


	RStdioMemory memory;
	memory.Open(inBuffer );
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		memory.fread( name , sizeof(unsigned char) * NAME_SHORT_CUT_SIZE );
		ASSERT(memory.ftell() <= inDataSize);

		memory.fread( &block , sizeof(block) );
		ASSERT(memory.ftell() <= inDataSize);

		memory.fread( &nameIndex , sizeof(nameIndex) );
//		ASSERT(memory.ftell() <= inDataSize);

		//名前は先頭一致で検索.
		if ( memcmp( name , this->NameShortCut , this->NameSize) != 0 )
		{
			continue;
		}
		//名前があるようなので、そのブロックを詳しく知りたい。
		const RGISSmallParent* parent = this->Cache->Load(block);
		if (parent == NULL)
		{
			continue;
		}
		//ある程度絞り込めたので、実際に検索命令を出してみよう。
		parent->FindEki(&FoundData , Name , nameIndex);
	}

//	ASSERT(!(memory.ftell() <= inDataSize));
}


