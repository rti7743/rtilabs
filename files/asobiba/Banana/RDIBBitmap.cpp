// RDIBBitmap.cpp: RDIBBitmap クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RDIBBitmap.h"
#include "RStdioFile.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RDIBBitmap::RDIBBitmap()
{
	ImageMemory = NULL;
	MemoryDC = NULL;
	HBitmap = NULL;

}

RDIBBitmap::~RDIBBitmap()
{
	Release();
}

//内容の保存(主にデバッグ用
void RDIBBitmap::Save(const string & inFilename) const throw(RException)
{
	int imageSize = (this->Width*this->Height*this->Bit/8);
	int size = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+imageSize;

	BITMAPFILEHEADER head = {0};
	head.bfType='M'*256+'B';
	head.bfSize=size;
	head.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER info = {0};
	info.biSize=sizeof(BITMAPINFOHEADER);
	info.biWidth=this->Width;
	info.biHeight=-this->Height;
	info.biPlanes=1;
	info.biBitCount=this->Bit;

	RStdioFile file;
	file.Open(inFilename , "w+");
	file.fwrite( &head , sizeof(head));
	file.fwrite( &info , sizeof(info));
	file.fwrite( ImageMemory , imageSize);
}

void RDIBBitmap::Create(int inW , int inH , int inBit) throw(RWin32Exception)
{
//	ASSERT(inBit == 1 || inBit == 2 || inBit == 4 || inBit == 8 || inBit == 16 || inBit == 24 || inBit == 32);
	ASSERT(inBit == 8 || inBit == 16 || inBit == 24 || inBit == 32);
	ASSERT(MemoryDC == NULL);
	ASSERT(HBitmap == NULL);

	BITMAPINFO bm_info = {0};
	memset(&bm_info,0,sizeof(BITMAPINFO));
	HDC hdc = GetDC(NULL);
	bm_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bm_info.bmiHeader.biWidth = inW;
 	bm_info.bmiHeader.biHeight = -(inH+1);	// + 1 しないとヤバです (^^;;
	bm_info.bmiHeader.biPlanes = 1;
	bm_info.bmiHeader.biBitCount = inBit;
	bm_info.bmiHeader.biCompression = BI_RGB;


	this->HBitmap = CreateDIBSection( hdc, &bm_info , DIB_RGB_COLORS, (void **)&this->ImageMemory, NULL, 0 );
	if( ! this->HBitmap )
	{
		Release();
		throw RWin32Exception(EXCEPTIONTRACE + "CreateDIBSectionで失敗");
	}
	this->MemoryDC = CreateCompatibleDC( hdc );
	if( ! this->MemoryDC )
	{
		Release();
		throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleDCで失敗");
	}
    HGDIOBJ hobj = SelectObject(this->MemoryDC, this->HBitmap);
    if(NULL == hobj) 
	{
		Release();
		throw RWin32Exception(EXCEPTIONTRACE + "SelectObjectで失敗");
    }
	//ふつーの DC の削除.
	ReleaseDC(NULL,hdc);
	
	this->Width = inW;
	this->Height = inH;
	this->Bit = inBit;

	return ;
}

void RDIBBitmap::Release()
{
	if (MemoryDC) DeleteObject(MemoryDC);
	if (HBitmap) DeleteObject(HBitmap);

	ImageMemory = NULL;
	MemoryDC = NULL;
	HBitmap = NULL;
}
