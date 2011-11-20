#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#ifndef GRAPHICS_INLINEH
#define GRAPHICS_INLINEH

#include <.\Graphics\rti_graphics.h>
#include <.\Comm\rti_Comm.h>

_GRAPHICS_BEGIN //namespace begin

//戻り値なしの警告を黙らせる. (eax で直接に返す)
#pragma warning(disable: 4035)

/*****************************************************************/
//そこに点をうつ
/*****************************************************************/
inline void PSet(BMD bmd1,int rx1,int ry1,long rgb)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    _asm{
		mov edi,bmd1
        mov eax,ry1
		mov ebx,[edi+BMD_W_POSSITION]	//w
		mov esi,[edi+BMD_BM_POSSITION]	//bm
        mul ebx
        add eax,rx1 ;(rx1+ry1*bmd1->w)

        mov edi,eax ;*3 は X<< 2 - X で計算する
        shl edi,2
        sub edi,eax

        add edi,esi   ;転送開始アドレス
        mov eax,[edi]
        mov ebx,rgb        ;あまりをくりぬく
        and eax,0xff000000
        and ebx,0x00ffffff ;不要な部分を切り落とす

        or  eax,ebx
        mov [edi],eax
    }
	CommDraw->GraphicsUnLock(bmd1);
}
/*****************************************************************/
//ダイレクトにそこに点をうつ
/*****************************************************************/
inline void DirectPSet(BYTE *data1,long rgb)
{
    _asm{
		mov edi,data1
        mov eax,[edi]
        mov ebx,rgb        ;あまりをくりぬく
        and eax,0xff000000
        and ebx,0x00ffffff ;不要な部分を切り落とす

        or  eax,ebx
        mov [edi],eax
    }
}

/*****************************************************************/
//ダイレクトにそこに点をうつ
/*****************************************************************/
inline void DirectPSet(BYTE *data1,BYTE *data2)
{
    _asm{
		mov edi,data1
		mov esi,data2
        mov ebx,[esi]
        mov eax,[edi]
        and ebx,0x00ffffff ;不要な部分を切り落とす
        and eax,0xff000000

        or  eax,ebx
        mov [edi],eax
    }
}

/*****************************************************************/
//ダイレクトにそこに点をうつ & 色補正
/*****************************************************************/
inline void DirectPSet(BYTE *data1,long rgb,int AddRGB)
{
    _asm{
		mov edi , data1		//データ書き込み先
		mov esi , rgb			//元の色
		mov ecx , AddRGB		//補正データ.
		mov edx , [edi]		//edx に元々のデータの読み込み(24Bitなので 3 バイト単位アクセスのため.)
		//eax ebx は、ワーク.
		// edx にためこんでいきます.

		//Rの処理
		mov eax , esi		//元
		mov ebx , ecx		//補正
		and edx , 0xff000000 //送り先のところで 3 バイト分(RGB)確保.

        and eax , 0x00ff0000  //元のデータの切り取り
        and ebx , 0x00ff0000  //補正のデータの切り取り
        add eax , ebx         //結合(補正)
        test eax, 0xff00ffff //オーバー補正.
        jz  Non_R_Over
            mov eax , 0x00ff0000  //255 越えているので押し戻す.
    Non_R_Over:
        or   edx , eax        //書き込み.

		//Gの処理
		mov eax , esi		//元
		mov ebx , ecx		//補正

        and eax , 0x0000ff00  //元のデータの切り取り
        and ebx , 0x0000ff00  //補正のデータの切り取り
        add eax , ebx         //結合(補正)
        test eax, 0xffff00ff //オーバー補正.
        jz  Non_G_Over
            mov eax , 0x0000ff00  //255 越えているので押し戻す.
    Non_G_Over:
        or   edx , eax        //書き込み.

		//Bの処理
		//最後なので、台帳の esi(元)と ecx(補正)をそのまま使う.
        and esi , 0x000000ff  //元のデータの切り取り
        and ecx , 0x000000ff  //補正のデータの切り取り
        add esi , ecx         //結合(補正)
        test esi, 0xffffff00 //オーバー補正.
        jz  Non_B_Over
            mov esi , 0x000000ff  //255 越えているので押し戻す.
    Non_B_Over:
        or   edx , esi        //書き込み.
        //こうして、作成した edx を書き戻す.
        mov [edi],edx
    }
}

