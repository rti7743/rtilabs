// RGISFindCityToNumberConvert.cpp: RGISFindCityToNumberConvert クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISFindCityToNumberConvert.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISFindCityToNumberConvert::RGISFindCityToNumberConvert()
{

}

RGISFindCityToNumberConvert::~RGISFindCityToNumberConvert()
{
	Delete();
}

void RGISFindCityToNumberConvert::Create(const string & inName , unsigned long inCode)
{
	CityToNumberConvert::iterator i = Address.begin();
	CityToNumberConvert::iterator e = Address.end();
	for( ; i != e ; i++)
	{
		if ( (*i)->Code == inCode && (*i)->Name == inName) return ;
	}

	FindCityToNumberConvertData * p = new FindCityToNumberConvertData;
	p->Code = inCode;
	p->Name = inName;
	Address.push_back( p );
}

void RGISFindCityToNumberConvert::Write(const string & inDir) throw (RException)
{
	RStdioFile file;


	file.Open(inDir + "addresscityToNumberConvert" , "a+b");
	CityToNumberConvert::iterator i = Address.begin();
	CityToNumberConvert::iterator e = Address.end();
	for( ; i != e ; i++)
	{
		file.fwrite( &((*i)->Code) , sizeof(unsigned long) );
		file.fwrite( (*i)->Name.c_str() , (*i)->Name.size() + 1 );
	}
}

void RGISFindCityToNumberConvert::Delete()
{
	CityToNumberConvert::iterator i = Address.begin();
	CityToNumberConvert::iterator e = Address.end();
	for( ; i != e ; i++)
	{
		delete (*i);
	}
	Address.clear();
}

JoinHeadFormatScript RGISFindCityToNumberConvert::getPackInfo() 
{
	JoinHeadFormatScript ret;
	ret.TempFilename = "addresscityToNumberConvert";
	ret.BlockStartSimbol = 'C';
	ret.DataSize = sizeof(unsigned long);
	ret.HeadType = HeadTypeFixedAndString;

	return ret;
}

