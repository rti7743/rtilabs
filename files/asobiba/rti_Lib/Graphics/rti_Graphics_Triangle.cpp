
#include "rti_Graphics_Triangle.h"
#include "../Base/rti_debug.h"

class TTriangleSuperDraw
{
private://共有ルーチンなのら.
	//////////描画のための情報の取得.
	//座標 A B C を受け取り線形補完情報を取得する.
	static int AcquisitionOfInformationgForDrawing(_NN_WorkAreaP* outLRData ,_XYZP *outStartLR ,  _XYZP *inA ,_XYZP *inB ,_XYZP *inC);
	//左に三角形
	static int LeftTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//右に三角形
	static int RightTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//正三角形(?) 実は、二等辺三角形
	static int EquilateralTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//逆三角形
	static int ReverseTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//Bが左右のどっちに来るかの判別式.
	static bool LeftOrRight(const _XYZP* Top  ,const _XYZP* Median ,const _XYZP* Buttom);


	//////////クリッピング
	//勇気のクリッピング
	static int ClippingProcessing(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos);
	//上方向におけるクリッピング.
	static int ClippingProcessingUp(_NN_WorkAreaP* outLRData , _XYZP* outStartPos , int inStopPos);
	//下方向におけるクリッピング.
	static int ClippingProcessingDown(_NN_WorkAreaP* outLRData , _XYZP* inStartPos , int inStopPos);
public:
	//メイン.
	static void TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData);
//個別ルーチンなのら.
private:
	//二つの頂点間の Y にたいする傾きを求める.
	static void HokanLine(_WorkAreaP *WorkP,const _XYZP *B,const _XYZP *E);
	//各要素のプラス.	YCount 進んだときの変化.
	static void StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos , int YCounter);
	//各要素のプラス.
	static void StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos );
	//希望のドロー
	static void MainDraw(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos , _BetaData * inData);
};

//単純四角形絵画.
void TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData)
{
	TTriangleSuperDraw::TriangleDraw(inA,inB,inC,inData);
}



//二つの頂点間の Y にたいする傾きを求める.
inline void TTriangleSuperDraw::HokanLine(_WorkAreaP *WorkP,const _XYZP *B,const _XYZP *E)
{
    Fixed DY , X, Z;

    DY = E->y - B->y;
    X  = E->x - B->x;
    Z  = E->z - B->z;
    //とりあえず、　0　Div 回避
    if (DY==0)
    {
        WorkP->StepX  = 0;
        WorkP->StepZ  = 0;
        return ;
    }

    WorkP->StepX  = FixedDiv(X , DY);
    WorkP->StepZ  = FixedDiv(Z , DY);
    return ;
}

