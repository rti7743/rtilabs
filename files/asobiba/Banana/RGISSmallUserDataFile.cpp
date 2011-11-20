// RGISSmallUserDataFile.cpp: RGISSmallUserDataFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallUserDataFile.h"
#include "RStdioFile.h"
#include "RStdioMemory.h"
#include "RStdioMemoryWrite.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////



RGISSmallUserDataFile::RGISSmallUserDataFile()
{

}

RGISSmallUserDataFile::~RGISSmallUserDataFile()
{

}

void RGISSmallUserDataFile::Create(const string & inPath)
{
	this->Path = inPath;
}

//指定されたブロックのユーザ定義データを読み込みます
//戻り値は、内部で new されたメモリです。
//このクラスはこのメモリを管理しないので、適切な場所で開放する必要があります。
//予定通りであれば、 RGISSmallParent で UserData というメンバで管理され、その暮らすかは記された同時に消滅します.
char* RGISSmallUserDataFile::Load(unsigned long inBlock ,const RZlib * inZlib) const
{
	return FileLoad(inBlock , inZlib , 0 );
}

//指定されたブロックのユーザ定義データを読み込みます
//戻り値は、内部で new されたメモリです。
//このクラスはこのメモリを管理しないので、適切な場所で開放する必要があります。
char* RGISSmallUserDataFile::FileLoad(unsigned long inBlock ,const RZlib * inZlib,int inOverplusAlooc ) const
{
	unsigned char * src = NULL;
	unsigned char * dest = NULL;
	try
	{
		RStdioFile file;
		file.Open(Path + num2str(inBlock) ,"rb");

		//ファイルの先頭に解凍すると何バイトになるかという情報があります.
		unsigned long fullSize;				//unzipすると成る正式な大きさ.
		file.fread( &fullSize , sizeof(fullSize) );
		//先頭以外のデータはすべて圧縮されています.
		int zipSize = file.getSize() - sizeof(fullSize);

		//ファイルを読み込むためのメモリを確保します。
		src = new unsigned char[zipSize];
		dest = new unsigned char[fullSize + inOverplusAlooc];

		//zipされたデータを読み込みます.
		file.fread(src , zipSize);

		//解凍の実行.
		unsigned long realSize = fullSize;
		inZlib->UnCompress(dest , &realSize , src , zipSize);
		ASSERT(realSize == fullSize);

		//まず、もう使わないソース領域の破棄.
		delete [] src;	src  = NULL;
	}
	catch(RException e)
	{
		delete [] src;
		delete [] dest;
		
		return NULL;
	}

	//解凍に使ったメモリの開放.
	delete [] src;

	return (char*)dest;
}

//ファイルにセーブする.
void RGISSmallUserDataFile::FileSave(unsigned long inBlock ,const char * inData , unsigned long inSize,const RZlib * inZlib) const throw(RException)
{
	unsigned char * dest = NULL;
	try
	{
		RStdioFile file;
		file.Open(Path + num2str(inBlock) ,"wb");

		//圧縮した後のデータをおさめる領域. (ちょっと大きめに確保します)
		unsigned long zipSize = inSize + 1024;
		dest = new unsigned char[zipSize];
		inZlib->Compress2(dest , &zipSize , (unsigned char*)inData , inSize);

		//書き込み開始.

		file.fwrite( &inSize , sizeof(inSize) );
		file.fwrite( dest , zipSize );

	}
	catch(RException e)
	{
		delete [] dest;
		
		throw;
	}

	delete [] dest;

	ASSERT( FileTest(inBlock , inZlib) );
}

class RGISSmallUserDataFileHeader
{
public:
	void Create(const char* inData)
	{
		RStdioMemory mem;
		mem.Open(inData);

		Simbol = mem.readGetUC();
		DataSize = mem.readGetUL();
		DataCount = mem.readGetUL();

		DataStart = mem.ptr();
	}

	//データサイズと件数の変更
	void Change(unsigned char inSimbol , unsigned long inDataSize , unsigned long inDataCount)
	{
		Simbol = inSimbol;
		DataSize = inDataSize;
		DataCount = inDataCount;
	}

	int getHeaderSize() const
	{
		return sizeof(Simbol) + sizeof(DataSize) + sizeof(DataCount) ;
	}

	void Write(char * outData) const
	{
		RStdioMemoryWrite mem;
		mem.Open(outData);

		mem.writeGetUC(Simbol);
		mem.writeGetUL(DataSize);
		mem.writeGetUL(DataCount);
	}

	// D などのデータシンボル
	unsigned char getSimbol() const
	{
		return Simbol;
	}
	// このデータの長さ(bytes) ヘッダを除く.
	unsigned long getDataSize() const
	{
		return DataSize;
	}
	// データの要素の数
	unsigned long getDataCount() const
	{
		return DataCount;
	}
	//実データ
	const char* getDataStart() const
	{
		return DataStart;
	}
	//次のデータのヘッダー部分へ
	const char* getNextBlock() const
	{
		return DataStart + DataSize;
	}


private:
	unsigned char Simbol;				// D などのデータシンボル
	unsigned long DataSize;				// このデータの長さ(bytes) ヘッダを除く.
	unsigned long DataCount;			// データの要素の数

	const char* DataStart;				// 実データ
};



//追加.
void RGISSmallUserDataFile::Append(unsigned long inBlock ,const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException)
{
	Modify(inBlock , NULL , 0 , inData , inSize , inCount , inZlib);
}


//削除
void RGISSmallUserDataFile::Delete(unsigned long inBlock ,const int * inDeleteNumber , int inSize ,const RZlib * inZlib) const throw(RException)
{
	Modify(inBlock , inDeleteNumber , inSize , "" , 0 , 0 , inZlib);
}

