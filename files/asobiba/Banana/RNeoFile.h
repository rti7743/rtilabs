// RNeoFile.h: RNeoFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNEOFILE_H__71D8133C_8042_44CE_8066_72444E4692A9__INCLUDED_)
#define AFX_RNEOFILE_H__71D8133C_8042_44CE_8066_72444E4692A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNeoIStreamble.h"

/**
 * class RNeoFile:	ファイル一般を扱うクラス.
 *
 * @author 
 */
class RNeoFile  : public RNeoIStreamble
{
public:
	/**
	 * RNeoFile:
	 *
	 */
	RNeoFile()
	{
		FilePointer = NULL;
	}
	/**
	 * ~RNeoFile:
	 *
	 */
	virtual ~RNeoFile()
	{
		Close();
	}

	/**
	 * Open:	開く
	 *
	 * @param inFilename					ファイル名
	 * @param inMode						fopen のモード
	 * @return void	
	 * @throw RIOCannotOpenException		ファイルが開けない
	 */
	void Open(string inFilename,string inMode) throw (RIOCannotOpenException)
	{
		Close();

		FilePointer = ::fopen(inFilename.c_str() , inMode.c_str() );
		if (!FilePointer)
		{
			throw RIOCannotOpenException("ファイル %s が開けません" , inFilename.c_str() );
		}
	}

	/**
	 * Close:	閉じる.
	 *
	 * @return void 
	 */
	void Close()
	{
		if (FilePointer)
		{
			fclose(FilePointer);
			FilePointer = NULL;
		}
	}

	/**
	 * Read:	読み込み
	 *
	 * @param outPtr 
	 * @param inSize 
	 * @return int 
	 */
	virtual int Read(char* outBuffer , int inBuferSize)
	{
		return ::fread(outBuffer , 1 , inBuferSize , FilePointer);
	}
	virtual int Write(const char* inBuffer , int inBuferSize)
	{
		return ::fwrite(inBuffer , 1 , inBuferSize , FilePointer);
	}

	int scan(char* Format,...)
	{
		return ::fscanf( FilePointer , Format,(char*)(&Format+1));
	}
	int print(char* Format,...)
	{
		return ::fprintf( FilePointer , Format,(char*)(&Format+1));
	}

	long seek(long inP , int inFlg = SEEK_SET)
	{
		return ::fseek(FilePointer , inP , inFlg);
	}

	long tell() const
	{
		return ::ftell(FilePointer);
	}

	long getSize() 
	{
		long now = tell();

		seek(0,SEEK_END);

		long ret = tell();
		seek(now);
		return ret;
	}

	//おわりっすか?
	bool IsEof() const
	{
		return feof(FilePointer) != 0;
	}

	FILE*	getHandle() const
	{
		return FilePointer;
	}
private:
	FILE	*FilePointer;

};

#endif // !defined(AFX_RNEOFILE_H__71D8133C_8042_44CE_8066_72444E4692A9__INCLUDED_)