//各要素のプラス.	YCount 進んだときの変化.
inline void TTriangleSuperDraw::StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos , int YCounter)
{
	outLPos->x += inLData->StepX * YCounter;
	outLPos->z += inLData->StepZ * YCounter;
	outRPos->x += inRData->StepX * YCounter;
	outRPos->z += inRData->StepZ * YCounter;
}
//各要素のプラス.
inline void TTriangleSuperDraw::StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos )
{
	outLPos->x += inLData->StepX ;
	outLPos->z += inLData->StepZ ;
	outRPos->x += inRData->StepX ;
	outRPos->z += inRData->StepZ ;
}
//希望のドロー
void TTriangleSuperDraw::MainDraw(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos , _BetaData * inData)
{
	int			YCounter , YStop, PosCounter;
	int			XLen ;
	//スタート地点.
	YCounter = FI(inStartPos[__TRIANGLE_DRAW_LEFT].y);
	//とりあえずの目標. (超えるべからず)
	YStop = inLRData[0].Y;	//最初の目標セット.
	//現在の場所変えカウンター　三角形だから最大で 1 です.
	PosCounter = 0;
	//エリアに簡単にアクセスできるようにポインタで穴をあけます.
	_WorkAreaP	*Lwp = &inLRData[0].Data[__TRIANGLE_DRAW_LEFT];
	_WorkAreaP	*Rwp = &inLRData[0].Data[__TRIANGLE_DRAW_RIGHT];
	_XYZP		*Lsp = &inStartPos[0]		,	*Rsp = &inStartPos[1];

	int			Rgb = inData->rgb;

	int		AddPitch2;
	BYTE*	Data2 = SeekData(inData->OutBmd,0,YCounter, 0,0,&AddPitch2);
    int Bmd2W = inData->OutBmd->w;
	Fixed* ZBuffer = inData->ZBuffer->GetZBuffer(0,YCounter);
//	ASSERT(inData->ZBuffer->GetPitch() == Bmd2W);

	for( ; ; YCounter++)
	{
		if (YCounter >= YStop)
		{//取り合えずの目標に到達.
			//まず、先に完了チェック!! 完了していたらおしまーい.
			if (PosCounter == inStopPos) break;
			//次へ.
			PosCounter++;
			//とりあえずの目標の更新. (超えるべからず)
			YStop =	inLRData[PosCounter].Y;
			Lwp = &inLRData[PosCounter].Data[__TRIANGLE_DRAW_LEFT];
			Rwp = &inLRData[PosCounter].Data[__TRIANGLE_DRAW_RIGHT];
		}
		//X ラインを塗りつぶしていきます.
		XLen = FI(Rsp->x - Lsp->x ) +1;
		if (XLen > 0)
		{//Xラインのクリッピング.
			int StartX = FI(Lsp->x);			
			BYTE	*D2 ;
			Fixed	*ZBufferWork;
			Fixed	Z , StepZ;
            Z  = Lsp->z;	StepZ  = (Rsp->z - Lsp->z)   / XLen;
			//範囲の外からスタートしたときは、範囲内に押し戻す.
			if (StartX < 0)
			{
				StartX = -StartX;	//符号反転.
				XLen -= StartX;		//転送範囲を縮める..
				D2 = Data2 + 0 + 0 + 0;
				ZBufferWork = ZBuffer;
                Z  += StepZ  * StartX;	// テクスチャ転送位置移動 (固定 * 正数)
				StartX = 0;
			}
			else
			{   //範囲内の正常な転送です.
				D2 = Data2 + (StartX+StartX+StartX); //( * 3 は足し算で代用.)
				ZBufferWork = ZBuffer + StartX;
			}
			//転送先のヤツが横幅を超えないように....
			int Temp = ( Bmd2W - (StartX + XLen) );
			if (Temp < 0 )  XLen+=Temp;  //
			//転送開始
			for (register int l = 0 ; l < XLen ; l++)
			{
				if ( *ZBufferWork > Z)
				{
					*ZBufferWork = Z;
					DirectPSet(D2, Rgb );
				}
				D2 += 3;
				ZBufferWork++;
				Z  += StepZ;
			}
		}
		//各要素のプラス.
		StepUp( Lwp , Rwp , Lsp , Rsp);
        Data2 += AddPitch2;
		ZBuffer += Bmd2W;
	}
}










//true 
		//    /A
		//  /  |
		//B/   |
		//  \  |
		//   \ |
		//    \C
//false
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
//Bが左右のどっちに来るかの判別式.
bool TTriangleSuperDraw::LeftOrRight(const _XYZP* Top  ,const _XYZP* Median ,const _XYZP* Buttom)
{
	int Mpos;
	if (Top->x == Buttom->x)
	{
		Mpos = Top->x;
	}
	else
	{
		Mpos = (Top->y - Buttom->y) * 
			    (Median->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;
	}

	if (Top->x > Buttom->x)
	{
		if (Mpos > Median->y)	return	true;
		else					return 	false;
	} 
	else if (Top->x < Buttom->x)
	{
		if (Mpos >= Median->y)	return 	false;
		else					return	true;
	}
	else
	{
		if (Median->x < Top->x)	return	true;
		else					return 	false;
	}
}

//座標 A B C を受け取り線形補完情報を取得する.
//返り血は、描画するポイントの数.
int TTriangleSuperDraw::AcquisitionOfInformationgForDrawing
			(_NN_WorkAreaP* outLRData ,_XYZP *outStartLR ,  _XYZP *inA ,_XYZP *inB ,_XYZP *inC)
{
	_XYZP* YSort[3];
	int Ay = FI(inA->y),By = FI(inB->y), Cy = FI(inC->y);

	// Y でソート.
	if (Ay < By)
	{ // A < B
		if (By < Cy)
		{ // A < B < C
			YSort[0] = inA;			YSort[1] = inB;			YSort[2] = inC;
		}
		else if (Ay < Cy)
		{ //  A < C < B
			YSort[0] = inA;			YSort[1] = inC;			YSort[2] = inB;
		}
		else
		{ // C < A < B
			YSort[0] = inC;			YSort[1] = inA;			YSort[2] = inB;
		}
	}
	else
	{ // B < A
		if (Cy < By)
		{ // C < B < A
			YSort[0] = inC;			YSort[1] = inB;			YSort[2] = inA;
		}
		else if (Ay < Cy)
		{ //  B < A < C
			YSort[0] = inB;			YSort[1] = inA;			YSort[2] = inC;
		}
		else
		{ // B < C < A
			YSort[0] = inB;			YSort[1] = inC;			YSort[2] = inA;
		}
	}

	//絵画の必要なし.
	if (YSort[0]->y >= IF(240) || YSort[2]->y < 0) return -1;

	//
	Ay = FI(YSort[0]->y),By = FI(YSort[1]->y), Cy = FI(YSort[2]->y);

	if (Ay == By)
	{
		if (By == Cy)
		{// Line ..
			return -1;
		}
		else
		{//
			//  A--------B
			//   \      /
			//    \    /
			//     \  /
			//      \/
			//      C
			//A と B は A の方が、 X 座標的に小さくなければならない.
			if ( FI(YSort[0]->x) > FI(YSort[1]->x) ) 
			{
				register _XYZP*	Temp;			
				Temp = YSort[1];	YSort[1] = YSort[0];	YSort[0] = Temp;
			}
			outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[1];
			return ReverseTriangle(outLRData , YSort);
		}
	}
	else if (By == Cy)
	{//
		//      A
		//      /\
		//     /  \
		//    /    \
		//   /      \
		//  B--------C
		//B と C は A の方が、 X 座標的に小さくなければならない.
		if ( FI(YSort[1]->x) > FI(YSort[2]->x) ) 
		{
			register _XYZP*	Temp;			
			Temp = YSort[1];	YSort[1] = YSort[2];	YSort[2] = Temp;
		}
		outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[0];
		return EquilateralTriangle(outLRData , YSort);
	}
	else if ( LeftOrRight(YSort[0],YSort[1] , YSort[2]) )
	{
		//    /A
		//  /  |
		//B/   |
		//  \  |
		//   \ |
		//    \C
		outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[0];
		return LeftTriangle(outLRData , YSort);
	}
	else
	{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
		outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[0];
		return RightTriangle(outLRData , YSort);
	}
	//なしもなし.
	return -1;
}

//左に三角形
int TTriangleSuperDraw::LeftTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
		//    /A
		//  /  |
		//B/   |
		//  \  |
		//   \ |
		//    \C
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[1] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[2] );
	P->Y = FI(YPointes[1]->y);

	P = &outLRData[1];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]  ,YPointes[1] , YPointes[2] );
	P->Data[__TRIANGLE_DRAW_RIGHT] = outLRData[0].Data[__TRIANGLE_DRAW_RIGHT];
	//HokanLine( &outLRData[1].Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[2] );

	P->Y = FI(YPointes[2]->y);

	return 1;
}

