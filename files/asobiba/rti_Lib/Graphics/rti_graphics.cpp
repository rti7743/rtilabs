/*****************************************************************/
/*****************************************************************/
// RTI's  Graphics Lib ... (C) Copylight 1999 ... 
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Comm\rti_Comm.h>
#include <math.h>

_GRAPHICS_BEGIN //namespace begin

/*****************************************************************/
//簡易クリッピング
/*****************************************************************/
bool EasyClipingEx(BMD bmd,int *rx1,int *ry1,int *rw1,int *rh1,
				 int rw2,int rh2,int rx2,int ry2,
				 int *sw,int *sh,int *ew,int *eh)
{
    //簡易終了位置クリッピング
/*
    //マイナス方向だったら補正.
    if(*rx1 < 0)
    {
        *rw1 += *rx1;  // *rx1 はマイナス.
        *rx1 = 0;
    }
    //マイナス方向だったら補正.
    if(*ry1 < 0)
    {
        *rh1 += *ry1;  // *ry1 はマイナス.
        *ry1 = 0;
    }
    //画像の横幅より大きければ　没
    if(*rx1 >= bmd->w)  return false;
    //画像の高さより大きければ　没
    if(*ry1 >= bmd->h)  return false;
    
    int p;
    p = rw2 - rx2 ;
    if (p < rw1)    *ew = p;
    else            *ew = rw1;
    if (rx2 < 0 ) *sw = abs(rx2) , *ew = *ew - *sw;
    else          *sw = 0;

    p = rh2 - ry2 ;
    if (p < rh1)    *eh = p;
    else            *eh = rh1;
    if (ry2 < 0 ) *sh = abs(ry2) , *eh = *eh - *sh;
    else          *sh = 0;

    if (*ew <= 0 || *eh <= 0 ) return false;
	return true;
*/
    bool r = false;
    _asm
    {
		mov			edi,bmd
       //マイナス方向だったら補正.
	   mov         eax,dword ptr [rx1]
	   mov         ecx,dword ptr [rw1]
	   mov		   ebx,dword ptr [eax]	//rx1の中身
	   mov         edx,dword ptr [ecx]	//rw1の中身
  	   cmp         ebx,0
	   jge         NONE_RX1_HOSEI
			add			edx,ebx	//*rw1 += *rx1;  // *rx1 はマイナス.
			mov			ebx,0	//*rx1 = 0;
			mov			[ecx],edx
			mov			[eax],ebx
	NONE_RX1_HOSEI:
		//画像の横幅より大きければ　没
		//if(*rx1 >= bmd->w)  return false;
		mov ecx,[edi+BMD_W_POSSITION]	//機種依存!!	bmd->w
		cmp	ebx,ecx
		jge	END_PROC	//没

       //マイナス方向だったら補正.
	   mov         eax,dword ptr [ry1]
	   mov         ecx,dword ptr [rh1]
	   mov		   ebx,dword ptr [eax]	//ry1の中身
	   mov         edx,dword ptr [ecx]	//rh1の中身
  	   cmp         ebx,0
	   jge         NONE_RY1_HOSEI
			add			edx,ebx	//*rh1 += *ry1;  // *ry1 はマイナス.
			mov			ebx,0	//*ry1 = 0;
			mov			[ecx],edx
			mov			[eax],ebx
	NONE_RY1_HOSEI:
		//画像の横幅より大きければ　没
		//if(*rx1 >= bmd->h)  return false;
		mov ecx,[edi+BMD_H_POSSITION]	//機種依存!!	bmd->h
		cmp	ebx,ecx
		jge	END_PROC	//没


        //ワイドの ew
        mov eax,rw2 //eax は rw2
        mov ebx,rx2 //ebx は rx2
        mov ecx,rw1 // ecx を rw1 にする.
		mov ecx,[ecx]	//rw1 は ポインタなので実態化する
        sub eax,ebx // rw2 - rx2

        mov edi,ecx // ediは *ew のこと に rw1を入れる
        cmp eax,ecx //p と rw1 の比較
        jge W_NEXT1
            mov edi,eax //*ew に p を入れる
    W_NEXT1:

        //ワイドの sw
        mov esi,0 // esiは *sw のこと に 0を入れる
        test ebx,0x80000000 // rx2 がマイナス ? 
        jz  W_NEXT2        //マイナスぢゃないなら 0 でよいので抜ける
        neg  ebx            //rx2 の絶対値(正数にして)
            mov esi,ebx         //*sw に入れる
            sub edi,ebx         //*ew = *ew - *sw
    W_NEXT2:
        //メモリに書き戻し
        mov eax,ew
        mov ebx,sw
        mov dword ptr [eax],edi
        mov dword ptr [ebx],esi
            mov edx,edi  // 最後の比較用に ew を保存(edxは使われていないので)

        //ヘイトの eh
        mov eax,rh2 //eax は rh2
        mov ebx,ry2 //ebx は ry2
        mov ecx,rh1 // ecx を rh1 にする.
		mov ecx,[ecx]	//rh1 は ポインタなので実態化する
        sub eax,ebx // rh2 - ry2

        mov edi,ecx // ediは *eh のこと に rh1を入れる
        cmp eax,ecx //p と rh1 の比較
        jge H_NEXT1
            mov edi,eax //*eh に p を入れる
    H_NEXT1:

        //ヘイトの sh
        mov esi,0 // esiは *sh のこと に 0を入れる
        test ebx,0x80000000 // ry2 がマイナス ? 
        jz  H_NEXT2        //マイナスぢゃないなら 0 でよいので抜ける
        neg  ebx            //ry2 の絶対値(正数にして)
            mov esi,ebx         //*sh に入れる
            sub edi,ebx         //*eh = *eh - *sh
    H_NEXT2:
        //メモリに書き戻し
        mov eax,eh
        mov ebx,sh
        mov dword ptr [eax],edi
        mov dword ptr [ebx],esi

        //絵画していいのかなぁ...
	    //if (*ew <= 0 || *eh <= 0 ) return false;
        cmp edx,0
        jle END_PROC
        cmp edi,0
        jle END_PROC
            //成立した合図
            mov r,1
    END_PROC:
    }
    return r;            
}


/*****************************************************************/
//データの頭だし
/*****************************************************************/
BYTE *SeekData(BMD bmd,int rx,int ry,
			  int sw,int sh,int *lpAddPitch)
{
    *lpAddPitch = (bmd->w + bmd->w + bmd->w);
	return (bmd->bm+(rx+sw)*3+(ry+sh)* (*lpAddPitch) );

}




static void linerInterpolate(BMD bmd,Fixed x,Fixed y,int *rgb);
static BYTE linePol(int c1,int c2,int c3,int c4,Fixed xr,Fixed yr);



//そのグラフィックスを sizex sizey に切り裂いたときの個数
//と、Xの数、Yの数を CX,CYに返す
int GraphicsCutTile(BMD bmd,int sizex,int sizey,short *cx,short *cy)
{
	int t,TableSize;
    t = bmd->w / sizex;
    if (bmd->w % sizex) t ++;
    TableSize = (*cx) = t;

    t = bmd->h / sizey;
    if (bmd->h % sizey) t ++;
    TableSize *= (*cy = t);
	return TableSize;
}

