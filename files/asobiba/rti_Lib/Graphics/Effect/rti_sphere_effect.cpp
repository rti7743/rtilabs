/*****************************************************************/
/*****************************************************************/
//極座標関係.       半分 EL からの移植です(汗)
//でも、極座標ってすてきですよね (逃)
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"
#ifdef SPHERE_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_sphere_effect.h>
#include <.\Comm\rti_Comm.h>
#include <.\Base\rti_error_message.h>
#include <math.h>

/************************************************************/
//コンストラクタ
/************************************************************/
TSphereEffect::TSphereEffect()
{
    m_PointChangeTable = NULL;
}

/************************************************************/
//デストラクタ
/************************************************************/
TSphereEffect::~TSphereEffect()
{
    Init();
}

/************************************************************/
//初期化
/************************************************************/
void TSphereEffect::Init()
{
    delete m_PointChangeTable;
}

/************************************************************/
//ステータスセット
/************************************************************/
bool TSphereEffect::Set(BMD bmd,BMD OutBmd)
{
    //各種データを頂きます.
    m_bmd    = bmd;
    m_OutBmd = OutBmd;
	return true;
}


/*------------------------------------------------------------------------------*/
/*-                                                                            -*/
/*-  トンネル変換情報の生成                                 ※ ハイカラー専用  -*/
/*-                                                                            -*/
/*-    int Sw     : スプライトのＸサイズ                                       -*/
/*-    int Sh     : スプライトのＹサイズ                                       -*/
/*-    int Dw     : 描画先のＸサイズ ( ～640 )                                 -*/
/*-    int Dh     : 描画先のＹサイズ ( ～480 )                                 -*/
/*-    int Cx     : トンネル中央のＸ座標                                       -*/
/*-                 省略/TUNNEL_CENTER = 中央                                  -*/
/*-                 0～                = 任意の位置                            -*/
/*-    int Cy     : トンネル中央のＹ座標                                       -*/
/*-                 省略/TUNNEL_CENTER = 中央                                  -*/
/*-                 0～                = 任意の位置                            -*/
/*-    int Size   : トンネルの大きさ ( 見た目 )                                -*/
/*-                 省略/TUNNEL_DEFAULT_SIZE = 8                               -*/
/*-                 2～                      = 任意の大きさ                    -*/
/*-    int Center : トンネル中央の設定                                         -*/
/*-                 省略 = TUNNEL_NORMAL/普通                                  -*/
/*-    int Type   : トンネル形状の設定                                         -*/
/*-                 省略 = TUNNEL_TYPE_CIRCLE/円                               -*/
/*-    int Line   : トンネル曲線の設定                                         -*/
/*-                 省略 = TUNNEL_LINE_NORMAL/普通                             -*/
/*-                                                                            -*/
/*------------------------------------------------------------------------------*/