//右に三角形
int TTriangleSuperDraw::RightTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[2] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[1] );
	P->Y = FI(YPointes[1]->y);

	P = &outLRData[1];
	P->Data[__TRIANGLE_DRAW_LEFT] = outLRData[0].Data[__TRIANGLE_DRAW_LEFT];
	//HokanLine( &outLRData[0].Data[__TRIANGLE_DRAW_LEFT]  ,YPointes[0] , YPointes[2] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[1] , YPointes[2] );
	P->Y = FI(YPointes[2]->y);

	return 1;
}

//正三角形(?) 実は、二等辺三角形
int TTriangleSuperDraw::EquilateralTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
		//      A
		//      /\
		//     /  \
		//    /    \
		//   /      \
		//  B--------C
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[1] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[2] );
	P->Y = FI(YPointes[1]->y);
	
	return 0;
}


//逆三角形
int TTriangleSuperDraw::ReverseTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
			//  A--------B
			//   \      /
			//    \    /
			//     \  /
			//      \/
			//      C
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[2] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[1] , YPointes[2] );
	P->Y = FI(YPointes[2]->y);
	
	return 0;
}

//勇気のクリッピング
int TTriangleSuperDraw::ClippingProcessing(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos)
{
	//ASSERT(inLRData != NULL);
	//ASSERT(inStartPos != NULL);
	//ASSERT(StopPos >= 0);
	//ASSERT(StopPos <= 1);

	int Pos;
	//上のクリッピング.
	Pos = ClippingProcessingUp(inLRData,inStartPos,inStopPos);
	if (Pos < 0) return -1;
	//下のクリッピング.
	return  ClippingProcessingDown(inLRData,inStartPos,Pos);
}

