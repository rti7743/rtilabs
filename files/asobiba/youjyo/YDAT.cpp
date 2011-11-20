// YDAT.cpp: YDAT クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YDAT.h"
#include "YDATReader.h"
#include <sys/stat.h>
#include <sys/utime.h>
#include "imagehlp.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

/**
 * YDAT:	コンストラクタ
 *
 * @param inBoardName		板の名前
 * @param inDatNumber		DATの名前
 * @return  
 */
YDAT::YDAT(DATNUMBER	inDatNumber)
{
	IndexMM.Create(MAX_NUMBER_FULL * sizeof(DWORD) );
	this->Index = (DWORD*) IndexMM.getMemory();

	DatMM.Create(MAX_DAT_SIZE * sizeof(char) );
	this->Dat = (char*) DatMM.getMemory();

	this->DatNumber = inDatNumber;

	this->Index[0] = '\0';
	this->Dat[0] = '\0';
	this->No = 0;
}

/**
 * ~YDAT:		デストラクタ
 *
 * @return  
 */
YDAT::~YDAT()
{
	DatMM.Release();
	IndexMM.Release();
}

/**
 * IndexLoad:		INDEXファイルの読み込み.
 *
 * @throw		RIOCannotOpenException	ファイルけません.
 * @return void 
 */
void YDAT::IndexLoad(const string inBoardName)	
{
	string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\" + num2str(this->DatNumber) + ".idx";
	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "indexファイルが開けませんでした" + filename);
	}

	int no = fread(this->Index,sizeof(DWORD),MAX_NUMBER_FULL,fp);

	this->No = no - 1;
	ASSERT(this->No >= 0) ;
	ASSERT(this->No <= MAX_NUMBER_FULL) ;

	fclose(fp);

}

/**
 * DATLoad:		DATファイルの読み込み.
 *
 * @throw		RIOCannotOpenException	ファイルけません.
 * @return void 
 */
void YDAT::DATLoad(const string inBoardName) throw(RIOCannotOpenException)
{
	ASSERT(this->Dat[0] == '\0');

	string filename =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\" + num2str(this->DatNumber) + ".dat";
	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "DATファイルが開けませんでした" + filename);
	}

	const int readbyte = fread( this->Dat ,sizeof(char), Index[this->No] ,fp);
	ASSERT (readbyte == Index[this->No] );
	fclose(fp);

	//最終更新時刻を求める.
	struct _stat statbuf;
	if ( _stat( filename.c_str() ,  &statbuf) == -1)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "stat の取得に失敗" + filename);
	}
	this->LastModified = statbuf.st_mtime;

	//ザブジェクとのロード.
	if (this->No >= 1)
	{
		this->Subject = YDATReader::Subject( this->Dat + this->Index[0] , this->Index[1] -  this->Index[0] );
	}
}


/**
 * ReadAt:			inStartNo ～ inEndNo まで読み込む
 *
 * @param outRPack	読み込んだ内容が保存されるバッファ.
 * @param inStartNo 開始地点
 * @param inEndNo	終了地点
 * @return bool
 */
bool YDAT::ReadAt(RPack * outRPack,unsigned int inStartNo,unsigned int inEndNo)  const 
{
	ASSERT(this->Dat[0] != '\0');

	if (inStartNo < 0) inStartNo = 0;
	if (inEndNo >= No) inEndNo = No;
	
	if (inStartNo >= inEndNo) return false;

	RAutoReadSpinLock al(&Lock) ;
	outRPack->Write(this->Dat + this->Index[inStartNo] , this->Index[inEndNo] -  this->Index[inStartNo]);

	return true;
}

/**
 * ReadAtGoEND:		inStartNo ～ ALL まで読み込む
 *
 * @param outRPack	読み込んだ内容が保存されるバッファ.
 * @param inStartNo 開始地点
 * @return bool 
 */
bool YDAT::ReadAtGoEND(RPack * outRPack,unsigned int inStartNo)  const 
{
	ASSERT(this->Dat[0] != '\0');

	return this->ReadAt(outRPack,inStartNo,No);
}

/**
 * ReadByte:		inStartByte ～ inEndByte まで読み込む
 *
 * @param outRPack		読み込んだ内容が保存されるバッファ.
 * @param inStartByte	開始バイト
 * @param inEndByte		終了バイト
 * @return bool 
 */
bool YDAT::ReadByte(RPack * outRPack,DWORD inStartByte,DWORD inEndByte)  const 
{
	ASSERT(this->Dat[0] != '\0');

	if (inEndByte >= this->Index[No]) inEndByte = this->Index[No];
	if (inStartByte >= inEndByte) return false;

	RAutoReadSpinLock al(&Lock) ;
	outRPack->Write(this->Dat + inStartByte,inEndByte - inStartByte);

	return true;
}

