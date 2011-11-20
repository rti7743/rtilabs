// RGISFindHintAddress.h: RGISFindHintAddress クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISFINDHINTADDRESS_H__0B685386_A637_4850_B319_3D4A3AAF07BC__INCLUDED_)
#define AFX_RGISFINDHINTADDRESS_H__0B685386_A637_4850_B319_3D4A3AAF07BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISJoin.h"

struct FindHintAddressData
{
	unsigned long Code ;		//01010 のような町コード.
	unsigned long Block;		//マップブロック
};

class RGISFindHintAddress
{
	typedef vector<FindHintAddressData*> UniqAddress;
	UniqAddress	Address;
public:
	RGISFindHintAddress();
	virtual ~RGISFindHintAddress();

	void Create(unsigned long inBlock,unsigned long inCode);
	void Write(const string & inDir);

	static JoinHeadFormatScript getPackInfo() ;
private:
	void Delete();

private:
};

#endif // !defined(AFX_RGISFINDHINTADDRESS_H__0B685386_A637_4850_B319_3D4A3AAF07BC__INCLUDED_)