/*****************************************************************/
//ダイレクトにそこに点をうつ & そこに色補正をかける.
/*****************************************************************/
inline void DirectPSet(BYTE *data1,BYTE *data2,int AddRGB)
{
    _asm{
        //転送元、転送先アドレスをレジスターへ
		mov edi,data1
		mov esi,data2
        //色補正データの読み込み
        mov ecx,AddRGB
        //転送もと、転送先から読み込み (転送先から読み込むのは、3バイト単位だから.)
        mov ebx,[esi]
        mov eax,[edi]
        //ebx に元のデータがはいってます.
        //ecx には、色補正データが入ってます.
        //eax は、補正したデータをためこんでいきます.
        //esi edx は、ワークエリア.

        //Rの処理.
        mov esi , ebx //元々のデータを入れる.
        mov edx , ecx //補正データを入れる.
        and eax , 0xff000000 //送り先のところで 3 バイト分(RGB)確保.
        
        and esi,0x00ff0000  //元のデータの切り取り
        and edx,0x00ff0000  //補正のデータの切り取り
        add esi,edx         //結合(補正)
        test esi,0xff00ffff //オーバー補正.
        jz  Non_R_Over
            mov esi,0x00ff0000  //255 越えているので押し戻す.
    Non_R_Over:
        or   eax,esi        //書き込み.
        //Gの処理.
        mov esi , ebx //元々のデータを入れる.
        mov edx , ecx //補正データを入れる.
        
        and esi,0x0000ff00  //元のデータの切り取り
        and edx,0x0000ff00  //補正のデータの切り取り
        add esi,edx         //結合(補正)
        test esi,0xffff00ff //オーバー補正.
        jz  Non_G_Over
            mov esi,0x0000ff00  //255 越えているので押し戻す.
    Non_G_Over:
        or   eax,esi        //書き込み.
        //Bの処理.
            //これが最後なので台帳の　元(ebx) 補正データ(ecx)を直接使う.
        and ebx,0x000000ff  //元のデータの切り取り
        and ecx,0x000000ff  //補正のデータの切り取り
        add ecx,ebx         //結合(補正)
        test ecx,0xffffff00 //オーバー補正.
        jz  Non_B_Over
            mov ecx,0x000000ff  //255 越えているので押し戻す.
    Non_B_Over:
        or   eax,ecx        //書き込み.
        //こうして、作成した eax を書き戻す.
        mov [edi],eax
    }
}

/*****************************************************************/
//そこの色を得る
/*****************************************************************/
inline long PGet(const BMD bmd1,int rx1,int ry1)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return 0;
    //もろくそ　きわどいプログラムでごめんなさいさい
    _asm{
		mov eax,bmd1 
        mov ecx,ry1
		mov ebx,rx1
		mov esi,[eax + BMD_W_POSSITION]	// w の位置
		mov edi,[eax + BMD_H_POSSITION]	// h の位置
		mov edx,[eax + BMD_BM_POSSITION]	// bm の位置

		mov eax,0			//戻り値として確保.
		//簡易クリッピング
		cmp esi,ebx	//wチェック
		jb	NO_GET
		cmp edi,ecx	//hチャック
		jb	NO_GET
		test ecx,0x80000000	//マイナスチェック(機種依存)
		jnz	NO_GET
		test ebx,0x80000000	//マイナスチェック(機種依存)
		jnz	NO_GET

        imul esi,ecx	//esi = ry1 * w
        add esi,ebx ;(rx1+ry1*bmd1->w)

        mov edi,esi ;*3 は X<< 2 - X で計算する
        shl edi,2
        sub edi,esi

        add edi,edx   ;転送開始アドレス
        mov eax,[edi]
        and eax,0x00ffffff
	NO_GET:
    }
	CommDraw->GraphicsUnLock(bmd1);
	return ;
}

