// RFileDB.cpp: RFileDB クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RFileDB.h"

const DWORD CELL_FLG_SIZE = sizeof(DWORD) * 2;
const int CELL_ALIVE_FLG	=	0x00000000;
const int CELL_DELETE_FLG	=	0x00000001;

#define IS_FLG_SET(flg,d)	(flg & d)
#define FLG_SET(flg,d)		(flg | d)
#define FLG_RESET(flg,d)	(flg & (~d))

/*
データ構造:
	+0	+1	+2	+3	+4	+5	+6	+7	+8	+9	+10	+11
	(DWORD)			(DWORD)
	0	0	0	0	0	0	0	4	a	b	c	d
	|----flg----|	|--length---|	|---data----|

お約束:
	1.flg が CELL_DELETE_FLG の時、そのデータは削除されたものとして扱う。
	2.length の数だけデータは続く。

FAQ:
  Q:flg が 4バイトなのは無駄なのでは?
  A:そうですね。 まぁ、手抜きです。

*/	

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RFileDB::RFileDB()
{
	File = NULL;
	IndexFile = NULL;
	Index = NULL;
}

RFileDB::~RFileDB()
{
	Close();
}

/**
 * Close:			データベースのクローズ
 *
 * @return void 
 */
void RFileDB::Close()
{
	if (File)
	{
		fclose(File);
		File = NULL;
	}
	if (IndexFile)
	{
		fclose(IndexFile);
		IndexFile = NULL;
	}
	delete [] Index;
	Index = NULL;
}

/**
 * Create:					新規作成
 *
 * @param inFilename		ファイル名	あれば上書き
 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
 * @return void 
 */
void RFileDB::Create(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename , inHashTableSize , "w+b");
}

/**
 * Load:					読み込み
 *
 * @param inFilename		ファイル名
 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
 * @return void 
 */
void RFileDB::Load(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename , inHashTableSize , "r+b");

	//INDEXの読み取り.
	DWORD buffer[3];
	while( !feof(IndexFile) )
	{
		fread( buffer , sizeof(DWORD) , 3 , IndexFile);
		InsertIndex(buffer[0] , buffer[1], buffer[2]  );
	}
}

/**
 * InsertIndex:		index に挿入
 *
 * @param inKey		検索キー
 * @param inPos		挿入場所
 * @param inSize	データのサイズ
 * @return void 
 */
void RFileDB::InsertIndex(DWORD inKey , DWORD inPos , DWORD inSize)
{
	const DWORD key = inKey % HashSize;
	(Index[key])[inKey] = OneParts(inPos, inSize);
}

/**
 * FindIndex:		index より検索
 *
 * @param inKey		検索キー
 * @param outPos	データの場所
 * @param outSize	データのサイズ
 * @return bool		見つかった場合　true 
 */
bool RFileDB::FindIndex(DWORD inKey , DWORD * outPos , DWORD * outSize) const
{
	const DWORD key = inKey % HashSize;
	SecondIndexDef * sd = & Index[key];
	SecondIndexDef::const_iterator i = sd->find(inKey);
	if ( i == sd->end() ) return false;

	*outPos = ((*i).second).first;
	*outSize = ((*i).second).second;
	return true;
}

/**
 * UpdateIndex:		index の更新
 *
 * @param inKey		検索キー
 * @param inPos		データの場所
 * @param inSize	データサイズ
 * @return void 
 */
void RFileDB::UpdateIndex(DWORD inKey , DWORD inPos, DWORD inSize)
{
	InsertIndex(inKey , inPos , inSize);
}

/**
 * DeleteIndex:		index の削除
 *
 * @param inKey		検索キー
 * @return void 
 */
void RFileDB::DeleteIndex(DWORD inKey )
{
	const DWORD key = inKey % HashSize;
	SecondIndexDef * sd = & Index[key];
	sd->erase(inKey);
}

/**
 * Open:					データベースのオープン
 *
 * @param inFilename		データベース名
 * @param inHashTableSize	ハッシュテーブルの大きさ 素数推奨
 * @param mode				fopenのモード
 * @return void 
 */
