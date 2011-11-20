#include <.\rti_define.h>
#include <.\Base\rti_glookup.h>

#ifndef ___VECTOR_H
#define ___VECTOR_H


//ベクトルの正規化
inline void Normalize(XYZFixed *V)
{
   register unsigned long len = farst_hypot3D( (V->x),(V->y),(V->z) );
   V->x = FixedDiv(V->x , len);
   V->y = FixedDiv(V->y , len);
   V->z = FixedDiv(V->z , len);
};
//外積を求める
inline XYZFixed CrossProduct(const XYZFixed* A,const XYZFixed* B)
{
        XYZFixed ret;
	    ret.x = FixedMul(A->y,B->z) - FixedMul(A->z,B->y);
    	ret.y = FixedMul(A->z,B->x) - FixedMul(A->x,B->z);			// aベクトルとbベクトルの外積
	    ret.z = FixedMul(A->x,B->y) - FixedMul(A->y,B->x);			// により法線ベクトルを求める
        return ret;
}

//外積を求める その2
inline void CrossProduct(XYZFixed *R,const XYZFixed* A,const XYZFixed* B)
{
	    R->x = FixedMul(A->y,B->z) - FixedMul(A->z,B->y);
    	R->y = FixedMul(A->z,B->x) - FixedMul(A->x,B->z);			// aベクトルとbベクトルの外積
	    R->z = FixedMul(A->x,B->y) - FixedMul(A->y,B->x);			// により法線ベクトルを求める
}


//内積を求める(A,Bは正規化されていること.)
inline Fixed DotProduct(const XYZFixed* A,const XYZFixed* B)
{
        return        FixedMul(A->x , B->x) +
					  FixedMul(A->y , B->y) +
					  FixedMul(A->z , B->z) ;
}

inline void RevVector(XYZFixed *v)
{
	v->x = -v->x ;	v->y = -v->y ;	v->y = -v->y ;
}

//足し算 Ans = vA+vB
inline void VectorAdd(XYZFixed *Ans , XYZFixed const *A,XYZFixed const *B)
{
	Ans->x = A->x + B->x;	Ans->y = A->y + B->y;	Ans->z = A->z + B->z;
}
//引き算 Ans = vA-vB
inline void VectorSub(XYZFixed *Ans , XYZFixed const *A,XYZFixed const *B)
{
	Ans->x = A->x - B->x;	Ans->y = A->y - B->y;	Ans->z = A->z - B->z;
}

//掛け算 Ans = vA*B
inline void VectorMul(XYZFixed *Ans , XYZFixed const *A,Fixed B)
{
	Ans->x = FixedMul(A->x , B);	Ans->y = FixedMul(A->y , B);	Ans->z = FixedMul(A->z , B);
}
//割り算 Ans = vA/B
inline void VectorDiv(XYZFixed *Ans , XYZFixed const *A,Fixed B)
{
	Ans->x = FixedDiv(A->x , B);	Ans->y = FixedDiv(A->y , B);	Ans->z = FixedDiv(A->z , B);
}

//掛け算 Ans = vA<<B
inline void VectorSal(XYZFixed *Ans , XYZFixed const *A,int B)
{
	Ans->x = A->x << B ;	Ans->y = A->y << B;		Ans->y = A->y << B;
}

//割り算 Ans = vA>>B
inline void VectorSar(XYZFixed *Ans , XYZFixed const *A,int B)
{
	Ans->x = A->x >> B ;	Ans->y = A->y >> B;		Ans->y = A->y >> B;
}

//比較
inline bool VectorCmp(XYZFixed const *A , XYZFixed const *B)
{
	return (A->x == B->x && A->y == B->y && A->z == B->z);
}

//猶予付き比較
inline bool VectorCmp(XYZFixed const *A , XYZFixed const *B , Fixed J)
{
	return (
		( (A->x-J) < B->x  &&  (A->x+J) > B->x ) &&
		( (A->y-J) < B->y  &&  (A->y+J) > B->y ) &&
		( (A->z-J) < B->z  &&  (A->z+J) > B->z )   );
}

//代入
inline void VectorSet(XYZFixed *A , XYZFixed const *B)
{
	A->x = B->x;
	A->y = B->y;
	A->z = B->z;
}

//デバック
#define DEBUG_VECTOR(v) DEBUG3(#v"	%lf	,	%lf	,	%lf",\
						Fixed2Float(v.x),Fixed2Float(v.y),Fixed2Float(v.z) )

#endif
