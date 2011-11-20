#include <./Lim3D/rti_Matrix.h>


//コンストラクタ
TMatrix::TMatrix()
{
}

//コピーコンストラクタ
TMatrix::TMatrix(TMatrix &Ma)
{
	memcpy( m_Matrix , Ma.m_Matrix , sizeof(m_Matrix) );
}

//単位行列の作成.
void TMatrix::Unit()
{
    m_Matrix[0] = IF(1);m_Matrix[1] = 0;    m_Matrix[2] = 0;	m_Matrix[3] = 0;
    m_Matrix[4] = 0    ;m_Matrix[5] = IF(1);m_Matrix[6] = 0;	m_Matrix[7] = 0;
    m_Matrix[8] = 0    ;m_Matrix[9] = 0;    m_Matrix[10]= IF(1);m_Matrix[11]= 0;
    m_Matrix[12]= 0    ;m_Matrix[13]= 0;    m_Matrix[14]= 0;	m_Matrix[15]= IF(1);
}

//高速逆行列
void TMatrix::FastRev(TMatrix* inMat)
{
    //説明!!(手抜き)
    /*
           | a b c |             | a d g |
           | d e f |     ======> | b e h |
           | g h i |             | c f i |
    */
	Fixed   * theSrcMatrix = inMat->m_Matrix;
	Fixed	theAns[4+4+4+4];
	theAns[0] = theSrcMatrix[0];
	theAns[1] = theSrcMatrix[4];
	theAns[2] = theSrcMatrix[8];
	theAns[3] = theSrcMatrix[12];
	theAns[4] = theSrcMatrix[1];
	theAns[5] = theSrcMatrix[5];
	theAns[6] = theSrcMatrix[9];
	theAns[7] = theSrcMatrix[13];
	theAns[8] = theSrcMatrix[2];
	theAns[9] = theSrcMatrix[6];
	theAns[10]= theSrcMatrix[10];
	theAns[11]= theSrcMatrix[14];
	theAns[12]= theSrcMatrix[3];
	theAns[13]= theSrcMatrix[7];
	theAns[14]= theSrcMatrix[11];
	theAns[15]= theSrcMatrix[15];
	memcpy(m_Matrix , theAns , sizeof(m_Matrix) );
}

//ベクトルとのかけ算.
void TMatrix::VectorMul(XYZFixed *Dest,XYZFixed const *Src)
{
      	Fixed x,y,z;
        x = (Src->x) - m_Matrix[3];
        y = (Src->y) - m_Matrix[7];
		z = (Src->z) - m_Matrix[11]; 
        Dest->x =FixedMul(m_Matrix[0],x) + FixedMul(m_Matrix[4],y) + FixedMul(m_Matrix[8],z)  + m_Matrix[12];
        Dest->y =FixedMul(m_Matrix[1],x) + FixedMul(m_Matrix[5],y) + FixedMul(m_Matrix[9],z)  + m_Matrix[13];
        Dest->z =FixedMul(m_Matrix[2],x) + FixedMul(m_Matrix[6],y) + FixedMul(m_Matrix[10],z) + m_Matrix[14];
}

