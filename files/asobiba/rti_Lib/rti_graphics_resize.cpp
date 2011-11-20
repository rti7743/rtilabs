#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE
/*****************************************************************/
//自由変換
/*****************************************************************/

#include <.\Graphics\rti_graphics_resize.h>

    struct _WorkArea
    {
        Fixed StepX,StepTX,StepTY;
    };
#define WORK_AREA_STEPX	0	//機種依存します.!!
#define WORK_AREA_STEPTX	4	//機種依存します.!!
#define WORK_AREA_STEPTY	8	//機種依存します.!!

#define WORK_AREA_GET(p,lr) ( (p<<1)+lr)
//各頂点　が Top Buttom Left Right のどれなのか決める
static bool SettingVector(_XYT *XY,_XYT **_Top,_XYT **_Buttom,_XYT **_Left,_XYT **_Right);
//イカさない四角形かどうか調べる
static bool IsOffenderSquare(const _XYT *Left,const _XYT *Right,
                      const _XYT *Top,const _XYT *Buttom);
//各頂点間の増分を調べる
static void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E);
//各頂点間の増分を調べる(イカさない四角形バージョン)
static int HokanVectorOffenderSquareRightSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare);
//各頂点間の増分を調べる(イカさない四角形バージョン)
static int HokanVectorOffenderSquareLeftSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare);
//各頂点間の増分を調べる(ふつーの四角形)
static int HokanVectorNormalSquare(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare);
//二つの頂点間の Y にたいする傾きを求める.
static int HokanVector(_WorkArea *WA,int *StopY,
       const _XYT *Left,const _XYT *Right,
       const _XYT *Top,const _XYT *Buttom,bool OffenderSquare);
//Y座標のクリッピング.
static bool YClipping(int &y,int *StopY,BMD const bmd2);
//XY を分解して、それぞれの進む値を WA に格納
//また、それぞれの停車ポイントを StopY に格納します.
// なお、 WA , StopY は配列ですので注意してください.
//また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
//また、転送開始 Y 座標を返します.
static int XYBrokeringAndSettingWorkArea(_XYT *XY,_WorkArea *WA,
                                    int *StopY,const BMD bmd);

static _WorkArea *lpWA ;			   //現在の _WorkArea .
static Fixed X1,X2,TX1,TX2,TY1,TY2;    //実転送開始場所.
static _WorkArea *WAL,*WAR;			   //現在の左右を担当している_WorkArea.
static int DeltaXYTStart = 0;		   //スタートする _WorkArea の場所.
static int DeltaXYTMax;				   //終了する _WorkArea の場所.
/*****************************************************************/
//自由変換 メイン部
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY)
{
    _WorkArea WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
    int *lpStopY;
    register int Temp;		//作業領域.
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		

	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,bmd2))	return;

    int AddPitch2;
    BYTE *data2;
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;

    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1);
        if (DrawLen > 0)
        {
            int StartX;
            //テクスチャを横断する準備
            register BYTE *d2;
            Fixed LocalTX   ,   LocalStepTX;
            Fixed LocalTY   ,   LocalStepTY;
            LocalTX = TX1;      LocalStepTX = (TX2-TX1) / DrawLen;
            LocalTY = TY1;      LocalStepTY = (TY2-TY1) / DrawLen;
            StartX = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (StartX < 0)
            {
                StartX = - StartX;  //マイナスなのでプラスに変換
                DrawLen -= StartX;    //転送範囲を縮める. .
                LocalTX += LocalStepTX * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                LocalTY += LocalStepTY * StartX; // テクスチャ転送位置移動 (固定 * 正数)
                d2 = data2 + 0 + 0 + 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
            }

            //転送先のヤツが横幅を超えないように....
//            Temp = ( Bmd2W - (FI(X1) + DrawLen) );
//            if (Temp < 0 )  DrawLen+=Temp;  //(Tempはマイナスです(^^;; )
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //(Tem            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                DirectPSet(d2, SeekBmAddressSp(bmd1,FI(LocalTX),FI(LocalTY) ) );
//                DirectPSet(d2, SeekBmAddress(bmd1,FI(LocalTX),     FI(LocalTY) ) );
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;
            }
        }

        X1 += WAL->StepX;        X2 += WAR->StepX;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
    }
}
/*****************************************************************/
//自由四角形ハッチング.
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb)
{
    _WorkArea WA[3*2]; //Y が 1進むときの傾き
    int StopY[3];       //Y が 1進むときの傾き　を切り替える Y の場所
    int *lpStopY;
    register int Temp;		//作業領域.
	int y ;                 //Y は、トップからスタートです.

    //XY を分解して、それぞれの進む値を WA に格納
    //また、それぞれの停車ポイントを StopY に格納します.
    // なお、 WA , StopY は配列ですので注意してください.
    //また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
    //また、転送開始 Y 座標を返します.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //転送開始 _WorkArea の設定.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		

	//Y 座標に対するクリッピングを実行します.
	// y , StopY は書き換えられる可能性があります
	if (!YClipping(y,StopY,bmd2))	return;

    int AddPitch2;
    BYTE *data2;
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;

    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //次の停止場所へ
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ライン方向に絵画していきます.
        int DrawLen = FI(X2 - X1);
        if (DrawLen > 0)
        {
            //テクスチャを横断する準備
            register BYTE *d2;
            Temp = FI(X1);      
            //範囲の外からスタートしたときは、範囲内に押し戻す.
            if (Temp < 0)
            {
                Temp = - Temp;  //マイナスなのでプラスに変換
                DrawLen -= Temp;    //転送範囲を縮める. .
                d2 = data2 + 0 + 0 + 0;
            }
            else
            {   //範囲内の正常な転送です.
                d2 = data2 + (Temp+Temp+Temp); //( * 3 は足し算で代用.)
            }

            //転送先のヤツが横幅を超えないように....
            Temp = ( Bmd2W - (FI(X1) + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //(Tempはマイナスです(^^;; )
            //転送開始
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                DirectPSet(d2, Rgb );
                d2 += 3;
            }
        }

        X1 += WAL->StepX;        X2 += WAR->StepX;
        data2 += AddPitch2;
    }
}

