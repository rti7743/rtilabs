// RDiskUtil.h: RDiskUtil クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_)
#define AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWin32Exception.h"

class RDiskUtil  
{
public:
	RDiskUtil();
	virtual ~RDiskUtil();

	//コピー(ディレクトリの場合以下全部)
	static void Copy(const string & inFilename,const string & inTo, bool isSilent = true) throw(RWin32Exception);
	//移動(ディレクトリの場合以下全部)
	static void Move(const string & inFilename,const string & inTo, bool isSilent = true) throw(RWin32Exception);
	//削除(ディレクトリの場合以下全部)
	static void Delete(const string & inFilename, bool isSilent = true) throw(RWin32Exception);
	//ゴミ箱へ
	static void Gomi(const string & inFilename, bool isSilent = true) throw(RWin32Exception);
	//ディレクトリの作成
	static void MakeDirectory(const string & inDirectoryName);
	//ファイルが存在するか?
	static bool Exist(const string & inFileName);

private:
	//ファイル名は \0\0 で終わっている必要があるため付加する
	static char* getDoubleNULLFilename(const string & inStr);
};

#endif // !defined(AFX_RDISKUTIL_H__C6D468DE_A4B5_4BE7_8219_7B87B0385EE1__INCLUDED_)
