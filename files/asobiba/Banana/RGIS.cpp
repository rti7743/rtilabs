// RGIS.cpp: RGIS クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGIS.h"
#include "RStdioFile.h"




//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGIS::RGIS()
{

}

RGIS::~RGIS()
{

}

//Keyを文字列として返します. Keyは必ず 5桁の文字列になる.
string RGIS::KeyToString(int inKey) const
{
	char buffer[MAX_PATH];
	sprintf(buffer , "%05d" , inKey);

	return buffer;
}

void RGIS::SmallPack(const string inPath,const string inTo,int inKey ,int inScale)
{
	const string keyString = KeyToString(inKey);

	Gis2500.SmallPack(inPath , inTo , keyString ,&Gis2500Info ,&NameDatabase , inScale);
	Gis25000.SmallPack(inPath , inTo , keyString,&Gis2500Info ,&NameDatabase , inScale);
	AddressMng.SmallPack(inPath , inTo , keyString , inScale);



}

//結合スクリプトの取得.
int RGIS::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) 
{
	RGIS25000		Gis25000;
	RGIS2500		Gis2500;
	RGISAddressMng	AddressMng;

	//結合スクリプトの取得.
	int segmentCount = Gis25000.getJoinScript(&ioScripts[0] , inSize);
	segmentCount += Gis2500.getJoinScript(&ioScripts[segmentCount] , inSize - segmentCount);
	segmentCount += AddressMng.getJoinScript(&ioScripts[segmentCount] , inSize - segmentCount);
	ioScripts[segmentCount++] = RGISNameDatabase::getPackInfo("NX" , 'N');



	return segmentCount;
}



//結合スクリプトの取得.
int RGIS::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) 
{
	RGIS25000		Gis25000;
	RGIS2500		Gis2500;
	RGISAddressMng	AddressMng;

	//結合スクリプトの取得.
	int segmentCount = Gis25000.getJoinHeadScript(&ioHeadScripts[0] , inSize);
	segmentCount += Gis2500.getJoinHeadScript(&ioHeadScripts[segmentCount] , inSize - segmentCount);
	segmentCount += AddressMng.getJoinHeadScript(&ioHeadScripts[segmentCount] , inSize - segmentCount);

	return segmentCount;
}
