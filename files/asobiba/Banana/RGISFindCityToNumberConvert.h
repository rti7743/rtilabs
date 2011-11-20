// RGISFindCityToNumberConvert.h: RGISFindCityToNumberConvert クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISFINDCITYTONUMBERCONVERT_H__B0D692D5_F4F2_4010_B05E_ED4EFE234BB1__INCLUDED_)
#define AFX_RGISFINDCITYTONUMBERCONVERT_H__B0D692D5_F4F2_4010_B05E_ED4EFE234BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "comm.h"
#include "RGISJoin.h"

struct FindCityToNumberConvertData
{
	unsigned long	Code ;		//01010 のような町コード.
	string			Name;	//名前 
};

class RGISFindCityToNumberConvert
{
	typedef vector<FindCityToNumberConvertData*> CityToNumberConvert;
	CityToNumberConvert	Address;
public:
	RGISFindCityToNumberConvert();
	virtual ~RGISFindCityToNumberConvert();

	void RGISFindCityToNumberConvert::Create(const string & inName , unsigned long inCode);
	void Write(const string & inDir);

	static JoinHeadFormatScript getPackInfo() ;
private:
	void Delete();
};
#endif // !defined(AFX_RGISFINDCITYTONUMBERCONVERT_H__B0D692D5_F4F2_4010_B05E_ED4EFE234BB1__INCLUDED_)