/**
 * ReadByteGoEND:		inStartByte ～ ALL まで読み込む
 *
 * @param outRPack		読み込んだ内容が保存されるバッファ.
 * @param inStartByte	開始バイト
 * @return bool 
 */
bool YDAT::ReadByteGoEND(RPack * outRPack,int inStartByte)  const 
{
	ASSERT(this->Dat[0] != '\0');

	return this->ReadByte(outRPack,inStartByte,this->Index[No]);
}

/**
 * Append:				書き込み
 *
 * @param	inRPack		書き込むデータ
 */
void YDAT::Append(const RPack * inRPack) throw(RIOCannotWriteException)
{
	RAutoWriteSpinLock al(&Lock) ;

	if (No >= MAX_NUMBER) throw RIOCannotWriteException(EXCEPTIONTRACE + "これ以上書き込みできません");
	if (this->Index[No]+inRPack->getSize() >= MAX_DAT_SIZE) throw RIOCannotWriteException(EXCEPTIONTRACE + "最大容量をオーバーしています");

	DWORD start = this->Index[No];
	memcpy ( &this->Dat[start] , inRPack->getMemory() , inRPack->getSize() );
	this->Index[No+1] = 0 + start + inRPack->getSize();

	this->No++;

	this->LastModified = time(NULL);
}

/**
 * Append:				書き込み(Admin 理論限界値位置まで)
 *
 * @param	inRPack		書き込むデータ
 */
void YDAT::AppendAdmin(const RPack * inRPack) throw(RIOCannotWriteException)
{
	RAutoWriteSpinLock al(&Lock) ;

	if (No >= MAX_NUMBER_FULL-1) throw RIOCannotWriteException(EXCEPTIONTRACE + "これ以上書き込みできません");
	if (this->Index[No]+inRPack->getSize() >= MAX_DAT_SIZE) throw RIOCannotWriteException(EXCEPTIONTRACE + "最大容量をオーバーしています");

	DWORD start = this->Index[No];
	memcpy ( &this->Dat[start] , inRPack->getMemory() , inRPack->getSize() );
	this->Index[No+1] = 0 + start + inRPack->getSize();

	this->No++;

	this->LastModified = time(NULL);
}


/**
 * Overwrite:				オーバーライド
 *
 * @param inDeleteNo 
 * @param inOverwrite 
 * @return void 
 */
void YDAT::Overwrite(int inDeleteNo , const RPack * inOverwrite) throw(RIOCannotWriteException)
{
	RAutoWriteSpinLock al(&Lock) ;

	if(inDeleteNo < 0) throw RIOCannotWriteException(EXCEPTIONTRACE + "上書きする範囲が変です");
	if (this->No < inDeleteNo)throw RIOCannotWriteException(EXCEPTIONTRACE + "上書きする範囲が変です");

	//オリジナルの文字列の長さ
	DWORD start = this->Index[inDeleteNo];
	DWORD next = this->Index[inDeleteNo+1];
	DWORD orinalLen = next - start;

	//上書き文字列の長さ
	DWORD OverwriteLen = inOverwrite->getSize();

	//ずらします。
	memmove( this->Dat + start + OverwriteLen , this->Dat + next , this->Index[this->No] - next );
	//上書きします。
	memcpy(this->Dat + start , inOverwrite->getMemory() , OverwriteLen);

	//indexを計算しなおす.
	DWORD sa;
	if (OverwriteLen > orinalLen)
	{
		//もとより増えるので差分をプラスします。
		sa = OverwriteLen - orinalLen;
		//実は容量オーバーとかしてませんよね?
		if (this->Index[this->No] + sa >= MAX_DAT_SIZE)
		{
			throw RIOCannotWriteException(EXCEPTIONTRACE + "DATの容量がフルなので、上書きできませんでした");
		}

		for(int i = inDeleteNo+1 ; i <= this->No ; i ++)
		{
			this->Index[i] += sa;
		}
	}
	else
	{
		//もとより減るので差分をマイナスします。
		sa = orinalLen - OverwriteLen;
		for(int i = inDeleteNo+1 ; i <= this->No ; i ++)
		{
			this->Index[i] -= sa;
		}
	}
	
}

/**
 * IndexLoad:		INDEXファイルの書き込み
 *
 * @throw		RIOCannotOpenException	ファイルけません.
 * @return void 
 */
void YDAT::IndexSave(const string inBoardName) const  throw(RIOCannotOpenException) 
{
	if (this->No <= 0)  return;

	string directory =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\";
	MakeSureDirectoryPathExists(directory.c_str());
	string filename = directory + num2str(this->DatNumber) + ".idx";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "indexファイルが開けませんでした" + filename);
	}

	fwrite(this->Index,sizeof(DWORD), this->No + 1,fp);

	fclose(fp);

}

/**
 * DATLoad:		DATファイルの書き込み
 *
 * @throw		RIOCannotOpenException	ファイルけません.
 * @return void 
 */
