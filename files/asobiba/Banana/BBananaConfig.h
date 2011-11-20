// BBananaConfig.h: BBananaConfig クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BBANANACONFIG_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_)
#define AFX_BBANANACONFIG_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

const int COLOR_MAX	=	255;

enum
{
	COLOR_MOJI			=	10,		//文字
	COLOR_CAPTION		=	11,		//caption
	COLOR_PEN1			=	20,		//ペン
	COLOR_PEN2			=	21,		//ペン
	COLOR_PEN3			=	22,		//ペン
	COLOR_PEN4			=	23,		//ペン
	COLOR_PEN5			=	24,		//ペン
	COLOR_PEN6			=	25,		//ペン
	COLOR_TRACKLOG		=	26,		//トラックログ	ペン7番をトラックログとして利用する.
	COLOR_TATEMORO		=	30,		//建物枠
	COLOR_JYOUTI		=	30,		//じょうち枠
	COLOR_TEIENRO		=	40,		//庭園路
	COLOR_ISHIDAN		=	41,		//石段
	COLOR_DOURO			=	42,		//一般道
	COLOR_KOUSOKU_DOURO	=	43,		//高速道路
	COLOR_EKI			=	50,		//駅
	COLOR_TETUDO_JR		=	51,		//普通鉄道JR
	COLOR_TETUDO		=	52,		//普通鉄道
	COLOR_ROMENDENSYA	=	53,		//路面電車
	COLOR_TIKATETU		=	54,		//地下式鉄道
	COLOR_TETUDOU_SONOTA=	55,		//その他
	COLOR_MIZU			=	100,	//水
	COLOR_KOUDO200		=	101,	//高度200未満
	COLOR_KOUDO400		=	102,	//高度400未満
	COLOR_KOUDO600		=	103,	//高度600未満
	COLOR_KOUDO800		=	104,	//高度800未満
	COLOR_KOUDO1000		=	105,	//高度1000未満
	COLOR_KOUDO1500		=	106,	//高度1500未満
	COLOR_KOUDO2000		=	107,	//高度2000未満
	COLOR_KOUDO2500		=	108,	//高度2500未満
	COLOR_KOUDO3000		=	109,	//高度3000未満
	COLOR_KOUDO4000		=	110,	//高度4000未満
	COLOR_KOUDO5000		=	111,	//高度5000未満
	COLOR_KOUDOOVER		=	112,	//高度over未満
};


class BBananaConfig  
{
private:
	BBananaConfig();
public:
	virtual ~BBananaConfig();


	static BBananaConfig* getInstance();

	void Load();
	void Save();

	void UpMapDirectory(const string & inMapFilename , const string & inUserDataDirectory);
	void UpNowPoint(const LPOINT & inNowXY , int inScale , int inDeg);

	const string& getUserDataDirectory() const
	{
		return this->UserDataDirectory;
	}
	const string& getMapFilename() const
	{
		return this->MapFilename;
	}
	const LPOINT& getPoint() const
	{
		return this->Point;
	}
	const int getScale() const
	{
		return this->Scale;
	}
	const int getDeg() const
	{
		return this->Deg;
	}
	const bool getRotateFlg() const
	{
		return this->RotateFlg;
	}
	void setRotateFlg(bool inFlg) 
	{
		this->RotateFlg = inFlg;
	}

	unsigned short getColor(unsigned short inColor) const
	{
		return this->Color[inColor];
	}
	const unsigned short* getColors() const
	{
		return this->Color;
	}

private:
	string UserDataDirectory;
	string MapFilename;
	LPOINT Point;
	int Scale;
	int Deg;
	bool RotateFlg;

	unsigned short Color[COLOR_MAX];
};

#endif // !defined(AFX_BBANANACONFIG_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_)
