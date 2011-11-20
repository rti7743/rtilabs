// RGISSmallFileRead.cpp: RGISSmallFileRead クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallFileRead.h"
#include "RStdioMemory.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallFileRead::RGISSmallFileRead()
{
	this->Index = NULL;
}

RGISSmallFileRead::~RGISSmallFileRead()
{
	Delete();
}

void RGISSmallFileRead::Delete()
{
	if (Index != NULL)
	{
		delete [] this->Index;
		this->Index = NULL;
	}

	this->File.Close();
}

//WinCE などで、サスペンドして復帰すると、
//ファイルハンドルが無効になる現象がある。
//これを回避するために、もしハンドルが無効になっていたら、
//指定したファイルを開きなおすことで対処する.
void RGISSmallFileRead::KeepHanlde() throw(RException)
{
	File.KeepHanlde(this->Filename , "rb");
	if ( this->FileSize != this->File.getSize() )
	{
		throw RException(EXCEPTIONTRACE + "復帰させたファイルのサイズが以前と違います。");
	}
}


void RGISSmallFileRead::Create(const string & inFilename , const string & inUserDataPath) throw(RException)
{
	Delete();

	//ZLIB準備!
	Zlib.Create();
	//ユーザデータ管理を初期化.
	UserDataFile.Create(inUserDataPath);

	//ファイルを開いて.
	this->File.Open(inFilename , "rb");
	this->FileSize = this->File.getSize();
	this->Filename = inFilename;

	char buffer[4];
	//ヘッダーの読み込み
	this->File.fread( buffer , 4 );
	if (!(buffer[0] == 'K' && buffer[1] == 'M' && buffer[2] == 'A' && buffer[3] == 'P'))
	{
		throw RException(EXCEPTIONTRACE + "先頭にKMAPのプレフィックスがありません");
	}

	//バージョンの読み込み.
	unsigned char version;
	this->File.fread( &version , sizeof(version) );
	if (version != 1)
	{
		throw RException(EXCEPTIONTRACE + PrintFormat("未知のバージョンです %d" , (int)version) );
	}

	//index長の読み込み
	unsigned long indexCount = 0;
	this->File.fread( &indexCount , sizeof(indexCount) );

	//indexの読み込み.
	Index = new IndexArea[indexCount];
	this->File.fread( Index , sizeof(IndexArea) * indexCount);

	//目次の長さを記録.
	this->IndexCount = indexCount;

	//ヘッダーの長さを読む
	unsigned long headerSize = 0;
	this->File.fread( &headerSize , sizeof(headerSize) );
	this->HeaderSize = headerSize;

	//ヘッダーの位置を記録.
	this->HeaderPos = File.ftell();
}