/*****************************************************************/
//ダイレクトにそこの色を得る
/*****************************************************************/
inline long DirectPGet(const BYTE *data1)
{
//	long w;
    _asm{
		mov	edi,data1

        mov eax,[edi]
        and eax,0x00ffffff
//        mov w,eax
    }
//    return w;
}

/*****************************************************************/
//その地点のデータアドレスを取得
/*****************************************************************/
inline BYTE* SeekBmAddress(const BMD bmd1,int rx1,int ry1)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return 0;
	_asm
	{
		mov eax,bmd1 
		mov esi,rx1
        mov edi,ry1
		mov ecx,[eax + BMD_W_POSSITION]	// w の位置		
		mov ebx,[eax + BMD_H_POSSITION]	// h の位置		
		mov edx,[eax + BMD_BM_POSSITION]	// bm の位置

		//簡易クリッピング
		cmp ecx,esi	//wチェック
		jb	NO_GET
		cmp ebx,edi	//hチャック
		jb	NO_GET
		test esi,0x80000000	//マイナスチェック(機種依存)
		jnz	NO_GET
		test edi,0x80000000	//マイナスチェック(機種依存)
		jnz	NO_GET

		imul	edi,ecx		//esi = ry1 * w
		add		edi,esi		//(rx1+ry1*bmd1->w)

		mov		esi,edi		//*3 は X<< 2 - X で計算する
		shl		esi,2
		sub		esi,edi

        add esi,edx   ;転送開始アドレス
        mov eax,esi
	NO_GET:
	}
	CommDraw->GraphicsUnLock(bmd1);
	return ;
}

/*****************************************************************/
//その地点のデータアドレスを取得Sp
/*****************************************************************/
inline BYTE* SeekBmAddressSp(const BMD bmd1,int rx1,int ry1)
{
	if ( !CommDraw->GraphicsLock(bmd1) )	return 0;
	_asm
	{
		mov eax,bmd1 
		mov esi,rx1
        mov edi,ry1
		mov ecx,[eax + BMD_W_POSSITION]	// w の位置		
		mov ebx,[eax + BMD_H_POSSITION]	// h の位置		
		mov edx,[eax + BMD_BM_POSSITION]	// bm の位置

		//簡易クリッピング
		cmp ecx,esi	//wチェック
		jb	NO_GET
		cmp ebx,edi	//hチャック
		jb	NO_GET
		test esi,0x80000000	//マイナスチェック(機種依存)
		jnz	NO_GET
		mov ecx,[eax + BMD_POW2W_POSSITION]	// Pow2W の位置
		test edi,0x80000000	//マイナスチェック(機種依存)
		jnz	NO_GET

		shl		edi,cl
//		imul	edi,ecx		//esi = ry1 * w
		add		edi,esi		//(rx1+ry1*bmd1->w)

		mov		esi,edi		//*3 は X<< 2 - X で計算する
		shl		esi,2
		sub		esi,edi

        add esi,edx   ;転送開始アドレス
        mov eax,esi
	NO_GET:
	}
	CommDraw->GraphicsUnLock(bmd1);
	return ;
}

inline void AddColor(DWORD* ioRGB , DWORD inAddRGB)
{
	int theR,theG,theB;
	int theR2,theG2,theB2;

	theR = ((*ioRGB)& 0x00ff0000) >> 16;
	theG = ((*ioRGB)& 0x0000ff00) >>  8;
	theB = ((*ioRGB)& 0x000000ff) ;

	theR2 = ((inAddRGB) >> 16) & 0xff;
	theG2 = ((inAddRGB) >>  8) & 0xff;
	theB2 = ((inAddRGB)      ) & 0xff;

	theR += theR2;	theG += theG2;	theB += theB2;
	
	if (theR > 0xff) theR = 0xff;
	if (theG > 0xff) theG = 0xff;
	if (theB > 0xff) theB = 0xff;

	*ioRGB = (theR << 16) + (theG << 8) + (theB);
}

//戻り値なしの警告をしゃべらせる.
#pragma warning(error: 4035)

_GRAPHICS_END   //namespace end


#endif


#endif //LIFE END
