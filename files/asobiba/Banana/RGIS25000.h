// RGIS25000.h: RGIS25000 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS25000_H__E327100B_5364_46C2_B461_4BE798304F17__INCLUDED_)
#define AFX_RGIS25000_H__E327100B_5364_46C2_B461_4BE798304F17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotFoundException.h"
#include "RGISSlp.h"
#include "RGISSlm.h"
#include "RGISDouro.h"
#include "RGISTetudo.h"
#include "RDrawObject.h"
#include "RGISKawa.h"
#include "RGISSuiikiKai.h"
#include "RGISEki.h"
#include "RGISGyousei.h"
#include "RGISChimei.h"
#include "RGISHashi.h"
#include "RGISTonneru.h"
#include "RGISKoukyou.h"
#include "RGISDraw.h"
#include "RGISNode.h"
#include "RGISKoudo.h"
#include "RGISKoudo2.h"
#include "RGISFindHintEki.h"
#include "RGIS2500Info.h"
#include "RGISFindCityToNumberConvert.h"
#include "RGISGyoseiD.h"


class RGIS25000  
{
public:
	RGIS25000();
	virtual ~RGIS25000();

	void Draw(RGISDraw* ioDraw);

	void SmallPack(const string inPath,const string inTo,const string inNumString,
		MessageGIS2500Info * gis2500Info, RGISNameDatabase * ioNameDatabase, int inScale);

	static int GetMaxID(const string &inFilename , const char *idParse) throw(RException);
	static string RGIS25000::GetName(const char * inBuffer) throw(RException);

	//結合スクリプトの取得.
	int getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const;
	//結合スクリプトの取得.
	int getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const;


public:
	RGISSlm		Slm;
	RGISSlp		Slp;
private:

	RGISDouro	Douro;
	RGISTetudo	Tetudo;
	RGISKawa	Kawa;
	RGISSuiikiKai	SuiikiKai;
	RGISEki			Eki;
	RGISGyousei		Gyousei;
	RGISChimei		Chimei;
	RGISHashi		Hashi;
	RGISTonneru		Tonneru;
	RGISKoukyou		Koukyou;
	RGISNode		DouroNode;
	RGISFindHintEki	FindHintEki;

	RGISGyoseiD					GyoseiD;
	RGISFindCityToNumberConvert	FindCityToNumberConvert;

//	RGISKoudo		Koudo;
	RGISKoudo2		Koudo;
};

#endif // !defined(AFX_RGIS25000_H__E327100B_5364_46C2_B461_4BE798304F17__INCLUDED_)
