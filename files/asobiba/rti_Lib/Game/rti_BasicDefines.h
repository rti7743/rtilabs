/*
#include <.\Base\rti_glookup.h>

//•ûŒü‚ð‚ ‚ç‚í‚·.
enum Houkou
{
	TOP,	RIGHT,	BUTTON,	LEFT
};

class TBasicDefines;

class TBasicDefines
{
public:
	TBasicDefines(){X = 0; Y = 0; Sx = 0 ; Sy = 0; Angle = 0; Speed = 0;};

	Fixed X,Y;	//X Y ƒf[ƒ^
	Fixed Sx,Sy;//‘¬“x
	unsigned char Angle;	//‰ñ“]Šp
	Fixed Speed;			//‘¬“x

	int GetX(){return FI(X);};
	int GetY(){return FI(Y);};
	int GetSpeed(){return FI(Speed);};
	unsigned char GetAngle(){return Angle;};
	void SetX(int x){X = IF(x);};
	void SetY(int y){Y = IF(y);};
	void SetSpeed(int speed){Speed = IF(speed);};
	void SetAngle(unsigned char angle){Angle = angle;};

	//“G‹@‚ð’Ç‚Á‚©‚¯‚é.
	void PathFinder(TBasicDefines *TBO)
	{
		unsigned char Seeta;
	    Fixed x =  TBO->X - X;
		Fixed y =  TBO->Y - Y;
		Seeta = (unsigned char) DDD (atan2( FI(y) , FI(x) ) );
		Angle = Seeta;
	    Sx = FixedMul(FixedLTBCos256[ Seeta ] , Speed) ;
	    Sy = FixedMul(FixedLTBSin256[ Seeta ] , Speed) ;
	};
	//ƒz[ƒ~ƒ“ƒO
	void Homing(TBasicDefines *TBO)
	{
		//Ž©•ª‚Æ–Ú•W‚Æ‚ÌÀ•W‚Ì·‚©‚ç–Ú•WŠp‚ð‹‚ß‚é
	    Fixed kdx =  TBO->X - X;
		Fixed kdy =  TBO->Y - Y;
		double rad = atan2( FI(kdy) , FI(kdx) );

		//·Šp‚ð‹‚ß‚é(–Ú•WŠp - isŠp)
		double subdir = rad - RRR(Angle);	//360À•WŒn

		//‰“‰ñ‚è‚µ‚È‚¢‚½‚ß‚Ìˆ—(180 ˆÈ“à‚Å‚È‚¯‚ê‚ÎA‹tŠp)
		if (subdir > RRR(180) )		subdir -= RRR(360);
		if (subdir < RRR(-180) )	subdir += RRR(360);

		//–Ú•W•ûŒü‚ÖA­‚µŒü‚©‚¹‚é.(isŠp += ·Šp * Š„‡)
		double dir = subdir * 0.05;	//·‚É‘Î‚µ‚Ä‚ÌŠ„‡.
		Angle += (unsigned char)DDD256(dir);

		//isŠp‚©‚çˆÚ“®—Ê‚ð‹‚ß‘«‚µž‚Þ
		Sx = FixedMul(FixedLTBCos256[Angle] , Speed);
		Sy = FixedMul(FixedLTBSin256[Angle] , Speed);
	};
	void MoveRef()
	{
		X += Sx;
		Y += Sy;
	};
};

*/