void RFileDB::Open(const string& inFilename , int inHashTableSize , const char* mode) throw(RIOCannotOpenException)
{
	Close();

	File = fopen(inFilename.c_str() , mode);
	if (File == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + " データベース" + inFilename + "が開けません");
	}
	const string index = inFilename + ".idx";
	IndexFile = fopen(index.c_str() , mode);
	if (IndexFile == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + " データベース" + inFilename + "のインデックス" + index + "が開けません");
	}

	Index = new SecondIndexDef[inHashTableSize];

	HashSize = inHashTableSize;
}
/**
 * Save:					ディスクに index を書き込みます。 indexは OnMemory なのでセーブしないと無意味です
 *
 * @return void 
 */
void RFileDB::Save() 
{
	DWORD writeBuffer[3];

	fseek( IndexFile , 0 , SEEK_SET ) ;
	for(int i = 0 ; i < HashSize ; i++)
	{
		SecondIndexDef* sid = &Index[i];

		SecondIndexDef::iterator end = sid->end() ;
		for(SecondIndexDef::const_iterator i = sid->begin() ; i != end ; ++i)
		{
			//検索キー
			writeBuffer[0] = ((*i).first);
			//データの場所
			writeBuffer[1] = ((*i).second).first;
			//データの長さ
			writeBuffer[2] = ((*i).second).second;

			fwrite(writeBuffer , sizeof(DWORD) , 3 , IndexFile);
		}
	}
}

/**
 * Flush:					データを flush します。
 *
 * @return void 
 */
void RFileDB::Flush()
{
	fflush(File);
	fflush(IndexFile);
}

/**
 * Select:					検索
 *
 * @param inKey				検索キー
 * @param outData			データを書き込むバッファ
 * @return void 
 */
void RFileDB::Select(DWORD inKey , RBuffer * outData) const throw(RNotFoundException)
{
	DWORD	seekPos,size;
	if( !FindIndex(inKey, &seekPos , &size ) )	throw RNotFoundException("見つかりません");

	if ( fseek(File , seekPos+CELL_FLG_SIZE , SEEK_SET ) != 0) throw RNotFoundException("シークに失敗しました");

	outData->reserve(size);

	//vector は連続的だから代入できるはず。 そう信じたい
	//証拠
	//	http://anubis.dkuug.dk/JTC1/SC22/WG21/docs/lwg-defects.html#69
	//	http://www.tietew.jp/cppll/archive/4822
	fread( &((*outData)[0]) , sizeof(char) , size , File);
}

/**
 * Insert:					挿入
 *
 * @param inKey				検索キー
 * @param inData			挿入するデータ
 * @return void 
 */
void RFileDB::Insert(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
{
	DWORD seekPos , size;
	if( FindIndex(inKey , &seekPos , &size) )throw RNotFoundException("すでに値があります");

	if ( fseek(File , 0 , SEEK_END ) != 0) throw RNotFoundException("シークに失敗しました");

	InsertIndex( inKey , ftell(File) , inData.size());

	DWORD flg[2];	//control , size
	flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
	flg[1] = inData.size();
	fwrite( flg , sizeof(DWORD) , 2 , File);
	fwrite( &(inData[0]) , sizeof(char) , flg[1] , File);
}

/**
 * Update:					変更
 *
 * @param inKey				検索キー
 * @param inData			変更するデータ
 * @return void 
 */
void RFileDB::Update(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
{
	DWORD	seekPos,size;
	if( !FindIndex(inKey, &seekPos , &size ) )	throw RNotFoundException("見つかりません");

	if (inData.size() == (size) )
	{	//もともとの大きさといっしょ
		if ( fseek(File , seekPos+CELL_FLG_SIZE , SEEK_SET ) != 0) throw RNotFoundException("シークに失敗しました");
		fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);
	}
	else
	{
		DWORD flg[2];
		if ( fseek(File , seekPos , SEEK_SET ) != 0) throw RNotFoundException("シークに失敗しました");

		if ( (inData.size()+CELL_FLG_SIZE) < size )
		{	//もともとの大きさより小さくなる セルを分割できる.
			flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
			flg[1] = inData.size();

			fwrite( flg , sizeof(DWORD) , 2 , File);
			fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);

			//残りのセルは削除されたと報告.
			flg[0] = FLG_SET(0,CELL_DELETE_FLG);
			flg[1] = size - inData.size() - CELL_FLG_SIZE;
			ASSERT(flg[1] >= 0)
			fwrite( flg , sizeof(DWORD) , 2 , File);
		}
		else
		{	//もともとのサイズより大きくなる.
			DWORD free = CheckFreeSize(seekPos + size) + size;
			if (free >= inData.size() + CELL_FLG_SIZE)
			{//空き領域に格納できる.
				flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
				flg[1] = inData.size();
				fwrite( flg , sizeof(DWORD) , 2 , File);
				fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);

				//空き領域は分断する.
				flg[0] = FLG_SET(0,CELL_DELETE_FLG);
				flg[1] = free - inData.size();
				fwrite( flg , sizeof(DWORD) , 2 , File);
			}
			else
			{//どうしようない...
				//現在の領域には収まりきれないので、
				//現在の領域を破棄してファイルの最後に追加します.
				flg[0] = FLG_SET(0,CELL_DELETE_FLG);
				flg[1] = size;
				fwrite( flg , sizeof(DWORD) , 2 , File);

				fseek(File , 0 , SEEK_END );	//ファイルの最後に

				//インデックス値の更新.
				UpdateIndex(inKey , ftell(File) , inData.size());

				flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
				flg[1] = inData.size();
				fwrite( flg , sizeof(DWORD) , 2 , File);
				fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);

			}
		}
	}
}