/*****************************************************************/
//拡大縮小　線形補完
/*****************************************************************/
void BigDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int rw2,int rh2)
{
    int x,y;
    Fixed fx,fy;
    Fixed sfx,sfy;
    int ew,eh;
    int rgb;

	if (rw2 != 0)
	{
		sfx = FixedDiv( IF(rw1) , IF(rw2) ) ; //比率計算
	}
	else
	{
		sfx = 0;
	}
	if (rh2 != 0)
	{
	    sfy = FixedDiv( IF(rh1) , IF(rh2) ) ;
	}
	else
	{
		sfy = 0;
	}

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;

    ew = rw2-1;
    eh = rh2-1;
    for (y = 0 ,fy = ry1+1 ; y < eh-1 ;y ++ , fy += sfy)
    {
        for (x = 0 ,fx = rx1+1; x < ew-1 ;x ++ , fx += sfx)
        {
            //取得
            linerInterpolate(bmd1,fx,fy,&rgb ); 
            //転送
            PSet(bmd2,x+rx2,y+ry2,rgb);
        }
    }

	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

void linerInterpolate(BMD bmd,Fixed x,Fixed y,int *rgb)
{
    int px,py;
    Fixed xrate,yrate;
    BYTE r,g,b;
    long col1,col2,col3,col4;

    px =(int)Fixed2Int(x);
    py =(int)Fixed2Int(y);

    xrate = x-Int2Fixed(px);
    yrate = y-Int2Fixed(py);

    //色取得
    col1 = PGet(bmd,px+1,py-1);
    col2 = PGet(bmd,px+1,py+1);
    col3 = PGet(bmd,px-1,py-1);
    col4 = PGet(bmd,px-1,py+1);

    g = linePol( ((int)col1 & 0x000000ff) ,
                 ((int)col2 & 0x000000ff) ,
                 ((int)col3 & 0x000000ff) ,
                 ((int)col4 & 0x000000ff) ,
                 (xrate),(yrate));
    r = linePol( ((int)((col1 & 0x0000ff00) >> 8) ) ,
                 ((int)((col2 & 0x0000ff00) >> 8) ) ,
                 ((int)((col3 & 0x0000ff00) >> 8) ) ,
                 ((int)((col4 & 0x0000ff00) >> 8) ) ,
                 (xrate),(yrate) );
    b = linePol( ((int)((col1 & 0x00ff0000) >> 16) ) ,
                 ((int)((col2 & 0x00ff0000) >> 16) ) ,
                 ((int)((col3 & 0x00ff0000) >> 16) ) ,
                 ((int)((col4 & 0x00ff0000) >> 16) ) ,
                 (xrate),(yrate) );
    //とりあえず (^^;
    *rgb = RTIRGB(r,g,b);
}


BYTE linePol(int c1,int c2,int c3,int c4,Fixed xr,Fixed yr)
{
    Fixed d,e,f;
    d = ( (IF(1)-(xr)) * c1) + (xr * c3);
    e = ( (IF(1)-(xr)) * c2) + (xr * c4);
    f = FixedMul(d,(Int2Fixed(1)-yr) )+ FixedMul(e,yr);
    return (BYTE)( Fixed2Int(FixedRound(f)) );
}


/*****************************************************************/
//ぼかし
/*****************************************************************/
void ShadeDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

    _asm{
		dec ew ;こいつらはあらかじめ減算しておく
		dec eh
        mov edx,eh
        mov edi,data1 ;転送元
        mov esi,data2 ;転送先
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            push esi
            push edx  //ちょっと拝借 LOOP2の中で使うので..
            
            mov ecx,ew
//            xor bx,bx    ;中で作業用に使います LOOP2のことね
        LOOP2:           ;ループカウンタに ecx を使用中

//                mov edi,data1 ;転送元
//                mov esi,data2 ;転送先
				push ecx         ;ちょっと拝借

				//上
				mov edx,edi
				sub edx,AddPitch1
                mov eax,[edx] ;いっきにいただきます
                and eax,0x00ffffff ;不要な部分を切り落とす
                push eax
				//下
				mov edx,edi
				add edx,AddPitch1
                mov eax,[edx] ;いっきにいただきます
                and eax,0x00ffffff ;不要な部分を切り落とす
                push eax
				//左
				mov edx,edi
				sub edx,3
                mov eax,[edx] ;いっきにいただきます
                and eax,0x00ffffff ;不要な部分を切り落とす
                push eax
				//右
				mov edx,edi
				sub edx,3
                mov eax,[edx] ;いっきにいただきます
                and eax,0x00ffffff ;不要な部分を切り落とす
                push eax

                //0x000000ffについて計算
				//eaxは加算ようです
				pop eax ;右の要素
				pop ebx ;右の要素
                and eax,0x000000ff
                and ebx,0x000000ff
                add eax,ebx
				pop ebx ;右の要素
				pop edx ;右の要素  ;他の二つはあとで合成
                sub esp,16    ;4*4バイト(pop pop)戻しておく
                and ebx,0x000000ff
                and edx,0x000000ff
                add ebx,edx
				add eax,ebx		   ;合成
                shr eax,2          ;平均を出す
                and eax,0x000000ff ;念のため再クリッピング
                mov ecx,eax        ;ecxは合計ように拝借

                //0x0000ff00について計算
				//eaxは加算ようです
				pop eax ;右の要素
				pop ebx ;右の要素
                and eax,0x0000ff00
                and ebx,0x0000ff00
                add eax,ebx
				pop ebx ;右の要素
				pop edx ;右の要素  ;他の二つはあとで合成
                sub esp,16    ;4*4バイト(pop pop)戻しておく
                and ebx,0x0000ff00
                and edx,0x0000ff00
                add ebx,edx		   ;合成
				add eax,ebx		   ;合成
                shr eax,2          ;平均を出す
                and eax,0x0000ff00 ;念のため再クリッピング
                or  ecx,eax

                //0x00ff0000について計算
				//eaxは加算ようです
				pop eax ;右の要素
				pop ebx ;右の要素
                and eax,0x00ff0000
                and ebx,0x00ff0000
                add eax,ebx
				pop ebx ;右の要素
				pop edx ;右の要素  ;他の二つはあとで合成
                and ebx,0x00ff0000
                and edx,0x00ff0000
                add ebx,edx		   ;合成
				add eax,ebx		   ;合成
                shr eax,2          ;平均を出す
                and eax,0x00ff0000 ;念のため再クリッピング
                or  ecx,eax

                mov ebx,[esi]      ;あまりをくりぬく
                and ebx,0xff000000
                or  ebx,ecx

                mov [esi],ebx

                add edi,3
                add esi,3

				pop ecx         ;ちょっと拝借を戻す
            dec ecx
            cmp ecx,1
            ja  LOOP2       ;LOOP2終了 ECX解放

//            pop data2 ;データ復帰
//            pop data1
            pop edx
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y軸要素をプラス
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        cmp edx,1
        ja  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 1 ; y < eh-1 ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 1 ; x < ew-1 ;x ++ )
        {
            GetPoint = data1 - AddPitch1; //上
			B=  *(GetPoint);
			G=  *(GetPoint +1);
			R=  *(GetPoint +2);
            GetPoint = data1 + AddPitch1; //下
			B+=  *(GetPoint);
			G+=  *(GetPoint +1);
			R+=  *(GetPoint +2);
            GetPoint = data1 - 3;  //左
			B+=  *(GetPoint);
			G+=  *(GetPoint +1);
			R+=  *(GetPoint +2);
            GetPoint = data1 + 3; //右
			B+=  *(GetPoint);
			G+=  *(GetPoint +1);
			R+=  *(GetPoint +2);
			
			R = R >> 2;
			G = G >> 2;
			B = B >> 2;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
*/
}

/*****************************************************************/
//半透明
/*****************************************************************/
void Blend50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

    _asm{
        mov edx,eh
        mov edi,data1 ;転送元
        mov esi,data2 ;転送先
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            push esi
            push edx  //ちょっと拝借 LOOP2の中で使うので..
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

                mov eax,[edi] ;いっきにいただきます
                mov ebx,[esi]      ;
                test eax,0x00ffffff
                jz   NON_DRAW //透明は無視
				push ebx      //メモリアクセスはなるべくイヤ

                push eax
                push ebx

                //0x000000ffについて計算
                and eax,0x000000ff
                and ebx,0x000000ff
                add eax,ebx
                shr eax,1	;1バイトシフト / 2
                and eax,0x000000ff ;念のため再クリッピング
                mov edx,eax

                //0x0000ff00について計算
                pop ebx
                pop eax
                sub esp,8    ;4*2バイト(pop pop)戻しておく
                and eax,0x0000ff00
                and ebx,0x0000ff00
                add eax,ebx
                shr eax,1   ;1バイトシフト / 2
                and eax,0x0000ff00 ;念のため再クリッピング
                or edx,eax

                //0x00ff0000について計算
                pop ebx
                pop eax
                and eax,0x00ff0000
                and ebx,0x00ff0000
                add eax,ebx
                shr eax,1   ;1バイトシフト /2 
                and eax,0x00ff0000 ;念のため再クリッピング
                or edx,eax

				pop ebx;あまりをくりぬく
                and ebx,0xff000000
                or  ebx,edx

                mov [esi],ebx
NON_DRAW:

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

            pop edx
            pop esi
            pop edi
			//画像を次のラインへ
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
            /*
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);
			B += *(data2) ;
            G += *(data2 +1) ;
			R += *(data2 +2) ;
            R = R >> 1;
            G = G >> 1;
            B = B >> 1;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
            */

}

/*****************************************************************/
//半以上または以下透明
/*****************************************************************/
void BlendDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed fblend,Fixed fglend,Fixed frlend)
{
    int x,y;

    Fixed frlend2,fglend2,fblend2;
    fblend2 = Int2Fixed(1) - fblend;
    fglend2 = Int2Fixed(1) - fglend;
    frlend2 = Int2Fixed(1) - frlend;

	int AddPitch1,AddPitch2;
    int R,G,B;
    int R2,G2,B2;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);

			B2 = *(data2) ;
            G2 = *(data2 +1) ;
			R2 = *(data2 +2) ;
            B= (BYTE)Fixed2Int( ( fblend2*R2) + ( fblend*R) ) ;
            G= (BYTE)Fixed2Int( ( fglend2*G2) + ( fglend*G) ) ;
            R= (BYTE)Fixed2Int( ( frlend2*B2) + ( frlend*B) ) ;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//グラデーションブレンド
