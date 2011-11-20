// RCreateFile.h: RCreateFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCREATEFILE_H__BFA7A2A1_1F05_482D_8409_5A8D0FF13A84__INCLUDED_)
#define AFX_RCREATEFILE_H__BFA7A2A1_1F05_482D_8409_5A8D0FF13A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWin32Exception.h"

class RIStream
{
public:
	virtual int Read(void* inPtr , DWORD inSize ) throw(RWin32Exception) = 0;
	virtual int Write(const void* inPtr , int inSize ) throw(RWin32Exception) = 0;
};


class RCreateFile  : public RIStream
{
public:
	RCreateFile()
	{
		FilePointer = NULL;
	}
	virtual ~RCreateFile()
	{
		Close();
	}

	//開く.
	void Open(
			const string & inFileName,                  // ファイル名
			DWORD dwDesiredAccess,                      // アクセスモード
			DWORD dwShareMode = 0,                      // 共有モード
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, // セキュリティ記述子
			DWORD dwCreationDisposition = OPEN_ALWAYS,  // 作成方法
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL, // ファイル属性
			HANDLE hTemplateFile = NULL                 // テンプレートファイルのハンドル
			) throw (RWin32Exception)
	{

		Close();

#ifndef _WIN32_WCE
		FilePointer = 
			::CreateFile(inFileName.c_str() , dwDesiredAccess , dwShareMode , 
				lpSecurityAttributes , dwCreationDisposition , dwFlagsAndAttributes , hTemplateFile);
#else
		MY_USES_CONVERSION;
		FilePointer = 
			::CreateFile( _A2W(inFileName.c_str()) , dwDesiredAccess , dwShareMode , 
				lpSecurityAttributes , dwCreationDisposition , dwFlagsAndAttributes , hTemplateFile);
#endif
		if (FilePointer == INVALID_HANDLE_VALUE)
		{
			FilePointer = NULL;
			throw RWin32Exception("CreateFile(%s) が失敗しました" , inFileName.c_str() );
		}

	}

	//読み取りとして開く(もしなければエラー)
	void ReadFileOpen(const string & inFileName) throw (RWin32Exception)
	{
		this->Open(inFileName , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_READONLY , NULL);
	}
	//新規書き込みとして開く(なければ作成、あったら上書き)
	void WriteFileOpen(const string & inFileName) throw (RWin32Exception)
	{
		this->Open(inFileName , GENERIC_READ | GENERIC_WRITE , 0 , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL);
	}
	//追加として開く(なければ作成、あったら普通に開く)
	void AppendFileOpen(const string & inFileName) throw (RWin32Exception)
	{
		this->Open(inFileName , GENERIC_READ | GENERIC_WRITE , 0 , NULL , OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL);
	}

	//閉じる.
	void Close()
	{
		if (FilePointer == NULL) return ;

		CloseHandle(FilePointer);
		FilePointer = NULL;
	}

	virtual int Read(void* inPtr , DWORD inSize ) throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);

		DWORD readBytes;
		if ( ::ReadFile(FilePointer,inPtr,inSize , &readBytes , NULL) == FALSE)
		{
			throw RWin32Exception("ReadFile が失敗しました");
		}
		return readBytes;
	}
	virtual int Write(const void* inPtr , int inSize ) throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);

		DWORD writeBytes;
		if ( ::WriteFile(FilePointer,inPtr,inSize , &writeBytes , NULL) == FALSE)
		{
			throw RWin32Exception("WriteFile が失敗しました");
		}
		return writeBytes;
	}


	//サイズの取得
	DWORD GetSize()
	{
		ASSERT(FilePointer != NULL);
		return GetFileSize(FilePointer,NULL);
	}
	//指定場所へ移動
	void Seek(LONG inOffset,DWORD inSeek = FILE_BEGIN) throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);
		if (SetFilePointer(FilePointer,inOffset,NULL,inSeek) == 0xFFFFFFFF)
		{
			throw RWin32Exception("SetFilePointera(seek) が失敗しました");
		}
	}
	//現在位置の取得
	DWORD Tell()  throw(RWin32Exception)
	{
		ASSERT(FilePointer != NULL);

		DWORD tell = SetFilePointer(FilePointer,0,NULL,FILE_CURRENT);
		if (tell == 0xFFFFFFFF)
		{
			throw RWin32Exception("SetFilePointer(tell) が失敗しました");
		}

		return tell;
	}

	//ハンドルの取得(危険なのであんまり使うな!)
	HANDLE getHandle() const
	{
		return FilePointer;
	}
private:
	HANDLE	FilePointer;
};


class StreamChecker
{
public:
	StreamChecker(){}
	~StreamChecker(){}
	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead) = 0;
	virtual int getSeparater() const = 0;
	virtual int getNext() const = 0;
};


//連続する改行コードまで
class StreamCheckerForSecutiveReturnCode : public StreamChecker
{
public:
	StreamCheckerForSecutiveReturnCode()
	{
		Separater = 0;
		ConectStart = 0;
	}
	~StreamCheckerForSecutiveReturnCode()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		if (inAllReadyRead -4  < 0) inAllReadyRead = 0;

		for (int i = inAllReadyRead; i < (inNowRead-3) ; i ++) 
		{
			if ( (inBuffer[i] == '\r' && inBuffer[i+1] == '\n' && inBuffer[i+2] == '\r' && inBuffer[i+3] == '\n') )
			{
				ConectStart = i + 4;
				Separater = i;
				return false;
			}
			else if (	(inBuffer[i] == '\r' && inBuffer[i+1] == '\r') ||
						(inBuffer[i] == '\r' && inBuffer[i+1] == '\r')  )
			{
				ConectStart = i + 2;
				Separater = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
};
//改行コードまで
class StreamCheckerForReturnCode : public StreamChecker
{
public:
	StreamCheckerForReturnCode()
	{
		Separater = 0;
		ConectStart = 0;
	}
	~StreamCheckerForReturnCode()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		for (int i = inAllReadyRead; i < inNowRead ; i ++) 
		{
			if ( inBuffer[i] == '\n' )
			{
				Separater = i - 1;
				ConectStart = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
};

class RStream
{
public:
	RStream()
	{
		Buffer = NULL;
		Stream = NULL;
	}
	~RStream()
	{
		delete [] Buffer;
	}

	void Create(RIStream* ioStream , int inTotalBufer = 65535)
	{
		ASSERT(Buffer == NULL);

		Buffer =	new char[inTotalBufer];
		BufferLength = inTotalBufer;
		BufferUse = 0;

		Stream = ioStream;
	}

	int Read(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException);


	


	static void test();
private:
	RIStream*	Stream;
	char	*	Buffer;
	int			BufferLength;
	int			BufferUse;
};
#endif // !defined(AFX_RCREATEFILE_H__BFA7A2A1_1F05_482D_8409_5A8D0FF13A84__INCLUDED_)
