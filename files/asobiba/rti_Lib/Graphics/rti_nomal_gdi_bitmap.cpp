#include "StartWithExeFile.h"
#ifdef NOMAL_BITMAL_LIFE
#include <.\Graphics\rti_nomal_gdi_bitmap.h>

/********************************************************/
//ビットマップロード
/********************************************************/
void PictureLoad(char *filename,HWND hwnd,NBMD Pic)
{
	HDC hdc;
    BITMAP bmp_info;
	hdc = GetDC(hwnd);                     //デバイスコンテキスト取得

	Pic->memDC = CreateCompatibleDC(hdc); //メモリDC取得
	ReleaseDC(hwnd,hdc);                   //DC 開放

    if(!Pic->memDC) return ;

	Pic->hBit = LoadImage(Application->hInstance,filename,IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);         //ビットマップハンドル制作
    if(!Pic->hBit) 
    {
      	DeleteDC(Pic->memDC);       //メモリDC開放
        return ;
    }
    GetObject(Pic->hBit, sizeof(BITMAP), &bmp_info);
    Pic->w = bmp_info.bmWidth;
    Pic->h = bmp_info.bmHeight;

	SelectObject(Pic->memDC,Pic->hBit);   //ビットマップ選択
}

/********************************************************/
//ビットマップ破棄
/********************************************************/
void PictureDestroy(NBMD Pic)
{
  	DeleteDC(Pic->memDC);       //メモリDC開放
    DeleteObject(Pic->hBit);    //ビットマップハンドル削除
}

/********************************************************/
//空の領域作成
/********************************************************/
void PictureCreate(int x,int y,HWND hwnd,NBMD Pic)
{
	HDC hdc;
    hdc = GetDC(hwnd);
	Pic->memDC = CreateCompatibleDC(hdc); //メモリDC取得
	ReleaseDC(hwnd,hdc);					//DC 開放

	Pic->hBit = CreateCompatibleBitmap(hdc,x,y);//ビットマップ領域作成
	Pic->w = x;
	Pic->h = y;

	SelectObject(Pic->memDC,Pic->hBit); //ビットマップ選択
}

#endif //END LIFE
