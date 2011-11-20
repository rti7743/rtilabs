// RGISSmallFindCityToNumberConvert.h: RGISSmallFindCityToNumberConvert クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLFINDCITYTONUMBERCONVERT_H__74F4B620_BC20_489C_9C9E_ED864ED29FAC__INCLUDED_)
#define AFX_RGISSMALLFINDCITYTONUMBERCONVERT_H__74F4B620_BC20_489C_9C9E_ED864ED29FAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallHeaderImp.h"
#include "RGISSmallIntelligentCache.h"

struct AddressToCodeMatchingTable
{
	string				Address;		//北海道中央区みたいな住所
	unsigned long		Code;			//01101 のようなコード
};


class RGISSmallFindCityToNumberConvert : public RGISSmallHeaderImp  
{
public:
	RGISSmallFindCityToNumberConvert();
	virtual ~RGISSmallFindCityToNumberConvert();


	void Delete();
	void Create() throw(RException) ;
	//シンボルの解決
	virtual void Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) ;
	//シンボルの取得.
	virtual unsigned char getSimbol() 
	{
		return 'C';
	}

	const AddressToCodeMatchingTable* getTable() const
	{
		return this->Table;
	}

	int getTableCount() const
	{
		return this->TableCount;
	}

private:
	AddressToCodeMatchingTable*	Table;
	int							TableCount;

};
#endif // !defined(AFX_RGISSMALLFINDCITYTONUMBERCONVERT_H__74F4B620_BC20_489C_9C9E_ED864ED29FAC__INCLUDED_)
