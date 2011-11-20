// RSequenceFileDB.cpp: RSequenceFileDB クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RSequenceFileDB.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RSequenceFileDB::RSequenceFileDB()
{
	SequenceFile = NULL;
}

RSequenceFileDB::~RSequenceFileDB()
{
	Close();
}

/**
 * Close:			データベースのクローズ
 *
 * @return void 
 */
void RSequenceFileDB::Close()
{
	if (SequenceFile)
	{
		fclose(SequenceFile);
		SequenceFile = NULL;
	}
}

/**
 * Open:					データベースのオープン
 *
 * @param inFilename		データベース名
 * @param mode				fopenのモード
 * @return void 
 */
void RSequenceFileDB::Open(const string& inFilename , const char* mode) throw(RIOCannotOpenException)
{
	Close();

	const string sequence = inFilename + ".sequence";
	SequenceFile = fopen(sequence.c_str() , mode);
	if (SequenceFile == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + " データベース" + inFilename + "のシーケンス" + sequence + "が開けません");
	}
}

/**
 * Create:					新規作成
 *
 * @param inFilename		ファイル名	あれば上書き
 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
 * @return void 
 */
void RSequenceFileDB::Create(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename,"w+b");

	DB.Create(inFilename , inHashTableSize);

	Next = 0;
}

/**
 * Load:					読み込み
 *
 * @param inFilename		ファイル名
 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
 * @return void 
 */
void RSequenceFileDB::Load(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename,"r+b");

	DB.Load(inFilename , inHashTableSize);

	fread(&Next , sizeof(int) , 1 , SequenceFile);
}

/**
 * Save:					ディスクに index を書き込みます。 indexは OnMemory なのでセーブしないと無意味です
 *
 * @return void 
 */
void RSequenceFileDB::Save()
{
	fseek( SequenceFile , 0 , SEEK_SET ) ;
	fwrite(&Next , sizeof(int) , 1 , SequenceFile);

	DB.Save();
}


/**
 * Flush:					データを flush します。
 *
 * @return void 
 */
void RSequenceFileDB::Flush()
{
	fflush(SequenceFile);

	DB.Flush();
}

