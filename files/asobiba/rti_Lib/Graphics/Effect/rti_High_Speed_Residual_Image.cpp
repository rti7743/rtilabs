#include "StartWithExeFile.h"
#ifdef HIGH_SPEED_RESIDUAL_IMAGE_LIFE
/***************************************************************/
//高速残像処理  (C) Copylight R.T.I  since 2000...
//超高速な残像処理を行いますです.
/***************************************************************/


#include <.\Base\rti_debug.h>
#include <.\Base\rti_error_message.h>
#include <.\Graphics\Effect\rti_High_Speed_Residual_Image.h>
#include <.\Comm\rti_Comm.h>

/***************************************************************/
//コンストラクタ
/***************************************************************/
THighSpeedResidualImageEffect::THighSpeedResidualImageEffect()
{
	//念のため初期化しておきます.
	BufferUse = 0;
	Buffer = NULL;
}

/***************************************************************/
//デストラクタ
/***************************************************************/
THighSpeedResidualImageEffect::~THighSpeedResidualImageEffect()
{
	Init();
}

/***************************************************************/
//バッファのクリア.
/***************************************************************/
void THighSpeedResidualImageEffect::Init()
{
	if (!Buffer) return ;
	int i;
	for(i = 0 ; i < 2 ; i++)
	{
		delete [] Buffer[i];
	}
	delete [] Buffer;
}

/***************************************************************/
//セット
/***************************************************************/
bool THighSpeedResidualImageEffect::Set(BMD IOBmd)
{
	int AllSize;
	AllSize = (IOBmd->w * IOBmd->h * 3);
	if ( (AllSize%4) != 0)
	{
		FriendlyError("THighSpeedResidualImageEffect::Set",
			"指定画像登録","サイズが4の倍数ではない","W*H*3 が 4 の倍数になるようにしろ!");
		return false;
	}
	//念のため初期化
	Init();
	//登録 
	IOBmdSizeDiv4 =	AllSize / 4;
	THighSpeedResidualImageEffect::IOBmd = IOBmd;
	BufferUse = 0;
	//バッファの作成.
	Buffer = new BYTE*[2];
	Buffer[0] = new BYTE[AllSize];
	Buffer[1] = new BYTE[AllSize];

	//ロック
	if ( !CommDraw->GraphicsLock(IOBmd) )	return false;
	memcpy(Buffer[0],IOBmd->bm,AllSize);
	memcpy(Buffer[1],IOBmd->bm,AllSize);
	//アンロック
	CommDraw->GraphicsUnLock(IOBmd);
	return true;
}

/***************************************************************/
//絵画.
/***************************************************************/
bool THighSpeedResidualImageEffect::Draw()
{
	//ロック
	if ( !CommDraw->GraphicsLock(IOBmd) )	return false;

	//クラスのメンバアクセスは 数ステップ分の無駄が発生するために(ﾏj?) ローカルに変更.
	BYTE*  Src = IOBmd->bm;
	BYTE** Dest= Buffer;
	int IoDiv4 = IOBmdSizeDiv4;
	//さぁ、汗の力を思い知らせてやる!!
	_asm
	{
		mov esi,Src	//esi に元画像が入ってます.
		mov eax,Dest
		mov edi,[eax]		//最初のバッファ
		mov ecx,[eax+4]		//次のバッファ
		mov edx,IoDiv4	//最大転送回数(4バイトづつアクセスして4バイトづつ計算格納します.)
						//これにより、従来より、 133% の効率がでるはず(たぶん).
						//仮想 MMX ってかぁ.
		//大転送 edx はループカウンターに使ってます.
	TRANCE_LOOP:
			//ちなみに、 eax ebx があいてます.
			//まず、Srcよりいただきます.
			mov  eax,[esi]
			mov  ebx,[edi]
			push eax		//オリジナルの保存.
			shr  ebx,1		// /2
			shr  eax,1		// /2
			and  ebx,0x7f7f7f7f	//クッリピング.
			and  eax,0x7f7f7f7f	//クッリピング.
			
			//0番バッファをいただく.
			add	 eax,ebx	//Src と 0番の半透明を出す.

			pop  ebx		//オリジナルデータ復帰.
			mov  [edi],ebx	//オリジナルに格納.

			//現在 ebx が、あいてます.  eax には、 Srcと0番の半透明がはいってます.
			shr  eax,1		    // /2
			mov  ebx,[ecx]
			shr  ebx,1		    // /2
			and  eax,0x7f7f7f7f	//クリップ
			and  ebx,0x7f7f7f7f	//クリップ

			add  eax,ebx		//Srcと0番の半透明 に、 1番の平均をいれて、完成!
			mov  [esi],eax
		/*
			//まず、 0 番バッファにソースの内容をコピーします.
			//でも、 0 番バッファの内容をソースに半透明で合成しなくてはなりません.
			mov eax , [edi]			//0番
			shr eax,1				// /2
			and eax,0x7f7f7f7f		//不要ビットの切り捨て

			mov ebx , [esi]
			push ebx		//オリジナルの保存
			shr ebx,1		// /2
			and ebx,0x7f7f7f7f//不要ビットの切り捨て
			add eax,ebx		//0 と Src を加えます.(平均値)
			pop ebx			//オリジナル復帰
			mov [edi],ebx	//0番にオリジナル画像をコピー

			//さて、次は 1番バッファの処理をします.
			//ここでは、 eax に前回( (Src+0番)/2) があります.
			shr eax,1		// /2
			and eax,0x7f7f7f7f//不要ビットの切り捨て
			mov ebx,[ecx]
			shr ebx,1		// /2
			and ebx,0x7f7f7f7f//不要ビットの切り捨て
			add eax,ebx

			mov [esi],eax	//それを送り返します.
		*/
			add ecx ,4
			add edi ,4
			add esi ,4
		dec edx
        jnz  TRANCE_LOOP
	}
	//アンロック
	CommDraw->GraphicsUnLock(IOBmd);

	BYTE *T;
	T = (Buffer[0]);
	(Buffer[0]) = (Buffer[1]);
	(Buffer[1]) = T;

	return true;
}

#endif	//LIFE END
