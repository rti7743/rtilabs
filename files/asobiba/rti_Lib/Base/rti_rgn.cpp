/*****************************************************************/
/*****************************************************************/
//リュージョン
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef RGN_LIFE

#include <windows.h>
#include <.\Base\rti_rgn.h>
#include <.\Base\rti_debug.h>
#include <.\VCL\rti_vcl_apprication.h>

TRgn::TRgn(HWND hwnd,char *filename,char flg)
{

    //リージョンの元になる絵を読む
    LoadBmp(filename,hwnd);  

    RECT rect;
    int x,y,w,h;
    RegionData Data;

	if(flg==ALIGN_BMPSIZE)
	{ //ウィンドサイズを絵のサイズにする
	    BITMAP bmp_info;
	    GetObject(RgnhBit, sizeof(BITMAP), &bmp_info);
		MoveWindow(hwnd,0,0,bmp_info.bmWidth,bmp_info.bmHeight,TRUE);
	}

    GetWindowRect(hwnd,&rect);
    w = rect.right - rect.left;
    h = rect.bottom - rect.top ;

    OffsetRect(&rect, -rect.left , -rect.top);
    hRGNMaster= CreateRectRgnIndirect(&rect);

    int sx,ex;
    bool black_next = false;
    Data.Count = 0;
    Data.Size = 32; Data.iType = RDH_RECTANGLES;
    RECT *r;
    r = &(Data.Bounds);
    r->left   = 0;
    r->top    = 0;
    r->right  = w;
    r->bottom = h;
    for (y = 0; y <= h ; y++)
    {
        for (x = 0; x <= w ; x++)
        {
            if (GetPixel(RgnmemDC,x,y))
            {
                if (black_next)
                {//ここで黒はおしまい
                    ex = x;
                    black_next = false;
                    //型抜き
                    RgnCut(sx,ex,y,&Data);
                }
            }
            else
            {//黒だったら
                if (! black_next)
                {//ここから黒がスタート
                    sx = x;
                    black_next = true;
                }
            }
        }
        //まだ、 X の長方形ゲットが進行中だったら
        if (black_next) 
		{
			RgnCut(sx,x,y,&Data);
            black_next = false;
		}
    }
    if (Data.Count > 0) 
    {
        CreateRegion(&Data);
    }
    SetWindowRgn(hwnd,hRGNMaster,TRUE);
}

TRgn::~TRgn()
{
       	DeleteDC(RgnmemDC);       //メモリDC開放
	    DeleteObject(RgnhBit);    //ビットマップハンドル削除
        DeleteObject(hRGNMaster);
}

void TRgn::LoadBmp(char *filename,HWND hwnd)
{
	HDC hdc;
	hdc = GetDC(hwnd);                     //デバイスコンテキスト取得

	RgnmemDC = CreateCompatibleDC(hdc); //メモリDC取得
	RgnhBit = LoadImage(Application->GethInstance(),filename,IMAGE_BITMAP,
			0,0,LR_LOADFROMFILE);         //ビットマップハンドル制作

	SelectObject(RgnmemDC,RgnhBit);   //ビットマップ選択

	ReleaseDC(hwnd,hdc);					//DC 開放
}


void TRgn::RgnCut(int sx,int ex,int y,RegionData *Data)
{
    if (Data->Count == 4000) {
            CreateRegion(Data);
            Data->Count = 0;
    }
    RECT *r;
    r = &(Data->Rects[Data->Count]);
    r->left   = sx;
    r->top    = y;
    r->right  = ex;
    r->bottom = y+1;
    Data->Count++;
}


void TRgn::CreateRegion(RegionData *Data)
{
    XFORM Form;

    Form.eM11 = 1; Form.eM12 = 0; Form.eM21 = 0; Form.eM22 = 1;
    Form.eDx = 0; Form.eDy = 0;
    Data->RgnSize = Data->Count * 16;
    //穴
    HRGN hCutRgn = ExtCreateRegion(&Form, 32 + Data->Count * 16, (RGNDATA *)Data);
    //穴あけ
    CombineRgn(hRGNMaster,hRGNMaster,hCutRgn,RGN_XOR);
    //あける穴のデータは不要なので破棄
    DeleteObject(hCutRgn);
}

#endif //LIFE END
