// NativeSupportPicture.cpp: NativeSupportPicture クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "NativeSupportPicture.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

NativeSupportPicture::NativeSupportPicture()
{
	this->OlePicture = NULL;
	this->Bm = NULL;
}
//解放.
NativeSupportPicture::~NativeSupportPicture()
{
	if ( this->OlePicture )
	{
		this->OlePicture->Release();
		this->OlePicture = NULL;
	}

	if (this->Bm)
	{
		GlobalFree(this->Bm);
		this->Bm = NULL;
	}
}

//ロード
//失敗は RNotSupportException
void NativeSupportPicture::Load(const string inFilename) throw()
{
	ASSERT(this->Bm == NULL);
	ASSERT(this->OlePicture == NULL);

	HANDLE file = ::CreateFile(inFilename.c_str() , GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) throw RNotSupportException("ファイル %s がみつかりません");

	// ファイルサイズの取得
	DWORD fileSize = ::GetFileSize(file, NULL);
	// 読み込みようバッファの確保
	this->Bm = ::GlobalAlloc(GPTR, fileSize);
	DWORD readByte;
	::ReadFile(file, this->Bm , fileSize, &readByte, NULL);
	CloseHandle(file);

	IStream *iStream;
	if(!SUCCEEDED(::CreateStreamOnHGlobal( this->Bm ,FALSE,&iStream))) 
	{
		throw RNotSupportException("CreateStreamOnHGlobal に失敗しました");
	}
	if(!SUCCEEDED(::OleLoadPicture(iStream,fileSize, TRUE, IID_IPicture, (LPVOID*)&this->OlePicture)))
	{
		iStream->Release();
		throw RNotSupportException("OleLoadPicture に失敗しました");
	}
	iStream->Release();

}

//イメージのサイズの取得
void NativeSupportPicture::getImageSize(LONG *outWidth,LONG *outHeight) const
{
	LONG width,height;

	this->OlePicture->get_Width(&width);
	this->OlePicture->get_Height(&height);

	HDC hdc = ::GetDC(NULL);
	width  = ::MulDiv(width,::GetDeviceCaps(hdc,LOGPIXELSX),2540);
	height = ::MulDiv(height,::GetDeviceCaps(hdc,LOGPIXELSY),2540);
	::ReleaseDC(NULL,hdc);


	*outWidth	= width ;
	*outHeight	= height ;
}

//イメージをデバイスコンテキストに描画
//失敗は RNotSupportException
void NativeSupportPicture::getImage(HDC ioMemDC , LONG inWidth,LONG inHeight)  const throw()
{
	LONG orignalWidth,orignalHeight;
	this->OlePicture->get_Width(&orignalWidth);
	this->OlePicture->get_Height(&orignalHeight);

	if ( this->OlePicture->Render(ioMemDC, 0, 0, inWidth,inHeight , 0, orignalHeight, orignalWidth, -orignalHeight, NULL) != S_OK)
	{
		throw RNotSupportException("デバイスコンテキストに書き込めませんでした");
	}
}