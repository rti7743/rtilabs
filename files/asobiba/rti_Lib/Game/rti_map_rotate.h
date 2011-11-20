#include "StartWithExeFile.h"
#ifdef ROTATE_MAP

#ifndef ___MAP_ROTATEH
#define ___MAP_ROTATEH
/**************************************************************************************/
//円筒スクロールマップ
/**************************************************************************************/

#include <.\Game\rti_map.h>

class TMapRotate	: public TMap
{
protected:
public:
	//コンストラクタ
	TMapRotate(){};
	//マップを絵画します(円筒スクロール)
	void DrawMain(BMD bmd,int w,int h);

	virtual int GetScreenX(int x)
		{
				int X = x - Nowx;
				if (X > MAP_FULLX) X -= MAP_FULLX;	//補正
				if (X < 0) X += MAP_FULLX;			//補正
				return X;
		};
	virtual int GetScreenY(int y)
		{
				int Y = y - Nowy;
				if (Y > MAP_FULLY) Y -= MAP_FULLY;	//補正
				if (Y < 0) Y += MAP_FULLY;			//補正
				return Y;
		};

	//マップを絵画します
	virtual void Draw(BMD bmd){TMapRotate::DrawMain(bmd,SCREENXSIZE,SCREENYSIZE);};	
	//主人公移動などによるマップスクロール(円筒)
	virtual void MapMasterMove(short nx,short ny,int speed);
	//指定したポイントを画面の中心にする.
	virtual void SetCeneter(short nx,short ny);
	//そのポイントを通過できるかチェック
	virtual bool If(short nx,short ny);			
    //円筒スクロールの場合のキャラクタ位置の補正
    void MapHosei(int *x,int *y);
	//円筒スクロールの場合のキャラクタ位置の補正(オーナーロードだぴょん)
	void MapHosei(short *x,short *y);
};


#endif

#endif //LIFE END