/*****************************************************************/
//各頂点　が Top Buttom Left Right のどれなのか決める
/*****************************************************************/
bool SettingVector(_XYT *XY,_XYT **_Top,_XYT **_Buttom,_XYT **_Left,_XYT **_Right)
{
    /*
    *     T          T                  R
    *    /\          |------------------|
    *   /  \         |                  |
    *  /    / R      |                  |
    *L/    /         |                  |
    *  \  /         L--------------------B  
    *   \/
    *   B
    *   上のようになります.
    * T とは、 y が最小　　y == y があったら、x が小さい方.
    */
    _XYT *Left,*Right,*Top,*Buttom;
    _XYT *Temp;
    int i;
    int BackupI1,BackupI2;
    int OkFlg = 0;
    bool OffenderSquare = false;
    //Top を求める.
    BackupI1 = BackupI2 = 0;
    Buttom = Top = &XY[0];
    for(i = 0; i < 4; i++)
    {
        Temp = &XY[i];
        if ( Temp->y < Top->y  ||
                    ( Temp->y == Top->y && Temp->x < Top->x))
        {
            Top = Temp;   BackupI1 = i;
        }
        if (Temp->y > Buttom->y  || 
                    ( Temp->y == Buttom->y && Temp->x > Buttom->x))
        {
            Buttom = Temp;    BackupI2 = i;
        }
    }
    //Top の場所を記録
    SETBIT(OkFlg,BackupI1);
    //Buttom の場所を記録
    SETBIT(OkFlg,BackupI2);
    Left = NULL;
    //Top Buttom 以外の 2つから、 Left Right を求める.
    for(i = 0; i < 4; i++)
    {
        if (! GETBIT(OkFlg,i) )
        {
            Temp = &XY[i];
            //残りはどちらかしかないので、片方ではなかったら　もう片方です.
            if (Left==NULL)
            {
                Left = Temp;
            }
            else
            {
                Right = Temp;
                break;  //Left , Right が求まったので　これ以上のループ続行は無意味.
            }
        }
    }

    if (Left->x > Right->x)
    {   //交換
        Temp = Left ;   Left = Right;   Right = Temp;
    }
/*
    if ( (Top->x > Left->x && Top->x > Right->x
                && Buttom->x > Left->x && Buttom->x > Right->x) ||
         (Top->x < Left->x && Top->x < Right->x
                && Buttom->x < Left->x && Buttom->x < Right->x))
*/
    if ( IsOffenderSquare(Left,Right,Top,Buttom) )
    {   
        /*      
        *    T
        *    |\
        *    | \        こんな、菱形になっていない図形は、
        *    |  \ L     イレギュラー(イングリッシュでいうと格好良いですね)
        *    |   |      な、図形として別途の処理をします.
        *    |   |      また、このときの図形の L R は、    
        *    |  / R     かならず、 Left->y > Right->y でなくては、
        *    | /        なりまへん.
        *    |/
        *    B
        */
        OffenderSquare = true;  //上の図のような四角形です.
        if (Left->y > Right->y)
        {
            Temp = Left ;   Left = Right;   Right = Temp;
        }
    }

    *_Top = Top;
    *_Buttom = Buttom;
    *_Left = Left;
    *_Right = Right;
    //イカさない四角形かどうかを返すのです.
    return OffenderSquare;
}
 
