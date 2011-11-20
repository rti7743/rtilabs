// RImage.h: RImage クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIMAGE_H__2B031C10_4BED_493C_A0AE_ED2A64C76C68__INCLUDED_)
#define AFX_RIMAGE_H__2B031C10_4BED_493C_A0AE_ED2A64C76C68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RImage;

#include "comm.h"

class RImage
{
public:
	RImage();
	virtual ~RImage();

	//読み込み
	bool Load(const string inFilename);
	//解放
	void Release();
	//HDC
	const HDC			getHDC() const				{	return	MemoryDC; }
	int					getContentsWidth() const	{	return	ContentsWidth;}
	int					getContentsHeight() const	{	return	ContentsHeight;}
	//タイトルを取得する
	string getTitle() const ;

private:
	//ビットマップを定義する.
	//エラー RDoNotInitaliseException 
	void AttachHBitmap(const string inAlt ,const BITMAPINFO* inBitmapInfo,const void* inBitmapData) throw();
	//空のビットマップを定義する.
	//HDC はメモリデバイスなので、そこに Bitblt してください。
	//エラー RDoNotInitaliseException 
	HDC AttachCanvas(const string inAlt ,int inWidth,int inHegiht) throw();
private:
	HBITMAP				Bitmap;
	HDC					MemoryDC;
	string				Alt;

	DWORD				ContentsWidth;
	DWORD				ContentsHeight;
};

#endif // !defined(AFX_RIMAGE_H__2B031C10_4BED_493C_A0AE_ED2A64C76C68__INCLUDED_)
