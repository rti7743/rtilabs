
#include <./Game/rti_SujiOut.h>
#include <./Graphics/rti_BackSurface.h>

//数字データをセット.
void TSujiOut::Set(BMD bmd , int width ,int hight)
{
	m_Bmd	=	bmd;		//画像データ
	m_Width	=	width;	//一つの数字の横幅.
	m_Hight	=	hight;	//一つの数字の縦幅.
}

#define MAX_SUJI_LEN 20

//数列を表示する.
void TSujiOut::Draw(int Suji , int DrawX, int DrawY)
{
	int Show;
	char Buffer[MAX_SUJI_LEN];
	wsprintf(Buffer , "%d" ,Suji);

	for(int i = 0; Buffer[i] != '\0' ; i++ )
	{
		//表示する数.
		Show = Buffer[i] - '0';
		//表示.
		RtiGraphics::Draw( m_Bmd , Show * m_Width , 0 ,  
			m_Width , m_Hight , Sprite , DrawX + i * m_Width,DrawY );
	}
}

