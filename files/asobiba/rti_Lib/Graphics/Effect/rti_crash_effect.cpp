/*****************************************************************/
/*****************************************************************/
//　揺らす 未完成なり～
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"

#ifdef CRASH_LIFE

#define CRASH_RETURN_X  0x01
#define CRASH_RETURN_Y  0x02

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_crash_effect.h>

/******************************************************************/
//コンストラクタ
/******************************************************************/
TCrashEffect::TCrashEffect()
{
}

/******************************************************************/
//デストラクタ
/******************************************************************/
TCrashEffect::~TCrashEffect()
{
}

/******************************************************************/
//セット
/******************************************************************/
void TCrashEffect::Set(BMD _bmd,unsigned int _speedx,unsigned int _speedy,
                        unsigned int _shiftx,unsigned int _shifty)
{
    OutBmd = _bmd;
    Y = X = 0;
    Flg = 0;
    //マイナス方向へのシフトしか許可していないのでマイナスにする.
    ShiftX = - (int)_shiftx;
    ShiftY = - (int)_shifty;
	Speedx = - (int)_speedx;
	Speedy = - (int)_speedy;

}

/******************************************************************/
//クラッシュ (false だったらおしまい)
/******************************************************************/
bool TCrashEffect::Draw()
{
    bool ret = true;

    if ( CrashMove(&X,&Speedx,ShiftX,CRASH_RETURN_X) &
         CrashMove(&Y,&Speedy,ShiftY,CRASH_RETURN_Y)   ) ret = false;

    //ずらして絵画
    MoveDraw(OutBmd,X,Y);
    return ret;
}

/******************************************************************/
//クラッシュ移動
/******************************************************************/
bool TCrashEffect::CrashMove(int *_P,int *_Speed,
                             int _Shift,unsigned char ReturnFlg)
{
    //移動
    *_P += *_Speed;
    if (Flg & ReturnFlg)
    {
        //戻りだよーん
        if (*_P >= 0)
        {
            //Xの補正
            *_P = 0;
            return true;
        }
    }
    else
    {
        //行きだよーん
        if (*_P <= _Shift)
        {   //指定した位置をすぎたら.
            //反転
            *_Speed = - *_Speed;
            //戻りに入る
            Flg |= ReturnFlg;
            //Xの補正
            *_P = _Shift;
        }
    }
    return false;
}

/*****************************************************************/
//絵を指定した位置に移動させる 残りは空白で埋める.
/*****************************************************************/
void TCrashEffect::MoveDraw(BMD bmd1,int rx2,int ry2)
{

    int w = bmd1->w;
    int h = bmd1->h;
	int AddPitch1,AddPitch2;
	BYTE *data1,*data2;

    //トリアエズ、マイナス方向の揺れにしか対応してません m(__)m
    if (rx2 > 0 ) rx2 = 0;
    if (ry2 > 0 ) ry2 = 0;

    int sw,sh,ew,eh,rx1,ry1,rw1,rh1;
    rx1 = ry1 = 0;    rw1 = bmd1->w; rh1 = bmd1->h;
	if (!EasyClipingEx(bmd1,&rx1,&ry1,&rw1,&rh1,
						bmd1->w,bmd1->h,rx2,ry2,
						&sw,&sh,&ew,&eh) )			return;

	CommDraw->GraphicsLock(bmd1);
	CommDraw->GraphicsLock(bmd2);
	data1 = SeekData(bmd1,0,0, sw,sh,&AddPitch1);
	data2 = SeekData(bmd1,rx2,ry2, sw,sh,&AddPitch2);
    _asm{
        //できるだけ、 dword 単位で転送して、
        //端数は byte 単位で転送するための計算.
        mov eax,ew    //3*(ew)  X*3 = X<<2 -X
        mov ebx,eax
        shl eax,2
        sub eax,ebx   //eaxが答えです

        //ずらしたことによる余白のサイズを求める
        mov ebx,AddPitch1   
        sub ebx,eax         //これで、 ebx に余白のサイズかはいりました.

        //絵を DWORD BYTE に分離する.
        mov ecx,eax   //わり算とあまりが必要なので.. 
        shr ecx,2     //eax / 4 のこと. DWORD
        and eax,3     //eax % 4 のこと. BYTE

        //ずらしたヤツも効率のよい転送がしたいにょー
        //レジスターがたりないので EAX の上位を借りる(^^;;;; << あぶねー
        mov edx,ebx
        shr ebx,2     //ebx / 4 のこと. DWORD
        and edx,3     //ebx % 4 のこと. BYTE
        shl edx,16      //半分まで移動して
        or  eax,edx     //eax の上位を兼用する.
        //-----------EAX----------------
        //上位:余白のBYTE | 下位 絵のBYTE

        mov edx,eh
        mov esi,data1 ;転送元
        mov edi,data2 ;転送先
        cld
    LOOP1:              ;ループカウンタに edx を使用中
            push esi
            push ecx
            push eax
            //絵をまとめて転送します
            rep movsd       //DOWRD 転送
            mov ecx,eax
            and ecx,0x0000ffff  //上位をマスク
            rep movsb       //BYTE 転送
            //余白を書きます.
            mov ecx,eax
            shr ecx,16  //eax の上位に余白のバイトサイズがあるので.
            mov eax,0   //eax をクリア
            rep stosb       //都合により 先にBYTE転送.
            mov ecx,ebx
            rep stosd       //DWORD 転送.

            pop eax     //eax復帰
            pop ecx
            pop esi

            add esi,AddPitch2
        dec edx             ;LOOP1終了 EDX解放
        jnz  LOOP1

        //下の空白を埋めます.
        mov ecx,h       //絵の高さ
        sub ecx,eh      //未転送高さを求めます.
        imul ecx,AddPitch1  // それの長さを求めます.
        mov ebx,ecx   //わり算とあまりが必要なので.. 
        shr ecx,2     //ecx / 4 のこと.
        and ebx,3     //ecx % 4 のこと.

        mov eax,0
        rep stosd       //DWORD 転送

        mov ecx,ebx
        rep stosd       //BYTE 転送
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);

}


#endif  //LIFE END
