
#ifndef __SUJI_OUT
#define __SUJI_OUT

#include <./Graphics/rti_Graphics.h>

class TSujiOut
{
public:
	//数字データをセット.
	void Set(BMD bmd , int width ,int hight);
	//数列を表示する.
	void Draw(int Suji , int DrawX, int DrawY);
protected:
	BMD m_Bmd;		//画像データ
	int m_Width;	//一つの数字の横幅.
	int m_Hight;	//一つの数字の縦幅.
};


#endif					//
