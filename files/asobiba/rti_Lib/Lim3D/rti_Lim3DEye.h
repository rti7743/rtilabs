#ifndef	___LIM3D_EYE__H	//重複防止
#define ___LIM3D_EYE__H

#include <./Lim3D/rti_Lim3DPoint.h>
#include <./Lim3D/rti_Matrix.h>
#include <./Lim3D/rti_SuperListClient.h>

class TLim3DEye : public __TSuperListClient
{
public:
	//この視点を使ってレンダリングする.
	void			Select();
	//回転行列の作成.
	void			MakeMatrix();
	//視点変換.
	void			EyeConbert(XYZFixed * Eye , XYZFixed * World );
	//逆視点変換.
	void			RevEyeConbert(XYZFixed * Eye , XYZFixed * World );
	//視点の回転角度を指定.
	void			SetRotate(Angle x,Angle y,Angle z);
	//視点の場所を指定.
	void			SetPos(Fixed x,Fixed y,Fixed z);
	//視点の回転角度を取得.
	void			GetRotate(Angle *x,Angle *y,Angle *z);
	//視点の場所を取得.
	void			GetPos(Fixed *x,Fixed *y,Fixed *z);
	//視点の回転角度を増やす.
	void			AddRotate(int x,int y,int z);
	//視点の場所を移動する.
	void			AddPos(Fixed x,Fixed y,Fixed z);
	//場所情報クラスへのベタアクセス
	TLim3DPoint*	GetPoint();
	//行列クラスへのベタアクセス
	TMatrix*		GetMatrix();
	//逆行列クラスへのベタアクセス
	TMatrix*		GetRevMatrix();
protected:
	TLim3DPoint				m_Point;		//位置
	TMatrix					m_Matrix;		//視点回転行列.
	TMatrix					m_RevMatrix;	//視点回転逆行列.
private:
};

//モデル定義
typedef TLim3DEye* EYE;


#endif				//重複防止