/**
 * Delete:					削除
 *
 * @param inKey				検索キー
 * @return void				
 */
void RFileDB::Delete(DWORD inKey ) throw(RNotFoundException)
{
	DWORD	seekPos,size;
	if( !FindIndex(inKey, &seekPos , &size ) )	throw RNotFoundException("見つかりません");

	if ( fseek(File , seekPos , SEEK_SET ) != 0) throw RNotFoundException("シークに失敗しました");

	DeleteIndex( inKey  );

	DWORD flg;
	flg = FLG_SET(0,CELL_DELETE_FLG);
	fwrite( &flg , sizeof(DWORD) , 1 , File);
}

/**
 * IsHit:					存在チェック
 *
 * @param inKey				検索キー
 * @return bool 
 */
bool RFileDB::IsHit(DWORD inKey ) const
{
	DWORD	seekPos,size;
	return FindIndex(inKey, &seekPos , &size );
}


/**
 * Vacuum:					不要なデータを整理します	(Windowsでいえば デフラグ)
 *
 * @return void 
 */
void RFileDB::Vacuum( )
{
}


/**
 * CheckFreeSize:			現在の場所よりどれだけのフリースペースを確保できるか調べます
 *
 * @param inStart			開始する場所
 * @return DWORD			確保できるサイズ
 */
DWORD RFileDB::CheckFreeSize(DWORD inStart)
{
	DWORD free = 0;
	DWORD nextPoint = inStart;

	DWORD flg[2];	//control , size

	while( !feof(File) )
	{
		fseek(File ,nextPoint , SEEK_SET );	//次のセルに移動
		if ( !fread( flg , sizeof(DWORD) , 2 , File) )		//次のセルの情報を取得.
		{
			return free;
		}

		if ( !IS_FLG_SET(flg[0],CELL_DELETE_FLG) ) return free;

		free += flg[1];
		nextPoint += (flg[1] + CELL_FLG_SIZE);
	}
	return free;
}

