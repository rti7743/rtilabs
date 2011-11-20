// RSequenceFileDB.h: RSequenceFileDB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSEQUENCEFILEDB_H__74389509_8B23_4A60_A668_8B73AF9FFF67__INCLUDED_)
#define AFX_RSEQUENCEFILEDB_H__74389509_8B23_4A60_A668_8B73AF9FFF67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RFileDB.h"

//連番が保証されているデータベース.
class RSequenceFileDB  
{
public:
	RSequenceFileDB();
	virtual ~RSequenceFileDB();

	/**
	 * Create:					新規作成
	 *
	 * @param inFilename		ファイル名	あれば上書き
	 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
	 * @return void 
	 */
	void Create(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException);

	/**
	 * Load:					読み込み
	 *
	 * @param inFilename		ファイル名
	 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
	 * @return void 
	 */
	void Load(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException);

	/**
	 * Save:					ディスクに index を書き込みます。 indexは OnMemory なのでセーブしないと無意味です
	 *
	 * @return void 
	 */
	void Save();

	/**
	 * Flush:					データを flush します。
	 *
	 * @return void 
	 */
	void Flush();

	/**
	 * Select:					検索
	 *
	 * @param inKey				検索キー
	 * @param outData			データを書き込むバッファ
	 * @return void 
	 */
	void Select(DWORD inKey , RBuffer* outData) const throw(RNotFoundException)
	{
		DB.Select(inKey , outData);
	}

	/**
	 * Insert:					挿入
	 *
	 * @param inData			挿入するデータ
	 * @return void 
	 */
	int Insert(const RBuffer& inData ) throw(RNotFoundException)
	{
		DB.Insert(Next , inData);

		return Next++;
	}

	/**
	 * Update:					変更
	 *
	 * @param inKey				検索キー
	 * @param inData			変更するデータ
	 * @return void 
	 */
	void Update(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
	{
		DB.Update(inKey , inData);
	}
	/**
	 * Delete:					削除
	 *
	 * @param inKey				検索キー
	 * @return void				
	 */
	void Delete(DWORD inKey ) throw(RNotFoundException)
	{
		DB.Delete(inKey);
	}
	/**
	 * IsHit:					存在チェック
	 *
	 * @param inKey				検索キー
	 * @return bool 
	 */
	bool IsHit(DWORD inKey )  const
	{
		return DB.IsHit(inKey);
	}

	/**
	 * Seek:					Nextキーの移動 (危険)
	 *
	 * @param inNext			設定したいNext値
	 */
	void Seek(int inNext)
	{
		Next = inNext;
	}


	/**
	 * Vacuum:					不要なデータを整理します	(Windowsでいえば デフラグ)
	 *
	 * @return void 
	 */
	void Vacuum( )
	{
		DB.Vacuum();
	}
private:	
	/**
	 * Close:			データベースのクローズ
	 *
	 * @return void 
	 */
	void RSequenceFileDB::Close();
	/**
	 * Open:					データベースのオープン
	 *
	 * @param inFilename		データベース名
	 * @param mode				fopenのモード
	 * @return void 
	 */
	void RSequenceFileDB::Open(const string& inFilename , const char* mode) throw(RIOCannotOpenException);

private:
	int			Next;
	RFileDB		DB;

	FILE*		SequenceFile;
};

#endif // !defined(AFX_RSEQUENCEFILEDB_H__74389509_8B23_4A60_A668_8B73AF9FFF67__INCLUDED_)