/*****************************************************************/
//イカさない四角形かどうか調べる
/*****************************************************************/
bool IsOffenderSquare(const _XYT *Left,const _XYT *Right,
                      const _XYT *Top,const _XYT *Buttom)
{
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  こんな感じの図形
        *    |   |          か、どうか調べる.
        *    |  / R
        *    | /
        *    |/
        *    B
        */
    /*
        判別アルゴリズム.
            上のような図形の場合、 画面を 線TB に二つに分割したとすると、
            点 L , R は 線TB の　右側　か　左側のどちらかに両方ともに
            あらわれます.
            これが、菱形などになると、 点 L , R は、 線TB の
            右と左に分離します.
            その特性を使うことによって、判別しています.
    */
    if (Top->x != Buttom->x)
    {
        int Rpos ;
        int Lpos ;
        Rpos = (Top->y - Buttom->y) * 
            (Right->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;
        Lpos = (Top->y - Buttom->y) *
            (Left->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;

        if ( (Lpos < Left->y && Rpos < Right->y) ||
               (Lpos > Left->y && Rpos > Right->y) )    return true;
    }
    else
    {
        if ( (Top->x < Left->x && Top->x < Right->x) ||
               (Top->x > Left->x && Top->x > Right->x) )    return true;
    }
    //ふつーの四角形.
    return false;
}

/*****************************************************************/
//各頂点間の増分を調べる
/*****************************************************************/
int HokanVector(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
    //規約違反な図形かな?
    //規約違反の図形は、下の図みたいなやつ.
    //菱形や長方形ぢゃないタイプです.
    if ( OffenderSquare )
    {
        if (Buttom->x < Right->x)
        {
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  こんな感じの図形
        *    |   |          は、特別なルーチンが必要です(涙)
        *    |  / R
        *    | /
        *    |/
        *    B
        */
            return HokanVectorOffenderSquareRightSmarll
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
        }
        else
        {
        /*      
        *       T
        *      /|
        *     / |
        *  L /  | 
        *    |  |  こんな感じの図形
        *    |  |          は、特別なルーチンが必要です(涙)
        *    |  | 
        *  R \  |
        *     \ |
        *      \|
        *       B
        */
            return HokanVectorOffenderSquareLeftSmarll
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
        }
    }
    else
    {
        //通常の　長方形、正方形、　菱形、台形など.
        return HokanVectorNormalSquare
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
    }
}

/*****************************************************************/
//各頂点間の増分を調べる(イカさない四角形バージョン)
/*****************************************************************/
int HokanVectorOffenderSquareRightSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  こんな感じの図形
        *    |   |          は、特別なルーチンが必要です(涙)
        *    |  / R
        *    | /
        *    |/
        *    B
        */
            //T -> L について.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Left);
            //T -> B について.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Buttom);

            //第一ストップポイント
            StopY[0] = Left->y;    //L の Y で一休み
            if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,1)],Left,Right);  //WaR = L => R まで
                //WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[0] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 0;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[1] = Buttom->y;     //B の Y で一休み
                return 1;
            }
            else
            {
                if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(1,1)],Left,Right);  //WaR = L => R まで
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[1] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 1;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(2,1)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(2,0)] = WA[WORK_AREA_GET(1,0)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[2] = Buttom->y;     //B の Y で一休み
                return 2;
            }
}

