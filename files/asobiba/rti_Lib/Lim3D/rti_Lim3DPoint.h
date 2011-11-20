#ifndef	___LIM3D_POINT__H	//èdï°ñhé~
#define ___LIM3D_POINT__H

#include <./rti_define.h>
#include <./Base/rti_debug.h>

class TLim3DPoint
{
public:
	TLim3DPoint()
	{
		AngleX = AngleY = AngleZ = 0;
		Pos.x = Pos.y = Pos.z = 0;
	}

	//îjä¸
	bool Release()
	{
		delete this;
		return true;
	}
	Fixed GetPosX(){return Pos.x;}
	Fixed GetPosY(){return Pos.y;}
	Fixed GetPosZ(){return Pos.z;}
	void SetPosX(Fixed X){Pos.x = X;}
	void SetPosY(Fixed Y){Pos.y = Y;}
	void SetPosZ(Fixed Z){Pos.z = Z;}
	Angle1024 GetAngleX(){return __ANGLE(AngleX);}
	Angle1024 GetAngleY(){return __ANGLE(AngleY);}
	Angle1024 GetAngleZ(){return __ANGLE(AngleZ);}
	void SetAngleX(Angle1024 X){AngleX = X;}
	void SetAngleY(Angle1024 Y){AngleY = Y;}
	void SetAngleZ(Angle1024 Z){AngleZ = Z;}

	//inPoint ÇÃÉRÉsÅ[Ç…Ç»ÇÈ.
	void CopyForm(TLim3DPoint * inPoint);

	XYZFixed *GetPos(){return &Pos;}

	//åªç›ÇﬁÇ¢ÇƒÇ¢ÇÈï˚å¸(ZX)Ç≈ÅA inN ï™êiÇﬁ
	void MoveZX(Fixed  inN);

	void DEBUG()
	{
        DEBUG3("\
        MyEye->SetPos( %d , %d , %d);\n\
        MyEye->SetRotate( %d , %d , %d);\n\
        ",	Pos.x , Pos.y , Pos.z , AngleX ,  AngleY , AngleZ         );
	}

protected:
	Angle1024	AngleX,AngleY,AngleZ;
	XYZFixed	Pos;
private:
};

#endif				//èdï°ñhé~