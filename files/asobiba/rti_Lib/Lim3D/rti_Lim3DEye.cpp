#include <./Lim3D/rti_Lim3DMaster.h>

//この視点を使ってレンダリングする.
void TLim3DEye::Select()
{
	//視点セレクト.
	TLim3DMaster::SetSelectEye(this);
	//回転行列を作る.
	MakeMatrix();
}

//回転行列の作成.
void TLim3DEye::MakeMatrix()
{
	//視点行列を作成.
	m_Matrix.MakeTransYXZ( __ANGLE_REV( m_Point.GetAngleX() ),
						   __ANGLE_REV( m_Point.GetAngleY() ),
						   __ANGLE_REV( m_Point.GetAngleZ() ),
						m_Point.GetPosX() , m_Point.GetPosY() , m_Point.GetPosZ() );
	//逆行列を作成します.
	m_RevMatrix.FastRev( &m_Matrix );
}

//視点変換.
void TLim3DEye::EyeConbert(XYZFixed * Eye , XYZFixed * World )
{
        //視点は、 Y X Z の順で回転させなくてはならない.
        //物体は、 Z X Y の順で回転させるんですけどね.
        //ムズイですね.
        
        //回転を計算
		m_Matrix.VectorMul(Eye,World);
}

//逆視点変換.
void TLim3DEye::RevEyeConbert(XYZFixed * Eye , XYZFixed * World )
{
        //回転を計算
		m_RevMatrix.VectorMul(Eye,World);
}

//視点の回転角度を指定.
void			TLim3DEye::SetRotate(Angle x,Angle y,Angle z)
{
	m_Point.SetAngleX(x);
	m_Point.SetAngleY(y);
	m_Point.SetAngleZ(z);
}
//視点の場所を指定.
void			TLim3DEye::SetPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(x);
	m_Point.SetPosY(y);
	m_Point.SetPosZ(z);
}
//視点の回転角度を取得.
void			TLim3DEye::GetRotate(Angle *x,Angle *y,Angle *z)
{
	*x = m_Point.GetAngleX();
	*y = m_Point.GetAngleY();
	*z = m_Point.GetAngleZ();
}
//視点の場所を取得.
void			TLim3DEye::GetPos(Fixed *x,Fixed *y,Fixed *z)
{
	*x = m_Point.GetPosX();
	*y = m_Point.GetPosY();
	*z = m_Point.GetPosZ();
}
//視点の回転角度を増やす.
void			TLim3DEye::AddRotate(int x,int y,int z)
{
	m_Point.SetAngleX(m_Point.GetAngleX() + x);
	m_Point.SetAngleY(m_Point.GetAngleY() + y);
	m_Point.SetAngleZ(m_Point.GetAngleZ() + z);
}
//視点の場所を移動する.
void			TLim3DEye::AddPos(Fixed x,Fixed y,Fixed z)
{
	m_Point.SetPosX(m_Point.GetPosX() + x);
	m_Point.SetPosY(m_Point.GetPosY() + y);
	m_Point.SetPosZ(m_Point.GetPosZ() + z);
}

//場所情報クラスへのベタアクセス
TLim3DPoint* TLim3DEye::GetPoint()
{
	return &m_Point;
}

//行列クラスへのベタアクセス
TMatrix* TLim3DEye::GetMatrix()
{
	return &m_Matrix;
}


//逆行列クラスへのベタアクセス
TMatrix* TLim3DEye::GetRevMatrix()
{
	return &m_RevMatrix;
}