/*****************************************************************/
void BlendGradetionDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
						BMD bmd2,int rx2,int ry2,
						Fixed fblend,Fixed fglend,Fixed frlend,
						unsigned char mode,int start,int size)
{
    int x,y;

    Fixed frlend2,fglend2,fblend2;
    Fixed frlend3,fglend3,fblend3;
    fblend2 = Int2Fixed(1) - fblend;
    fglend2 = Int2Fixed(1) - fglend;
    frlend2 = Int2Fixed(1) - frlend;

	int Gr ;
	int AddPitch1,AddPitch2;
    int R,G,B;
    int R2,G2,B2;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

	frlend3 = frlend2;
	fglend3 = fglend2;
	fblend3 = fblend2;
    for (y = 0 ; y < eh ;y ++ )
    {
		frlend2 = frlend3;
		fglend2 = fglend3;
		fblend2 = fblend3;
		Gr = 0;
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);

			B2 = *(data2) ;
            G2 = *(data2 +1) ;
			R2 = *(data2 +2) ;
			if (x > start)
			{
				if (++Gr > size)
				{
					Gr = 0;
					frlend2 -= (2048);
					fglend2 -= (2048);
					fblend2 -= (2048);
					if (frlend2 < 0) frlend2 = 0;
					if (fglend2 < 0) fglend2 = 0;
					if (fblend2 < 0) fblend2 = 0;
				}
			}
		    frlend = Int2Fixed(1) - frlend2;
			fglend = Int2Fixed(1) - fglend2;
			fblend = Int2Fixed(1) - fblend2;

            B= (BYTE)Fixed2Int( (fblend2*R2) + (fblend*R) ) ;
            G= (BYTE)Fixed2Int( (fglend2*G2) + (fglend*G) ) ;
            R= (BYTE)Fixed2Int( (frlend2*B2) + (frlend*B) ) ;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}

/*****************************************************************/
//半明度調整
/*****************************************************************/
void Bright50Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//    int R,G,B;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*

    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;転送元
        mov esi,data2 ;転送先
    LOOP1:              ;ループカウンタに edx を使用中
//            push data1   ;データ待避
//            push data2
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

//                mov edi,data1 ;転送元
//                mov esi,data2 ;転送先
                
                mov eax,[edi] ;いっきにいただきます
                push eax

                //0x000000ffについて計算
                and eax,0x000000ff
                shr eax,1
                and eax,0x000000ff ;念のため再クリッピング
                mov ebx,eax

                //0x0000ff00について計算
                pop eax
                sub esp,4    ;4*1バイト(pop pop)戻しておく
                and eax,0x0000ff00
                shr eax,1   ;1バイトシフト
                and eax,0x0000ff00 ;念のため再クリッピング
                or ebx,eax

                //0x00ff0000について計算
                pop eax
                and eax,0x00ff0000
                shr eax,1   ;1バイトシフト
                and eax,0x00ff0000 ;念のため再クリッピング
                or ebx,eax

                mov eax,[esi]      ;あまりをくりぬく
                and eax,0xff000000
                or  eax,ebx

                mov [esi],eax

//                add data1,3
//                add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

//            pop data2 ;データ復帰
//            pop data1
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y軸要素をプラス
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
            /*
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);
			B += *(data2) ;
            G += *(data2 +1) ;
			R += *(data2 +2) ;
            R = R >> 1;
            G = G >> 1;
            B = B >> 1;
			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
            */

}