//上方向におけるクリッピング.
int TTriangleSuperDraw::ClippingProcessingUp(_NN_WorkAreaP* outLRData , _XYZP* outStartPos , int inStopPos)
{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
	//上方向におけるクリッピング.
	int		YA , YB , YC ;
	YA = FI(outStartPos[__TRIANGLE_DRAW_LEFT].y);
	if (YA < 0 )
	{//開始点のAがマイナス...
		YB = FI(outLRData[0].Y);
		if (YB < 0)
		{//中間の B もマイナス...
			StepUp(&(outLRData[0].Data[__TRIANGLE_DRAW_LEFT]) , &(outLRData[0].Data[__TRIANGLE_DRAW_RIGHT]) ,
					&outStartPos[__TRIANGLE_DRAW_LEFT] ,&outStartPos[__TRIANGLE_DRAW_RIGHT],  -YB );
			YC = FI(outLRData[1].Y);
			//さらに、 YCも0だったら描画できません. ..しかし、これは前のルーチンで除外されてます.
			//if(YC < 0) return -1;
			outLRData[0] = outLRData[1];		//Bが消えることで、1 が 0 にのし上がるため.
			StepUp(&(outLRData[0].Data[__TRIANGLE_DRAW_LEFT]) , &(outLRData[0].Data[__TRIANGLE_DRAW_RIGHT]) ,
					&outStartPos[__TRIANGLE_DRAW_LEFT] ,&outStartPos[__TRIANGLE_DRAW_RIGHT],  (-YC)+(-YB) );
			//スタート地点をずらす.
			outStartPos[__TRIANGLE_DRAW_LEFT].y  = IF(0);	outStartPos[__TRIANGLE_DRAW_RIGHT].y = IF(0);
			return 0;
		}
		else
		{//中間 B はプラス. 分岐点は?
			//各要素のプラス.
			StepUp(&(outLRData[0].Data[__TRIANGLE_DRAW_LEFT]) , &(outLRData[0].Data[__TRIANGLE_DRAW_RIGHT]) ,
					&outStartPos[__TRIANGLE_DRAW_LEFT] ,&outStartPos[__TRIANGLE_DRAW_RIGHT],  -YA );
			//スタート地点をずらす.
			outStartPos[__TRIANGLE_DRAW_LEFT].y  = IF(0);	outStartPos[__TRIANGLE_DRAW_RIGHT].y = IF(0);
			return 0;	//A と B 間の間から.
		}
	}
	//ノークリッピング
	return inStopPos;
}
//下方向におけるクリッピング.
int TTriangleSuperDraw::ClippingProcessingDown(_NN_WorkAreaP* outLRData , _XYZP* inStartPos , int inStopPos)
{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
	//下方向におけるクリッピング.
	int		YA , YB , YC ;
	//YA に関しては下方向のクリッピングは調べない.
	//前にYAは絶対に 240より大きくないと証明されているからだ.

	YA = FI(inStartPos[__TRIANGLE_DRAW_LEFT].y);
	YB = outLRData[0].Y;
	YC = outLRData[1].Y;
	if (YB >= 240 )
	{//YBの時点でアウト. A-B間.
		outLRData[0].Y = 240 ;
		//YCまでの到達はできないので、ポイントは 0 になります.
		return 0;
	}
	else if (YC >= 240 )
	{//YCは沈んでいます. B-C間.
		outLRData[1].Y = 240 ;
		return 1;
	}
	//ノークリッピング
	return inStopPos;
}

//
void TTriangleSuperDraw::TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData)
{
	//絵画の必要なし X バージョン.
	if (inA->x < 0 && inB->x < 0 && inC->x < 0) return ;
	if (inA->x >= IF(320) && inB->x >= IF(320) && inC->x >= IF(320) ) return ;
	//補完情報.
	_NN_WorkAreaP		LRData[3];
	//スタート地点.
	_XYZP				StartPos[2];
	//愛の座標取得
	//座標 A B C を受け取り線形補完情報を取得する.
	//返り血は、描画するポイントの数.
	int StopPos = AcquisitionOfInformationgForDrawing( LRData , StartPos , inA , inB , inC);
	if (StopPos < 0) return ;

	//勇気のクリッピング
	StopPos = ClippingProcessing( LRData , StartPos , StopPos);
	if (StopPos < 0) return ;

	try
	{
		//希望のドロー.
		MainDraw(LRData , StartPos , StopPos , inData);
	}
	catch(...)
	{
		DEBUG3("	_XYZP A = {	%d	,	%d	,	%d	};",inA->x,inA->y,inA->z);
		DEBUG3("	_XYZP B = {	%d	,	%d	,	%d	};",inB->x,inB->y,inB->z);
		DEBUG3("	_XYZP C = {	%d	,	%d	,	%d	};",inC->x,inC->y,inC->z);

		_XYZP DebugA,DebugB,DebugC;
		DebugA.x = FI(inA->x);	DebugA.y = FI(inA->y);	DebugA.z = FI(inA->z);
		DebugB.x = FI(inB->x);	DebugB.y = FI(inB->y);	DebugB.z = FI(inB->z);
		DebugC.x = FI(inC->x);	DebugC.y = FI(inC->y);	DebugC.z = FI(inC->z);

		DEBUG3("			{	%d	,	%d	,	%d	};",DebugA.x,DebugA.y,DebugA.z);
		DEBUG3("			{	%d	,	%d	,	%d	};",DebugB.x,DebugB.y,DebugB.z);
		DEBUG3("			{	%d	,	%d	,	%d	};",DebugC.x,DebugC.y,DebugC.z);
		exit(0);
	}
}