//合成.
void TMatrix::Synthesis(TMatrix *Ma)
{
	Fixed   * theSrcMatrix = Ma->m_Matrix;
	Fixed     theAns[4+4+4+4];

	theAns[0] = FixedMul( m_Matrix[0] , theSrcMatrix[0] )
					+ FixedMul( m_Matrix[1] , theSrcMatrix[4] )
					+ FixedMul( m_Matrix[2] , theSrcMatrix[8] )
					+ FixedMul( m_Matrix[3] , theSrcMatrix[12] );

	theAns[1] = FixedMul( m_Matrix[0] , theSrcMatrix[1] )
					+ FixedMul( m_Matrix[1] , theSrcMatrix[5] )
					+ FixedMul( m_Matrix[2] , theSrcMatrix[9] )
					+ FixedMul( m_Matrix[3] , theSrcMatrix[13] );

	theAns[2] = FixedMul( m_Matrix[0] , theSrcMatrix[2] )
					+ FixedMul( m_Matrix[1] , theSrcMatrix[6] )
					+ FixedMul( m_Matrix[2] , theSrcMatrix[10] )
					+ FixedMul( m_Matrix[3] , theSrcMatrix[14] );

	theAns[3] = FixedMul( m_Matrix[0] , theSrcMatrix[3] )
					+ FixedMul( m_Matrix[1] , theSrcMatrix[7] )
					+ FixedMul( m_Matrix[2] , theSrcMatrix[11] )
					+ FixedMul( m_Matrix[3] , theSrcMatrix[15] );


	theAns[4] = FixedMul( m_Matrix[4] , theSrcMatrix[0] )
					+ FixedMul( m_Matrix[5] , theSrcMatrix[4] )
					+ FixedMul( m_Matrix[6] , theSrcMatrix[8] )
					+ FixedMul( m_Matrix[7] , theSrcMatrix[12] );

	theAns[5] = FixedMul( m_Matrix[4] , theSrcMatrix[1] )
					+ FixedMul( m_Matrix[5] , theSrcMatrix[5] )
					+ FixedMul( m_Matrix[6] , theSrcMatrix[9] )
					+ FixedMul( m_Matrix[7] , theSrcMatrix[13] );

	theAns[6] = FixedMul( m_Matrix[4] , theSrcMatrix[2] )
					+ FixedMul( m_Matrix[5] , theSrcMatrix[6] )
					+ FixedMul( m_Matrix[6] , theSrcMatrix[10] )
					+ FixedMul( m_Matrix[7] , theSrcMatrix[14] );

	theAns[7] = FixedMul( m_Matrix[4] , theSrcMatrix[3] )
					+ FixedMul( m_Matrix[5] , theSrcMatrix[7] )
					+ FixedMul( m_Matrix[6] , theSrcMatrix[11] )
					+ FixedMul( m_Matrix[7] , theSrcMatrix[15] );


	theAns[8] = FixedMul( m_Matrix[8] , theSrcMatrix[0] )
					+ FixedMul( m_Matrix[9] , theSrcMatrix[4] )
					+ FixedMul( m_Matrix[10] , theSrcMatrix[8] )
					+ FixedMul( m_Matrix[11] , theSrcMatrix[12] );

	theAns[9] = FixedMul( m_Matrix[8] , theSrcMatrix[1] )
					+ FixedMul( m_Matrix[9] , theSrcMatrix[5] )
					+ FixedMul( m_Matrix[10] , theSrcMatrix[9] )
					+ FixedMul( m_Matrix[11] , theSrcMatrix[13] );

	theAns[10] = FixedMul( m_Matrix[8] , theSrcMatrix[2] )
					+ FixedMul( m_Matrix[9] , theSrcMatrix[6] )
					+ FixedMul( m_Matrix[10] , theSrcMatrix[10] )
					+ FixedMul( m_Matrix[11] , theSrcMatrix[14] );

	theAns[11] = FixedMul( m_Matrix[8] , theSrcMatrix[3] )
					+ FixedMul( m_Matrix[9] , theSrcMatrix[7] )
					+ FixedMul( m_Matrix[10] , theSrcMatrix[11] )
					+ FixedMul( m_Matrix[11] , theSrcMatrix[15] );


	theAns[12] = FixedMul( m_Matrix[12] , theSrcMatrix[0] )
					+ FixedMul( m_Matrix[13] , theSrcMatrix[4] )
					+ FixedMul( m_Matrix[14] , theSrcMatrix[8] )
					+ FixedMul( m_Matrix[15] , theSrcMatrix[12] );

	theAns[13] = FixedMul( m_Matrix[12] , theSrcMatrix[1] )
					+ FixedMul( m_Matrix[13] , theSrcMatrix[5] )
					+ FixedMul( m_Matrix[14] , theSrcMatrix[9] )
					+ FixedMul( m_Matrix[15] , theSrcMatrix[13] );

	theAns[14] = FixedMul( m_Matrix[12] , theSrcMatrix[2] )
					+ FixedMul( m_Matrix[13] , theSrcMatrix[6] )
					+ FixedMul( m_Matrix[14] , theSrcMatrix[10] )
					+ FixedMul( m_Matrix[15] , theSrcMatrix[14] );

	theAns[15] = FixedMul( m_Matrix[12] , theSrcMatrix[3] )
					+ FixedMul( m_Matrix[13] , theSrcMatrix[7] )
					+ FixedMul( m_Matrix[14] , theSrcMatrix[11] )
					+ FixedMul( m_Matrix[15] , theSrcMatrix[15] );

	memcpy(m_Matrix , theAns , sizeof(m_Matrix) );
}