/*****************************************************************/
//明度調整
/*****************************************************************/
void BrightDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,
                      Fixed fblend,Fixed fglend,Fixed frlend)
{
    int x,y;

	int AddPitch1,AddPitch2;
    int R,G,B;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
			B=  *(data1);
			G=  *(data1 +1);
			R=  *(data1 +2);
            B= (fblend*R);
            G= (fglend*G);
            R= (frlend*B);

			*(data2) = B;
            *(data2 +1) = G;
			*(data2 +2) = R;
            data1 += 3 ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}


/*****************************************************************/
//加算アルファブレンド 半透明のシフト抜きの使い回し(劇汗)
/*****************************************************************/
void AddDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//    int R,G,B;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;転送元
        mov esi,data2 ;転送先
    LOOP1:              ;ループカウンタに edx を使用中
//            push data1   ;データ待避
//            push data2
            push edi
            push esi
            push edx  //ちょっと拝借 LOOP2の中で使うので..
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

//                mov edi,data1 ;転送元
//                mov esi,data2 ;転送先
                
                mov eax,[edi] ;いっきにいただきます
                mov ebx,[esi]      ;
                and eax,0x00ffffff ;不要な部分を切り落とす
                and ebx,0x00ffffff ;不要な部分を切り落とす
                push eax
                push ebx

                //0x000000ffについて計算
                and eax,0x000000ff
                and ebx,0x000000ff
                add eax,ebx
				cmp eax,0x000000ff
				jbe	None_Hosei1
					mov	eax,0x000000ff	//補正
			None_Hosei1:
                mov edx,eax

                //0x0000ff00について計算
                pop ebx
                pop eax
                sub esp,8    ;4*2バイト(pop pop)戻しておく
                and eax,0x0000ff00
                and ebx,0x0000ff00
                add eax,ebx
				cmp eax,0x0000ff00
				jbe	None_Hosei2
					mov	eax,0x0000ff00	//補正
			None_Hosei2:
                or edx,eax

                //0x00ff0000について計算
                pop ebx
                pop eax
                and eax,0x00ff0000
                and ebx,0x00ff0000
                add eax,ebx
				cmp eax,0x00ff0000
				jbe	None_Hosei3
					mov	eax,0x00ff0000	//補正
			None_Hosei3:
                or edx,eax

                mov ebx,[esi]      ;あまりをくりぬく
                and ebx,0xff000000
                or  ebx,edx

                mov [esi],ebx

//                add data1,3
//                add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

//            pop data2 ;データ復帰
//            pop data1
            pop edx
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y軸要素をプラス
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//減算アルファブレンド
/*****************************************************************/
void SubDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//    int R,G,B;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;


    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);

/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;転送元
        mov esi,data2 ;転送先
    LOOP1:              ;ループカウンタに edx を使用中
//            push data1   ;データ待避
//            push data2
            push edi
            push esi
            push edx  //ちょっと拝借 LOOP2の中で使うので..
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

//                mov edi,data1 ;転送元
//                mov esi,data2 ;転送先
                
                mov eax,[edi] ;いっきにいただきます
                mov ebx,[esi]      ;
                and eax,0x00ffffff ;不要な部分を切り落とす
                and ebx,0x00ffffff ;不要な部分を切り落とす
                push eax
                push ebx

                //0x000000ffについて計算
                and eax,0x000000ff
                and ebx,0x000000ff
                sub eax,ebx
                //マイナスにつっこんだときの補正
                jns None_Hosei1
                    mov     eax,0
            None_Hosei1:
                mov edx,eax

                //0x0000ff00について計算
                pop ebx
                pop eax
                sub esp,8    ;4*2バイト(pop pop)戻しておく
                and eax,0x0000ff00
                and ebx,0x0000ff00
                sub eax,ebx
                //マイナスにつっこんだときの補正
                jns None_Hosei2
                    mov     eax,0
            None_Hosei2:
                or edx,eax

                //0x00ff0000について計算
                pop ebx
                pop eax
                and eax,0x00ff0000
                and ebx,0x00ff0000
                sub eax,ebx
                //マイナスにつっこんだときの補正
                jns None_Hosei3
                    mov     eax,0
            None_Hosei3:
                or edx,eax

                mov ebx,[esi]      ;あまりをくりぬく
                and ebx,0xff000000
                or  ebx,edx

                mov [esi],ebx

//                add data1,3
//                add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

//            pop data2 ;データ復帰
//            pop data1
            pop edx
            pop esi
            pop edi

//            mov ebx,AddPitch1 ;Y軸要素をプラス
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//BitBlt の AND 効果 (^^;;  通過処理はなしです.
/*****************************************************************/
void AndDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        mov edx,eh
        mov edi,data1 ;転送
        mov esi,data2 ;転送
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

                mov eax,[edi] ;いっきにいただきます
                mov ebx,[esi]      ;転送先からゲット

                or eax,0xff000000 //転送先にある 上位8ビットを壊さないように
                and eax,ebx       // And 処理
                mov [esi],eax     //書き込み

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//BitBlt の OR(PAINT) 効果 (^^;;  通過処理はなしです.
/*****************************************************************/
void OrDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        mov edx,eh
        mov edi,data1 ;転送
        mov esi,data2 ;転送
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

                mov eax,[edi] ;いっきにいただきます
                mov ebx,[esi]      ;転送先からゲット

                and eax,0x00ffffff //転送先にある 上位8ビットを壊さないように
                or eax,ebx       // Or 処理
                mov [esi],eax     //書き込み

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//通過無視の絵画
/*****************************************************************/
void DirectDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int y;
//    int copysize;
//    int copysizemod;

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;

 
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/

//    copysize = 3*(ew) ;
//    copysizemod = copysize % 4;
//    copysize = copysize / 4;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    for (int y = eh ; y >= 0 ;y -- )
    {
        //最強の高速化!? ↓
        memcpy( (data2),(data1),3*(ew) );
		data1+=AddPitch1;
		data2+=AddPitch2;
    }
*/
  

    _asm{
        //できるだけ、 dword 単位で転送して、
        //端数は byte 単位で転送するための計算.
        mov eax,ew    //3*(ew)  X*3 = X<<2 -X
        shl eax,2
        sub eax,ew   //eaxが答えです

        mov ecx,eax   //わり算とあまりが必要なので.. 
        shr ecx,2     //eax / 4 のこと.
        and eax,3     //eax % 4 のこと.

        mov edx,eh
        mov esi,data1 ;転送元
        mov edi,data2 ;転送先
        cld
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            push esi
            push ecx
            //まとめて転送します
            rep movsd
            mov ecx,eax
            rep movsb

            pop ecx
            pop esi
            pop edi
            add esi,AddPitch1
            add edi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}




/*****************************************************************/
//反転絵画
/*****************************************************************/
void RevDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1)
{
//    int x,y;

	int AddPitch1;
//    BYTE *backupPoint;
	BYTE *data1;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx1,ry1,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    p = bmd1->w - rx1 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx1 < 0 ) sw = abs(rx1) , ew = ew - sw;
    else          sw = 0;

    p = bmd1->h - ry1 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry1 < 0 ) sh = abs(ry1) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