/*****************************************************************/
//各頂点間の増分を調べる(イカさない四角形バージョン)
/*****************************************************************/
int HokanVectorOffenderSquareLeftSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
        /*      
        *       T
        *      /|
        *     / |
        *  L /  | 
        *    |  |  こんな感じの図形
        *    |  |          は、特別なルーチンが必要です(涙)
        *    |  | 
        *  R \  |
        *     \ |
        *      \|
        *       B
        */
            //T -> L について.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Left);
            //T -> B について.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Buttom);
            //第一ストップポイント
            StopY[0] = Left->y;    //L の Y で一休み
            if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,0)],Left,Right);  //WaR = L => R まで
                //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[0] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 0;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(1,0)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[1] = Buttom->y;     //B の Y で一休み
                return 1;
            }
            else
            {
                HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Right);  //WaR = L => R まで
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaL は、前回の続きを実行する.

                //第二ストップポイント
                StopY[1] = Right->y;     //R の Y で一休み
                if (Right->y == Buttom->y)   return 1;   //R と B が同じ高さだったらおしまい.
                HokanLine(&WA[WORK_AREA_GET(2,0)],Right,Buttom); //WaR = R => B まで
                WA[WORK_AREA_GET(2,1)] = WA[WORK_AREA_GET(1,1)];               //WaL は、前回の続きを実行する.
                //第三ストップポイント
                StopY[2] = Buttom->y;     //B の Y で一休み
                return 2;
            }
}
/*****************************************************************/
//各頂点間の増分を調べる(ふつーの四角形)
/*****************************************************************/
int HokanVectorNormalSquare(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
    //T -> L について.
    HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Left);
    //T -> R について.
    HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Right);
    //第一ストップポイント
    if (Right->y < Left->y)
    {
        StopY[0] = Right->y;    //R の Y で一休み
        if (Right->y == Buttom->y)   return 0;   //R と B が同じ高さだったらおしまい.
		if (Top->y == StopY[0])
		{	//上の辺が同じ高さにある四角形だった場合
			HokanLine(&WA[WORK_AREA_GET(0,1)],Right,Buttom);  //WaR = R => B まで
			//WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

			StopY[0] = Left->y;     //L の Y で一休み
	        if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B まで
			WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];               //WaR は、前回の続きを実行する.
			StopY[1] = Buttom->y;
			return 1;
		}
		else
		{
			HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom);  //WaR = R => B まで
			WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL は、前回の続きを実行する.

			StopY[1] = Left->y;     //L の Y で一休み
	        if (Left->y == Buttom->y)   return 1;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(2,0)],Left,Buttom);  //WaL = L => B まで
			WA[WORK_AREA_GET(2,1)] = WA[WORK_AREA_GET(1,1)];               //WaR は、前回の続きを実行する.
			StopY[2] = Buttom->y;
			return 2;
		}
    }
    else
    {
        StopY[0] = Left->y;    //L の Y で一休み
        if (Left->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
		if (Top->y == StopY[0])
		{	//上の辺が同じ高さにある四角形だった場合
	        HokanLine(&WA[WORK_AREA_GET(0,0)],Left,Buttom);  //WaL = L => B まで
		    //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaR は、前回の続きを実行する.

			StopY[0] = Right->y;     //R の Y で一休み
	        if (Right->y == Buttom->y)   return 0;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B まで
			WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];               //WaL は、前回の続きを実行する.

			StopY[1] = Buttom->y;
			return 1;
		}
		else
		{
	        HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B まで
		    WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaR は、前回の続きを実行する.

			StopY[1] = Right->y;     //R の Y で一休み
	        if (Right->y == Buttom->y)   return 1;   //L と B が同じ高さだったらおしまい.
		    HokanLine(&WA[WORK_AREA_GET(2,1)],Right,Buttom); //WaR = R => B まで
			WA[WORK_AREA_GET(2,0)] = WA[WORK_AREA_GET(1,0)];               //WaL は、前回の続きを実行する.

			StopY[2] = Buttom->y;
			return 2;
		}
    }
}

