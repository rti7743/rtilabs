#ifndef ___QUATERNIONH
#define ___QUATERNIONH

/************************************************************************/
//クウォータニオン
/************************************************************************/
/*
#include <.\Base\rti_glookup.h>
#include <.\rti_define.h>
#include <.\Base\rti_vector.h>

struct Quaternion
{
	Fixed w;
	XYZFixed v;
};


//回転行列を作成.
void QuaternionMatrix(MATRIX3D Ma, Quaternion const *lpQuaternion);
//足し算. qA+qB
void QuaternionAdd(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//引き算. qA-qB
void QuaternionSub(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//掛け算 qA * qB
void QuaternionMul(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//割り算 qA / qB
void QuaternionDiv(Quaternion *lpQuaternion,
		 Quaternion const *lpQuaternionA,Quaternion const *lpQuaternionB);
//共役
void Conjugate(Quaternion *lpQuaternion);
//ノルム
Fixed Norm(Quaternion const *lpQuaternion);
//絶対値.
Fixed Abs(Quaternion const *lpQuaternion);
//正規化
void Normalize(Quaternion *lpQuaternion);
//逆クウォータニオン
void RevQuaternion(Quaternion *lpQuaternion);
//指定した jiku により、 Angle(ディグリー)させたときのクウォータニオンを求める.
void QuaternionFromRotation(Quaternion *lpQuaternion,XYZFixed  const *jiku,Angle256 Angle);
//球面線形補完
void QuaternionSlerp(Quaternion *lpQuaternion,
	 Quaternion const *lpQuaternionStart,Quaternion const *lpQuaternionEnd, Fixed alpha);
//単位クウォータニオン
void QuaternionUnit(Quaternion *lpQuaternion);

//デバック
#define DEBUG_QUATERNION(q) DEBUG3(#q "	%lf	,	%lf	,	%lf	,	%lf",\
						Fixed2Float(q.w),Fixed2Float(q.v.x),Fixed2Float(q.v.y),Fixed2Float(q.v.z) )
*/
#endif
