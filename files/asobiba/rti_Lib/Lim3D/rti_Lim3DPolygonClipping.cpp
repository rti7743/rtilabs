#include <./LIm3D/rti_Lim3DPolygonClipping.h>

//四角形の場合で、次の頂点へ移動.
#define ONE_STEP_BACK_VEC(NowPoint) (NowPoint-1 < 0 ? 3 : NowPoint-1) 
#define ONE_STEP_NEXT_VEC(NowPoint) (NowPoint+1 > 3 ? 0 : NowPoint+1) 

//三角形の場合で、次の頂点へ移動.
#define ONE_STEP_BACK_VEC3(NowPoint) (NowPoint-1 < 0 ? 2 : NowPoint-1) 
#define ONE_STEP_NEXT_VEC3(NowPoint) (NowPoint+1 > 2 ? 0 : NowPoint+1) 

//pdp1 と pdp2 の間にある クリップ平面 (Z_CLIPPING) による分割.
void TLim3DPolygonClipping::LineCut( DrawUse * du ,
					const PolygonDataEx* pdp1 , const PolygonDataEx* pdp2 )
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp1 != NULL);
	__RTI_CHECKER(pdp2 != NULL);

	XYZFixed*    Eye1 = &(pdp1->LinkVecPointer->Eye);
	XYZFixed*    Eye2 = &(pdp2->LinkVecPointer->Eye);
	XY*	         Texture1 = (XY*) &(pdp1->Texture);
	XY*	         Texture2 = (XY*) &(pdp2->Texture);

    Fixed TrueLine = Eye1->z - Eye2->z ;
    Fixed t        = FixedDiv( ( IF(Z_CLIPPING) - Eye2->z) , TrueLine);
    Fixed ima      = IF(1) - t ;
	du->Pos.x = FixedMul(Eye2->x , ima) + FixedMul(Eye1->x , t);
    du->Pos.y = FixedMul(Eye2->y , ima) + FixedMul(Eye1->y , t);
    du->Pos.z = IF(Z_CLIPPING);
    du->Texture.x = (short) FI(Texture2->x * ima + Texture1->x * t );
    du->Texture.y = (short) FI(Texture2->y * ima + Texture1->y * t );
}

//pdp を du にふつーにコピーする.
void TLim3DPolygonClipping::NormalCopy( DrawUse * du ,	const PolygonDataEx* pdp )
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	
	du->Pos = pdp->LinkVecPointer->Eye;
	du->Texture = (pdp->Texture);
}

//四角形において 3点クリップされる.	>>三角形になる.
bool TLim3DPolygonClipping::ClippingShiftIntoTriangle
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

    /*
    *      T  
    *     /\  
    * ================ Z ↓消える
    *   /    \
    *  /     / R
    *L/     / 
    * \    /
    *  \  /  
    *   \/
    *   B
    */
	int NoClip;
	int i ;
	//クリップされていない頂点を探しまするー
	for( i = 0 , NoClip = 0		;	i < 3	;	 i++	, NoClip ++)
	{
		if ( ClippingNo[i] != NoClip )	break;
	}
	//NoClip に格納された、クリップされていない頂点を T と名づけるアルヨ.
	// L と T の間のクリップ点を求めるアルヨ.
	LineCut( &du[0] , &pdp[NoClip] , &pdp[ ONE_STEP_BACK_VEC(NoClip) ] );
	//T をコピー.
	NormalCopy( &du[1] , &pdp[NoClip] );
	// T と R の間のクリップ点を求めるアルヨ.
	LineCut( &du[2] , &pdp[NoClip] , &pdp[ ONE_STEP_NEXT_VEC(NoClip) ] );

	return true;
}


