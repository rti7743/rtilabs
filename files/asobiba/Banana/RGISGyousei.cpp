// RGISGyousei.cpp: RGISGyousei クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISGyousei.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISGyousei::RGISGyousei()
{
	Count = 0;
	Points= NULL;

}

RGISGyousei::~RGISGyousei()
{
	Delete();
}


void RGISGyousei::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISGyousei::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "GK(ID{GK%d})");
	if (this->Count <= 0) return ;

	this->Points = new GyouseiData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	int cvID;
	char cvBuffer[10240];

	string buffer;
	int i;
	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		cvBuffer[0] = '\0';

		int id;
		int rsize = sscanf(buffer.c_str() , "GK(ID{GK%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1から始まるため.
		if (id >= (int)Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のIDが変です!");
		}
		GyouseiData* p = &Points[id];

		const char * toOption = strstr(buffer.c_str() , "JT{");
		rsize = sscanf(toOption , "JT{%2s}SR{%2s}",
				p->JT.ptr() , p->SR.ptr() );
		if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		const char * cvStart = strstr(buffer.c_str() , "CV(");
		if (cvStart == NULL)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のデータのCV形式が辺です!");
		}
		rsize = sscanf(cvStart , "CV(ID{CV%d}){%10240[^}]}",				&cvID , cvBuffer);
		if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		p->CV.Create(cvBuffer);
	}
}



void RGISGyousei::Draw(const RGISDraw * inDraw) const
{
}




void RGISGyousei::SmallPack(const string & inFilename,const RGISSlp * inSlp)  const throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "wb");

	FILE * fp = file.getHandle();

	//prefix
	//'GK'	GKという文字 2byte
	//ver	バージョン unsigned char
	//count	データの数 ushort

	unsigned char uc;
	unsigned short us;
	unsigned long ul;

	fwrite( "GK" , sizeof(char) , 2 , fp);				//GK
	uc = 1;		fwrite( &uc , sizeof(uc) , 1 , fp);		//ver 1
	us = Count;	fwrite( &us , sizeof(us) , 1 , fp);		//データの数

	//2bytes
	//sr	uchar
	//cv	uchar
	//----cvの数だけ繰り返す
	//cv1	long,long	....
	UPOINT po;
	for(unsigned int n = 0 ; n < Count ; n++)
	{
		const GyouseiData* p = &Points[n];
		uc = p->SR.OnePack();					fwrite( &uc , sizeof(uc) , 1 , fp);	//種別
		uc = (unsigned short) p->CV.getCount();	fwrite( &uc , sizeof(uc) , 1 , fp);	//cv の数 (たぶん 256もないはず)

		int count = p->CV.getCount();
		for(int i = 0; i < count ; i++)
		{
			unsigned short pt = p->CV.getPoint(i) ;
			if (pt != 0 )	{	po = inSlp->getPoint(pt - 1);	}
			else			{	po.x = 0;	po.y = 0;	}

			ul = po.x;	fwrite( &ul , sizeof(ul) , 1 , fp);
			ul = po.y;	fwrite( &ul , sizeof(ul) , 1 , fp);
		}
	}
}

void RGISGyousei::test()
{
	try
	{
		//国土地理院の了承がないとテストデータでも侵害になるかもしれないので、
		//テストルーチンは動かしません
		/*
		RGISGyousei dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.gk",&ndb);

		unsigned long r;
		const GyouseiData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 2);
		rr = dk.getPoint(0);

		ASSERT( (r = rr->CV.getPoint(0)) == 20909);
		ASSERT( (r = rr->CV.getPoint(1)) == 20907);
		ASSERT( (r = rr->CV.getPoint(2)) == 20905);
		ASSERT( (rrr = rr->SR) == "2C");
		*/
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
