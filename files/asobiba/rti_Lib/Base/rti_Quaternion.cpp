
/************************************************************************/
//クウォータニオン
/************************************************************************/
/*
#include <.\Base\rti_Quaternion.h>



//回転行列を作成.
void QuaternionMatrix(MATRIX3D Ma, Quaternion const *lpQuaternion)
{
	
	//lpQuaternion は、正規化されていること.
	Fixed x,y,z,w;
	Fixed xx,yy,zz,xy,xz,wx,wy,wz,yz;
	x = lpQuaternion->v.x;	y = lpQuaternion->v.y;	z = lpQuaternion->v.z;	w = lpQuaternion->w;
	xx = FixedMul(x,x);
	yy = FixedMul(y,y);
	zz = FixedMul(z,z);
	xy = FixedMul(x,y);
	xz = FixedMul(x,z);
	wx = FixedMul(w,x);
	wy = FixedMul(w,y);
	wz = FixedMul(w,z);
	yz = FixedMul(y,z);
	Ma[0] = IF(1) - (yy+zz) * 2;
	Ma[1] = (xy-wz) * 2;
	Ma[2] = (xz+wy) * 2;

	Ma[3] = (xy+wz) * 2;
	Ma[4] = IF(1) - (xx+zz) * 2;
	Ma[5] = (yz-wx) * 2;

	Ma[6] = (xz-wy) * 2;
	Ma[7] = (yz+wx) * 2;
	Ma[8] = IF(1) - (xx+yy) * 2;
	
}

//足し算. A+B
void QuaternionAdd(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{	// q1 + q2 = (a1+a2 , V1+V2)
	//スカラー部.
	lpQuaternion->w = lpQuaternionA->w + lpQuaternionB->w;
	//ベクトル部.
	VectorAdd( &lpQuaternion->v , &lpQuaternionA->v , &lpQuaternionB->v );
}
//引き算. A-B
void QuaternionSub(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{	//q1 - q2 = (a1 - a2 , V1 - V2)
	//スカラー部.
	lpQuaternion->w = lpQuaternionA->w - lpQuaternionB->w;
	//ベクトル部.
	VectorSub( &lpQuaternion->v , &lpQuaternionA->v , &lpQuaternionB->v );
}

//掛け算 A * B
void QuaternionMul(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{	//q1q2 = (a1a2 - V1.V2   , a1V2 + a2V1 + V1*V2)
	//とりあえず、さきにベクトル部を計算.
	XYZFixed Va,Vb,VCross;
	VectorMul(&Va, &lpQuaternionB->v , lpQuaternionA->w);
	VectorMul(&Vb, &lpQuaternionA->v , lpQuaternionB->w);
	CrossProduct ( &VCross , &lpQuaternionA->v , &lpQuaternionB->v);
	VectorAdd(&Va ,&Va , &Vb);			//合成
	VectorAdd(&Va ,&Va , &VCross);		//Va は、ベクトル部.
	//スカラー部の計算.
	lpQuaternion->w = FixedMul(lpQuaternionA->w , lpQuaternionB->w)
							- DotProduct(&lpQuaternionA->v,&lpQuaternionB->v);
	//ベクトル部の代入
	lpQuaternion->v = Va;
}
//割り算 A / B
void QuaternionDiv(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB)
{
	//割り算 q1 / q2 = q1 q2^-1
	Quaternion Q = *lpQuaternionB;	//B は壊してはいけない.
	RevQuaternion(&Q);				//逆クウォータニオンに変換.

	QuaternionMul(lpQuaternion , lpQuaternionA , &Q);
}
//共役
void Conjugate(Quaternion *lpQuaternion)
{	//ベクトル部だけ符号反転 'q = (a , -V)
	lpQuaternion->v.x = -lpQuaternion->v.x;	lpQuaternion->v.y = -lpQuaternion->v.y;	lpQuaternion->v.z = -lpQuaternion->v.z;
}
//ノルム
Fixed Norm(Quaternion const *lpQuaternion)
{	//norm(q) = |q| = (a^2 + V.V)
	return FixedMul(lpQuaternion->w , lpQuaternion->w) 
		+ DotProduct(& (lpQuaternion->v) , &(lpQuaternion->v) );
}
//絶対値.
Fixed Abs(Quaternion const *lpQuaternion)
{	// ||q|| = abs(q) = sqrt(a^2+V.V)
//	return sqrt( Norm(lpQuaternion) );
	return Float2Fixed( sqrt( Fixed2Float(Norm(lpQuaternion)) ) );
}
//正規化
void Normalize(Quaternion *lpQuaternion)
{	//normalize(q) = q / ||q||
	Fixed abs = Abs(lpQuaternion);
	lpQuaternion->w = FixedDiv(lpQuaternion->w , abs);
	VectorDiv( &lpQuaternion->v , &lpQuaternion->v , abs);
}	
//逆クウォータニオン
void RevQuaternion(Quaternion *lpQuaternion)
{	// q^-1 = q' / |q| = ( a/(a^2+V.V) , -V/(a^2+V.V) )
	Fixed norm = Norm(lpQuaternion);//ノルム作成.
	Conjugate(lpQuaternion);		//共役に変換.

	FixedDiv(lpQuaternion->w , norm);
	VectorDiv( &lpQuaternion->v , &lpQuaternion->v , norm);
}

//指定した jiku により、 Angle(ディグリー)させたときのクウォータニオンを求める.
void QuaternionFromRotation(Quaternion *lpQuaternion,XYZFixed  const *jiku,Angle256 Angle)
{
	Angle /= 2;					//一回の回転で 2Θ回転します.	だから、その半分を取ります.
	Fixed SinHarf = FixedLTBSin256[Angle];
	//q(Θ) = ( cos(Θ/2) , Vsin(Θ/2) )
	lpQuaternion->w = FixedLTBCos256[Angle];
	VectorMul( &lpQuaternion->v , jiku , SinHarf);

}

//球面線形補完
void QuaternionSlerp(Quaternion *lpQuaternion,
	 Quaternion const *lpQuaternionStart,Quaternion const *lpQuaternionEnd, Fixed alpha)
{

}

//単位クウォータニオン
void QuaternionUnit(Quaternion *lpQuaternion)
{
	lpQuaternion->w = IF(1);
	lpQuaternion->v.x = 0;
	lpQuaternion->v.y = 0;
	lpQuaternion->v.z = 0;
}


*/
