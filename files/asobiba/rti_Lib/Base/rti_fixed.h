#ifndef ___FIXEDH
#define ___FIXEDH

/****************************************************************************/
//固定小数点(16:16)  あえてクラス化ないのはなぜ ? うーんつ、 asm で扱いやすくするためかな(汗)
/****************************************************************************/
#include <math.h>

//戻り値なしの警告を黙らせる. (eax で直接に返す)
#pragma warning(disable: 4035)

//固定小数点    整数 16 : 小数 16 の固定小数点どえす
typedef long Fixed;

//マス
#define fxsin(x) (FixedLTBSin256[((unsigned char)(x))])
#define fxcos(x) (FixedLTBCos256[((unsigned char)(x))])
#define fxtan(x) (FixedDiv(fxsin(x),fxcos(x)))
#define fxpow(x,y) (FixedPow(x,y))
#define fxatan(x) (FixedAtan(x))
#define fxatan2(x,y) (FixedAtan( FixedDiv(x,y) ))
#define fxhypot(x,y) (fast_hypot(x,y) )
//パイの固定小数点
#define PAI_FIXED (205887) 
//最大値
#define FIXED_HIGH_VALUE	(0x7fffffff)
//最小値
#define FIXED_LOW_VALUE		(0x8000001)

//変換
#define Int2Fixed(x) (( (long)(x))<<16 )    //正数を固定小数点に変換
#define Fixed2Int(x) ((x)>>16 )             //固定小数点を正数に変換
#define Float2Fixed(x) ((long)((x)*65536.0) )   //浮動を固定に変換
#define Fixed2Float(x) ((double)((x)/65536.0) ) //固定を浮動に変換
#define FixedRound(x) (x+0x8000)                //四捨五入
#define FixedInt(x) (x&0xffff0000)              //小数点切り捨て
//固定小数点の短縮形...??
#define IF(x)   Int2Fixed(x)
#define FI(x)   Fixed2Int(x)

//掛け算と割り算.
//***************************************************
//固定小数点(<<16)同士の掛け算
//***************************************************
inline Fixed FixedMul(Fixed const num1, Fixed const num2)
{
		__asm
		{
			mov eax, num1
			mov edx, num2
			imul edx
			shrd eax, edx, 16
		}
}

//***************************************************
//固定小数点(<<16)同士のわり算
//***************************************************
inline Fixed FixedDiv(Fixed const num1, Fixed const num2)
{
	register Fixed result;
	try
	{
		__asm
		{
            mov edx,num1
            mov ebx,num2
            xor eax,eax
            sar edx,16

            shrd eax,edx,16
            idiv ebx
			mov result,eax
		}
	}
	catch(...)
	{
		return 0;
	}
	return result;
}


//***************************************************
//アークタンジェント(固定小数点バージョン) 参考:アルゴリズム辞典
//***************************************************
#define ATAN_N  24  /* 本文参照 */
inline Fixed FixedAtan(Fixed x)  
{
	int i, sgn;
    Fixed PowX;
	Fixed a;

	if      (x >  IF(1) ) {  sgn =  1;  x = FixedDiv(IF(1) , x);  }
	else if (x < IF(-1) ) {  sgn = -1;  x = FixedDiv(IF(1) , x);  }
	else                sgn =  0;
	a = 0;
    PowX = FixedMul(x,x);
	for (i = ATAN_N; i >= 1; i--)
    {
//		a = (i * i * x * x) / (2 * i + 1 + a);
        a = FixedDiv(  (i * i * PowX)  , ((i*2) + IF(1) + a) );
    }
//	if (sgn > 0) return  PI / 2 - x / (1 + a);
//	if (sgn < 0) return -PI / 2 - x / (1 + a);
//	/* else */   return           x / (1 + a);
	if (sgn > 0) return  (PAI_FIXED /2) - FixedDiv(x , IF(1) + a);
	if (sgn < 0) return -(PAI_FIXED /2) - FixedDiv(x , IF(1) + a);
	/* else */   return           FixedDiv(x , IF(1) + a);

}

//***************************************************
//べき乗
//***************************************************
inline Fixed FixedPow(Fixed a,Fixed b)
{
	return Float2Fixed (  pow(Fixed2Float(a),Fixed2Float(b) )  );
//	return IF(1);
}

//***************************************************
//2乗
//***************************************************
inline Fixed FixedPow2(Fixed a)
{
	return FixedMul(a,a);
//	return IF(1);
}


//戻り値なしの警告をしゃべらせる.
#pragma warning(error: 4035)

#endif
