// RGISSmall.h: RGISSmall クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALL_H__6EB5E7D7_5576_4F2B_8D2B_4DB4CD79BEBA__INCLUDED_)
#define AFX_RGISSMALL_H__6EB5E7D7_5576_4F2B_8D2B_4DB4CD79BEBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallFileRead.h"
#include "RGISSmallDraw.h"
#include "RGISSmallDrawEffect.h"
#include "RGISSmallParent.h"
#include "RGISAllTest.h"

class RGISSmall  
{
	friend class RGISAllTest;	//テストクラスがこいつの中をいじりまわせるように.
public:
	RGISSmall();
	virtual ~RGISSmall();

	void Create(const string & inFilename , const string & inUserDataPath) throw(RException);
	bool Load(unsigned long inBlock);

	void Draw(const LRECT * inGlobalDisplay ,const LRECT * inDisplay , RDrawObject  * ioDraw , const RGISSmallDrawEffect* inEffect);


private:
	//マップファイル.
	RGISSmallFileRead	File;
	//データ
	RGISSmallParent		Parents;
};

#endif // !defined(AFX_RGISSMALL_H__6EB5E7D7_5576_4F2B_8D2B_4DB4CD79BEBA__INCLUDED_)
