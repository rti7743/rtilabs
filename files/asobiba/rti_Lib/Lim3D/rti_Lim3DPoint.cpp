#include <./Lim3D/rti_Lim3DPoint.h>


//現在むいている方向(ZX)で、 inN 分進む
void TLim3DPoint::MoveZX(Fixed inN)
{
        Pos.x +=
            ( (Fixed) (FixedMul(FixedLTBSin1024[ __ANGLE_REV(AngleY) ] ,inN)  ) );
        Pos.z +=
            ( (Fixed) (FixedMul(FixedLTBCos1024[ __ANGLE_REV(AngleY) ] ,inN)  ) );
}


//inPoint のコピーになる.
void TLim3DPoint::CopyForm(TLim3DPoint * inPoint)
{
	Pos.x = inPoint->GetPosX();	Pos.y = inPoint->GetPosY();	Pos.z = inPoint->GetPosZ();
	AngleX = inPoint->GetAngleX();	AngleX = inPoint->GetAngleY();	AngleX = inPoint->GetAngleZ();
}

