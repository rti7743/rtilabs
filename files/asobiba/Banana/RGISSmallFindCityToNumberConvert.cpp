// RGISSmallFindCityToNumberConvert.cpp: RGISSmallFindCityToNumberConvert クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallFindCityToNumberConvert.h"
#include "RStdioMemory.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallFindCityToNumberConvert::RGISSmallFindCityToNumberConvert()
{
	this->Table = NULL;
	this->TableCount = 0;

}

RGISSmallFindCityToNumberConvert::~RGISSmallFindCityToNumberConvert()
{
	Delete();
}


void RGISSmallFindCityToNumberConvert::Create() throw(RException) 
{
	Delete();
}

void RGISSmallFindCityToNumberConvert::Delete()
{
	delete [] this->Table;
	this->Table = NULL;
}

//シンボルの解決
void RGISSmallFindCityToNumberConvert::Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) 
{
	ASSERT(inSimbol == getSimbol() );
	if (inSimbol != getSimbol() ) return ;

	ASSERT(this->Table == NULL);

	this->Table = new AddressToCodeMatchingTable[inDataCount];
	this->TableCount = inDataCount;

	unsigned long block;


	RStdioMemory memory;
	memory.Open(inBuffer );
	for(int i = 0 ; i < inDataCount ; i++ )
	{
		//最初にブロック.
		memory.fread( &block , sizeof(block) );
		ASSERT(memory.ftell() <= inDataSize);

		//次に名前 これは \0 で終わる.
		const char * cityName = memory.ptr();
		int cityNameLen = strlen(cityName) + 1;
		//名前分読み飛ばし.
		memory.fseek( cityNameLen , SEEK_CUR);

		this->Table[i].Address = cityName;
		this->Table[i].Code = block;
	}
}


