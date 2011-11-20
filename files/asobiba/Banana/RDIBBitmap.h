// RDIBBitmap.h: RDIBBitmap クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDIBBITMAP_H__67BD7A26_7F61_4C80_BA1D_BE22EC299BC5__INCLUDED_)
#define AFX_RDIBBITMAP_H__67BD7A26_7F61_4C80_BA1D_BE22EC299BC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWin32Exception.h"

/**
 * class RDIBBitmap:	デバイス独立ビットマップ作成支援とか
 *
 * @author rti
 */
class RDIBBitmap  
{
public:
	RDIBBitmap();
	virtual ~RDIBBitmap();

	/**
	 * Create:			作成します
	 *
	 * @param inW		横幅
	 * @param inH		縦幅
	 * @param inBit		色数(2のべき数)
	 * @return void 
	 */
	void	Create(int inW , int inH , int inBit) throw(RWin32Exception);
	/**
	 * Release:			破棄します
	 *
	 * @return void 
	 */
	void	Release();

	/**
	 * getImage:		メモリの内容の取得
	 *
	 * @return BYTE*	メモリ(間違っても delete しないこと)
	 */
	BYTE*	getImage() 
	{
		return ImageMemory;
	}
	/**
	 * getImage:		メモリの内容の取得
	 *
	 * @return BYTE*	メモリ(間違っても delete しないこと)
	 */
	BYTE*	getImage(int inX, int inY) 
	{
		return ImageMemory + (inX + inY * (Width )) * (Bit / 8);
	}
	/**
	 * getImage:		メモリの内容の取得
	 *
	 * @return const BYTE*	メモリ(間違っても delete しないこと)
	 */
	const BYTE*	getConstImage() const
	{
		return ImageMemory;
	}
	/**
	 * getImage:		メモリの内容の取得
	 *
	 * @return BYTE*	メモリ(間違っても delete しないこと)
	 */
	const BYTE*	getConstImage(int inX, int inY) const
	{
		return ImageMemory + (inX + inY * (Width )) * (Bit / 8);
	}
	/**
	 * getDC:		デバイスコンテンキストの取得
	 *
	 * @return HDC	デバイスコンテキスト(間違っても DeleteObject しないこと)
	 */
	HDC	getDC() 
	{
		return MemoryDC;
	}

	/**
	 * getDC:		デバイスコンテンキストの取得
	 *
	 * @return HDC	デバイスコンテキスト(間違っても DeleteObject しないこと)
	 */
	const HDC	getConstDC() const
	{
		return MemoryDC;
	}

	/**
	 * getWidth:	横幅の取得
	 *
	 * @return int 横幅
	 */
	int getWidth() const
	{
		return Width;
	}

	/**
	 * getHeight:	縦幅の取得
	 *
	 * @return int	縦幅
	 */
	int getHeight() const
	{
		return Height;
	}
	/**
	 * getBit:		色数の取得
	 *
	 * @return int	色数
	 */
	int getBit() const
	{
		return Bit;
	}
	//内容の保存(主にデバッグ用
	void Save(const string & inFilename) const throw(RException);

private:
	BYTE*		ImageMemory;
	HDC			MemoryDC;
	HBITMAP		HBitmap;

	int			Width;
	int			Height;
	int			Bit;
};

#endif // !defined(AFX_RDIBBITMAP_H__67BD7A26_7F61_4C80_BA1D_BE22EC299BC5__INCLUDED_)
