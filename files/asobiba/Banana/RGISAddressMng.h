// RGISAddressMng.h: RGISAddressMng クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISADDRESSMNG_H__F18BF5FC_6BA1_4863_9377_35B4C4CE1E7F__INCLUDED_)
#define AFX_RGISADDRESSMNG_H__F18BF5FC_6BA1_4863_9377_35B4C4CE1E7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "comm.h"
#include "RGISAddress.h"
#include "RGISNameDatabase.h"
#include "RGISFindHintAddress.h"

class RGISAddressMng  
{
public:
	RGISAddressMng();
	virtual ~RGISAddressMng();

	void Draw(RGISDraw* ioDraw);

	void SmallPack(const string inPath,const string inTo,const string inNumString , int inScale);
	//結合スクリプトの取得.
	int getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const;
	//結合スクリプトの取得.
	int getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const;

private:
	RGISAddress	Address;
	RGISNameDatabase NameDataBase;
	RGISFindHintAddress	FindHintAddress;
};

#endif // !defined(AFX_RGISADDRESSMNG_H__F18BF5FC_6BA1_4863_9377_35B4C4CE1E7F__INCLUDED_)
