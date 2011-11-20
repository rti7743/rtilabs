// RFileDB.h: RFileDB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFILEDB_H__81ACA9D7_0B1A_47FF_9E10_C67F1A17D165__INCLUDED_)
#define AFX_RFILEDB_H__81ACA9D7_0B1A_47FF_9E10_C67F1A17D165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotOpenException.h"
#include "RNotFoundException.h"

//ファイルをデータベースとして使用するDBもどき.
//postgresql みたいなデータ管理をやります。
//多分、早いはずなんですが、誰かベンチとれ(w
class RFileDB
{
public:
	RFileDB();
	virtual ~RFileDB();

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
	void Select(DWORD inKey , RBuffer* outData) const throw(RNotFoundException);

	/**
	 * Insert:					挿入
	 *
	 * @param inKey				検索キー
	 * @param inData			挿入するデータ
	 * @return void 
	 */
	void Insert(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException);
	/**
	 * Update:					変更
	 *
	 * @param inKey				検索キー
	 * @param inData			変更するデータ
	 * @return void 
	 */
	void Update(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException);
	/**
	 * Delete:					削除
	 *
	 * @param inKey				検索キー
	 * @return void				
	 */
	void Delete(DWORD inKey ) throw(RNotFoundException);
	/**
	 * IsHit:					存在チェック
	 *
	 * @param inKey				検索キー
	 * @return bool 
	 */
	bool IsHit(DWORD inKey )  const;
	/**
	 * Upsert:					Update / Insert
	 *
	 * @param inKey				検索キー
	 * @return bool 
	 */
	bool Upsert(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
	{
		if ( IsHit(inKey) )
		{
			Update(inKey,inData);
		}
		else
		{
			Insert(inKey,inData);
		}
	}


	/**
	 * Vacuum:					不要なデータを整理します	(Windowsでいえば デフラグ)
	 *
	 * @return void 
	 */
	void Vacuum( );

	//テスト
	static void RFileDB::test();
private:
	//ストレステスト用データ
	struct StTestData
	{
		int pos;
		int str;
	};

	//引数の順にデーターベースに格納されているかどうかテストする   テスト用
	static void RFileDB::AssertFinder(const RFileDB * inDB , int inNumber ,...);
	//ストレステスト用のデータを作成.
	static void RFileDB::StressTestMakeRandomData(StTestData * outData , const int inTestCount , const int inTestDataCount);


private:
	/**
	 * Close:			データベースのクローズ
	 *
	 * @return void 
	 */
	void Close() throw(RIOCannotOpenException);
	/**
	 * InsertIndex:		index に挿入
	 *
	 * @param inKey		検索キー
	 * @param inPos		挿入場所
	 * @param inSize	データのサイズ
	 * @return void 
	 */
	void InsertIndex(DWORD inKey , DWORD inPos, DWORD inSize);
	/**
	 * FindIndex:		index より検索
	 *
	 * @param inKey		検索キー
	 * @param outPos	データの場所
	 * @param outSize	データのサイズ
	 * @return bool		見つかった場合　true 
	 */
	bool FindIndex(DWORD inKey , DWORD * outPos , DWORD * outSize) const;
	/**
	 * UpdateIndex:		index の更新
	 *
	 * @param inKey		検索キー
	 * @param inPos		データの場所
	 * @param inSize	データサイズ
	 * @return void 
	 */
	void UpdateIndex(DWORD inKey , DWORD inPos, DWORD inSize);
	/**
	 * DeleteIndex:		index の削除
	 *
	 * @param inKey		検索キー
	 * @return void 
	 */
	void DeleteIndex(DWORD inKey );

	/**
	 * Open:					データベースのオープン
	 *
	 * @param inFilename		データベース名
	 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
	 * @param mode				fopenのモード
	 * @return void 
	 */
	void Open(const string& inFilename , int inHashTableSize , const char* mode);

	/**
	 * CheckFreeSize:			現在の場所よりどれだけのフリースペースを確保できるか調べます
	 *
	 * @param inStart			開始する場所
	 * @return DWORD			確保できるサイズ
	 */
	DWORD CheckFreeSize(DWORD inStart);

	//INDEX
	typedef pair<DWORD,DWORD> OneParts;
	typedef map<DWORD , OneParts>	SecondIndexDef;
	SecondIndexDef*		Index;

	//ハッシュサイズ.
	DWORD				HashSize;

	//INDEXを格納するファイル
	FILE*		IndexFile;
	//格納するファイル.
	FILE*		File;
};

#endif // !defined(AFX_RFILEDB_H__81ACA9D7_0B1A_47FF_9E10_C67F1A17D165__INCLUDED_)
