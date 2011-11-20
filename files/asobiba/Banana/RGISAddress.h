// RGISAddress.h: RGISAddress クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISADDRESS_H__7ABF18FB_0744_4664_A491_161DE190BCCC__INCLUDED_)
#define AFX_RGISADDRESS_H__7ABF18FB_0744_4664_A491_161DE190BCCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RGISDraw.h"
#include "RGISNameDatabase.h"
#include "RGISJoin.h"
#include "RGISFindHintAddress.h"

struct AddressData
{
//	string	Todoufuken;		//都道府県名
//	string	City;			//市区町村名
	string	Machi;			//大字・町丁目名
	unsigned short NameIndex;
	int		Banchi;			//地番
	int		Code;			//01010 のような町コード.
	UPOINT	xy;				//座標(UTM)
	bool	Use;
};


class RGISAddress  
{
public:
	RGISAddress();
	virtual ~RGISAddress();
	void Create(const string & inFilename,int inCode , RGISNameDatabase * ioNameDatabase, int inScale) throw(RException);
	void Delete();

	//スケールを変更し、ディスプレイ座標にします。
	void ConvertDisplayScal(int inShift) throw(RException);

	//地点の個数の取得.
	unsigned int getCount() const
	{
		return Count;
	}
	//地点の取得
	const AddressData* getPoint(unsigned int p) const
	{
		ASSERT(p < Count);
		ASSERT(Points != NULL);
		return &Points[p];
	}
	void Draw(const RGISDraw * inDraw ,const RGISNameDatabase * ioNameDatabase) const;


	void SmallPack(const string & inDir, const RGISNameDatabase* inNameData, RGISFindHintAddress * ioFindHintAddress)  const throw(RException);

	JoinTempFormatScript getPackInfo() const;

	static void test();
private:
	// " " でくくられている単語のククリをはずします。
	const string RGISAddress::noQoute(const string inStr);

private:
	unsigned int				Count;
	AddressData*	Points;

};

#endif // !defined(AFX_RGISADDRESS_H__7ABF18FB_0744_4664_A491_161DE190BCCC__INCLUDED_)
