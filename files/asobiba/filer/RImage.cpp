// RImage.cpp: RImage クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RImage.h"
#include "SusiePluginManager.h"
#include "NativeSupportPicture.h"
#include "Rfiles.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RImage::RImage()
{
	this->Bitmap = NULL;
	this->MemoryDC = NULL;

	ContentsWidth = 0;
	ContentsHeight = 0;

}

RImage::~RImage()
{
	this->Release();
}


//読み込み
bool RImage::Load(const string inFilename)
{
	const string ext = Rfiles::Filename2Ext( inFilename );

	//OSが対応しているなら、OSでロード.
	//文句はゲイツまで.
	try
	{
		NativeSupportPicture nativeSupportImage;
		nativeSupportImage.Load( inFilename );

		LONG width,height;
		nativeSupportImage.getImageSize(&width,&height);

		this->AttachCanvas("",width,height);
		nativeSupportImage.getImage( this->AttachCanvas("",width,height) , width,height);

		return true;
	}
	catch(RNotSupportException e)
	{
		TRACE("ネイティヴではサポートされてません。。。");

	}

	//ぢゃ、SUSIE にでもやらせるか。
	SusiePluginLoadedPicture	LoadedPicture;
	SusieCallable callBack;
	try
	{
		SusiePluginManager* manager = SusiePluginManager::getInstance(  );
		const SusiePluginWapper* plugin = manager->getSupportPlugin(inFilename , ext);
		plugin->GetPictureInfo( inFilename , &LoadedPicture);
		plugin->GetPicture( inFilename , &LoadedPicture , &callBack);

		this->AttachHBitmap("",  LoadedPicture.getBitmapInfo() , LoadedPicture.getBitmap() );

		return true;
	}
	//サポートされていないそーです。
	catch(RNotSupportException e)
	{
		printf("%s\n",e.getMessage() );
	}
	//SUSIEがエラーはきました。
	catch(RSusieConferException e)
	{
		printf("%s\n",e.getMessage() );
	}
	return false;
}



//ビットマップを定義する.
//エラー RDoNotInitaliseException 
void RImage::AttachHBitmap(const string inAlt ,const BITMAPINFO* inBitmapInfo,const void* inBitmapData) throw()
{
	this->Release();
	this->Alt = inAlt;

	int imageSize;
	if (inBitmapInfo->bmiHeader.biSizeImage)	imageSize = inBitmapInfo->bmiHeader.biSizeImage;
	else										imageSize = inBitmapInfo->bmiHeader.biWidth * inBitmapInfo->bmiHeader.biHeight * (inBitmapInfo->bmiHeader.biBitCount/8);

	this->ContentsWidth =  inBitmapInfo->bmiHeader.biWidth;
	this->ContentsHeight = inBitmapInfo->bmiHeader.biHeight;
	
	HDC hdc = GetDC(NULL);
	if (! hdc)
	{
		throw RDoNotInitaliseException("GetDCに失敗しました");
	}
	void* bitmapData;
	this->Bitmap = CreateDIBSection( hdc, inBitmapInfo , DIB_RGB_COLORS, (void **)&bitmapData, NULL, 0 );
	memcpy(bitmapData , inBitmapData ,	imageSize);
	this->MemoryDC = CreateCompatibleDC( hdc );
	if( ! this->MemoryDC )
	{
		DeleteObject( this->Bitmap  );
		this->Bitmap = NULL;
		this->MemoryDC = NULL;

		throw RDoNotInitaliseException("メモリDCが作れませんでした");
	}
	//関連付け
	SelectObject(  this->MemoryDC ,  this->Bitmap  );
	//ふつーの DC の削除.
	ReleaseDC(NULL,hdc);

}

//ビットマップを定義する.
//エラー RDoNotInitaliseException 
HDC RImage::AttachCanvas(const string inAlt ,int inWidth,int inHegiht) throw()
{
	this->Release();
	this->Alt = inAlt;

	this->ContentsWidth =  inWidth;
	this->ContentsHeight = inHegiht;
	
	HDC hdc = GetDC(NULL);
	if (! hdc)
	{
		throw RDoNotInitaliseException("GetDCに失敗しました");
	}
	this->Bitmap = ::CreateCompatibleBitmap(hdc , inWidth , inHegiht);

	this->MemoryDC = CreateCompatibleDC( hdc );
	if( ! this->MemoryDC )
	{
		DeleteObject( this->Bitmap  );
		this->Bitmap = NULL;
		this->MemoryDC = NULL;

		throw RDoNotInitaliseException("メモリDCが作れませんでした");
	}
	//関連付け
	SelectObject(  this->MemoryDC ,  this->Bitmap  );
	//ふつーの DC の削除.
	ReleaseDC(NULL,hdc);

	return this->MemoryDC;
}

//解放
void RImage::Release()
{
	if (this->Bitmap)
	{
		DeleteObject( this->Bitmap );
		this->Bitmap = NULL;
	}
	if (this->MemoryDC)
	{
		DeleteDC( this->MemoryDC );
		this->MemoryDC = NULL;
	}
}


//タイトルを取得する
string RImage::getTitle() const 
{
	return Alt;
}

