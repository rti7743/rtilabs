// RGISSmallDrawEffect.h: RGISSmallDrawEffect クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLDRAWEFFECT_H__293BDD06_3352_4E8B_A632_F5BCC036BD26__INCLUDED_)
#define AFX_RGISSMALLDRAWEFFECT_H__293BDD06_3352_4E8B_A632_F5BCC036BD26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMathAngle.h"

enum ParticularLevel
{
	LEVEL_FULL,				//すべての説明の表示
	LEVEL_NOT_BANCHI,		//番地レベルの表示をとりやめ
	LEVEL_NOT_CHIMEI,		//地名レベルの表示をとりやめ
	LEVEL_NOT_KOUKYOU,		//公共機関レベルの表示をとりやめ
	LEVEL_NOT_LOWLEVEL,		//路地などの小さい道レベルの表示の取りやめ
	LEVEL_NOT_MIDDLELEVEL,	//一般道などの普通の道レベルの表示の取りやめ
};



#include "RGraphics.h"	//暫定

class RGISSmallDrawEffect  
{
public:

public:
	RGISSmallDrawEffect();
	virtual ~RGISSmallDrawEffect();

	void InitDisplay(const LPOINT * inGlobalJiku ,int inScreenWidth,int inScreenHeight, unsigned int inScale,RAngle inDeg);



	//表示する点の座標変換
	LPOINT RevAttach(const POINT * inXY) const;
	//表示する点の座標変換
	POINT Attach(const LPOINT * inXY) const;
	//グローバル座標を表示する点の座標に変換
	RECT ConvertShowDisplay(const LRECT * inRect) const;
	//描画しなければいけないであろう地域の取得.
	const LRECT* getGlobalDrawDisplay() const
	{
		return &GlobalDrawDisplay ;
	}

	//暫定
	void Boxes(int inScreenWidth,int inScreenHeight,unsigned short * image , unsigned char * inLevel    ,const LPOINT * inBlockStart,const COLORREF16 * inPallet) const;

	//拡大したときにはしょるレベルの決定
	ParticularLevel getLevel() const;
private:
	//表示スケール.
	unsigned int Scale;

	LPOINT		Jiku;	//画面中心
	LRECT		GlobalDrawDisplay;	//描画しなければいけないであろう地域
	int			ScreenWidth;
	int			ScreenHeight;
	RAngle		Deg;
	RAngle		RevDeg;
	const RMathAngleFixed	* MathAngle;
};

#endif // !defined(AFX_RGISSMALLDRAWEFFECT_H__293BDD06_3352_4E8B_A632_F5BCC036BD26__INCLUDED_)