bool RGISSmallFileRead::Load(unsigned long inBlock , RGISSmallParentImp * ioParent)
{
	//指定されたブロックの検索.
	int i = 0;
	for( ; i < this->IndexCount ; i++)
	{
		if ( this->Index[i].Block == inBlock )
			break;
	}
	//見つからなかった?
	if ( i == this->IndexCount )
	{
		return false;
	}

	//ブロック長の計算.		次のブロックの開始位置を引けば言いのです。
	unsigned long blockStart = this->Index[i].Possition;
	unsigned long blockSize ;
	if (i+1 == this->IndexCount)
	{
		blockSize =  this->FileSize - blockStart;	//最後のブロックはファイル終端まで続いている.
	}
	else
	{
		blockSize =  this->Index[i + 1].Possition - blockStart;
	}

	//ユーザー定義データを読み込むバッファ.
	char * userData = NULL;

	unsigned char simbol;				// D などのデータシンボル
	unsigned long dataSize;				// このデータの長さ(bytes) ヘッダを除く.
	unsigned long dataCount;			// データのようその数

	//読み込み開始
	this->File.fseek( blockStart );

	//まず、圧縮データを解凍します.
	unsigned long fullSize;				//unzipすると成る正式な大きさ.
	this->File.fread( &fullSize , sizeof(fullSize) );

	unsigned char * dest = new unsigned char[fullSize];
	unsigned char * src = new unsigned char[blockSize];
	//ブロックサイズは、圧縮前サイズを記録している部分も含むため 引き算します
 	this->File.fread( src , blockSize - sizeof(fullSize));

	try
	{
		//このブロックのユーザデータを読み込みます.
		userData = UserDataFile.Load(inBlock , &Zlib);

		//解凍の実行.
		unsigned long realSize = fullSize;
		Zlib.UnCompress(dest , &realSize , src , blockSize);
		ASSERT(realSize == fullSize);

		//まず、もう使わないソース領域の破棄.
		delete [] src;	src  = NULL;

		//解凍したデータの解析.
		RStdioMemory mem;
		mem.Open((char*)dest);
		while( mem.ftell() < realSize )
		{
			simbol = mem.readGetUC();
			dataSize = mem.readGetUL();
			dataCount = mem.readGetUL();

			TRACE("シンボル %c データ長 %ld データの数 %d \n" , simbol , dataSize , (int)dataCount);

			ASSERT( isalpha(simbol) );

			try
			{
				//このシンボルのデータを処理せよ!
				ioParent->Parse(simbol , dataSize , dataCount , mem.ptr() );
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
				throw ;	//上のtryへ
			}

			mem.fseek(dataSize , SEEK_CUR);
		}

		//ユーザ定義データがある場合の処理.
		if (userData)
		{
			mem.Open((char*)userData);
			simbol = mem.readGetUC();
			dataSize = mem.readGetUL();
			dataCount = mem.readGetUL();

			try
			{
				//このシンボルのデータを処理せよ!
				ioParent->Parse(simbol , dataSize , dataCount , mem.ptr() );
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
				throw ;	//上のtryへ
			}
		}

		//今後使用するメモリを RGISSmallParent の管轄下におく(重要!!!!!)
		//つまり、このメモリの delete はすべて RGISSmallParent が行う.
		//忘れないように
		ioParent->setData(inBlock ,(char*) dest , userData);

	}
	catch(RException e)
	{
		delete [] src;
		delete [] dest;
		delete [] userData;

		return false;
	}

	//解凍に使ったメモリの開放.
	delete [] src;
//	delete [] dest;

	//記録されているブロック長と実際の値が、ぴたりといっちしなければいけない
	ASSERT((this->File.ftell() - blockStart) == blockSize);
	return true;
}


bool RGISSmallFileRead::LoadHeader(RGISSmallHeaderImp * ioHedaer)
{
	//ヘッダーの位置に移動
	this->File.fseek( HeaderPos );

	
	unsigned char simbol;				// D などのデータシンボル
	unsigned long dataSize;				// このデータの長さ(bytes) ヘッダを除く.
	unsigned long dataCount;			// データのようその数

	unsigned long pos = this->File.ftell();	
	for(  ;  pos < this->HeaderSize + HeaderPos;  pos = this->File.ftell() )
	{
		this->File.fread( &simbol , sizeof(simbol) );
		this->File.fread( &dataSize , sizeof(dataSize) );
		this->File.fread( &dataCount , sizeof(dataCount) );

		if (ioHedaer->getSimbol() == simbol)
		{	//見つけました!

			//読み込み.
			char * buffer = new char[dataSize + 1];
			this->File.fread(buffer , dataSize );

			try
			{
				//このシンボルのデータを処理せよ!
				ioHedaer->Parse(simbol , dataSize , dataCount , buffer);
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
				delete [] buffer;
				return false;
			}

			delete [] buffer;
			return true;
		}
		//見つからないので読み飛ばし.
		this->File.fseek(dataSize , SEEK_CUR);
	}

	return false;
}

//ユーザーデータの変更.
bool RGISSmallFileRead::UserDataModify(unsigned long inBlock, RGISSmallParentImp * ioParent,
									 const int * inDeleteNumber , int inDeleteCount ,
									 const char * inData , int inSize,int inCount) throw(RException)
{
	//データの変更.
	UserDataFile.Modify(inBlock, inDeleteNumber ,inDeleteCount ,  inData , inSize , inCount , &Zlib);

	//このブロックのユーザデータを読み込みます.
	char * userData = UserDataFile.Load(inBlock , &Zlib);
	//ユーザ定義データがある場合の処理.
	if (userData)
	{
		unsigned char simbol;				// D などのデータシンボル
		unsigned long dataSize;				// このデータの長さ(bytes) ヘッダを除く.
		unsigned long dataCount;			// データのようその数

		RStdioMemory mem;
		mem.Open((char*)userData);
		simbol = mem.readGetUC();
		dataSize = mem.readGetUL();
		dataCount = mem.readGetUL();

		try
		{
			//このシンボルのデータを処理せよ!
			ioParent->Parse(simbol , dataSize , dataCount , mem.ptr() );
		}
		catch(RException e)
		{
			TRACE( e.getMessage() );

			delete [] userData;

			return false;
		}

		ioParent->UserDataReload(userData);
	}
	return true;
}
