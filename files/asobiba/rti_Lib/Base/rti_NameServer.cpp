#include <./Base/rti_NameServer.h>

TStringHashTable::TStringHashTable()
{
	memset( m_HashTable , 0 , sizeof(TStringHashData*) * TSTRING_HASH_TABLE_MAX );
}

TStringHashTable::~TStringHashTable()
{
	Release();
}


/*********************************************************/
//データを追加する.
//引数				inKeyWord			追加するキーワード
//					inData				追加するデータ
//					inForce				同じキーがあった場合はキーワード%d ではめ込む
//戻り値			成功 = true
/*********************************************************/
bool TStringHashTable::Add(const char * inKeyWord , void* inData , bool inForce/*=false*/)
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;
	TStringHashData**	theOldSHD;

	//ハッシュ値を求める.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//ハッシュテーブルより引き抜く.
	theSHD = m_HashTable[theDataPos];
	theOldSHD = &m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{//同じキーのデータはあった.
			if ( theSHD->Cmp(inData) )
			{//データも同じだ!!
				theSHD->ReferenceCountUp();
				return true;
			}
			return false;
		}
		theOldSHD = theSHD->GetNextPointer();
		theSHD = theSHD->GetNext() ;
	}
	//新規作成.
	theSHD		= new TStringHashData;
	theSHD->Set(inKeyWord , theLen , inData );

	*theOldSHD	= theSHD;

	return true;
}

/*********************************************************/
//データを検索する.
//引数				inKeyWord			検索するキーワード
//戻り値			検索したデータ	失敗=NULL
/*********************************************************/
void* TStringHashTable::Get(const char * inKeyWord )
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;

	//ハッシュ値を求める.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//ハッシュテーブルより引き抜く.
	theSHD = m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//このデータを返す.
			return theSHD->GetData();
		}
		theSHD = theSHD->GetNext() ;
	}
	return NULL;
}

/*********************************************************/
//データを検索する.
//引数				inKeyWord			検索するキーワード
//戻り値			検索したデータ	失敗=NULL
/*********************************************************/
void** TStringHashTable::GetPointer(const char * inKeyWord )
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;

	//ハッシュ値を求める.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//ハッシュテーブルより引き抜く.
	theSHD = m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//このデータを返す.
			return theSHD->GetDataPointer();
		}
		theSHD = theSHD->GetNext() ;
	}
	return NULL;
}

/*********************************************************/
//データを変更する
//引数				inKeyWord			更新するキーワード
//					inData				更新するデータ
//戻り値			成功 = true
/*********************************************************/
bool TStringHashTable::Change(const char * inKeyWord , void* inData)
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData*	theSHD;

	//ハッシュ値を求める.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//ハッシュテーブルより引き抜く.
	theSHD = m_HashTable[theDataPos];

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//このデータを書き換える.
			theSHD->ReNew(inData);
			return true;
		}
		theSHD = theSHD->GetNext() ;
	}
	return false;
}

/*********************************************************/
//データを削除する
//引数				inKeyWord			削除するキーワード
//戻り値			成功 = true
/*********************************************************/
bool TStringHashTable::Delete(const char * inKeyWord )
{
	int					theDataPos;
	int					theLen = strlen(inKeyWord);
	TStringHashData**	theOldSHD;
	TStringHashData*	theSHD;

	//ハッシュ値を求める.
	theDataPos = GetDataPos(inKeyWord , theLen);
	//ハッシュテーブルより引き抜く.
	theSHD = m_HashTable[theDataPos];
	theOldSHD = & ( m_HashTable[theDataPos] );

	while (theSHD != NULL)
	{
		if ( theSHD->Cmp(inKeyWord , theLen) )
		{	//このデータを消す.
			theSHD->Release(theOldSHD);
			return true;
		}
		theOldSHD = theSHD->GetNextPointer();
		theSHD = theSHD->GetNext() ;
	}
	return false;
}

/*********************************************************/
//ハッシュテーブルの破棄
//引数				なし.
//戻り値			成功 = true
/*********************************************************/
bool TStringHashTable::Release()
{
	TStringHashData*	theSHD;
	int					theI ;
	for( theI = 0 ; theI < TSTRING_HASH_TABLE_MAX ; theI++)
	{
		theSHD = m_HashTable[theI];
		if (theSHD != NULL)	theSHD->ReleaseAll();
	}

	memset( m_HashTable , 0 , sizeof(TStringHashData*) * TSTRING_HASH_TABLE_MAX );
	return true;
}

/*********************************************************/
//ハッシュした値を返す.
//引数				inKeyWord		ハッシュするキーワード
//					inLen			キーワードの長さ.
//戻り値			ハッシュ値.
/*********************************************************/
unsigned int  TStringHashTable::GetDataPos(const char * inKeyWord , int inLen)
{
	unsigned int		theHash = 0;
	char*	theKeyWordPoint = (char*)inKeyWord;

	int theI;
	for(theI = 0 ; theI < inLen ; theI++) theHash += *(theKeyWordPoint++);

	return theHash % TSTRING_HASH_TABLE_MAX;
}




