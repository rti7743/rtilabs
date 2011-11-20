// RGISSmallUserDataFile.h: RGISSmallUserDataFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLUSERDATAFILE_H__660B80FE_B661_48A6_8258_FF588892231C__INCLUDED_)
#define AFX_RGISSMALLUSERDATAFILE_H__660B80FE_B661_48A6_8258_FF588892231C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RZlib.h"

class RGISSmallUserDataFile  
{
public:
	RGISSmallUserDataFile();
	virtual ~RGISSmallUserDataFile();

	void Create(const string & inPath) ;

	//指定されたブロックのユーザ定義データを読み込みます
	//戻り値は、内部で new されたメモリです。
	//このクラスはこのメモリを管理しないので、適切な場所で開放する必要があります。
	//予定通りであれば、 RGISSmallParent で UserData というメンバで管理され、その暮らすかは記された同時に消滅します.
	char* Load(unsigned long inBlock ,const RZlib * inZlib) const;

	//追加.
	void RGISSmallUserDataFile::Append(unsigned long inBlock ,const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException);
	//削除
	void RGISSmallUserDataFile::Delete(unsigned long inBlock ,const int * inDeleteNumber , int inSize ,const RZlib * inZlib) const throw(RException);

	//変更.
	void RGISSmallUserDataFile::Modify(unsigned long inBlock ,const int * inDeleteNumber , int inDeleteCount ,
								   const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException);
private:
	//指定されたブロックのユーザ定義データを読み込みます
	//戻り値は、内部で new されたメモリです。
	//このクラスはこのメモリを管理しないので、適切な場所で開放する必要があります。
	char* RGISSmallUserDataFile::FileLoad(unsigned long inBlock ,const RZlib * inZlib,int inOverplusAlooc) const;
	void RGISSmallUserDataFile::FileSave(unsigned long inBlock ,const char * inData , unsigned long inSize,const RZlib * inZlib) const throw(RException);
	//次のデータに移動.
	char* RGISSmallUserDataFile::NextData(const char* inData) const;

	//自己チェック ヘッダがないデータのみ.
	bool RGISSmallUserDataFile::MySelfTest(const char* inData , int inSize , int inCount) const;
	//自己チェック.
	bool RGISSmallUserDataFile::MySelfTest(const char* inData) const;
	//ファイルテスト.
	bool RGISSmallUserDataFile::FileTest(unsigned long inBlock ,const RZlib * inZlib) const;


private:

	//ユーザデータがあるパス.
	string Path;
};

#endif // !defined(AFX_RGISSMALLUSERDATAFILE_H__660B80FE_B661_48A6_8258_FF588892231C__INCLUDED_)
