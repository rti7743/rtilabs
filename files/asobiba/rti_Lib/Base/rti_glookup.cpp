#include <StartWithExeFile.h>
#ifdef GLOOKUP_LIFE

#include <.\Base\rti_glookup.h>
#include <.\Base\rti_debug.h>
#include <math.h>
//***************************************************
//ルックアップテーブルと使うと使うと便利な変数群
//固定小数点
//移動量や距離などを求める定理
//乱数ルーチンなど
//          Thank You For C MAGAZIN 
//          奥村晴彦 著  Ｃ言語による最新アルゴリズム事典
//***************************************************


double LTBSin1024[1024];
double LTBCos1024[1024];
Fixed  FixedLTBSin1024[1024];
Fixed  FixedLTBCos1024[1024];

//***************************************************
//ルックアップテーブル作成
//***************************************************
void lookup_making()
{

    int i;
    for (i = 0 ; i < 1024 ; i ++) 
    {
        LTBSin1024[i] = sin(RRR1024(i));
        LTBCos1024[i] = cos(RRR1024(i));
        FixedLTBSin1024[i] = Float2Fixed(LTBSin1024[i] );
        FixedLTBCos1024[i] = Float2Fixed(LTBCos1024[i] );
    }
    
}

//***************************************************
//hypot関数の高速番
//***************************************************
double fast_hypot_double(int x,int y)
{
    double len1,len2 ;

    x = abs(x);
    y = abs(y);
    if (x >= y)
    {
        len1 = x ; len2 = y;
    }
    else
    {
        len1 = y ; len2 = x;
    }
    return 0.9604 + len1 + 0.3978 *  len2;
}

//***************************************************
//hypot関数の高速番のint番 最速
//***************************************************
unsigned long fast_hypot(int x,int y)
{
/*
    unsigned long  len1,len2 ,t;

    x = abs(x);
    y = abs(y);
    if (x >= y)
    {
        len1 = x ; len2 = y;
    }
    else
    {
        len1 = y ; len2 = x;
    }
    t = len2 + (len2 >> 1);
    return (len1 - (len1 >> 5) - (len1 >> 7) + (t >> 2) + (t >> 6) );
*/
	unsigned long vret;
	_asm{
//    x = abs(x);
	    ;絶対値をもとめる
		mov eax,x
		test eax,0x80000000
		jz NON1
		neg eax
	NON1:

//    y = abs(y);
		;絶対値をもとめる
		mov ebx,y
		test ebx,0x80000000
		jz NON2
		neg ebx
	NON2:
//    if (x >= y)
//    {
//        len1 = x ; len2 = y;
//    }
//    else
//    {
//        len1 = y ; len2 = x;
//    }
	// len1 = eax  len2 = ebx
		cmp eax,ebx
		jae STEP2
		xchg eax,ebx //交換

	STEP2:
//    t = len2 + (len2 >> 1);
		mov edi,ebx //len2を代入
		shr edi,1   //len2 >> 1
		add edi,ebx //len2 +
	//edi は、 t です.

//    return (len1 - (len1 >> 5) - (len1 >> 7) + (t >> 2) + (t >> 6) );
		mov esi,eax //len1 を代入 リターンの値は esi に作ります

		mov edx,eax //(len1 >> 5)
		shr edx,5
		sub esi,edx

		mov eax,7  //(len1 >> 7) //もう、len1は参照しないので破壊する
		sub esi,eax

		mov edx,edi//(t >> 2)
		shr edx,2
		add esi,edx

		shr edi,6  //(t >> 6) //もう、tは参照しないので破壊する
		add esi,edi
		mov vret,esi
	}
	return vret;
}


//***************************************************
//(nx,ny)から(gx,gy)へspeed で移動したい時の移動量(sx,sy)を求める
//***************************************************
void path_finder(int nx,int ny,int gx,int gy,int *sx,int *sy,int speed)
{
    int x =  gx - nx;
    int y =  gy - ny;

	double seeta;
	seeta = atan2(y,x);
    *sx = (int) (cos( seeta )* speed) ;
    *sy = (int) (sin( seeta )* speed) ;
}
//***************************************************
//三次の移動量
//***************************************************
void path_finder3D(int nx,int ny,int nz,
				   int gx,int gy,int gz,
				   int *sx,int *sy,int *sz,int speed)
{
	int x = gx - nx;
	int y = gy - ny;
	int z = gz - nz;
	int n = farst_hypot3D(x,y,z) / speed;
    if (n)
    {
	    *sx = x / n;
	    *sy = y / n;
	    *sz = z / n;
    }
    else
    {
	    *sx = x ;
	    *sy = y ;
	    *sz = z ;
    }
}

