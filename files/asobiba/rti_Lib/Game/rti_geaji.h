#include "StartWithExeFile.h"
#ifdef GEAJI_LIFE

#ifndef ___GEAJIH
#define ___GEAJIH
#include <windows.h>
#include <.\Graphics\rti_graphics.h>
/*****************************************************************/
//ゲージ
/*****************************************************************/

class TGeaji
{
private:
    BMD bmd;    //絵
    int bmdx , bmdy , bmdw, bmdh;
    int max;   //最大値
    int min;   //最小値
    int distance;  //MAX と MIN の差
    int true_width;  //正式に絵画したい横幅
    int draw_width; //現在の絵画する横幅
    int value;      //正式なゲージの値
    int GeajiSpeed; //ゲージのスピード
public:
    TGeaji(){};
    ~TGeaji(){}; 
    void Set(BMD pbmd,int px,int py ,int pw,int ph,int pmax,int pmin,int _GeajiSpeed);
    void ReSetMax(int pmax);
    void NowValue(int value);
    void AfterValue(int value);
	void Draw(GraphicsProc Proc,int x,int y);
};

typedef TGeaji* GEAJI;

#endif

#endif //LIFE END
