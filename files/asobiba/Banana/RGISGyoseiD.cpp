// RGISGyoseiD.cpp: RGISGyoseiD クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISGyoseiD.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RGISNameDatabaseDisk.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISGyoseiD::RGISGyoseiD()
{
	Count = 0;
	Points= NULL;
	Code = 0;

}

RGISGyoseiD::~RGISGyoseiD()
{
	Delete();
}


void RGISGyoseiD::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISGyoseiD::Create(const string & inFilename , unsigned long inCode) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "GD(ID{GD%d})");
	if (this->Count <= 0) return ;

	this->Code = inCode;

	this->Points = new GyoseiDData[this->Count];
	memset(this->Points , 0 , sizeof(ChimeiData) * this->Count);

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string nameBuffer;
	char cvBuffer[10240];

	string buffer;
	int i;
	//未使用フラグを立てる.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		cvBuffer[0] = '\0';

		int id,ptID,pt;
		const char * cmMain = strstr(buffer.c_str() , "GD(");
		if (cmMain == NULL)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("読み込みに失敗しました %s ソース[%s]" ,inFilename.c_str() ,  buffer.c_str()  ) );
		}
		int rsize = sscanf(cmMain , "GD(ID{GD%d})",	&id );
		if (rsize != 1)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );
		}

		id = id -1;	//1から始まるため.
		if (id >= Count || id	 < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のIDが変です!");
		}
		GyoseiDData* p = &Points[id];

		const char * ptStart = strstr(buffer.c_str() , "PT(");
		if (ptStart == NULL)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のデータのCV形式が辺です!");
		}
		rsize = sscanf(ptStart , "PT(ID{PT%d}){%d}",	&ptID , &pt);
		if (rsize != 2) continue;
		p->PT = pt;

		try
		{
			nameBuffer = RGIS25000::GetName(buffer.c_str());
		}
		catch(RException e)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のデータの形式が辺です! このエラーはここから誘発されました:" + e.getMessage() );
		}

		p->Name = nameBuffer;
		p->Use = true;
	}
}


void RGISGyoseiD::SmallPack(const string & inPath , RGISFindCityToNumberConvert * ioFindCityToNumberConvert ) const throw(RException)
{
	for(int n = 0 ; n < Count ; n++)
	{
		const GyoseiDData* p = &Points[n];
		if (p->Use)	
		{
			ioFindCityToNumberConvert->Create(p->Name , this->Code);
		}
	}
}