//***************************************************
//三次の距離だぴょん  sqrt(lx^2 + ly ^2 + lz ^2)
//***************************************************
unsigned long farst_hypot3D(long lx,long ly,long lz)
{
	long len1,len2,len3,len,temp;
	len1 = abs(lx);
	len2 = abs(ly);
	len3 = abs(lz);

	if (len1 < len2)
		SWAP(len1,len2,temp);
	if (len2 < len3)
		SWAP(len2,len3,temp);
	if (len1 < len2)
		SWAP(len1,len2,temp);
	len = (len1 >> 1) + (len1 >> 2) + (len1 >> 3) + (len1 >> 4)
		+ (len2 >> 2) + (len2 >> 3) + (len2 >> 6) 
		+ (len3 >> 2) + (len3 >> 5) + (len3 >> 6) ;
	return len;
}


//***************************************************
//自機(nx,ny)が敵(gx,gy)とのあたり判定 faj は容認誤差
//***************************************************
bool HitCheck(int nx,int ny,int nw,int nh,
			  int gx,int gy,int gw,int gh,int faj)
{
    bool hit = false;
    _asm
    {
        //faj をあらかじめレジスタに.
        mov esi,faj
        //幅を調べる
        mov eax,nx
        sub eax,gx //sa = (short)(nx - gx);
        jns NON1    //負の数につっこんでいないかチェック
	       //負の数なので絶対値を求める
		neg eax     //符号反転(せいにする)
        //その eax (nx - gx) の結果が、幅と誤差の合計より
        //小さければ、あたり　と、判定します.
        //しかし、いちいち、幅と誤差(nw + esi) を求めて、
        //計算するより、 eaxから、引いた方が 1 ステップ
        //ケチることができます (^^;;
        sub eax,nw  //幅
        sub eax,esi //幅に誤差を加える esi = faj
        jns  ENDPPROC //はずれてます.
        jmp NEXT_STEP //次は高さの判定です.
	NON1: //nx - gx の結果は　せいだったら.
        //その eax (nx - gx) の結果が、幅と誤差の合計より
        //小さければ、あたり　と、判定します.
        //しかし、いちいち、幅と誤差(gw + esi) を求めて、
        //計算するより、 eaxから、引いた方が 1 ステップ
        //ケチることができます (^^;;
        sub eax,gw  //幅
        sub eax,esi //幅に誤差を加える esi = faj
        jns  ENDPPROC //はずれてます.

    NEXT_STEP:
        //高さを調べる
        mov eax,ny
        sub eax,gy //sa = (short)(ny - gy);
        jns NON2    //負の数につっこんでいないかチェック
	       //負の数なので絶対値を求める
		neg eax     //符号反転(せいにする)
        //その eax (ny - gy) の結果が、縦と誤差の合計より
        //小さければ、あたり　と、判定します.
        //しかし、いちいち、縦と誤差(nh + esi) を求めて、
        //計算するより、 eaxから、引いた方が 1 ステップ
        //ケチることができます (^^;;
        sub eax,nh  //縦 
        sub eax,esi  //縦に誤差を加える esi = faj
        jns  ENDPPROC //はずれてます.
        jmp TRUE_STEP //ヒットしてますよ!
	NON2: //nx - gx の結果は　せいだったら.
        //その eax (ny - gy) の結果が、縦と誤差の合計より
        //小さければ、あたり　と、判定します.
        //しかし、いちいち、縦と誤差(gh + esi) を求めて、
        //計算するより、 eaxから、引いた方が 1 ステップ
        //ケチることができます (^^;;
        sub eax,gh  //縦 
        sub eax,esi  //縦に誤差を加える esi = faj
        jns  ENDPPROC //はずれてます.

    TRUE_STEP:
        //ここにきたら、ヒットしている.
        mov hit,1   //ヒットしているよ!!
    ENDPPROC:
        //おしまい
    }
    return hit;
/*
	short sa;
	sa = (short)(nx - gx);
	if (sa < 0)
	{
		if ( abs(sa) > nw + faj) return false;
	}
	else
	{
		if ( abs(sa) > gw + faj) return false;
	}

	sa = (short)(ny - gy);
	if (sa < 0)
	{
		if ( abs(sa) > nh + faj) return false;
	}
	else
	{
		if ( abs(sa) > gh + faj) return false;
	}
    return true;
*/
}
//***************************************************
//自機(nx,ny,nz)が敵(gx,gy,gz)とのあたり判定 faj は容認誤差
//***************************************************
bool HitCheck3D(int nx,int ny,int nz,
				int nw,int nh,int nd,
			    int gx,int gy,int gz,
				int gw,int gh,int gd,
				int faj)
{
	short sa;
	sa = (short)(nx - gx);
	if (sa < 0)
		if ( abs(sa) > nw + faj) return false;
	else
		if ( abs(sa) > gw + faj) return false;

	sa = (short)(ny - gy);
	if (sa < 0)
		if ( abs(sa) > nh + faj) return false;
	else
		if ( abs(sa) > gh + faj) return false;

    sa = (short)(nz - gz);
	if (sa < 0)
		if ( abs(sa) > nd + faj) return false;
	else
		if ( abs(sa) > gd + faj) return false;
	return true;
}
//***************************************************
//自機(nx,ny)が敵(gx,gy)とのあたり判定 faj は容認誤差
//***************************************************
bool HitCheckP(int nx,int ny,int gx,int gy,int faj)
{
/*
	short sa;
	sa = (short)(nx - gx);
	if (abs(sa) > faj) return false;

	sa = (short)(ny - gy);
	if (abs(sa) > faj) return false;
    return true;
*/
/*
	bool bret;
	_asm{
		mov bret,1

		mov eax,nx //sa = (short)(nx - gx);
		sub eax,gx
		;絶対値をもとめる    //abs(sa)
		test eax,0x80000000
		jz NON1
		neg eax
	NON1:
		cmp eax,faj
		ja FALSE_RETURN_RTN

		mov eax,ny //sa = (short)(ny - gy);
		sub eax,gy
		;絶対値をもとめる    //abs(sa)
		test eax,0x80000000
		jz NON2
		neg eax
	NON2:
		cmp eax,faj
		jbe TRUE_RETURN_RTN
FALSE_RETURN_RTN:
		mov bret,0
TRUE_RETURN_RTN:
	}
	return bret;
*/
	bool bret;
	_asm{
		mov bret,0

		mov eax,nx //sa = (short)(nx - gx);
		sub eax,gx
		;絶対値をもとめる    //abs(sa)
		jns NON1
		neg eax
	NON1:
		cmp eax,faj
		ja RETURN_RTN

		mov eax,ny //sa = (short)(ny - gy);
		sub eax,gy
		;絶対値をもとめる    //abs(sa)
		jns NON2
		neg eax
	NON2:
		cmp eax,faj
		ja RETURN_RTN
		mov bret,1
RETURN_RTN:
	}
	return bret;

}


