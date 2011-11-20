#include <StartWithExeFile.h>
#ifdef RGN_LIFE

#ifndef ___RGNH
#define ___RGNH
#include <windows.h>

class TRgn
{
private:
    HDC RgnmemDC;            //メモリデバイスコンテキスト
    HBITMAP RgnhBit;         //ビットマップだよん
    HRGN hRGNMaster;      //リージョンハンドル
    struct RegionData {
        DWORD Size, iType, Count, RgnSize;
        RECT Bounds;
        RECT Rects[4000];
    };
    void LoadBmp(char *filename,HWND hwnd);
    void RgnCut(int sx,int ex,int y,RegionData *Data);
    void CreateRegion(RegionData *Data);
public:
    TRgn(HWND hwnd,char *filename,char flg);
    ~TRgn();

};

#define ALIGN_NONE 0
#define ALIGN_BMPSIZE 1

#endif

#endif //LIFE END