//四角形において 2点クリップされる.	>>四角形になる.
bool TLim3DPolygonClipping::ClippingShiftIntoSquare
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

    /*
    *      T  
    *     /\  
    *    /  \ 
    *   /    \
    *  /     / R
    *L================ Z ↓消える
    * \    /
    *  \  /  
    *   \/
    *   B
    */
	//クリップされる場所.
	int Clip0 = ClippingNo[0];
	int Clip1 = ClippingNo[1];

	int i ;
	for (i = 0 		 ; i < 4 	 ; i++ )
	{
		if ( Clip0 == i )
		{//これ、クリップされるアルヨ.
			//一つ前の点はクリップされるの ?
			int Use = ONE_STEP_BACK_VEC(i);
			//クリップされるならば、一つ先の点にする.
			if (Use == Clip1)	Use = ONE_STEP_NEXT_VEC(i);
			//そのクリップされない点 Use と、現在のクリップされるやつと間を求める.
			LineCut( &du[i] , &pdp[Clip0] , &pdp[ Use ] );
		}
		else	if ( Clip1 == i )
		{
			//一つ前の点はクリップされるの ?
			int Use = ONE_STEP_BACK_VEC(i);
			//クリップされるならば、一つ先の点にする.
			if (Use == Clip0)	Use = ONE_STEP_NEXT_VEC(i);
			//そのクリップされない点 Use と、現在のクリップされるやつと間を求める.
			LineCut( &du[i] , &pdp[Clip1] , &pdp[ Use ] );
		}
		else
		{	//クリップされないので、平和アルヨ.
			NormalCopy( &du[i] , &pdp[i] );
		}
	}
	return true;
}

//四角形において 1点クリップされる.	>>五角形になる.
bool TLim3DPolygonClipping::ClippingShiftIntoPentagon
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

    /*
    *      T  
    *     /\  
    *    /  \ 
    *   /    \
    *  /     / R
    *L/     / 
    * \    /
    * ================ Z ↓消える
    *   \/
    *   B
    */
	//クリップされる場所.
	int Clip0 = ClippingNo[0];
	int i , l ;
	for (i = 0 , l= 0 		 ; i < 4 	 ; i++ , l++)
	{
		if ( Clip0 == i )
		{//これ、クリップされるアルヨ.
			//一つ前の点はなにあるかー.時計周りならば、 R になりまする.
			//一つ前の点と、本来の点との間のクリップされる点をもとめるアルヨ.
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_BACK_VEC(i) ] );

			//次は、時計周りで、 L と B の間のクリップされる点をもとめまするー.
			l++;	//格納庫は、一つずれまするー
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_NEXT_VEC(i) ] );
		}
		else
		{	//クリップされないので、平和アルヨ.
			NormalCopy( &du[l] , &pdp[i] );
		}
	}
	return true;
}
//三角形において 1点クリップされる.	>>四角形になる.
bool TLim3DPolygonClipping::ClippingTriangleShiftIntoSquare
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

	/*
	*   T  ------------  M
	*      \          /
	*       \        /
	*        \      /
	* ==================Z ↓消える.
	*          \  /
	*           \/
	*           B
	*/
	//クリップされる場所.
	int Clip0 = ClippingNo[0];
	int i , l ;
	for (i = 0 , l= 0 		 ; i < 3 	 ; i++ , l++)
	{
		if ( Clip0 == i )
		{//これ、クリップされるアルヨ.
			//一つ前の点はなにあるかー.時計周りならば、 B になりまする.
			//一つ前の点と、本来の点との間のクリップされる点をもとめるアルヨ.
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_BACK_VEC3(i) ] );

			//次は、時計周りで、 B と T の間のクリップされる点をもとめまするー.
			l++;	//格納庫は、一つずれまするー
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_NEXT_VEC3(i) ] );
		}
		else
		{	//クリップされないので、平和アルヨ.
			NormalCopy( &du[l] , &pdp[i] );
		}
	}
	return true;
}

