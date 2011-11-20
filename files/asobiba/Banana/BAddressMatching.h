// BAddressMatching.h: BAddressMatching クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADDRESSMATCHING_H__5FCEF32B_C2D0_45A9_B15A_AE1237BF8BB6__INCLUDED_)
#define AFX_BADDRESSMATCHING_H__5FCEF32B_C2D0_45A9_B15A_AE1237BF8BB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOException.h"
#include "RGISSmallFindCityToNumberConvert.h"


class BAddressParse
{
public:
	BAddressParse()
	{
	}
	virtual ~BAddressParse()
	{
	}
	bool BAddressParse::Parse(const string &inAddress ,const  AddressToCodeMatchingTable* inMatchTable , int inTableSize);

	int getCode() const
	{
		return Code;
	}
	string getMachiCyoume() const
	{
		return MachiCyoume;
	}
	int getBanchi() const
	{
		return Banchi;
	}

	static void test();

private:
	//丁目という指示が明確にある場合...
	const char * BAddressParse::getChyoume(const char * inStr,const string inPrefix , int *outSkip) const;
	const char * BAddressParse::getChyoume(const char * inStr, int *outSkip) const;
	const char * BAddressParse::FindNot2Byte(const char *inStr) const;
	//都道府県があったら読み飛ばし.
	int BAddressParse::SkipTodoufuKen(const string & inAddress) const;

	const AddressToCodeMatchingTable* BAddressParse::ToCode(const string & inAddress , const  AddressToCodeMatchingTable* inMatchTable , int inTableSize) const;

private:
	int Code;				//01101 のようなコード
	string MachiCyoume;		//○○町○○丁目
	int	Banchi;				//×番
};

#endif // !defined(AFX_BADDRESSMATCHING_H__5FCEF32B_C2D0_45A9_B15A_AE1237BF8BB6__INCLUDED_)