//通常移動行列作成
void TMatrix::MakeTrans(Angle AngleX,Angle AngleY,Angle AngleZ,
											Fixed Tx, Fixed Ty,Fixed Tz)
{
	Fixed	fxCosX, fxSinX, fxCosY,fxSinY,fxCosZ,fxSinZ,
			fxCosXSinZ,fxSinXCosZ,fxCosXCosZ,fxSinXSinZ;
	Fixed fxSinAddXZ ,fxCosAddXZ ,fxSinSubXZ , fxCosSubXZ ;

	fxCosX = (FixedLTBCos1024[__ANGLE(AngleX)] );
	fxSinX = (FixedLTBSin1024[__ANGLE(AngleX)] );
	fxCosY = (FixedLTBCos1024[__ANGLE(AngleY)] );
	fxSinY = (FixedLTBSin1024[__ANGLE(AngleY)] );
	fxCosZ = (FixedLTBCos1024[__ANGLE(AngleZ)] );
	fxSinZ = (FixedLTBSin1024[__ANGLE(AngleZ)] );
	fxSinAddXZ = (FixedLTBSin1024[__ANGLE(AngleX+AngleZ)] );
	fxCosAddXZ = (FixedLTBCos1024[__ANGLE(AngleX+AngleZ)] );
	fxSinSubXZ = (FixedLTBSin1024[__ANGLE(AngleX-AngleZ)] );
	fxCosSubXZ = (FixedLTBCos1024[__ANGLE(AngleX-AngleZ)] );

	fxCosXSinZ = (fxSinAddXZ - fxSinSubXZ)/2;
	fxSinXCosZ = (fxSinAddXZ + fxSinSubXZ)/2;
	fxCosXCosZ = (fxCosAddXZ + fxCosSubXZ)/2;
	fxSinXSinZ = - ((fxCosAddXZ - fxCosSubXZ)/2);


	m_Matrix[0]= FixedMul(fxCosY,fxCosZ);
	m_Matrix[1]= (-FixedMul(fxCosY,fxSinZ) );
	m_Matrix[2]= (fxSinY);
	m_Matrix[3]= 0;

	m_Matrix[4]= (fxCosXSinZ + FixedMul(fxSinXCosZ, fxSinY)  );
	m_Matrix[5]= (fxCosXCosZ - FixedMul(fxSinXSinZ, fxSinY)  );
	m_Matrix[6]= (-FixedMul(fxSinX,fxCosY) );
	m_Matrix[7]= 0;

	m_Matrix[8]= (fxSinXSinZ - FixedMul(fxCosXCosZ, fxSinY)  );
	m_Matrix[9]= (fxSinXCosZ + FixedMul(fxCosXSinZ, fxSinY)  );
	m_Matrix[10]= FixedMul(fxCosX,fxCosY);
	m_Matrix[11]= 0;

	m_Matrix[12]= Tx;
	m_Matrix[13]= Ty;
	m_Matrix[14]= Tz;
	m_Matrix[15]= IF(1);

}

