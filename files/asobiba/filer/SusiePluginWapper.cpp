// SusiePluginWapper.cpp: SusiePluginWapper クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SusiePluginWapper.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//とりあえず初期化と。
SusiePluginLoadedPicture::SusiePluginLoadedPicture()
{
	memset(&Info , 0 , sizeof(PictureInfo) );
	Binfo = NULL;
	Bm = NULL;
	BitmapInfo	= NULL;
	Bitmap	= NULL;
}
//解放と。
SusiePluginLoadedPicture::~SusiePluginLoadedPicture()
{
	//ロックされているならUnlock.
	if (this->BitmapInfo)
	{
		ASSERT(this->Binfo != NULL);

		::LocalUnlock(this->Binfo);
		this->BitmapInfo = NULL;
	}
	//メモリ解放
	::LocalFree(this->Binfo);
	this->Binfo = NULL;

	//ロックされているならUnlock.
	if (this->Bitmap)
	{
		ASSERT(this->Bm != NULL);

		::LocalUnlock(this->Bm);
		this->Bitmap = NULL;
	}
	//メモリ解放
	::LocalFree(this->Bm);
	this->Bm = NULL;
}

//Bitmap情報の取得.
const BITMAPINFO* SusiePluginLoadedPicture::getBitmapInfo()
{
	ASSERT(this->Binfo != NULL);
	//すでにロックしている場合はそのまま。
	if (this->BitmapInfo != NULL)	return this->BitmapInfo;

	this->BitmapInfo = (BITMAPINFO*) ::LocalLock(this->Binfo);
	return this->BitmapInfo;
}

//Bitmap自体の取得,
const void* SusiePluginLoadedPicture::getBitmap()
{
	ASSERT(this->Bm != NULL);
	//すでにロックしている場合はそのまま。
	if (this->Bitmap != NULL)	return this->Bitmap;

	this->Bitmap = (void*) ::LocalLock(this->Bm);
	return this->Bitmap;
}





SusiePluginWapper::SusiePluginWapper()
{

}

SusiePluginWapper::~SusiePluginWapper()
{

}

//サポートされているかどうか調べる
bool SusiePluginWapper::IsSupported(const string inFilename,char* io2KMemory) const 
{
	bool ret;
	char * filename = strdup( inFilename.c_str() );
	try
	{
		ret = Plugin.IsSupported( filename ,io2KMemory );
	}
	catch(RSusieConferException e)
	{
		free(filename);
		throw;
	}
	free(filename);

	return ret;
}

//画像の情報の取得
//エラー RSusieConferException
void SusiePluginWapper::GetPictureInfo(const string inFilename,SusiePluginLoadedPicture *outPicture) const throw()
{
	char * filename = strdup( inFilename.c_str() );
	try
	{
		Plugin.GetPictureInfo( filename ,&(outPicture->Info) );
	}
	catch(RSusieConferException e)
	{
		free(filename);
		throw;
	}
	free(filename);
}

void SusiePluginWapper::GetPicture(const string inFilename,
			SusiePluginLoadedPicture *outPicture,	SusieCallable* inCallable) const throw()
{
	char * filename = strdup( inFilename.c_str() );
	try
	{
		Plugin.GetPicture( filename ,&(outPicture->Binfo),&(outPicture->Bm) ,inCallable);
	}
	catch(RSusieConferException e)
	{
		free(filename);
		throw;
	}
	free(filename);
}
