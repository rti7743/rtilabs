#include "StartWithExeFile.h"
#ifdef BARST_LIFE

#ifndef ___BARST_EFFECTH
#define ___BARST_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TBarstEffect 
{
private:
    void Init();

    BMD OutBmd; //出力
	BMD pbmd;
	struct BarstStruct;
	struct BarstStruct
	{
		bool alive;
		int org;       //もともとそいつがあるべきだったところ 
		int x,y,sx,sy; //現在地 と 移動量
		int w,h;       //幅と高さ
	};
	struct BarstStruct *list;
	int listmax;
	int eopuf;
	int px,py;
	int pw,ph;   //絵の縦幅と横幅
	int pww,phh; //分割移動
    int pbasepeed ;
public:
	//この幅と高さは、絵の高さではなくて、切り抜く四角形の長さです
	TBarstEffect();
    ~TBarstEffect();
	void ReSet(int x,int y,bool set_possition,int basespeed);
	int Draw();
	void RevReSet(int x,int y,bool set_possition,int basespeed);
	int RevDraw();

    bool Set(BMD bmd,BMD _OutBmd,bool desth,int x,int y,int w,int h,int basespeed);
};

#endif

#endif //LIFE END