void RFileDB::test()
{
	{
		RFileDB		db;
		db.Create("testdata/rfdbtest1.dat" , 13);

		//とりあえず 追加できるか?
		{
			RBuffer buffer;
			buffer = "test10";
			db.Insert(10,buffer);
			buffer = "test20";
			db.Insert(20,buffer);
			buffer = "test30";
			db.Insert(30,buffer);
			buffer = "test40";
			db.Insert(40,buffer);
			buffer = "test50";
			db.Insert(50,buffer);

			db.Select(10,&buffer);
			ASSERT( buffer == "test10" ); 
			db.Select(20,&buffer);
			ASSERT( buffer == "test20" ); 
			db.Select(30,&buffer);
			ASSERT( buffer == "test30" ); 
			db.Select(40,&buffer);
			ASSERT( buffer == "test40" ); 
			db.Select(50,&buffer);
			ASSERT( buffer == "test50" ); 
		}
		//削除できるか?
		{
			RBuffer buffer;
			db.Delete(10);
			db.Delete(30);
			db.Delete(50);

			ASSERT( db.IsHit(10) == false);
			ASSERT( db.IsHit(20) == true);
			ASSERT( db.IsHit(30) == false);
			ASSERT( db.IsHit(40) == true);
			ASSERT( db.IsHit(50) == false);

		}
		//変更できるか?
		{
			RBuffer buffer;

			//同じ大きさ
			buffer = "test20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "test20");

			//小さい
			buffer = "t20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "t20");

			//すこし大きい
			buffer = "test!!20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "test!!20");

			//とても大きい
			buffer = "teeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeest20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "teeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeest20");

			//小さい
			buffer = "t20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "t20");
		
			//同じ大きさ
			buffer = "test20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "test20");
		}
	}
	{
		RFileDB		db;
		RBuffer buffer;
		RBuffer t;
		db.Create("testdata/rfdbtest2.dat" , 13);

		buffer = "あにくん";
		db.Insert(3 , buffer);
		buffer = "あにぃ";
		db.Insert(2 , buffer);
		buffer = "にぃさま";
		db.Insert(0 , buffer);
		buffer = "おにぃちゃま";
		db.Insert(1 , buffer);

		buffer = "あにちゃま";
		db.Update(1 , buffer);
		buffer = "にぃさま";
		db.Update(0 , buffer);
		buffer = "あにき";
		db.Update(2 , buffer);
		buffer = "あにうえさま";
		db.Update(3 , buffer);

		t = "にぃさま";
		db.Select(0 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);

		t = "あにちゃま";
		db.Select(1 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);

		t = "あにき";
		db.Select(2 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);

		t = "あにうえさま";
		db.Select(3 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);
	}
	{
		RFileDB		db;
		RBuffer buffer;
		RBuffer t;
		db.Create("testdata/rfdbtest2.dat" , 13);

		buffer = "あにちゃま";
		db.Insert(9 , buffer);
		buffer = "あにぎみさま";
		db.Insert(0 , buffer);
		buffer = "あにちゃま";
		db.Insert(1 , buffer);
		buffer = "あにき";
		db.Insert(6 , buffer);
		buffer = "おにぃたま";
		db.Insert(2 , buffer);
		buffer = "にいや";
		db.Insert(3 , buffer);
		buffer = "あにちゃま";
		db.Insert(7 , buffer);
		buffer = "おにぃちゃん";
		db.Insert(4 , buffer);
		buffer = "あにちゃま";
		db.Insert(5 , buffer);
		buffer = "おにぃちゃん";
		db.Insert(8 , buffer);

		AssertFinder(&db ,10,  "あにぎみさま" , "あにちゃま" , "おにぃたま", "にいや", "おにぃちゃん", 
							   "あにちゃま", "あにき", "あにちゃま", "おにぃちゃん", "あにちゃま");
		buffer = "おにぃちゃま";
		db.Update(6 , buffer);
		AssertFinder(&db ,10,  "あにぎみさま" , "あにちゃま" , "おにぃたま", "にいや", "おにぃちゃん", 
							   "あにちゃま", "おにぃちゃま", "あにちゃま", "おにぃちゃん", "あにちゃま");

		buffer = "あにくん";
		db.Update(4 , buffer);
		AssertFinder(&db ,10,  "あにぎみさま" , "あにちゃま" , "おにぃたま", "にいや", "あにくん", 
							   "あにちゃま", "おにぃちゃま", "あにちゃま", "おにぃちゃん", "あにちゃま");

		buffer = "にぃさま";
		db.Update(0 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにちゃま" , "おにぃたま", "にいや", "あにくん", 
							   "あにちゃま", "おにぃちゃま", "あにちゃま", "おにぃちゃん", "あにちゃま");

		buffer = "あにき";
		db.Update(1 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにき" , "おにぃたま", "にいや", "あにくん", 
							   "あにちゃま", "おにぃちゃま", "あにちゃま", "おにぃちゃん", "あにちゃま");

		buffer = "おにぃちゃん";
		db.Update(7 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにき" , "おにぃたま", "にいや", "あにくん", 
							   "あにちゃま", "おにぃちゃま", "おにぃちゃん", "おにぃちゃん", "あにちゃま");

		buffer = "おにぃちゃま";
		db.Update(2 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにき" , "おにぃちゃま", "にいや", "あにくん", 
							   "あにちゃま", "おにぃちゃま", "おにぃちゃん", "おにぃちゃん", "あにちゃま");

		buffer = "あにぃ";
		db.Update(3 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにき" , "おにぃちゃま", "あにぃ", "あにくん", 
							   "あにちゃま", "おにぃちゃま", "おにぃちゃん", "おにぃちゃん", "あにちゃま");

		buffer = "あにき";
		db.Update(5 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにき" , "おにぃちゃま", "あにぃ", "あにくん", 
							   "あにき", "おにぃちゃま", "おにぃちゃん", "おにぃちゃん", "あにちゃま");

		buffer = "あにき";
		db.Update(8 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにき" , "おにぃちゃま", "あにぃ", "あにくん", 
							   "あにき", "おにぃちゃま", "おにぃちゃん", "あにき", "あにちゃま");

		buffer = "あにぃ";
		db.Update(9 , buffer);
		AssertFinder(&db ,10,  "にぃさま" , "あにき" , "おにぃちゃま", "あにぃ", "あにくん", 
							   "あにき", "おにぃちゃま", "おにぃちゃん", "あにき", "あにぃ");
	}
	//ストレス
	{
		printf("ストレステスト準備中 \r\n");
		RFileDB		db;
		db.Create("testdata/rfdbtest10.dat" , 13);

		
		RBuffer buffer;

//		const int testCount = 1000000;
		const int testCount = 100;

		//追加するデータ
		string callme[12];
		callme[0] = "おにぃちゃん";
		callme[1] = "おにぃさま";
		callme[2] = "にぃさま";
		callme[3] = "あにき";
		callme[4] = "おにぃちゃま";
		callme[5] = "あにぃ";
		callme[6] = "おにぃたま";
		callme[7] = "あにうえさま";
		callme[8] = "あにくん";
		callme[9] = "あにぎみさま";
		callme[10] = "あにちゃま";
		callme[11] = "にいや";
/*

		//追加するデータ
		string callme[12];
		callme[0] = "可憐 －かれん－ お兄ちゃん、大好き可憐の大好きなお兄ちゃん、お元気ですか？可憐は最近お兄ちゃんに会えない日が続いていて、とってもとっても淋しい気持ちでいっぱいです。可憐、本当はいつだってお兄ちゃんと一緒にいたいのに、ずぅっとお兄ちゃんに会えないでいるから……。だから、ピアノのお稽古でも、毎日悲しい曲ばかり弾いてしまうの。今日はお兄ちゃんと早く会えるように、マリア様にお祈りして寝ます。きっと今夜はお兄ちゃんの夢を見れるかな……。お兄ちゃん、大好きＰｒｏｆｉｌｅ●誕生日 ９月２３日●星座 乙女座●身長 １４８cmココがポイント可憐ちゃんはとにかくお兄ちゃんのことが大好き！　とってもお兄ちゃんに憧れていて、いつでも自分のそばにいてくれたらいいのにと思ってる、ちょっぴり甘えん坊で健気な女の子です。";
		callme[1] = "花穂 －かほ－ 花穂はいつだってお兄ちゃまを応援します！お兄ちゃま、お元気ですか？花穂はチアの練習を毎日一生懸命がんばってます！　早くお兄ちゃまの応援をしたくって、もう待ちきれないくらい!!　お兄ちゃまに「花穂のおかげでがんばれたよ」って言ってもらうのが、花穂のちっちゃい頃からの夢なんだぁ……　でも、それにはやっぱり毎日の練習をがんばらなくっちゃね。お兄ちゃま、花穂の応援楽しみにしててね！Ｐｒｏｆｉｌｅ●誕生日 １月７日●星座 山羊座●身長 １４３cmココがポイント花穂ちゃんはお兄ちゃんのことを応援するためにチアリーディングをやっている、とってもお兄ちゃん想いの妹。花を育てるのが好きで、意外にちょっとドジっぽいところもあるみたいです。";
		callme[2] = "衛 －まもる－ ボクと一緒に遊ぼうよ。ハァイ、あにぃ！　……やぁっと、起きたなぁ～？　もう、ずっと待ってたんだぞ！　あにぃは、まったくねぼすけなんだからなぁ……。……えへへ、今まで逃げられてばっかりだったけど、今日こそはつかまえたからね！　絶対にまるまる１日、ボクに付き合ってもらうんだからっ　……あにぃもさ、もっとふだんからいっぱいスポーツした方がイイと思うよ。体を動かすのってもうとにかく楽しいし、それに……そしたら、いろんなスポーツを２人一緒にできるじゃない？Ｐｒｏｆｉｌｅ●誕生日 １０月１８日●星座 天秤座●身長 １５０cmココがポイント衛ちゃんはスポーツ大好きの元気な妹。いつもいろんなスポーツをお兄ちゃんと一緒にやりたくてウズウズしています。なかでも得意なのはボード系で、ファッションもそれ風が多いみたい。";
		callme[3] = "咲耶 －さくや－ お兄様は絶対に私のモノなんだから。お兄様へ。この間お兄様がはいてたブーツ、スゴクかっこよかったから、おそろいの買おうと思ってお店に行ったら……もう売り切れてたの。なんだか運命が私とお兄様を引き離そうとしてるみたいに思えて、ちょっと泣けてしまったわ……。私、最近感傷的になっているのかしら？　そりゃあ、私は妹だけど……私くらいお兄様のことを愛してる女の子は、この世に1人もいないと思うの！　たとえ兄妹だって、お兄様と私は絶対に結ばれる運命にある……わよね？Ｐｒｏｆｉｌｅ●誕生日 １２月２０日●星座 射手座●身長 １５９cmココがポイント咲耶ちゃんはいつもおしゃれで自信たっぷり！　彼女のメッセージには、兄弟という関係を超えて、１人の女性としてお兄ちゃんに見てもらいたいという気持ちがあふれています。";
		callme[4] = "雛子 －ひなこ－ ヒナはおにいたまがいないと　サビシイサビシイ病になっちゃうよ！おにいたまへ。おにいたまが帰っちゃって、ヒナはサビシイサビシイ病にかかっちゃいました。毎日おにいたまがいないー、と思ったら、ちょっぴり涙が出ちゃうんだよ。あ、でも、スグごしごししたから、ママには見つからなかったよ。ヒナ、えらい？　……だから、早くまたヒナのトコロに会いに来てくーださい。すっごい、すっごい待ってます！　ヒナより。Ｐｒｏｆｉｌｅ●誕生日 ８月１５日●星座 獅子座●身長 １３２cmココがポイント雛子ちゃんは１２人の妹達の中で１番のおチビちゃん。淋しがりやで、いつもお兄ちゃんが一緒にいて遊んでくれることを望んでいます。子供だけに愛情表現の仕方がダイレクトなところも……。";
		callme[5] = "鞠絵 －まりえ－ 早く兄上様とお会いしたい……。親愛なる兄上様……。しばらくお目にかかれていませんけれど、お元気でいらっしゃいますか？　私も最近は少し調子がいいみたいです。あ、病院で兄上様にセーターを編んでみました。セーターを編むのは初めてだったから、あまり上手じゃないみたいなんですけれど……。兄上様が着てくださるとうれしいです。……私の想いがセーターの編み目を通じて伝わりますように……Ｐｒｏｆｉｌｅ●誕生日 ４月４日●星座 牡羊座●身長 １４８cmココがポイント鞠絵ちゃんは病弱で、おしとやかな遠慮深い女の子。お兄ちゃんのことが大好きなのに、体が弱くてお兄ちゃんと思うように会えないのをいつも気に病んでいるところがいじらしいのです。";
		callme[6] = "白雪 －しらゆき－ にいさま、たっぷり召し上がれ！ハーイ！　にいさま。最近はごはんちゃんと食べてますの？姫の作った料理じゃないと……、って言ってくださるのはウレシイけど、にいさまが痩せてしまったら、姫も悲しいわ。あぁん、ホントならいっつも姫がそばにいてお料理してあげられたらいいんですのに。……うふふ　いっそ姫をにいさまのお嫁さんにしたらどうかしら？　……きゃっＰｒｏｆｉｌｅ●誕生日 ２月１１日●星座 水瓶座●身長 １４０cmココがポイント白雪ちゃんはお料理が大好きで、お兄ちゃんにごはんを作ってあげるのが生きがいです。彼女の料理は多分に創造的で驚かされることも多いけど、そこがまた彼女の魅力だったりするんですね。";
		callme[7] = "鈴凛 －りんりん－ アニキのあったかーい援助、待ってるよハイ、アニキ！　そろそろまた、パソコンのニューモデルが出そろうシーズンだけど、もうアニキのターゲットは決まった？　私はねぇ……やっぱり、小型カメラ付きのモバイルノートを……っていうのはウソで、いまはメカ鈴凛の改造真っ最中だから、当分新しいマシンはいらないよ　ふふっ……アニキったら、安心した？　今度のグレードアップが完了したら、いよいよメカ鈴凛も最終形態！　……できあがったら、私の代わりにアニキの世話をさせようかなＰｒｏｆｉｌｅ●誕生日 ７月９日●星座 蟹座●身長 １５２cmココがポイント鈴凛ちゃんはメカニックな趣味を持つ電脳系の女の子。趣味にお金がかかりすぎるため、いつもお兄ちゃんに救いの手を求めています。最近１番の大作は自分ソックリのメカ鈴凛（現在製作中）。";
		callme[8] = "千影 －ちかげ－ …………やあ、兄くん…………。…………やあ、兄くん。………………元気そうだね。…………じつはこの間、ステキな猫のミイラを手に入れたんだが、よかったら兄くんも見にこないか？　…………とてもよく乾燥していて…………状態もじつに完璧なんだ。まったくすばらしいものだよ…………。…………いつかは私も、兄くんのミイラを作ってみたいな…………。…………愛するものの姿を永遠に保存しておきたい気持ちは…………古代も現代も変わりがないんだ…………ね、兄くん…………。Ｐｒｏｆｉｌｅ●誕生日 ３月６日●星座 魚座●身長 １５７cmココがポイントクールでミステリアスなムードの千影ちゃんは、見た目どおりカルトや魔術が好きな女の子。何を考えているのかよくわからないことが多いけど、それでもお兄ちゃんのことは好きみたい（？）";
		callme[9] = "春歌 －はるか－ せいいっぱい兄君さまをお守りいたしますわっようやく、お会いできました……ワタクシの兄君さま　ワタクシ、とってもうれしいです！　長い間離れ離れになっていましたけれど、やっとおそばでお仕えできる日が来ましたわっ。兄君さまにご満足いただける良妻賢母になれるよう、お茶もお華も唄も踊りもしっかりお稽古して来ましたの。あっ、それと武道も得意です……これからは何があっても　ワタクシが兄君さまをお守りいたしますわっ！Ｐｒｏｆｉｌｅ●誕生日 ５月１６日●星座 牡牛座●身長 １５６cmココがポイント春歌ちゃんはステキな大和撫子になってお兄ちゃんにお仕えしたいと思ってる女の子。着物姿もその願望のあらわれみたいなんだけど、わりに夢見がちな性格みたいで暴走ギミな点もチラリ？";
		callme[10] = "四葉 －よつば－ 兄チャマの秘密は　四葉がみーんなチェキしちゃうんデス！きゃ～、兄チャマ、見～つけたっ！　うふふっ、ついに兄チャマ見つけちゃった　四葉はね、兄チャマに会うためにはるばるイギリスからやって来たのよ！　名探偵の四葉にかかったら兄チャマもイチコロね。兄チャマ、四葉が来たからにはこれから隠し事なんかしてもぜんっぜんムダよ　兄チャマの秘密は、四葉がみ～んな暴いちゃう！四葉のカンペキ秘密調査で兄チャマのハートをチェキチェキチェキよっ!!Ｐｒｏｆｉｌｅ●誕生日 ６月２１日●星座 双子座●身長 １４９cmココがポイント名探偵を自称する四葉ちゃんは、お兄ちゃんマニアの女の子。はるかイギリスから、お兄ちゃんの秘密を発見するためにやって来ました。かなりトンでる性格の様子で先が思いやられます……。";
		callme[11] = "亞里亞 －ありあ－ アイス落としちゃったの……くすん。亞里亞は……フランスから来ました。あの……ママンが……ね、亞里亞が日本に帰ったらステキな兄やがいて亞里亞にいろんなことを教えてくれる……って言ってたの。兄や……それ本当？　ママンはじいやのこともそう言っていたけど、じいやはとっても怖くて亞里亞のこと……きっとキライだったの。……くすん。……でも、兄やはとても優しそうね　兄やは……亞里亞のこと、しかったりしない？Ｐｒｏｆｉｌｅ●誕生日 １１月２日●星座 蠍座●身長 １３９cmココがポイント亞里亞ちゃんはフランスから海を渡ってやって来た妹。臆病な性格で、困ったことになるとスグに泣き出してしまうらしいですが、優しいお兄ちゃんのことだけは大好きみたいです。";
*/
		StTestData *InsertMap = new StTestData[testCount];
		StTestData *UpdateMap = new StTestData[testCount];
		//ストレステスト用のデータを作成.
		StressTestMakeRandomData( InsertMap ,testCount , 12 );
		StressTestMakeRandomData( UpdateMap ,testCount , 12 );

		printf("ストレステスト開始 \r\n");
		time_t startTime = time(NULL);
		time_t stepStartTime = time(NULL);
		int i;

		for(i = 0; i < testCount ; i++)
		{
			db.Insert(InsertMap[i].pos , callme[ InsertMap[i].str ] );
		}

		printf("Step経過時間 : %d 秒\r\n", time(NULL) - stepStartTime );
		stepStartTime = time(NULL);
		//正常に追加されたのを確認.
		for(i = 0; i < testCount ; i++)
		{
			db.Select(InsertMap[i].pos , &buffer);
			ASSERT( memcmp(buffer.c_str() , callme[ InsertMap[i].str ].c_str() , callme[ InsertMap[i].str ].size() ) == 0);
		}

		printf("Step経過時間 : %d 秒\r\n", time(NULL) - stepStartTime );
		stepStartTime = time(NULL);

		for(i = 0; i < testCount ; i++)
		{
			db.Update(UpdateMap[i].pos , callme[ UpdateMap[i].str ] );
		}

		printf("Step経過時間 : %d 秒\r\n", time(NULL) - stepStartTime );
		stepStartTime = time(NULL);
		//正常に追加されたのを確認.
		for(i = 0; i < testCount ; i++)
		{
			db.Select(UpdateMap[i].pos , &buffer);
			ASSERT( memcmp(buffer.c_str() , callme[ UpdateMap[i].str ].c_str() , callme[ UpdateMap[i].str ].size() ) == 0);
		}
		printf("Step経過時間 : %d 秒\r\n", time(NULL) - stepStartTime );
		printf("経過時間 : %d 秒\r\n", time(NULL) - startTime );

		delete [] UpdateMap;
		delete [] InsertMap;
	}
}

