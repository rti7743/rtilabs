#include "StartWithExeFile.h"
#ifdef NOMAL_BITMAL_LIFE

#ifndef ___NOMAL_BITMALH
#define ___NOMAL_BITMALH

typedef struct PictureSp* NBMD;
struct PictureSp;
struct PictureSp
{
    HDC memDC;
    HBITMAP hBit;
    int w,h;
};

class TNomalBitmap
{
public:
	void PictureLoad(char *filename,HWND hwnd,NBMD Pic);
	void PictureDestroy(NBMD Pic);
	void PictureCreate(int x,int y,HWND hwnd,NBMD Pic);
};

#endif

#endif //END LIFE
