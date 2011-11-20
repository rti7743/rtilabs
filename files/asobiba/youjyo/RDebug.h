// RDebug.h: RDebug クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDEBUG_H__4A6D6880_2257_4DB5_A88E_198C4502ACE5__INCLUDED_)
#define AFX_RDEBUG_H__4A6D6880_2257_4DB5_A88E_198C4502ACE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RDebug  
{
public:
	RDebug();
	virtual ~RDebug();

	/**
	 * MemoryToFile:		メモリをファイルに
	 *
	 * @param inFilename	ファイル名
	 * @param inMemory		メモリ
	 * @param inSize		メモリサイズ
	 * @return void 
	 */
	static void RDebug::MemoryToFile(const string inFilename ,const void* inMemory , int inSize);

	/**
	 * MemoryToFile:		メモリをファイルに
	 *
	 * @param inFilename	ファイル名
	 * @param inMemory		メモリ
	 * @return void 
	 */
	static void RDebug::MemoryToFile(const string inFilename ,const char* inMemory );

	/**
	 * MemoryToFile:		メモリをファイルに
	 *
	 * @param inFilename	ファイル名
	 * @param inMemory		メモリ
	 * @return void 
	 */
	static void RDebug::MemoryToFile(const string inFilename ,string inMemory );

	/**
	 * FileCheck:			二つのファイルが同じかどうかチェック.
	 *
	 * @param inSrc			ファイル名1
	 * @param inDest		ファイル名2
	 * @return void 
	 */
	static void RDebug::FileVerify(const string inSrc,const string inDest);

};

#endif // !defined(AFX_RDEBUG_H__4A6D6880_2257_4DB5_A88E_198C4502ACE5__INCLUDED_)
