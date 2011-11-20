// RResorceImageLoad.cpp: RResorceImageLoad クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RResorceImageLoad.h"
#include "WindowsMain.h"
#include <ole2.h>
#include <olectl.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RResorceImageLoad::RResorceImageLoad()
{

}

RResorceImageLoad::~RResorceImageLoad()
{

}

void RResorceImageLoad::Load(RDIBBitmap * outImage, const string & inResourceName) throw(RWin32Exception)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();
#ifndef  _WIN32_WCE
	HANDLE handle = LoadBitmap(winMain->getInstance() , inResourceName.c_str()  );
#else
	MY_USES_CONVERSION;
	HANDLE handle = LoadBitmap(winMain->getInstance() , _A2W(inResourceName.c_str())  );
#endif
	if (handle == NULL)
	{
		throw RWin32Exception("リソース %s のイメージがロードできません" , inResourceName.c_str() );
  	}
	HDC dc = CreateCompatibleDC(NULL);
	if (dc == NULL)
	{
		throw RWin32Exception("リソースを描画するのに必要なDC が取得できません"  );
  	}

	SelectObject(dc , handle);

	outImage->Create( outImage->getWidth() , outImage->getHeight() , 16);

	BitBlt(outImage->getDC() , 0 , 0, outImage->getWidth() , outImage->getHeight() , dc , 0 , 0 , SRCCOPY);

	DeleteObject(handle);
	DeleteDC(dc);
}

void RResorceImageLoad::LoadFile(RDIBBitmap * outImage, const string & inFilename) throw(RException)
{
	BITMAP bmp_info;
	HDC memDC = NULL;
	HDC hdc   = NULL;
	HBITMAP hBit = NULL;
	try
	{
		//デバイスコンテキスト取得
		hdc = GetDC(NULL);
		//メモリDC取得
		memDC = CreateCompatibleDC(hdc);

		//DC 開放
		ReleaseDC(NULL,hdc);

		//ビットマップハンドル制作
#ifndef _WIN32_WCE
		hBit = (HBITMAP) LoadImage(NULL, inFilename.c_str() , IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
#else
		MY_USES_CONVERSION;
		hBit = SHLoadDIBitmap( _A2W(inFilename.c_str() ) );
#endif
		if (!hBit)
		{
			 throw RException(EXCEPTIONTRACE + "LoadImage に失敗:" + inFilename);
		}

		GetObject(hBit, sizeof(BITMAP), &bmp_info);
		SelectObject(memDC,hBit);   //ビットマップ選択

		outImage->Create( bmp_info.bmWidth , bmp_info.bmHeight  , 16);

		//画像転送(手抜き～)
		BitBlt( outImage->getDC() ,0,0,bmp_info.bmWidth , bmp_info.bmHeight , memDC,0,0,SRCCOPY);

		DeleteDC(memDC)  ;       //メモリDC開放
		DeleteObject(hBit) ;    //ビットマップハンドル削除
	}
	catch(RException e)
	{
		if (hdc)	ReleaseDC(NULL,hdc);		   //DC破棄
		if (memDC)  DeleteDC(memDC);       //メモリDC開放
		if (hBit)   DeleteObject(hBit);    //ビットマップハンドル削除
		throw;
	}
}

