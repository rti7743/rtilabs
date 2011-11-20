// RGISAddressMng.cpp: RGISAddressMng クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISAddressMng.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISAddressMng::RGISAddressMng()
{

}

RGISAddressMng::~RGISAddressMng()
{

}

void RGISAddressMng::Draw(RGISDraw* ioDraw)
{
	Address.Draw(ioDraw , &NameDataBase);
}



void RGISAddressMng::SmallPack(const string inPath,const string inTo,const string inNumString , int inScale)
{

	//読み込み
	string filename = inPath + "/address/" + inNumString + "JGD.csv";
	Address.Create(filename   , atoi(inNumString.c_str() ) , &NameDataBase , inScale);

	Address.ConvertDisplayScal(1);

	//書き込み
	string path = inTo + "/temp/" ;

	Address.SmallPack(path , &NameDataBase , &FindHintAddress);

	FindHintAddress.Write(path);
}

//結合スクリプトの取得.
int RGISAddressMng::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const
{
	ASSERT(inSize >= 2);
	ioScripts[0] = Address.getPackInfo();
	ioScripts[1] = NameDataBase.getPackInfo("NA" , 'M');
	return 2;
}
//結合スクリプトの取得.
int RGISAddressMng::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const
{
	ASSERT(inSize >= 1);
	ioHeadScripts[0] = FindHintAddress.getPackInfo();
	return 1;
}
