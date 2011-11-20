// RGIS2500IntArray.cpp: RGIS2500IntArray クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGIS2500IntArray.h"
#include "RGISSlm.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGIS2500PointArray::RGIS2500PointArray()
{
	this->Count = 0;
	this->Points = NULL;
}

RGIS2500PointArray::~RGIS2500PointArray()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGIS2500PointArray::Create(int inMax , const char ** inStrArray, const RGIS2500Head* inHead) throw(RParseException)
{
	this->Count = inMax;
	this->Points = new UPOINT[inMax];

	double x,y;
	for(int i = 0; i < inMax ; i++)
	{
		//取り込みますか..
		int shit = sscanf(inStrArray[i] ,"%lf,%lf",&x,&y);
		if (shit != 2)
		{
			throw RParseException(string("") + "変換に失敗" + num2str(i) + "番目");
		}
		//
		this->Points[i] = inHead->convertXY(x,y);
	}
}

//スケールを変更し、ディスプレイ座標にします。
void RGIS2500PointArray::ConvertDisplayScal(int inShift) throw(RException)
{
	int count = this->Count;
	for(int i = 0 ; i < count ; i++)
	{
		this->Points[i].x = RGISMathZantei::ConvertDisplayScalX( (unsigned long) this->Points[i].x);
		this->Points[i].y = RGISMathZantei::ConvertDisplayScalY( (unsigned long) this->Points[i].y);
	}
}









RGIS2500IntArray::RGIS2500IntArray()
{
	this->Count = 0;
	this->Points = NULL;
}

RGIS2500IntArray::~RGIS2500IntArray()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}


void RGIS2500IntArray::Create(int inMax , const char ** inStrArray) throw(RParseException)
{
	this->Count = inMax;
	this->Points = new int[inMax];

	int n;
	for(int i = 0; i < inMax ; i++)
	{
		//取り込みますか..
		int shit = sscanf(inStrArray[i] ,"%d",&n);
		if (shit != 1)
		{
			throw RParseException(string("") + "変換に失敗" + num2str(i) + "番目");
		}
		this->Points[i] = n;
	}
}