/*
    AddPitch1 = (bmd1->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
*/

    _asm{
        mov edx,eh
        mov edi,data1 ;転送元
        mov esi,AddPitch1 ;esiがあまっているので使う (^^;;
    LOOP1:              ;ループカウンタに edx を使用中
            push edi   ;データ待避
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中


                mov eax,[edi] ;いっきにいただきます
				mov ebx,eax   ;待避だよーん
				not eax       ;反転
                and eax,0x00ffffff ;ゼロってことは転送しなくてもよい
                and ebx,0xff000000
                or  ebx,eax
                mov [edi],ebx

                add edi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

            pop edi ;データ復帰

//            add edi,AddPitch1 ;Y軸要素をプラス
            add edi,esi ;Y軸要素をプラス
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 0 ; y < eh ;y ++ )
    {
        backupPoint = data1; //保存
        for (x = 0 ; x < ew ;x ++ )
        {
            *(data1) =  ~(*(data1)); //反転絵画
            *(data1+1) =  ~(*(data1+1)); //反転絵画
            *(data1+2) =  ~(*(data1+2)); //反転絵画
            data1 += 3;
        }
        data1 = backupPoint; //復帰
        data1 += AddPitch1;
    }
*/
}



/*****************************************************************/
//通過色ありで絵画
/*****************************************************************/
void Draw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
//    int x,y;

	int AddPitch1,AddPitch2;
//	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/
    _asm{
        mov edx,eh
        mov edi,data1 ;転送
        mov esi,data2 ;転送
    LOOP1:              ;ループカウンタに edx を使用中
//            push data1   ;データ待避
//            push data2
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

//                mov edi,data1 ;転送

                mov eax,[edi] ;いっきにいただきます

                and eax,0x00ffffff ;ゼロってことは転送しなくてもよい
                jz NON_PROC        ;andはフラグが変化するのでcmp抜き 

                ;ここにきたということは透明色ではないので転送
//                mov esi,data2 ;転送

                mov ebx,[esi]      ;あまりをくりぬく
                and ebx,0xff000000
                or  ebx,eax

                mov [esi],ebx

            NON_PROC: ;透明色だったりしたらここにくるかも

//              add data1,3
//              add data2,3
                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

            pop esi
            pop edi
//            pop data2 ;データ復帰
//            pop data1

//            mov ebx,AddPitch1 ;Y軸要素をプラス
//            mov ecx,AddPitch2
//            add data1,ebx
//            add data2,ecx
            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 0 ; y < eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        for (x = 0 ; x < ew ;x ++ )
        {
            if (   ( ((DWORD)*((DWORD*)(data1))) & 0xffffff00 )  )
            {
//				*(data2) = *(data1);
//				*(data2+1) = *(data1+1);
//				*(data2+2) = *(data1+2);
				memcpy(data2,data1,3);
            }
            data1+=3;
            data2+=3;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
*/
}

/*****************************************************************/
//クリア
/*****************************************************************/
void Cls(BMD bmd,BYTE clear)
{
	if ( !CommDraw->GraphicsLock(bmd) )	return;
	BYTE *dest = bmd->bm;
	int count = (bmd->w * bmd->h);
	count = count + count + count;	//count = (bmd->w * bmd->h) * 3 の掛け算は足し算で.

	int c = clear;
	_asm
	{
		mov edi,dest
		mov edx,count
		//eax に c c c c と配置してあげます.
		mov ebx,c
		mov eax,ebx	//初期は移動で転送ねっ
		shl ebx,8	//4ずらして
		or  eax,ebx //結合
		shl ebx,8	//以下同文
		or  eax,ebx
		shl ebx,8
		or  eax,ebx
		//ここで eax は c c c c となっているはずです.

		//では、転送の始まりです.
		cld	//正方向転送
		mov ecx,edi	//4 で割り切れるかどうか調べます.
		and ecx,3	// dest のアドレス & 3 
		sub edx,ecx	//転送するバイト数からその分を引く
		js	END_OF_RTN
		
		rep stosb
		//現在は 4 で割り切れるアドレスにいます.
		//ここから、count / 4 の値を DWORD 単位で転送します.
		mov ecx,edx
		mov ebx,ecx
		shr ecx,2	// / 2 のことです.
		and ebx,3	// & 3 のことです.
		rep stosd	// DWORD 転送
		mov ecx,ebx
		rep stosb	//BYTE転送
	END_OF_RTN:
	}
 
	CommDraw->GraphicsUnLock(bmd);
}

/*****************************************************************/
//線を描く
/*****************************************************************/
void Line(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb)
{
	int AddPicth;
	BYTE *data1;
    int w,h;

    w = bmd1->w;
    h = bmd1->h;
    //クリッピング
    if (rx1 < 0) rx1 = 0;
    if (ry1 < 0) rx1 = 0;
    if (rx1 > w) rx1 = w;
    if (ry1 > h) rx1 = h;
    if (rx2 < 0) rx2 = 0;
    if (ry2 < 0) rx2 = 0;
    if (rx2 > w) rx2 = w;
    if (ry2 > h) rx2 = h;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    data1 = bmd1->bm;
    AddPicth = bmd1->w * 3;

    int dx,dy ,sx,sy,x,y;
    int i,e;

    dx = abs(rx2 - rx1);                   // 変化量 X
    dy = abs(ry2 - ry1);                   // 変化量 Y
    sx = (rx2 - rx1) >= 0 ? 1 : -1;        // 正:1 負:-1 零:0
    sy = (ry2 - ry1) >= 0 ? 1 : -1;        // 正:1 負:-1 零:0
    
    x = rx1;
    y = ry1;

    if (dy < dx)
    {                    // 点線にしない為
        //X 方向 を 増加する場合
        e = -dx;
        for( i = 0 ; i < dx ; i++)
        {
            //PSet (x * tx, y * ty), QBColor(c)
            PSet(bmd1  , x , y , rgb );
            x = x + sx;                 // X 方向
            e = e + 2 * dy;             // DDA による補間
            if (e >= 0) 
            {
                y = y + sy ;            // Y方向
                e = e - 2 * dx;         // DDA による補間
            }
        }
    }
    else
    {
        // Y 方向 を 増加する場合
        e = -dy;
        for( i = 0 ; i < dy ; i ++)
        {
            //PSet (x * tx, y * ty), QBColor(c)
            PSet(bmd1  , x , y , rgb );
            y = y + sy;                  // Y 方向
            e = e + 2 * dx;              // DDA による補間
            if (e >= 0) 
            {
                x = x + sx ;             // X 方向
                e = e - 2 * dy;          // DDA による補間
            }
        }
    }

	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//四角を描く
/*****************************************************************/
void Box(BMD bmd1,int rx1,int ry1,int rx2,int ry2,long rgb)
{
    int AddPitch1;
	BYTE *backupdata;
	BYTE *data1;
    int w,h;
    int ew,eh;
    int x,y;

    w = bmd1->w;
    h = bmd1->h;
    rx1--; ry2 ++;
    //クリッピング
    if (rx1 < 0) rx1 = 0;
    if (ry1 < 0) ry1 = 0;
    if (rx1 > w) rx1 = w;
    if (ry1 > h) ry1 = h;
    if (rx2 < 0) rx2 = rx1;
    if (ry2 < 0) ry2 = ry1;
    if (rx2 > w) rx2 = w;
    if (ry2 > h) ry2 = h;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    AddPitch1 = (w * 3);
	data1= (bmd1->bm+(rx1*3)+(ry1*AddPitch1) );
//    rgb = (rgb << 8); //あらかじめシフトしておく
    ew = abs(rx2 - rx1); eh = abs(ry2 - ry1);

    for (y = 0 ; y < eh ; y ++)
    {
        backupdata = data1;
        for (x = 0 ; x < ew ; x ++)
        { 
//            PSet(bmd1,x+rx1,y+ry1,rgb);
			DirectPSet(data1,rgb);
            data1+=3;
        }
        data1 = backupdata;
        data1 += AddPitch1;
    }
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//円を描く
/*****************************************************************/
void Circle(BMD bmd1,int rx1,int ry1,int rr1,long rgb)
{

    int r;
    int x,y;

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
    for (r = 0 ; r < 1024 ; r++)
    {
        x = (int)(LTBSin1024[r]*rr1 +rx1);
        y = (int)(LTBCos1024[r]*rr1 +ry1);
        //クリッピング (^^;;
        if (x >= 0 && x <= bmd1->w && y >= 0 && y <= bmd1->h)
        {
            PSet(bmd1,x,y,rgb);
        }
    }
	CommDraw->GraphicsUnLock(bmd1);
}


/*****************************************************************/
//左右上下反転
/*****************************************************************/
void MillorDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,bool lr,bool ud)
{
    int x,y;

	int AddPitch1,AddPitch2;
	BYTE *BackupPoint1,*BackupPoint2;
	BYTE *data1,*data2;
    int countX; // X に1進んだときのアドレスの移動量
    int lr_millor_start_end_x;//左右反転の場合は始まりは端から

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    rh1 --; rw1 --;
    p = bmd2->w - rx2 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx2 < 0 ) sw = abs(rx2) , ew = ew - sw;
    else          sw = 0;

    p = bmd2->h - ry2 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry2 < 0 ) sh = abs(ry2) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
/*
    AddPitch1 = (bmd1->w * 3);
    AddPitch2 = (bmd2->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
	data2= (bmd2->bm+(rx2+sw)*3+(ry2+sh)*AddPitch2 );
*/

    if (lr)
    {
        countX = -3 ;
        lr_millor_start_end_x = AddPitch1-3;
    }
    else
    {
        countX = 3;
        lr_millor_start_end_x = 0; 
    }
    if (ud)
    { //上下反転は、画像アドレスを最下層のYに移動させてからすーたと
        data1 += eh * AddPitch1; 
        //上下反転のために符号反転
        AddPitch1 = -AddPitch1;
    }

    for (y = 0 ; y <= eh ;y ++ )
    {
        BackupPoint1 = data1;
        BackupPoint2 = data2;
        data1 += lr_millor_start_end_x;
        for (x = 0 ; x <= ew ;x ++ )
        {
			*(data2) = *(data1);
            *(data2 +1) = *(data1+1);
			*(data2 +2) = *(data1+2);
            data1 += countX ;
            data2 += 3 ;
        }
        data1 = BackupPoint1 ;
        data2 = BackupPoint2 ;
        data1 += AddPitch1 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}

/*****************************************************************/
//指定した範囲にモザイクをかける
/*****************************************************************/
void MosaicDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,int mw,int mh)
{
//    int x,y;
//    int wx,wy;
    int mwskip,mhskip;

	int AddPitch1;
//    BYTE *backupPoint;
//    BYTE *mbackupPoint;
//    BYTE *workbackupPoint;
	BYTE *data1;
//    BYTE R,G,B;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx1,ry1,
						&sw,&sh,&ew,&eh) )			return;