//***************************************************
//自機(nx,ny,nz)が敵(gx,gy,gz)とのあたり判定 faj は容認誤差
//***************************************************
bool HitCheckP3D(int nx,int ny,int nz,
			    int gx,int gy,int gz,
				int faj)
{
	short sa;
	sa = (short)(nx - gx);
	if (abs(sa) > faj) return false;

	sa = (short)(ny - gy);
	if (abs(sa) > faj) return false;

    sa = (short)(nz - gz);
	if (abs(sa) > faj) return false;

	return true;
}

//***************************************************
//テクスチャーの辺 dwArg_c が 2の n乗になっているかどうかしらべて
//p に、 n乗の n の部分を返す
//***************************************************
BOOL CheckPow2(unsigned char *const p, unsigned long const dwArg_c)
{
	unsigned long dwWHSize,bFastBit1,dwBitNum,i;
	//マップが２のｎ乗になっているかどうかを調べる

	dwWHSize=dwArg_c;
	for(bFastBit1=dwBitNum=i=0 ; i<32 ; i++)
	{
		/*立っているビットの個数を数える*/
		if((dwWHSize&(1<<i)) == (unsigned long)(1<<i))
		{
			dwBitNum++;
			if(dwBitNum == 1)
			{
				bFastBit1=i;
			}
		}
	}
	if(dwBitNum != 1)
	{
		//マップが２のｎ乗ではない
		*p = 0;
		return(FALSE);
	}

	*p=(unsigned char)bFastBit1;
	return TRUE;
}

#endif //LIFE END