/*
bool TSphereEffect::CreateTunnel(int Sw,int Sh,int Dw,int Dh,
						  int Cx,int Cy, int Size,
                          TUNNEL_CENTRAL Center, TUNNEL_SHAPE Type,
                          TUNNEL_CURVE Line)
{
	int x,y;     // 汎用カウンター
	Fixed f1,f2; // 汎用変数
	int xx,yy;   // スプライト参照位置
	int cx,cy;   // ディスプレイ中央位置
    Fixed curve; // トンネルの曲線
    XY *lpPointChangeTable; //メモリ確保した配列のポインタ

    //念のためメモリクリアを指示.
    Init();
    //メモリ確保
    m_PointChangeTable = new XY[Dw * Dh];
    lpPointChangeTable = m_PointChangeTable;

	TunnelSpriteX=Sw;
	TunnelSpriteY=Sh;
	TunnelDisplayX=Dw;
	TunnelDisplayY=Dh;
	// 画面の中央位置を取得
	cx=Cx;
	cy=Cy;
    
    //トンネルの中央を指定されたら..
	if (Cx==TUNNEL_CENTER) cx=Dw/2;
	if (Cy==TUNNEL_CENTER) cy=Dh/2;

    // トンネルのサイズを取得
	if (Size==TUNNEL_DEFAULT_SIZE) Size=8;
	if (Size<2) Size=2;

    curve = FixedDiv(IF(Sh) , 2* fxtan(PAI_FIXED*256/Size) ) /2;

	// トンネル中央の設定
	switch (Center)
	{
    case TUNNEL_WIDE_X:     // 左右に拡大
		cy=Dh;		break;
	case TUNNEL_TO_RIGHT:   // 右に歪む
		cx=Dw;    	break;
	case TUNNEL_TO_LEFT:    // 左に歪む
		cx=0;		break;
	}

	// Ｙ方向の処理
	for (y=0;y<Dh;y++)
	{
		// トンネル中央の設定
		switch (Center)
		{
		case TUNNEL_WIDE_X:         // 左右に拡大
			cy--;				    break;
		case TUNNEL_TO_RIGHT:       // 右に歪む
			if (--cx<0) cx=0;	    break;
		case TUNNEL_TO_LEFT:        // 左に歪む
			if (++cx>=Dw) cx=Dw;    break;
		case TUNNEL_WIDE_Y:         // 上下に拡大
			cx=Dw;  				break;
    	}

		// Ｘ方向の処理
		for (x=0;x<Dw;x++)
		{
			// トンネル中央の設定
			if (Center==TUNNEL_WIDE_Y) cx--;

			// トンネル形状の設定
			switch (Type)
			{
			case TUNNEL_TYPE_CIRCLE:        //円
				f1=Float2Fixed(sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy)));
				break;
			case TUNNEL_TYPE_BOX1:          //四角
				f1=IF(max(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_BOX2:          //菱形
				f1=IF(abs(x-cx)+abs(y-cy))/2;
				break;
			case TUNNEL_TYPE_CROSS:         //十字
				f1=IF(min(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_WALL:          //左右の壁
				f1=IF(abs(x-cx));
				break;
			case TUNNEL_TYPE_GROUND:        //上下の大地
				f1=IF(abs(y-cy));
				break;
			case TUNNEL_TYPE_FLASH:         //フラッシュ
				f1=IF(cx);
    			break;
			}

			// トンネル曲線の設定
			switch (Line)
			{
			case TUNNEL_LINE_NORMAL:        // 普通 ( 直線 )
				f2=curve/32;
				break;
			case TUNNEL_LINE_X:             // Ｘ曲線
				f2= FixedDiv( curve , (IF(abs(y-cy)+1)/10) );
				break;
			case TUNNEL_LINE_Y:             // Ｙ曲線
				f2= FixedDiv( curve , (IF(abs(x-cx)+1)/10) );
				break;
			case TUNNEL_LINE_BOX1:          // 四角曲線
				f2= curve / max(abs(x-cx),abs(y-cy));
				break;
			case TUNNEL_LINE_BOX2:          // 菱形曲線
				f2= FixedDiv( curve , IF( min(abs(x-cx),abs(y-cy)) ) / 2 );
				break;
			case TUNNEL_LINE_CROSS:         // 十字曲線
				f2= curve / min(abs(x-cx),abs(y-cy));
				break;
			}

			if (f2<0) f2=-f2;

			// 対応するスプライト位置の算出
			xx=FI( (long)(FixedDiv(IF(Sw),(2*PAI_FIXED))*atan2(y-cy,x-cx)) );
			yy=FI(FixedDiv(f2*(sqrt(cx*cx+cy*cy)-f1),f1));

			// XYがスプライト範囲内になるように補正
			for (;xx<0;xx+=Sw);
			for (;yy<0;yy+=Sh);
			if (xx>=Sw) xx%=Sw;
			if (yy>=Sh) yy%=Sh;

			// トンネルリストに保存
			lpPointChangeTable->x=xx;
			lpPointChangeTable->y=yy;
            //次の要素へ.(こうやって、テーブル全体にアクセスしていきます)
            lpPointChangeTable++;
		}
	}
    return true;
}

*/
bool TSphereEffect::CreateTunnel(int Sw,int Sh,int Dw,int Dh,
						  int Cx,int Cy, int Size,
                          TUNNEL_CENTRAL Center, TUNNEL_SHAPE Type,
                          TUNNEL_CURVE Line)
{
	int x,y;     // 汎用カウンター
	float f1,f2; // 汎用変数
	int xx,yy;   // スプライト参照位置
	int cx,cy;   // ディスプレイ中央位置
    float curve; // トンネルの曲線
    XY *lpPointChangeTable; //メモリ確保した配列のポインタ

	if ( !CheckPow2(&TunnelSpritePow2W,Sw) )
	{
		FriendlyError("TSphereEffect::CreateTunnel","トンネル作成","領域の W幅 が 2 のべき数でない","高速化のために画像の横幅は 2のべき数にしてください");
		return false;
	}
	TunnelSpritePow2MaskW = (1 << TunnelSpritePow2W) - 1;	//Clippingのためのマスクの再生
	if ( !CheckPow2(&TunnelSpritePow2H,Sh) )
	{
		FriendlyError("TSphereEffect::CreateTunnel","トンネル作成","領域の H幅 が 2 のべき数でない","高速化のために画像の縦幅は 2のべき数にしてください");
		return false;
	}
	TunnelSpritePow2MaskH = (1 << TunnelSpritePow2H) - 1;	//Clippingのためのマスクの再生

    //念のためメモリクリアを指示.
    Init();
    //メモリ確保
    m_PointChangeTable = new XY[Dw * Dh];
    lpPointChangeTable = m_PointChangeTable;

	TunnelDisplayX=Dw;
	TunnelDisplayY=Dh;
	// 画面の中央位置を取得
	cx=Cx;
	cy=Cy;
    
    //トンネルの中央を指定されたら..
	if (Cx==TUNNEL_CENTER) cx=Dw/2;
	if (Cy==TUNNEL_CENTER) cy=Dh/2;

    // トンネルのサイズを取得
	if (Size==TUNNEL_DEFAULT_SIZE) Size=8;
	if (Size<2) Size=2;

    curve = F(Sh/(F(2)*tan( (PAI/F(Size) )/F(2) )));

	// トンネル中央の設定
	switch (Center)
	{
    case TUNNEL_WIDE_X:     // 左右に拡大
		cy=Dh;		break;
	case TUNNEL_TO_RIGHT:   // 右に歪む
		cx=Dw;    	break;
	case TUNNEL_TO_LEFT:    // 左に歪む
		cx=0;		break;
	}

	// Ｙ方向の処理
	for (y=0;y<Dh;y++)
	{
		// トンネル中央の設定
		switch (Center)
		{
		case TUNNEL_WIDE_X:         // 左右に拡大
			cy--;				    break;
		case TUNNEL_TO_RIGHT:       // 右に歪む
			if (--cx<0) cx=0;	    break;
		case TUNNEL_TO_LEFT:        // 左に歪む
			if (++cx>=Dw) cx=Dw;    break;
		case TUNNEL_WIDE_Y:         // 上下に拡大
			cx=Dw;  				break;
    	}

		// Ｘ方向の処理
		for (x=0;x<Dw;x++)
		{
			// トンネル中央の設定
			if (Center==TUNNEL_WIDE_Y) cx--;

			// トンネル形状の設定
			switch (Type)
			{
			case TUNNEL_TYPE_CIRCLE:        //円
				f1 = F(hypot( (x-cx) ,(y-cy) ) );
				break;
			case TUNNEL_TYPE_BOX1:          //四角
				f1=F(max(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_BOX2:          //菱形
				f1=F((abs(x-cx)+abs(y-cy))/F(2));
				break;
			case TUNNEL_TYPE_CROSS:         //十字
				f1=F(min(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_WALL:          //左右の壁
				f1=F(abs(x-cx));
				break;
			case TUNNEL_TYPE_GROUND:        //上下の大地
				f1=F(abs(y-cy));
				break;
			case TUNNEL_TYPE_FLASH:         //フラッシュ
				f1=F(cx);
    			break;
			}

			// トンネル曲線の設定
			switch (Line)
			{
			case TUNNEL_LINE_NORMAL:        // 普通 ( 直線 )
				f2=curve/F(32);
				break;
			case TUNNEL_LINE_X:             // Ｘ曲線
				f2=curve/  F((abs(y-cy)+1)/F(10));
				break;
			case TUNNEL_LINE_Y:             // Ｙ曲線
				f2=curve/ F((abs(x-cx)+1)/F(10));
				break;
			case TUNNEL_LINE_BOX1:          // 四角曲線
				f2=curve/ F(max(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_LINE_BOX2:          // 菱形曲線
				f2=curve/ F((abs(x-cx)+abs(y-cy))/F(2));
				break;
			case TUNNEL_LINE_CROSS:         // 十字曲線
				f2=curve/ F(min(abs(x-cx),abs(y-cy)));
				break;
			}
 
			if (f2<0) f2=-f2;

			// 対応するスプライト位置の算出
			xx=(int)(Sw/(F(2)*PAI)*atan2(y-cy,x-cx));
			yy=(int)(f2*(sqrt(cx*cx+cy*cy)-f1)/f1);

			// XYがスプライト範囲内になるように補正
			for (;xx<0;xx+=Sw);
			for (;yy<0;yy+=Sh);
			if (xx>=Sw) xx%=Sw;
			if (yy>=Sh) yy%=Sh;

			// トンネルリストに保存
			lpPointChangeTable->x=xx;
			lpPointChangeTable->y=yy;
            //次の要素へ.(こうやって、テーブル全体にアクセスしていきます)
            lpPointChangeTable++;
		}
	}
    return true;
}

/************************************************************/
//絵画だよん.
/************************************************************/
bool TSphereEffect::Draw(int ux,int uy , int Rotate , int Dist)
{
    int  AddPitch2;
	int  _TunnelDisplayY,_TunnelDisplayX;
	int  _XPointChangeTablePitch;
	DWORD _RotateDist;
	DWORD _MaskWH;
    BYTE *data1,*data2;
	unsigned char bmd_Pow2W;
    XY *lpPointChangeTable;  //メモリ確保した配列のポインタ
    
	//m_bmd の幅が 2 のべき数ぢゃないとダメ.
	bmd_Pow2W = m_bmd->Pow2W;
	if (bmd_Pow2W == 0)	return false;
	bmd_Pow2W = 16 - bmd_Pow2W;	//16は、 Y が上位16ビットにいるから.
								//Y を >> 16 で元に戻して、それから << bmd_Pow2W する.
								//つまり、 16 - bmd_Pow2W だけ、>> する.

	if (bmd_Pow2W < 0) return false;	//65535 以上のテクスチャってあるんかいな.

	_RotateDist = (Dist<<16) + Rotate;
	//転送元画像は 2 のべき数です. だから、 & で高速クリッピングできます. その値の取得
	_MaskWH = (TunnelSpritePow2MaskH << 16) + TunnelSpritePow2MaskW;
	//転送する領域.
	_TunnelDisplayX = TunnelDisplayX;
	_TunnelDisplayY = TunnelDisplayY;
	//画像変換テーブルの Y が 1 進むときに 増える X バイト(バイトであることに注意)
	_XPointChangeTablePitch = sizeof(XY) * _TunnelDisplayX;

    //ロック
	if ( !CommDraw->GraphicsLock(m_bmd) )   	return false;
	if ( !CommDraw->GraphicsLock(m_OutBmd) )	return false;
    //絵画位置を取得.
	data1 = m_bmd->bm;
	data2 = SeekData(m_OutBmd,ux,uy, 0,0,&AddPitch2);
    //座標変換テーブル.
    lpPointChangeTable = m_PointChangeTable;
	
	/*
	* 
	*	ループカウンターの諸注意.
	*		クソバカ Intel(終わってる) の CPU 仕様により、
	*		シフトする数に ecx の cl を利用します.
	*		それで、そのために、 Yループ = ecx   Xループ = edx となります.
	*		原則では、 Y = edx  X = ecx ですけどね. つまり、入れ替わるわけですね.
	*	ループ中の大注意
	*		このループルーチンは 高速化のために 結構危ないことをしています.
	*		eax レジスタの上位 16ビットに Y    下位 16 ビットに X が入ります.
	*		まぁ、 MMX みたいな感じで疑似パックされているわけです.
	*		ここらあたりも、リトルエンディアンな機種を想定して作っているので、
	*		他のアーキテクチャをもつ PC に移植するさいに 要注意!!
	*/
    _asm{
        mov ecx,_TunnelDisplayY

        mov edi,lpPointChangeTable  ;画像変換テーブル
        mov esi,data2				;転送
    LOOP1:              ;ループカウンタに ecx を使用中
            push edi
            push esi

			push ecx	//内部でシフトを使うため、ecx を拝借します.
			mov  cl,bmd_Pow2W
           
            mov edx,_TunnelDisplayX
        LOOP2:           ;ループカウンタに edx を使用中
				//画像変換テーブルより値の取得.
				//Gx = lpXPointChangeTable->x + Rotate;
				//Gy = lpXPointChangeTable->y + Dist;
				mov   eax,dword ptr [edi]		//Y / Xで取得. 上位16ビットに X 、下位に 16ビット
				add	  eax,_RotateDist			//回転を付加
				//クッリピング
				//Gx &= _TunnelSpritePow2MaskW;
				//Gy &= _TunnelSpritePow2MaskH;
				and	  eax,_MaskWH
				//転送開始位置を取得
				//Temp = Gx + (Gy<<bmd_Pow2W);
				mov	  ebx,eax
				shr   eax,cl		//Y は、上位16ビット、それをおろして >> 16、テクスチャの幅 << 2PowW する.
									//で、 >> と << は、相殺できるので、>> cl だけになる.
									//とうぜん、 cl では、 16 - 2Pow が、入ってます.
									//なお、現在のルーチンでは、テクスチャの幅 65535 を超えるとおかしくなりますが、
									//そんな、バカでかいテクスチャはくることはないでしょうね.

				and	  ebx,0xffff	//下位16ビットの確定	つまり、 ebx が X になる.
				add	  ebx,eax		//Y と X の合成.
				//*3
				mov   eax,ebx
				shl	  ebx,1
				add   ebx,eax
				add   ebx,data1

				//転送開始
		        //DirectPSet(x_data2,(data1+(Temp+Temp+Temp) ) );
				//(data1+(Temp+Temp+Temp) )

                mov eax,[ebx] ;いっきにいただきます
                mov ebx,[esi]      ;あまりをくりぬく
                and eax,0x00ffffff ;不要な部分を切り捨てる.
                and ebx,0xff000000
                or  ebx,eax

                mov [esi],ebx

                add edi,4	//XYの増分(機種依存!!)
                add esi,3	
            dec edx
            jnz  LOOP2       ;LOOP2終了 ECX解放

			pop ecx			//内部で シフトを使ったため拝借していた ecx を元に戻します.

            pop esi			//転送先
            pop edi			//座標変換テーブル

            add esi,AddPitch2
            add edi,_XPointChangeTablePitch
        dec ecx             ;LOOP1終了 EDX解放
        jnz  LOOP1
    }

/*
    // データ転送
	for (y=0;y<_TunnelDisplayY;y++)
	{
        lpXPointChangeTable = lpPointChangeTable;
        x_data2 = data2;
		for (x=0;x<_TunnelDisplayX;x++)
		{
			Gx = lpXPointChangeTable->x + Rotate;
			Gy = lpXPointChangeTable->y + Dist;
			//クッリピング
			Gx &= _TunnelSpritePow2MaskW;
			Gy &= _TunnelSpritePow2MaskH;
			//転送開始位置を取得
			Temp = Gx + (Gy<<bmd_Pow2W);
			//転送開始
            DirectPSet(x_data2,(data1+(Temp+Temp+Temp) ) );
			//出力側をずらす
            x_data2+=3;
			//次の画像変換テーブルへ
            lpXPointChangeTable++;
		}
		// Ｙ方向に加算
		data2+=AddPitch2;
        lpPointChangeTable += _TunnelDisplayX;
	}
*/
    //アンロック
    CommDraw->GraphicsUnLock(m_OutBmd);
	CommDraw->GraphicsUnLock(m_bmd);
 
    return true;
}


#endif
