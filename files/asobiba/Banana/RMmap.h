// RMmap.h: RMmap クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMMAP_H__569625AA_1ADE_40D5_83F0_8CF362C49E4A__INCLUDED_)
#define AFX_RMMAP_H__569625AA_1ADE_40D5_83F0_8CF362C49E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWin32Exception.h"

//メモリマップ
class RMmap  
{
public:
	RMmap();
	virtual ~RMmap();

	/**
	 * Create:					メモリをただ割り当てる
	 *
	 * @param inMappingSize		最大割り当てサイズ
	 * @return void 
	 */
	void Create(DWORD inMappingSize) throw(RWin32Exception);
	/**
	 * Create:					ファイルをメモリに割り当てる
	 *
	 * @param inFilename		ファイル名
	 * @param inMappingSize		最大割り当てサイズ
	 * @return void 
	 */
	void Create(const string inFilename ,DWORD inMappingSize, DWORD inOpenFlg = OPEN_EXISTING) throw(RWin32Exception);

	/**
	 * Release:					メモリの破棄
	 *
	 * @return void 
	 */
	void Release();

	/**
	 * getMemory:				メモリの取得
	 *
	 * @return void*			メモリ
	 */
	void* getMemory() const
	{
		return MapMemory;
	}

	/**
	 * Flush:					マップのフラッシュ
	 *
	 * @param inFlushSize		フラッシュするサイズ   0を指定すると全部書き込みます。
	 *							なんか、サイズ指定しても無効になるなんで?
	 * @return void 
	 */
	void Flush(DWORD inFlushSize);

	static void test();

private:
	HANDLE		MapFile;			//ファイルハンドル
	HANDLE		MapObject;			//マップオブジェクト
	void*		MapMemory;			//マップしたメモリ.
};

#endif // !defined(AFX_RMMAP_H__569625AA_1ADE_40D5_83F0_8CF362C49E4A__INCLUDED_)
