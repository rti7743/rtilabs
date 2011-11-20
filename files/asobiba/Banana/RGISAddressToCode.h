// RGISAddressToCode.h: RGISAddressToCode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISADDRESSTOCODE_H__856737C4_9FCC_426B_9911_51D638564970__INCLUDED_)
#define AFX_RGISADDRESSTOCODE_H__856737C4_9FCC_426B_9911_51D638564970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"

struct AdressConvertData
{
	int	Code;
	string Todoufuken;
	string City;
};

class RGISAddressToCode  
{
public:
	RGISAddressToCode();
	virtual ~RGISAddressToCode();

	void Create(const string & inCodeFilename) throw(RException);
	void Delete();

	const string Convert(int inCode) const;
private:
	int					Count;
	AdressConvertData*	Points;

};

#endif // !defined(AFX_RGISADDRESSTOCODE_H__856737C4_9FCC_426B_9911_51D638564970__INCLUDED_)
