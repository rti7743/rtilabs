/**********************************************************/
// ピクチャーボックス  
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef PICTUREBOX_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_picturebox.h>

TPictureBox::TPictureBox(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
    :TGroupBox(hOwner,Caption,dwStyle,x,y,w,h,UserWndp)
{
	memDC = NULL;
	hBit = NULL;
}

TPictureBox::~TPictureBox()
{
	DestroyPicture();
} 

/**********************************************************/
/* 絵を指定 */
/**********************************************************/
void TPictureBox::SetPicture(char *FileName)
{
	//念のため開放
	DestroyPicture();

	HDC hdc;
    BITMAP bmp_info;
	hdc = GetDC(hWnd);                     //デバイスコンテキスト取得

	memDC = CreateCompatibleDC(hdc); //メモリDC取得
	hBit = LoadImage(Application->hInstance,FileName,IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);         //ビットマップハンドル制作
    GetObject(hBit, sizeof(BITMAP), &bmp_info);
    w = bmp_info.bmWidth;
    h = bmp_info.bmHeight;

	SelectObject(memDC,hBit);   //ビットマップ選択

	ReleaseDC(hWnd,hdc);					//DC 開放
}

/**********************************************************/
/* 絵を指定 */
/**********************************************************/
void TPictureBox::SetPicture(BMD bmd)
{
	//念のため開放
	DestroyPicture();

	HDC hdc;
	hdc = GetDC(hWnd);                     //デバイスコンテキスト取得

	memDC = CreateCompatibleDC(hdc); //メモリDC取得
	hBit  = CreateBitmap(bmd->bm_header.biWidth,
						 bmd->bm_header.biHeight,
						 bmd->bm_header.biPlanes,
						 bmd->bm_header.biBitCount,
						 bmd->bm); 
	w = bmd->bm_header.biWidth;
	h = bmd->bm_header.biHeight;

	SelectObject(memDC,hBit);   //ビットマップ選択

	ReleaseDC(hWnd,hdc);					//DC 開放
}

/**********************************************************/
/* メモリ開放 */
/**********************************************************/
void TPictureBox::DestroyPicture()
{
    if (memDC) 
	{
		DeleteDC(memDC);       //メモリDC開放
		memDC = NULL;
	}
    if (hBit) 
	{
	    DeleteObject(hBit);    //ビットマップハンドル削除
		hBit = NULL;
	}
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TPictureBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;

    switch(message)
    {
	case WM_PAINT:
        if (memDC)
        {
            BeginPaint(hwnd,&ps);            //PAINT専用のデバコン取得
		    BitBlt(ps.hdc ,0,0,	w,h,  memDC,0,0,SRCCOPY);
	                                    //メモリDCからディスプレイDCへ転送
		    EndPaint(hwnd,&ps);              //WM_PAINT専用のデバコン開放
        }
		break;
    }
    return TGroupBox::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif //LIFE END
