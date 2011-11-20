#ifndef __RTI_DEFINE_H
#define __RTI_DEFINE_H

#include <.\Base\rti_glookup.h>

typedef struct BMP_DATA_TABLE* BMD;
struct BMP_DATA_TABLE{
	int w,h;
	BYTE *bm ;
	int  Pow2W;				// W ‚ª 2 ‚Ì‚×‚«”‚¾‚Á‚½‚çA‚±‚±‚É ‚»‚Ì”‚ª¦‚³‚ê‚é.
    void* Tag;
};

//3ŸŒ³
struct XYZ
{
    short x,y,z;
};

//3ŸŒ³ŒÅ’è¬”
struct XYZFixed
{
    Fixed x,y,z;
};

//3ŸŒ³‰ñ“]Šp“x
struct XYZChar
{
    unsigned char x,y,z;
};

//2ŸŒ³
struct XY
{
	short x,y;
};
//2ŸŒ³ŒÅ’è
struct XYFixed
{
	Fixed x,y;
};

//RGB
struct _RGB
{
	unsigned char b,g,r;
};

typedef unsigned short Angle;			//‰ñ“]Šp“x(1024“x).
typedef unsigned short Angle1024;		//‰ñ“]Šp“x(1024“x).
#define __ANGLE_REV(x) (1023-(__ANGLE(x)))	//‹t‚É‚·‚é.
#define __ANGLE(x) ((x)&0x03ff)				//Šp“x•â³.

#ifdef _DEBUG
//ASSERT ‚ÌƒpƒNƒŠ
	#define __RTI_CHECKER(f) \
		do \
		{ \
			if (!(f) ) \
			{ \
				MSG msg;	\
				BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);	\
				if (bQuit)	PostQuitMessage(msg.wParam);	\
				__asm { int 3 }; \
			} \
		} while (0) 
#else
	#define __RTI_CHECKER(f) /*     */
#endif

#endif
