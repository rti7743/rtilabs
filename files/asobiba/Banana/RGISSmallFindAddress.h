// RGISSmallFindAddress.h: RGISSmallFindAddress クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLFINDADDRESS_H__DF098AF6_0332_4CA2_A4B1_F76DE060F6DD__INCLUDED_)
#define AFX_RGISSMALLFINDADDRESS_H__DF098AF6_0332_4CA2_A4B1_F76DE060F6DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallHeaderImp.h"
#include "RGISSmallIntelligentCache.h"
#include "RGISSmallFindObject.h"

class RGISSmallFindAddress : public RGISSmallHeaderImp  
{
public:
	RGISSmallFindAddress();
	virtual ~RGISSmallFindAddress();

	void Delete();

	void Create(unsigned long inCode ,  const string & inName, unsigned long inBanchi, RGISSmallIntelligentCache* ioCache) throw(RException) ;
	//シンボルの解決
	virtual void Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException) ;
	//シンボルの取得.
	virtual unsigned char getSimbol() 
	{
		return 'A';
	}

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return FoundData.size();
	}
	//地点の取得
	const RGISSmallFindObject* getPoint(unsigned int p) const
	{
		ASSERT(p < FoundData.size() );
		return FoundData[p];
	}
private:
	unsigned long Code ;
	string	Name;
	int		Banchi;
	RGISSmallIntelligentCache* Cache;

	FindPool		FoundData;
};

#endif // !defined(AFX_RGISSMALLFINDADDRESS_H__DF098AF6_0332_4CA2_A4B1_F76DE060F6DD__INCLUDED_)