/*
    //簡易終了位置クリッピング
    int sw,sh,ew,eh,p;
    rh1 --; rw1 --;
    p = bmd1->w - rx1 ;
    if (p < rw1)    ew = p;
    else            ew = rw1;
    if (rx1 < 0 ) sw = abs(rx1) , ew = ew - sw;
    else          sw = 0;

    p = bmd1->h - ry1 ;
    if (p < rh1)    eh = p;
    else            eh = rh1;
    if (ry1 < 0 ) sh = abs(ry1) , eh = eh - sh;
    else          sh = 0;

    if (ew <= 0 || eh <= 0 ) return;
*/

	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
/*
    AddPitch1 = (bmd1->w * 3);
	data1= (bmd1->bm+(rx1+sw)*3+(ry1+sh)*AddPitch1 );
*/
    mwskip = mw * 3;
    mhskip = mh * AddPitch1;
    int mwstep ,mhstep;


    _asm{
        mov edx,0
        mov edi,data1 ;転送
    LOOP1:              ;ループカウンタに edx を使用中
//            push data1   ;データ待避
            push edi   ;データ待避
            
            mov ecx,0
            xor bx,bx    ;中で作業用に使います LOOP2のことね
        LOOP2:           ;ループカウンタに ecx を使用中

                ;範囲オーバーは許さない
//                if ( (mw + x) >= ew)     mwstep = (mw+x)-ew;
//                else                     mwstep = mw;
                mov eax,mw
                add eax,ecx  ;ecx は xのことです
                cmp eax,ew
                jb ELSE1
                    sub eax,ew
                    jmp ENDIF1
                ELSE1:
                    mov eax,mw
            ENDIF1:
                mov mwstep,eax

                ;ここより第2のif文です
//                if ( (mh + y) >= eh)     mhstep = (mh+y)-eh;
//                else                     mhstep = mh;
                mov eax,mh
                add eax,edx  ;edx は yのことです
                cmp eax,eh
                jb ELSE2
                    sub eax,eh
                    jmp ENDIF2
                ELSE2:
                    mov eax,mh
            ENDIF2:
                mov mhstep,eax

                ;範囲オーバーチェック終了

//                mov edi,data1 ;転送元
                mov eax,[edi] ;いっきにいただきます
                and eax,0x00ffffff ;不要なデータの切り落とし

                ;ループカウンターのちょっと拝借
                push edx
                push ecx
//                push data1
                push edi


                //eax の中身を書きまくります
                //内部ループスタート
                mov edx,0
                LOOP3:              ;ループカウンタに edx を使用中
//                    push data1   ;データ待避
                    push edi   ;データ待避
            
                    mov ecx,0
                    LOOP4:           ;ループカウンタに ecx を使用中

                        ;以前に用意した eaxの内容を書きまくる
//                        mov edi,data1 ;転送
                        mov ebx,[edi]      ;あまりをくりぬく
                        and ebx,0xff000000
                        or  ebx,eax
                        mov [edi],ebx

//                        add data1,3
                        add edi,3
                    inc ecx
                    cmp ecx,mwstep
                    jb  LOOP4       ;LOOP2終了 ECX解放

//                    pop data1         ;データを復帰
                    pop edi         ;データを復帰

//                    mov ebx,AddPitch1 ;Y軸要素をプラス
//                    add data1,ebx
                    add edi,AddPitch1
                inc edx             ;LOOP1終了 EDX解放
                cmp edx,mhstep
                jb  LOOP3
                ;内部ループ終了
                ;ループカウンターのちょっと拝借を戻す

//                pop data1
                pop edi
                pop ecx
                pop edx

//                mov eax,mwskip
//                add data1,eax
                add edi,mwskip
            add ecx,mw
            cmp ecx,ew
            jb  LOOP2       ;LOOP2終了 ECX解放

//            pop data1 ;データ復帰
            pop edi ;データ復帰

//            mov eax,mh
//            mov ebx,mhskip ;Y軸要素をプラス
//            add data1,ebx
            add edi,mhskip
        add edx,mh             ;LOOP1終了 EDX解放
        cmp edx,eh
        jb  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd1);
