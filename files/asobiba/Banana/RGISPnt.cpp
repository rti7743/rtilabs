// RGISPnt.cpp: RGISPnt クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISPnt.h"
#include "RConv.h"
#include "RDiskUtil.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISPnt::RGISPnt()
{
	Count = 0;
	Points= NULL;
}

RGISPnt::~RGISPnt()
{
	Delete();
}

void RGISPnt::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISPnt::Create(const string & inFilename, int inScale) throw(RException)
{
	ASSERT(Points == NULL);

	RStdioFile file;
	file.Open(inFilename , "rb");
	FILE * fp = file.getHandle();

	string buffer;
	buffer = file.ReadLine();

	//ヘッダーの取得.
	RGIS2500Head	head;
	head.Create( buffer.c_str() ,inScale);

	//要素の総数の取得.
	this->Count = head.getCount();
	this->Points = new PntData[this->Count];

	int line = 0;
	for(int i = 0  ; !file.IsEof(); i++ , line ++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		//読み込み.		A1105,261,12,750.0,999.9
		double x,y;
		PntData* p = &Points[i];
		int rsize = sscanf(buffer.c_str() , "P%d,%d,%lf,%lf" , &p->Type , &p->No, &x , &y);
		if (rsize != 4)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );
		}

		//ポリゴンの各頂点データの読み込み.
		UPOINT up = head.convertXY(x,y);
		p->Point.x = up.x;
		p->Point.y = up.y;
	}
}


void RGISPnt::SmallPack(const RGISArc * inArc , const RGISAtr * inAtr , 
						const string & inPath  ,const string & inMark,RGISNameDatabase* ioNameDatabase,
						RGIS2500InfoVector* gis2500infoVector)
{
}


void RGISPnt::SmallPackNoWrite(const RGISArc * inArc , const RGISAtr * inAtr , RGIS2500InfoVector* gis2500infoVector)
{
	for(int n = 0 ; n < this->Count ; n++)
	{
		const PntData* p = &Points[n];

		const AtrData* atr = NULL;
		///このポリゴンに該当する atr を探す
		{
			int atrCountMax = inAtr->getCount();
			for(int m = 0 ; m < atrCountMax ; m++ )
			{
				const AtrData* temp = inAtr->getPoint(m);
				if ( temp->No == p->No )
				{
					atr = temp;
					break;
				}
			}
		}

		if (atr != NULL && !atr->attrib[0].empty() )
		{
			gis2500infoVector->push_back( new RGIS2500Info(&p->Point ,  atr->attrib[0] ) );
		}
	}
}
