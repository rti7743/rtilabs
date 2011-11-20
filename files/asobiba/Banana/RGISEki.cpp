// RGISEki.cpp: RGISEki クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISEki.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RGISEki::RGISEki()
{
	Count = 0;
	Points= NULL;

}

RGISEki::~RGISEki()
{
	Delete();
}


void RGISEki::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISEki::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "EK(ID{EK%d})");
	if (this->Count <= 0) return ;

	this->Points = new EkiData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string nameBuffer;

	string buffer;
	int i;
	//未使用フラグを立てる.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		int id;
		int rsize = sscanf(buffer.c_str() , "EK(ID{EK%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1から始まるため.
		if (id >= Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のIDが変です!");
		}
		EkiData* p = &Points[id];

		rsize = sscanf(buffer.c_str() , "EK(ID{EK%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		try
		{
			nameBuffer = RGIS25000::GetName(buffer.c_str());
			p->NameIndex = ioNameDatabase->setName(nameBuffer.c_str() );
		}
		catch(RException e)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のデータの形式が辺です! このエラーはここから誘発されました:" + e.getMessage() );
		}

		p->Use = true;
	}
}

void RGISEki::SmallPack(const string & inFilename,const RGISSlp * inSlp)  const throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "wb");

	FILE * fp = file.getHandle();

	//prefix
	//'EK'	EKという文字 2byte
	//ver	バージョン unsigned char
	//count	データの数 ushort

	unsigned char uc;
	unsigned short us;
	unsigned long ul;

	fwrite( "EK" , sizeof(char) , 2 , fp);				//EK
	uc = 1;		fwrite( &uc , sizeof(uc) , 1 , fp);		//ver 1
	us = Count;	fwrite( &us , sizeof(us) , 1 , fp);		//データの数

	//2bytes
	//name	ushort
	for(int n = 0 ; n < Count ; n++)
	{
		const EkiData* p = &Points[n];
		if (p->Use)	
		{
			us = (unsigned short) p->NameIndex;		fwrite( &us , sizeof(us) , 1 , fp);
		}
	}
}

void RGISEki::test()
{
	try
	{
		//国土地理院の了承がないとテストデータでも侵害になるかもしれないので、
		//テストルーチンは動かしません
		/*
		RGISEki dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.ek",&ndb);

		unsigned long r;
		const EkiData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 2);
		rr = dk.getPoint(0);
		*/
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
