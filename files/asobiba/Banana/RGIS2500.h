// RGIS2500.h: RGIS2500 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGIS2500_H__A64A40ED_9E98_40B6_98F1_5D579D1EA76C__INCLUDED_)
#define AFX_RGIS2500_H__A64A40ED_9E98_40B6_98F1_5D579D1EA76C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotFoundException.h"
#include "RDrawObject.h"
#include "RGISDraw.h"
#include "RGISPgn.h"
#include "RGISArc.h"
#include "RGISAtr.h"
#include "RGISPnt.h"
#include "RGISJoin.h"
#include "RGISNameDatabase.h"
#include "RGIS2500Info.h"

struct RGIS2500AreaPgn
{
	RGISArc	Arc;
	RGISPgn	Pgn;
	RGISAtr Atr;
	RGISPnt Pnt;
};


struct RGIS2500OneArea
{
	RGIS2500AreaPgn			Tatemono;		//建物
	RGIS2500AreaPgn			Zyouti;			//じょうち? 公園/学校の敷地...
	RGIS2500AreaPgn			Eki;			//駅
};


class RGIS2500  
{
public:
	RGIS2500();
	virtual ~RGIS2500();

	void SmallPack(const string inPath,const string inTo,const string inNumString, 
		MessageGIS2500Info * gis2500Info , RGISNameDatabase * ioNameDatabase, int inScale);
	//結合スクリプトの取得.
	int RGIS2500::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const;
	//結合スクリプトの取得.
	int RGIS2500::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const;

	int getKey() const
	{
		return Key;
	}
private:
	void Delete();
	//01100 等のディレクトリの中に分断されて入っているエリアの数を取得します.
	int getAreaCount(const string & inPath ) const throw(RNotFoundException , RException) ;
	//実際に読み込み
	void Read(const string & inPath , int inScale) throw(RException);
	//ひとつのエリアを読み込んで見ます。
	void ReadInPart(RGIS2500OneArea* ioArea , const string & inPath , int inScale) throw(RException);

	//Keyを文字列として返します. Keyは必ず 5桁の文字列になる.
	string KeyToString(int inKey) const;

	JoinTempFormatScript RGIS2500::PgnJoinScript(const string & inDirectoryName , unsigned char inBlockStart) const;

	int					Count;
	RGIS2500OneArea*	Points;

	int				Key;
};

#endif // !defined(AFX_RGIS2500_H__A64A40ED_9E98_40B6_98F1_5D579D1EA76C__INCLUDED_)
