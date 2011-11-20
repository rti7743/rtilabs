// GISType.cpp: GISType クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "GISType.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

GISINTArray::GISINTArray()
{
	this->Count = 0;
	this->Points = NULL;

}
GISINTArray::~GISINTArray()
{
	Delete();
}

GISINTArray::GISINTArray(const GISINTArray& inMySelf)
{
	Create(inMySelf.Points , inMySelf.Count );
}

GISINTArray& GISINTArray::operator=(const GISINTArray& inMySelf)
{
	Delete();
	Create(inMySelf.Points , inMySelf.Count );
	return *this;
}


void GISINTArray::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}



void GISINTArray::Create(const char* inStr) throw (RException)
{
	ASSERT(this->Points == NULL);

	if (inStr[0] == '\0') return ;

	// , の数を数えます。
	int count = 1 , i;
	for(i = 0 ; inStr[i] != '\0'; i++)
	{
		if (inStr[i] == ',')
		{
			count ++;
		}
	}

	this->Count = count;
	this->Points = new unsigned long[count];


	char * dummy;

	//変換しながら追加.
	int n = 0 , p = 0;
	for(i = 0 ; n < count; i++)
	{
		if (inStr[i] == ',' || inStr[i] == '\0')
		{
//			this->Points[n] = strtoul(  inStr + p  ,  &dummy , 10);
			this->Points[n] = atol( inStr + p );

			//念のためチェック.
			ASSERT(this->Points[n] > 0 && this->Points[n] <= 1000000);

			n++;
			p = i + 1;
		}
	}
}
void GISINTArray::Create(unsigned long* inPoints ,unsigned int inCount)
{
	this->Count = inCount;
	this->Points = new unsigned long[inCount];

	memcpy( this->Points , inPoints , sizeof(unsigned long) * inCount );
}



void GISINTArray::test()
{
	try
	{
		int r;
		{
			GISINTArray gia;
			gia.Create("123,456,789");

			ASSERT((r = gia.getCount()) == 3);
			ASSERT((r = gia.getPoint(0)) == 123);
			ASSERT((r = gia.getPoint(1)) == 456);
			ASSERT((r = gia.getPoint(2)) == 789);
		}
		{
			GISINTArray gia;
			gia.Create("123");

			ASSERT((r = gia.getCount()) == 1);
			ASSERT((r = gia.getPoint(0)) == 123);
		}
		{
			GISINTArray gia;
			gia.Create("");

			ASSERT((r = gia.getCount()) == 0);
		}
		{
			GISINTArray gia;
			gia.Create("001091,001112,001117");

			ASSERT((r = gia.getCount()) == 3);
			ASSERT((r = gia.getPoint(0)) == 1091);
			ASSERT((r = gia.getPoint(1)) == 1112);
			ASSERT((r = gia.getPoint(2)) == 1117);
		}
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}



void GISType::test()
{
	{
		unsigned char r;
		GISType type("01") ;
		ASSERT( (r = type.OnePack()) == 1 );
	}
	{
		unsigned char r;
		GISType type("21") ;
		ASSERT( (r = type.OnePack()) == 0x21 );
	}
	{
		unsigned char r;
		GISType type("51") ;
		ASSERT( (r = type.OnePack()) == 0x51 );
	}
}
