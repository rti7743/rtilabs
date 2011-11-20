#ifndef	___MAXTRIX___H	//重複防止
#define ___MAXTRIX___H

#include <./rti_define.h>
#include <./Base/rti_vector.h>

class TMatrix
{
	public:
		TMatrix();					//コンストラクタ
		TMatrix(TMatrix &Ma);				//コピーコンストラクタ

		void Unit();					//単位行列の作成.
		void FastRev(TMatrix* inMat);	//高速逆行列
		void VectorMul(XYZFixed *Dest,  //ベクトルとのかけ算.
                XYZFixed const *Src);	
		void Synthesis(TMatrix *Ma);		//合成.
		void MakeTrans(Angle AngleX,		//回転行列作成
			Angle AngleY,Angle AngleZ , Fixed Tx, Fixed Ty,Fixed Tz);
		void MakeTransYXZ(Angle AngleX,		//回転行列作成 (視点など)
			Angle AngleY,Angle AngleZ , Fixed Tx, Fixed Ty,Fixed Tz);
	protected:
		friend class TMatrix;
		Fixed	m_Matrix[4+4+4+4];
};


#endif			//重複防止
