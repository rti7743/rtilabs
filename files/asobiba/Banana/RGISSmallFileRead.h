// RGISSmallFileRead.h: RGISSmallFileRead クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLFILEREAD_H__40832C33_DE9C_4BD8_8B83_1934EF33A1B6__INCLUDED_)
#define AFX_RGISSMALLFILEREAD_H__40832C33_DE9C_4BD8_8B83_1934EF33A1B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RException.h"
#include "RStdioFile.h"
#include "RGISSmallParentImp.h"
#include "RGISSmallHeaderImp.h"
#include "RGISSmallUserDataFile.h"
#include "RZlib.h"

class RGISSmallFileRead  
{
public:
	RGISSmallFileRead();
	virtual ~RGISSmallFileRead();

	void Create(const string & inFilename, const string & inUserDataPath) throw(RException);
	void Delete();
	bool Load(unsigned long inBlock, RGISSmallParentImp * ioParent);
	bool LoadHeader(RGISSmallHeaderImp * ioHedaer);

	//ユーザーデータの変更.
	bool UserDataModify(unsigned long inBlock, RGISSmallParentImp * ioParent,
									 const int * inDeleteNumber , int inDeleteCount ,
									 const char * inData , int inSize,int inCount) throw(RException);


	//WinCE などで、サスペンドして復帰すると、
	//ファイルハンドルが無効になる現象がある。
	//これを回避するために、もしハンドルが無効になっていたら、
	//指定したファイルを開きなおすことで対処する.
	void KeepHanlde() throw(RException);

private:
	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};
private:
	//ファイル名
	string		Filename;
	//マップファイル.
	RStdioFile	File;
	//マップファイルサイズ.
	unsigned long FileSize;
	//目次.
	IndexArea*  Index;
	//目次の長さ
	unsigned long 	IndexCount;
	//ヘッダーの位置
	unsigned long	HeaderPos;
	//ヘッダーの長さ
	unsigned long	HeaderSize;

	//ユーザ定義データを管理
	RGISSmallUserDataFile UserDataFile;

	RZlib	Zlib;
};

#endif // !defined(AFX_RGISSMALLFILEREAD_H__40832C33_DE9C_4BD8_8B83_1934EF33A1B6__INCLUDED_)