/*
* Y X Z 回転
*			Orignal Matrix
* CosY * CosZ + SinY * SinX * SinZ		-CosY * SinZ + SinY * SinX * CosZ		SinY * CosX
* CosX * SinZ							 CosX * CosZ							-SinX
*-SinY * CosZ + CosY * SinX * SinZ		 SinY * SinZ + CosY * SinX * CosZ		CosY * CosX
*
*/
//視点移動行列 
void TMatrix::MakeTransYXZ(Angle AngleX,Angle AngleY,Angle AngleZ ,
											Fixed Tx, Fixed Ty,Fixed Tz)
{
	//以下のルーチンは、三角関数の加法定理の応用により、
	//掛け算を足し算その他で補っています.
	//多少ソースがみにくいかもしれません (^^;;
	Fixed	fxCosX, fxSinX, fxCosY,fxSinY,fxCosZ,fxSinZ;
	//共有 fxCos fxSin
	Fixed	fxCosAddYX,fxCosAddYZ,fxCosSubYX,fxCosSubYZ,
			fxSinAddYX,fxSinAddYZ,fxSinSubYX,fxSinSubYZ;
	//共有 fxSinYSinX  fxCosYCosX
	Fixed	fxSinYSinX,fxCosYSinX;

	fxCosX = (FixedLTBCos1024[__ANGLE(AngleX) ] );
	fxSinX = (FixedLTBSin1024[__ANGLE(AngleX) ] );
	fxCosY = (FixedLTBCos1024[__ANGLE(AngleY) ] );
	fxSinY = (FixedLTBSin1024[__ANGLE(AngleY) ] );
	fxCosZ = (FixedLTBCos1024[__ANGLE(AngleZ) ] );
	fxSinZ = (FixedLTBSin1024[__ANGLE(AngleZ) ] );
	fxCosAddYX = (FixedLTBCos1024[__ANGLE(AngleY+AngleX)] );
	fxCosAddYZ = (FixedLTBCos1024[__ANGLE(AngleY+AngleZ)] );
	fxCosSubYX = (FixedLTBCos1024[__ANGLE(AngleY-AngleX)] );
	fxCosSubYZ = (FixedLTBCos1024[__ANGLE(AngleY-AngleZ)] );
	fxSinAddYX = (FixedLTBSin1024[__ANGLE(AngleY+AngleX)] );
	fxSinAddYZ = (FixedLTBSin1024[__ANGLE(AngleY+AngleZ)] );
	fxSinSubYX = (FixedLTBSin1024[__ANGLE(AngleY-AngleX)] );
	fxSinSubYZ = (FixedLTBSin1024[__ANGLE(AngleY-AngleZ)] );
	//共有掛け算.
	fxSinYSinX =-(fxCosAddYX - fxCosSubYX) / 2;
	fxCosYSinX = (fxSinAddYX - fxSinSubYX) / 2;

	m_Matrix[0]= ((fxCosAddYZ + fxCosSubYZ)/2)  + FixedMul( fxSinYSinX , fxSinZ);
	m_Matrix[1]= (-(fxSinAddYZ - fxSinSubYZ)/2) + FixedMul( fxSinYSinX , fxCosZ);
	m_Matrix[2]= (fxSinAddYX + fxSinSubYX)/2;
	m_Matrix[3]= (Tx);

	m_Matrix[4]= FixedMul(fxCosX , fxSinZ);
	m_Matrix[5]= FixedMul(fxCosX , fxCosZ);
	m_Matrix[6]= (-fxSinX);
	m_Matrix[7]= (Ty);

	m_Matrix[8]= (- (fxSinAddYZ + fxSinSubYZ)/2 ) + FixedMul( fxCosYSinX , fxSinZ);
	m_Matrix[9]= ((-(fxCosAddYZ - fxCosSubYZ)/2)) + FixedMul( fxCosYSinX , fxCosZ);
	m_Matrix[10]= (fxCosAddYX + fxCosSubYX)/2;
	m_Matrix[11]= (Tz);

	m_Matrix[12]= 0;
	m_Matrix[13]= 0;
	m_Matrix[14]= 0;
	m_Matrix[15]= IF(1);

}