//三角形において 2点クリップされる.	>>三角形になる.
bool TLim3DPolygonClipping::ClippingTriangleShiftIntoTriangle
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);
	/*
	*         T
	*           /\
	*          /  \
	*         /    \
	* ========================↓Z   消える.
	*       /        \
	*      /          \
	*    M ------------  B
	*/
	//クリップされる場所.
	int Clip0 = ClippingNo[0];
	int Clip1 = ClippingNo[1];

	int i ;
	for (i = 0 		 ; i < 3 	 ; i++ )
	{
		if ( Clip0 == i )
		{//これ、クリップされるアルヨ.
			//一つ前の点はクリップされるの ?
			int Use = ONE_STEP_BACK_VEC3(i);
			//クリップされるならば、一つ先の点にする.
			if (Use == Clip1)	Use = ONE_STEP_NEXT_VEC3(i);
			//そのクリップされない点 Use と、現在のクリップされるやつと間を求める.
			LineCut( &du[i] , &pdp[Clip0] , &pdp[ Use ] );
		}
		else	if ( Clip1 == i )
		{
			//一つ前の点はクリップされるの ?
			int Use = ONE_STEP_BACK_VEC3(i);
			//クリップされるならば、一つ先の点にする.
			if (Use == Clip0)	Use = ONE_STEP_NEXT_VEC3(i);
			//そのクリップされない点 Use と、現在のクリップされるやつと間を求める.
			LineCut( &du[i] , &pdp[Clip1] , &pdp[ Use ] );
		}
		else
		{	//クリップされないので、平和アルヨ.
			NormalCopy( &du[i] , &pdp[i] );
		}
	}
	return true;
}



//クリッピング開始.
int TLim3DPolygonClipping::DoClipping(DrawUse * DrawU ,
								const PolygonDataEx* PolygonPhantom , int Many)
{
	__RTI_CHECKER(DrawU != NULL);
	__RTI_CHECKER(PolygonPhantom != NULL);
	__RTI_CHECKER(Many>=3);
	__RTI_CHECKER(Many<=4);

    register int i;
	//クリップされる頂点が入る.
	int  ClippingNo[4];
	int* lpClippingNo = ClippingNo;
    int  ClipingNum = 0;        //クリッピングされた数.

	const PolygonDataEx* pdp = PolygonPhantom;
    //その各頂点が見えるかどうか調べる.
    for(i = 0 ; i < Many ; i ++)
    {
        if (pdp->LinkVecPointer->Eye.z <= IF(Z_CLIPPING) )
        {   //クリッピングされるヤツ.
			ClipingNum ++;		//クリップされる頂点の数を増やして.
			*lpClippingNo = i;	//クリップされる頂点ナンバーを記録しておく,
            lpClippingNo ++;	//クリップ頂点格納場所をずらす.
        }
        pdp++;					//次のデータへ.
    }

	if (Many == 4)
	{//四角形
		switch(ClipingNum)
		{
	    case 0: //ふつーに絵画しても ok です
			NormalCopy( &DrawU[0] , &PolygonPhantom[0] );
			NormalCopy( &DrawU[1] , &PolygonPhantom[1] );
			NormalCopy( &DrawU[2] , &PolygonPhantom[2] );
			NormalCopy( &DrawU[3] , &PolygonPhantom[3] );
			return 4;
		    break;
	    case 1: //これが一番やっかい. 5 角形になります.
		    ClippingShiftIntoPentagon( DrawU , PolygonPhantom , ClippingNo);
			return 5;
		    break;
	    case 2: //ふつーに四角形.
		    ClippingShiftIntoSquare( DrawU , PolygonPhantom , ClippingNo);
			return 4;
	        break;
		case 3: //3三角形になります.
			ClippingShiftIntoTriangle( DrawU , PolygonPhantom , ClippingNo);
			return 3;
		    break;
		}
	}
	else
	{//三角形
		switch(ClipingNum)
		{
	    case 0: //ふつーに絵画しても ok です
			NormalCopy( &DrawU[0] , &PolygonPhantom[0] );
			NormalCopy( &DrawU[1] , &PolygonPhantom[1] );
			NormalCopy( &DrawU[2] , &PolygonPhantom[2] );
			return 3;
		    break;
	    case 1: //これが一番やっかい. 4 角形になります.
		    ClippingTriangleShiftIntoSquare( DrawU , PolygonPhantom , ClippingNo);
			return 4;
		    break;
	    case 2: //ふつーに三角形.
		    ClippingTriangleShiftIntoTriangle( DrawU , PolygonPhantom , ClippingNo);
			return 3;
	        break;
		}
	}
	//すべて消されてしまった....
    return 0;
}