/*
    for (y = 0 ; y <= eh ;y +=mh )
    {
        backupPoint = data1; //保存
        for (x = 0 ; x <= ew ;x +=mw )
        {
            R =*(data1);
            G =*(data1+1);
            B =*(data1+2);
            mbackupPoint = data1; //保存
            if ( (mw + x) >= ew)     mwstep = (mw+x)-ew;
            else                     mwstep = mw;
            if ( (mh + y) >= eh)     mhstep = (mh+y)-eh;
            else                     mhstep = mh;
            //モザイク処理ぃぃぃ
            for (wy = 0 ; wy < mhstep ;wy ++ )
            {
                workbackupPoint = data1; //保存
                for (wx = 0 ; wx < mwstep ;wx ++ )
                {
                    *(data1) =   R; //
                    *(data1+1) = G; //
                    *(data1+2) = B; //
                    data1 += 3;
                }
                data1 = workbackupPoint; //復帰
                data1 += AddPitch1;
            }

            data1 = mbackupPoint; //復帰
            data1 += mwskip ;
        }
        data1 = backupPoint; //復帰
        data1 += mhskip;
    }
*/
}






/*****************************************************************/
//フィルタ 指定した範囲にフィルタをかける
/*****************************************************************/
void FilterDraw(BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      int R,int G,int B)
{
	int AddPitch1;
	BYTE *data1;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx1,ry1,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);

    int RAdd,GAdd,BAdd;
    RAdd = (R<<16);
    GAdd = (G<<8 );
    BAdd = (B    );

    _asm{

        mov edx,eh
        mov edi,data1 ;転送
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

                mov eax,[edi] ;いっきにいただきます
                mov esi,eax //esi が台帳
                and eax,0xff000000 //ここからeax にどんどん足し込んでいく.

                //Rの処理
                mov ebx,esi
                and ebx,0x00ff0000
                add ebx,RAdd
                js R_END_STEP      //マイナスにつっこでいるのでなにもしない(初期が0なので 0 をわざわざ代入する必要なし)

                test ebx,0xff00ffff
                jz R_WRITE_STEP      //書き込み準備
                    mov ebx,0x00ff0000 //丸める
            R_WRITE_STEP:
                or eax,ebx // R の書き込む
            R_END_STEP: //Rの処理の終了

                //Gの処理
                mov ebx,esi
                and ebx,0x0000ff00
                add ebx,GAdd
                js G_END_STEP      //マイナスにつっこでいるのでなにもしない(初期が0なので 0 をわざわざ代入する必要なし)

                test ebx,0xffff00ff
                jz G_WRITE_STEP      //書き込み準備
                    mov ebx,0x0000ff00 //丸める
            G_WRITE_STEP:
                or eax,ebx // G の書き込む
            G_END_STEP: //Rの処理の終了

                //Bの処理
                mov ebx,esi
                and ebx,0x000000ff
                add ebx,BAdd
                js B_END_STEP      //マイナスにつっこでいるのでなにもしない(初期が0なので 0 をわざわざ代入する必要なし)

                test ebx,0xffffff00
                jz B_WRITE_STEP      //書き込み準備
                    mov ebx,0x000000ff //丸める
            B_WRITE_STEP:
                or eax,ebx // G の書き込む
            B_END_STEP: //Rの処理の終了

                //メモリに書き戻す
                mov [edi],eax

                add edi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

            pop edi
            add edi,AddPitch1
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd1);

}