/*****************************************************************/
//二つの頂点間の Y にたいする傾きを求める.
/*****************************************************************/
void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E)
{
    int DY ;
    Fixed X,TX,TY;
    if (B->y > E->y)
    {
        DY = B->y - E->y;
        X  = IF(B->x - E->x);
        TX  = IF(B->tx - E->tx);
        TY  = IF(B->ty - E->ty);
    }
    else
    {
        DY = E->y - B->y;
        X  = IF(E->x - B->x);
        TX  = IF(E->tx - B->tx);
        TY  = IF(E->ty - B->ty);
    }
    //とりあえず、　0　Div 回避
    if (DY==0)
    {
        WA->StepX  = X;
        WA->StepTX = TX;
        WA->StepTY = TY;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepTX = TX / DY;
    WA->StepTY = TY / DY;
    return ;
}


//Y 座標に対するクリッピングを実行します.
bool YClipping(int &Y,int *StopY,BMD const bmd2)
{
	register int Temp;
    int *lpStopY;
	//Yの転送開始アドレスが 0 未満だった処理 (超ややこしい(;;))
	for(lpStopY = &StopY[0] , DeltaXYTStart = 0 ; Y < 0; DeltaXYTStart ++)
	{
		if (*lpStopY >= 0)
		{	//0 以上になった.
			Temp = (- Y)  ;	// y の数だけ そのプラスの *lpStopY から食いちぎる.
			//各要素をプラスする.
			X1 += WAL->StepX * Temp;			TX1 += WAL->StepTX * Temp;			TY1 += WAL->StepTY * Temp;
			X2 += WAR->StepX * Temp;			TX2 += WAR->StepTX * Temp;			TY2 += WAR->StepTY * Temp;
			Y = 0;
			break;
		}
		else
		{	//まだ 0 に到達しない場合....
			if (DeltaXYTStart == DeltaXYTMax) return false; //すべてマイナスに突っ込んでいる.
			Temp = (-Y) + *lpStopY ;	// 符号反転した ストップ Y の値分、各要素をプラスします.
			//各要素をプラスする.
			X1 += WAL->StepX * Temp;			TX1 += WAL->StepTX * Temp;			TY1 += WAL->StepTY * Temp;
			X2 += WAR->StepX * Temp;			TX2 += WAR->StepTX * Temp;			TY2 += WAR->StepTY * Temp;
			Y = *lpStopY;
		}
        lpStopY++;  //次の停止場所へ
        lpWA+=2;
        WAL = lpWA;
	    WAR = (lpWA+1);
	}
    
    //Y座標のオーバー転送の防止
    for(lpStopY = &StopY[DeltaXYTStart] , Temp = DeltaXYTStart;Temp <= DeltaXYTMax;lpStopY++ , Temp++)
    {
        if(*lpStopY > bmd2->h)
        {
            if (Temp == 0) return false;    //もっとも上が最低ライン以下、転送する必要なし.
            DeltaXYTMax = Temp;
            *lpStopY = (bmd2->h - 1);  //はい ここで.
            break; 
        }
    }
	return true;
}

//XY を分解して、それぞれの進む値を WA に格納
//また、それぞれの停車ポイントを StopY に格納します.
// なお、 WA , StopY は配列ですので注意してください.
//また、 X1 TX1 TY1 X2 TX2 TY2 に値のセットも行います.
//また、転送開始 Y 座標を返します.
int XYBrokeringAndSettingWorkArea(_XYT *XY,_WorkArea *WA,
                                    int *StopY,const BMD bmd)
{
    _XYT *Left,*Right,*Top,*Buttom;
    bool OffenderSquare;

    //四方の座標を決定.
    OffenderSquare = SettingVector(XY,&Top,&Buttom,&Left,&Right);
    if ( (Top->y < 0 && Buttom->y < 0) || (Top->y >= bmd->h && Buttom->y >= bmd->h))
        return bmd->h;
    /*
    *     T          L                  T
    *    /\          |------------------|
    *   /  \         |                  |
    *  /    / R      |                  |
    *L/    /         |                  |
    *  \  /         B--------------------R  
    *   \/
    *   B
    * 基本的な図形の菱形は、
    * T.y ～　B.y までの移動になるが、
    * T.y ～　R.y    0回目転送
    * R.y ～　L.y    1回目転送
    * L.y ～　B.y    2回目転送
    * の最大　3 回転送にわけて処理する.
    */
    DeltaXYTStart = 0;				//だから、最初に使うワークエリアは最初です.
    DeltaXYTMax = HokanVector(WA,StopY,
                    Left,Right,Top,Buttom,OffenderSquare);
    //スタート位置の決定.
    //X1 TX1 TY1 X2 TX2 TY2 はスタティックなグローバル変数ですので注意.
    _XYT *SL,*SR;                   //スタート地点
    //スタート地点設定
    SL = SR = Top;
    if(Top->y == Right->y) SR = Right; //綺麗な四角形の SR は Rightからです.
//    else if (Top->y == Left->y) SR = Left; //特別な綺麗な四角形の SR は Leftからです.
    X1 = IF(SL->x);            X2 = IF(SR->x);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
    //転送開始 Y 座標.
    return Top->y;
}

#endif //LIFE END