void YDAT::DATSave(const string inBoardName) const throw(RIOCannotOpenException)  
{
	ASSERT(this->Dat[0] != '\0');
	if (this->No <= 0)  return;

	string directory =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\";
	MakeSureDirectoryPathExists(directory.c_str());
	string filename = directory + num2str(this->DatNumber) + ".dat";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "DATファイルが開けませんでした" + filename);
	}

	const int writebyte = fwrite( this->Dat ,sizeof(char), Index[this->No] ,fp);
	ASSERT (writebyte == Index[this->No] );
	fclose(fp);

	struct _utimbuf utimebuf;
	utimebuf.actime  = this->LastModified ;
	utimebuf.modtime = this->LastModified  ;
	_utime( filename.c_str() , &utimebuf );


}


void YDAT::test()
{
	//一回だけテスト
	//かきこみてすと.
	{
		YDAT * dat = new YDAT(1234567);
		{
			RPack pack;
			pack.Write("すれがたった!!",sizeof("すれがたった!!") );
			dat->Append(&pack);
			ASSERT(dat->No == 1);
			ASSERT(dat->Index[dat->No] == sizeof("すれがたった!!") );
		}
		dat->Save("test");
		delete dat;
	}
	//よみこみてすと
	{
		YDAT * dat = new YDAT(1234567);
		dat->IndexLoad("test");
		ASSERT( dat->No == 1 );
		ASSERT(dat->Index[dat->No] == sizeof("すれがたった!!") );
		dat->DATLoad("test");
		ASSERT( dat->No == 1 );

		{
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "すれがたった!!" , pack.getSize() ) == 0 );
		}
		delete dat;
	}

	//3回テスト
	//かきこみてすと.
	{
		YDAT * dat = new YDAT(1234567);
		{
			RPack pack;
			pack.Write("すれがたった!!",sizeof("すれがたった!!") );
			dat->Append(&pack);
			ASSERT(dat->Index[dat->No] == sizeof("すれがたった!!") );

			pack.Write("クララもたった!!",sizeof("クララもたった!!") );
			dat->Append(&pack);
			ASSERT(dat->Index[dat->No] - dat->Index[dat->No - 1] == sizeof("クララもたった!!") );

			pack.Write("通報しますた",sizeof("通報しますた") );
			dat->Append(&pack);
			ASSERT(dat->Index[dat->No] - dat->Index[dat->No - 1] == sizeof("通報しますた") );
			ASSERT(dat->No == 3);
		}
		dat->IndexSave("test");
		dat->DATSave("test");
		delete dat;
	}
	//よみこみてすと
	{
		YDAT * dat = new YDAT(1234567);
		dat->IndexLoad("test");
		ASSERT( dat->No == 3 );
		ASSERT(dat->Index[1] == sizeof("すれがたった!!") );
		ASSERT(dat->Index[2] - dat->Index[1] == sizeof("クララもたった!!") );
		ASSERT(dat->Index[3] - dat->Index[2] == sizeof("通報しますた") );

		dat->DATLoad("test");
		ASSERT( dat->No == 3 );

		{
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "すれがたった!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "クララもたった!!" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "すれがたった!!\0クララもたった!!\0通報しますた" , pack.getSize() ) == 0 );
			
		}
		delete dat;
	}

	
	//上書きテスト.
	{
		YDAT * dat = new YDAT(1234567);
		dat->IndexLoad("test");
		ASSERT( dat->No == 3 );
		ASSERT(dat->Index[1] == sizeof("すれがたった!!") );
		ASSERT(dat->Index[2] - dat->Index[1] == sizeof("クララもたった!!") );
		ASSERT(dat->Index[3] - dat->Index[2] == sizeof("通報しますた") );

		dat->DATLoad("test");
		ASSERT( dat->No == 3 );

		{
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "すれがたった!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "クララもたった!!" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "すれがたった!!\0クララもたった!!\0通報しますた" , pack.getSize() ) == 0 );
		}
		//上書き実行 2番目を上書き. もとより小さい上書き
		{
			RPack overwrite;
			overwrite.Write("くぽー" , sizeof("くぽー"));
			dat->Overwrite( 1 , &overwrite);

			const char* r;
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "すれがたった!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "くぽー" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "すれがたった!!\0くぽー\0通報しますた" , pack.getSize() ) == 0 );

		}
		//上書き実行 2番目をさらに上書き. もとよりでかい上書き
		{
			RPack overwrite;
			overwrite.Write("ウエーハッハッ" , sizeof("ウエーハッハッ"));
			dat->Overwrite( 1 , &overwrite);

			const char* r;
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "すれがたった!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "ウエーハッハッ" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "通報しますた" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "すれがたった!!\0ウエーハッハッ\0通報しますた" , pack.getSize() ) == 0 );

		}
		delete dat;
	}
	puts("YDAT::test() OK");
}