//追加.
void RGISSmallUserDataFile::Modify(unsigned long inBlock ,const int * inDeleteNumber , int inDeleteCount ,
								   const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException)
{
	RGISSmallUserDataFileHeader header;

	//追加されるデータの自己テスト.
	ASSERT(MySelfTest(inData,inSize,inCount) );

	//ファイルを読み込み.
	char * src = FileLoad(inBlock , inZlib , inSize );
	if (src == NULL)
	{
		//削除がない場合は、ただの追加なわけでファイルがなくてもエラーにしない.
		if (inDeleteCount != 0)
		{
			throw RException(EXCEPTIONTRACE + "ユーザ定義ファイルが開けません");
		}
		//ファイルを新規作成して追加.
		int realSize = header.getHeaderSize() + inSize;
		char * data = new char[realSize];
		header.Change( 'Z' , inSize , inCount);
		header.Write(data);
		memcpy(data + header.getHeaderSize() , inData , inSize);

		//書き込むないようテスト
		ASSERT(MySelfTest(data) );

		FileSave(inBlock , data , realSize  , inZlib);

		delete [] data;

		return ;
	}
	char * nowSrc = src ;

	//元ファイルのテスト.
	ASSERT(MySelfTest(src) );
	ASSERT(MySelfTest(src) );

	header.Create(src);
	int realSize = header.getDataSize();

	char * dest = new char[ header.getHeaderSize() + header.getDataSize() + inSize ];
	char * nowDest = dest ;

	int writeCount = 0;

	//ヘッダー部分のコピー.
	memcpy(dest , src , header.getHeaderSize() );
	nowDest += header.getHeaderSize();
	nowSrc += header.getHeaderSize();

	if (inDeleteCount == 0)
	{
		//削除は行わない.  すべてのデータをコピーする.
		int size = header.getDataSize() ;
		memcpy(nowDest , nowSrc , size );
		nowDest += size;

		writeCount = header.getDataCount();

		//単純にコピーした結果をテスト
		ASSERT(MySelfTest(dest) );
	}
	else
	{
		//データをスキャンして削除しなければならないデータ以外を dest に書き写す.
		for(unsigned int i = 0 ; i < header.getDataCount() ; i ++)
		{
			//次のデータへ移動.
			char * p = NextData(nowSrc);

			//このデータは削除対象ですか?
			for(int skip = 0 ; skip < inDeleteCount ; skip ++ )
			{
				if ( i == inDeleteNumber[skip] ) break;
			}
			if (skip == inDeleteCount)
			{
				//削除しなくいいらしいので書き込む.
				memcpy(	nowDest , nowSrc , p - nowSrc);
				nowDest += p - nowSrc;
				writeCount++;
			}

			nowSrc = p;
		}
	}
	//追加するデータ.
	memcpy(nowDest, inData , inSize);
	nowDest+= inSize;
	writeCount += inCount;

	int writeSize = nowDest - dest ;

	//ヘッダを更新.
	header.Change( header.getSimbol() ,  writeSize - header.getHeaderSize(), writeCount);
	header.Write(dest);


	//書き込むないようテスト
	ASSERT(MySelfTest(dest) );

	//データの書き込み.
	FileSave(inBlock , dest , writeSize , inZlib);

	delete [] dest;
	delete [] src;
}

//次のデータに移動.
char* RGISSmallUserDataFile::NextData(const char* inData) const
{
	int header = 
			sizeof(unsigned short)	+		//x
			sizeof(unsigned short)	;		//y
	RStdioMemory Mem;
	Mem.Open(inData + header);

	unsigned char type;
	unsigned char size;
	while(1)
	{
		type = Mem.readGetUC();	//type
		size = Mem.readGetUC();	//size
		if (type == 0xff && size == 0) break;
		Mem.fseek(size , SEEK_CUR);
	}
	return (char*) Mem.ptr();
}

//自己チェック.
bool RGISSmallUserDataFile::MySelfTest(const char* inData) const
{
	RGISSmallUserDataFileHeader header;
	header.Create(inData);
	if (  header.getSimbol() != 'Z')
	{
		ASSERT(0);
		return false;
	}

	//すべてのデータをスキャンしてみる.
	char * data = (char*) header.getDataStart();
	const char * dataOver = data + header.getDataSize();
	for(unsigned int i = 0 ; i < header.getDataCount() ; i ++)
	{
		//次のデータへ移動.
		data = NextData(data);

		if (dataOver < data)
		{
			ASSERT(0);
			return false;
		}
	}

	//データのサイズを記述しているので等しくなるはず.
	if (dataOver != data)
	{
		ASSERT(0);
		return false;
	}

	return true;
}

//自己チェック ヘッダがないデータのみ.
bool RGISSmallUserDataFile::MySelfTest(const char* inData , int inSize , int inCount) const
{
	//すべてのデータをスキャンしてみる.
	char * data = (char*) inData;
	const char * dataOver = data + inSize;
	for(int i = 0 ; i < inCount ; i ++)
	{
		//次のデータへ移動.
		data = NextData(data);

		if (dataOver < data)
		{
			ASSERT(0);
			return false;
		}
	}

	//データのサイズを記述しているので等しくなるはず.
	if (dataOver != data)
	{
		ASSERT(0);
		return false;
	}

	return true;

}

//ファイルテスト.
bool RGISSmallUserDataFile::FileTest(unsigned long inBlock ,const RZlib * inZlib) const
{
	char * p = FileLoad(inBlock , inZlib , 0);
	if ( MySelfTest(p) == false)
	{
		delete [] p ;

		ASSERT(0);
		return false;
	}

	delete [] p ;

	return true;

}
