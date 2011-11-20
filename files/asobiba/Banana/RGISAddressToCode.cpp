// RGISAddressToCode.cpp: RGISAddressToCode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISAddressToCode.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISAddressToCode::RGISAddressToCode()
{

}

RGISAddressToCode::~RGISAddressToCode()
{
	Delete();
}

void RGISAddressToCode::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISAddressToCode::Create(const string & inCodeFilename) throw(RException)
{
	RStdioFile file;
	file.Open(inCodeFilename , "rb");
	FILE * fp = file.getHandle();

	string buffer;
	buffer = file.ReadLine();

	this->Count = RFileUtil::LineCount(inCodeFilename);
	if (this->Count <= 0) return ;

	//要素の総数の取得.
	this->Points = new AdressConvertData[this->Count];

	char TodoufukenBuffer[1024];
	char CityBuffer[1024];
	int Number;
	for(int i = 0  ; !file.IsEof(); i++ )
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;
		//読み込み.
		AdressConvertData* p = &Points[i];
		//
		int rsize = sscanf(buffer.c_str() , "%1024[^,],%1024[^,],%d" , 
			TodoufukenBuffer,CityBuffer,&Number);
		if (rsize != 3) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inCodeFilename.c_str() , rsize , buffer.c_str()  ) );

		p->Todoufuken = TodoufukenBuffer;
		p->City = CityBuffer;
		p->Code = Number;
	}
}

const string RGISAddressToCode::Convert(int inCode) const
{
	for(int i = 0  ; i < this->Count ; i++ )
	{
		const AdressConvertData* p = &Points[i];
		if (p->Code == inCode)	return p->Todoufuken + p->City;
	}
	ASSERT(0);
	return "";
}