/*****************************************************************/
//シフト絵画 青がシフトの要素です. (EL は赤だけどね )
// また、 S は、 0 ～ 31 の値をとります.
// それ以上はだめ
/*****************************************************************/
void ShiftDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int S)
{

	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;
    //簡易クリッピング
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        mov ebx,eh     // 内部でかけ算を使うために ebx にしている
                       //紛らわしいが、ゆるしてちょ
        mov edi,data1 ;転送
        mov esi,data2 ;転送
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            push esi
            
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

                mov eax,[edi] ;いっきにいただきます
                and eax,0x000000ff //不要な部分を殺す
                jz END_STEP        //黒は無視

                sub eax,S     //シフト値を減算する.
                js  END_STEP       //負の数だったら無視

                cmp eax,ebx  //eax - eh(の残り)
                ja  END_STEP // > だったら　無視

                mul AddPitch2
                add eax,esi     // eax の部分ずらす

                mov edx,[eax]      //絵画部分を取得.
                and edx,0x00ffffff //不要な部分を落とす

                mov eax,[esi]      //書き込む部分を取得
                and eax,0xff000000 
                or  eax,edx        //くりぬき合成
                mov [esi],eax      //絵画

            END_STEP:

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec ebx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//回転
/*****************************************************************/
void RotateDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,Angle seeta)
{
    int x,y;
    int rx,ry;
	int tx,ty;
    int min_x,max_x,min_y,max_y;
	Fixed Sin,Cos;	//サインコサインのテンポラリだにょー
	Fixed RSin,RCos;//サインコサインのテンポラリだにょー

	int AddPitch1,AddPitch2;
	BYTE *BackupPoint2;
	BYTE *data1,*data2;

	//サイン コサインを取得!!
	Sin = FixedLTBSin1024[ __ANGLE(seeta) ];
	Cos = FixedLTBCos1024[ __ANGLE(seeta) ];
	//逆サイン 逆コサインを取得!!
	RSin = FixedLTBSin1024[__ANGLE_REV(seeta)];
	RCos = FixedLTBCos1024[__ANGLE_REV(seeta)];
    //テクスチャの各頂点を jiku を中心に回転させますぅ.
	//これによって、最大、最低の転送範囲を取得しますぅ.
	/*
		min
		------------------------
        |                      |
        |                      |
        |          jiku        |
        |                      |
        |                      |
        |                      |
		------------------------
								max
    */
	//左上
    tx = x = rx1 - jikux;
    ty = y = ry1 - jikuy;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    min_x = max_x = rx ;
    min_y = max_y = ry ;
	//右上
    tx = x + rw1;
    ty = y ;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    if (min_x > rx) min_x = rx;
    if (max_x < rx) max_x = rx;
    if (min_y > ry) min_y = ry;
    if (max_y < ry) max_y = ry;
	//左下
    tx = x ;
    ty = y + rh1;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    if (min_x > rx) min_x = rx;
    if (max_x < rx) max_x = rx;
    if (min_y > ry) min_y = ry;
    if (max_y < ry) max_y = ry;
	//右下
    tx = x + rw1;
    ty = y + rh1;
	rx = FI(tx*Cos-ty*Sin);
	ry = FI(tx*Sin+ty*Cos);
    if (min_x > rx) min_x = rx;
    if (max_x < rx) max_x = rx;
    if (min_y > ry) min_y = ry;
    if (max_y < ry) max_y = ry;
    //min から初めて　max までデータを転送します.
    //でも、その前に　受け取る側の開始アドレスを求めます.
    //min max シリーズは軸座標です.
    //それを加えて(ほどんど　マイナスなので引き算だけど)転送開始場所もとめ.
    x = rx2 + min_x;    
    y = ry2 + min_y;    
	max_x ++ ; max_y++;

	//クリッピング
	if (x < 0)
	{		x = 0;		min_x = -rx2;	}
	//クリッピング
	if (y < 0)
	{		y = 0;		min_y = -ry2;	}
	//クリッピング
	if (x+(max_x - min_x) > bmd2->w)
	{		max_x = bmd2->w - x + min_x - 1;	}
	if (y+(max_y - min_y) > bmd2->h)
	{		max_y = bmd2->h - y + min_y - 1;	}
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	//転送開始アドレス取得
	data1 = SeekData(bmd1,rx1,ry1, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,x,y, 0,0,&AddPitch2);

	//デバッグ用(転送範囲を白いボックスで塗りつぶす)
//	Box(Sprite,rx2 + min_x,ry2 + min_y,rx2 + max_x,ry2 + max_y,0xffffff);
	Fixed Xx,Xy;
	Fixed Yx,Yy;
	//初期値を求める.
	Yx = (min_x * RCos)-(min_y * RSin);
	Yy = (min_x * RSin)+(min_y * RCos);
	//テクスチャを横断して画面に水平に貼り付けます.
	//こうすることによって、付加がぐびびびびびびひびびびびびびびっと、減ります.
	//科学の勝利です.(意味なし)
	/*

		Texter             Screen
	|---------------|      |-----------------------|
    |  -^           |      |   ------------------> |
    | |             |  ->  |   ------------------> |
    |-              |      |   ------------------> |
    |---------------|      |                       |
     斜めに横断            |                       |
                           -------------------------
						   まっすぐ絵画
    */
	//転送開始
    for (y = min_y ; y <= max_y ;y ++ )
    {
		Xx = Yx;
		Xy = Yy;
        BackupPoint2 = data2;
        for (x = min_x ; x <= max_x ;x ++ )
        {
			//回転.
			rx = FI(Xx) +jikux;
			ry = FI(Xy) +jikuy;
    		//クリッピング処理
			if (rx>=0 && rx<rw1 && ry>=0 && ry<rh1 )
            {
		    //転送
				DirectPSet(data2 , data1+(rx+rx+rx)+ry*AddPitch1);
            }
			Xx += RCos;
			Xy += RSin;
            data2 += 3 ;
        }
		Yx -= RSin;
		Yy += RCos;
        data2 = BackupPoint2 ;
        data2 += AddPitch2 ;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}


/*****************************************************************/
//3D回転
/*****************************************************************/
void Rotate3DDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2,int jikux,int jikuy,int jikuz,
					  unsigned char seetax,unsigned char seetay,unsigned char seetaz)
{
}

/*****************************************************************/
//グレイスケールで絵画
/*****************************************************************/
void GrayScaleDraw(const BMD bmd1,int rx1,int ry1,int rw1,int rh1,
                      BMD bmd2,int rx2,int ry2)
{
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    int sw,sh,ew,eh;

 
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd2->w,bmd2->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;
	if ( !CommDraw->GraphicsLock(bmd1) )	return;
	if ( !CommDraw->GraphicsLock(bmd2) )	return;
	data1 = SeekData(bmd1,rx1,ry1, sw,sh,&AddPitch1);
	data2 = SeekData(bmd2,rx2,ry2, sw,sh,&AddPitch2);
	/*
	*	RGB to YIQ カラー	(C MAGAZIN 2000-2月号 アルゴリズムラボより)
	*
	*  Y     (0.299   0.587   0.114)(R)
	*  I  =  (0.596  -0.274  -0.322)(G)
	*  Q     (0.211  -0.522   0.322)(B)
	*
	*  で、グレースケールにするには、 Y だけを求めればよいそうなので(笑)
	*  Y = 0.299 * R + 0.587 * B + 0.114 * G をやります.
	*  このとき、浮動小数点がでるといやなので、全体を * 256 倍します.
	*  Y = (77 * R + 28 * B  + 151 * G ) / 256 
	*  最後の 256 は、>> 8 で代用するとして(笑)  
	*  この計算式によって Y 成分を求めて、グレースケールにします.
	*/
    _asm{
        mov edx,eh
        mov edi,data1 ;転送
        mov esi,data2 ;転送
    LOOP1:              ;ループカウンタに edx を使用中
            push edi
            push esi
			push edx	;ループカウンター edx をちょっと拝借します.      
            mov ecx,ew
        LOOP2:           ;ループカウンタに ecx を使用中

                mov eax,[edi] ;いっきにいただきます
				//現在自由に使えるのは、 eax ebx edx の二つだけです. (厳しいにょー)
				// eax は、(元)オリジナルの保存.
				// ebx は、ワークエリア.
				// edx は、計算結果(Y要素)をためてきます.

				//B を求めます.
				mov ebx,eax				//とりあえず、ebx (計算領域に移動.)
				and ebx,0x000000ff		//不要な部分を切り落とします.
				imul edx,ebx,28				//Bの計算.//edx にどんどん要素をためていきます.

				//Gを求めます.
				mov ebx,eax				//とりあえず、ebx (計算領域に移動.)
				and ebx,0x0000ff00		//不要な部分を切り落とします.
				shr ebx,8
				imul ebx,151			//Gの計算.
				add  edx,ebx			//edx にどんどん要素をためていきます.
				//Rを求めます.
				mov ebx,eax				//とりあえず、ebx (計算領域に移動.)
				and ebx,0x00ff0000		//不要な部分を切り落とします.
				shr ebx,16
				imul ebx,77				//Rの計算.
				add  edx,ebx			//edx にどんどん要素をためていきます.

				shr  edx,8				//最後に / 256 します.
				mov  eax,edx
				shl  eax,8
				or   edx,eax
				shl  eax,8
				or   edx,eax

                and edx,0x00ffffff ;要らないところをくり貫く.
                mov ebx,[esi]       //先からゲット.
                and ebx,0xff000000	//先のところで書いてはいけないところをクリア.
                or  ebx,edx			//結合
                mov [esi],ebx		//書き込み

                add edi,3
                add esi,3
            dec ecx
            jnz  LOOP2       ;LOOP2終了 ECX解放

			pop edx			 ;ループカウンター edx を拝借していたのをもとに戻します.
            pop esi
            pop edi

            add edi,AddPitch1
            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

_GRAPHICS_END   //namespace end


#endif //LIFE END