//引数の順にデーターベースに格納されているかどうかテストする   テスト用
void RFileDB::AssertFinder(const RFileDB * inDB , int inNumber ,...)
{
	va_list ap;
	va_start(ap, inNumber);

	RBuffer buffer;
	va_start(ap, inNumber);

	string str;
	for(int i = 0; i < inNumber ; i++ )
	{
		str = va_arg( ap, const char*);
		inDB->Select( i , &buffer);

		ASSERT( memcmp(buffer.c_str() , str.c_str() , str.size() ) == 0);
	}
	va_end(ap);
}


//ストレステスト用のデータを作成.
void RFileDB::StressTestMakeRandomData(StTestData * outData , const int inTestCount , const int inTestDataCount)
{
	srand((unsigned)time(NULL));

	//ユニークになるように.
	char * flg = new char[inTestCount];
	memset(flg , 0 , sizeof(char) * inTestCount);

	int lastUniq = 0;
	//ランダムに追加
	for(int i = 0; i < inTestCount ; i++)
	{
		int r = rand() % inTestCount;

		if ( flg[r] != 0)
		{	//あいていないなら空きを探す
			for(int l = lastUniq ; l < inTestCount ; l ++)
			{
				if ( flg[l] == 0)
				{
					r = l;
					lastUniq = l;
					break;
				}
			}
		}
		flg[r] = 1;
		outData[i].pos = r;
		outData[i].str = rand() % inTestDataCount ;
	}

	delete [] flg;
}
