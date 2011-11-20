#include "StartWithExeFile.h"
#ifdef FEADE_LIFE

#ifndef ___FEADE_EFFECTH
#define ___FEADE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TFeadeEffect
{
private:
    BMD OutBmd; //出力
    int GoalR,GoalG,GoalB;   //目標とする色
    int NowR,NowG,NowB;   //現在の色
    int SpeedR,SpeedG,SpeedB;   //変えるスピード
    int NowFream;         //現在のフレーム数
public:
	TFeadeEffect();
    ~TFeadeEffect();
    void Set(BMD OutBmd,int nr,int ng,int nb,int gr,int gg ,int gb,int speed);
	bool Draw();
    int NowFreamReat(){return NowFream;};
};

typedef TFeadeEffect* EFREADE;

#endif

#endif //LIFE END
