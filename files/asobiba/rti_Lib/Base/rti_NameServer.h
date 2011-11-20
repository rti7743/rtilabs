#include <string.h>

#define TSTRING_HASH_TABLE_MAX					127

class TStringHashTable
{

	class TStringHashData
	{
	public:
		TStringHashData()
		{
			//キーワード文字列
			m_KeyWord			= NULL;
			//キーワードの長さ
			m_KeyWordLen		= 0;
			//保持しているデータ
			m_Data				= NULL;;
			//このデータを参照している数.
			m_ReferenceCount	= 0;
			//シノニムレコードの場合のリスト.
			m_Next				= NULL;
		}
		~TStringHashData()
		{
			if (m_KeyWord) delete [] m_KeyWord;
		}
	public:
		//キーワードと一致するか  (一致 = true)
		bool	Cmp(const char* inChallengerString , int inChallengerStringLen )
		{
			return (inChallengerStringLen == m_KeyWordLen &&
					memcmp(inChallengerString , m_KeyWord , inChallengerStringLen) == 0);
		}
		//データと一致するか  (一致 = true)
		bool	Cmp(void* inChallengerData )
		{
			return (inChallengerData == m_Data);
		}
		//データの格納.
		void	Set(const char* inKeyWord , int inKeyWordLen , void* inData )
		{
			if (m_ReferenceCount == 0)
			{	//文字列確保.
				m_KeyWord = new char[inKeyWordLen];
				memcpy(m_KeyWord , inKeyWord , inKeyWordLen);
				m_KeyWordLen	=	inKeyWordLen;
				m_Data			=	inData;
			}
			m_ReferenceCount++;
		}
		//新たに共有できるレコードがあった.
		void	ReferenceCountUp(){ m_ReferenceCount++; }
		//保持しているデータの書き換え.
		void	ReNew(void* inData){ m_Data = inData;}
		//保持しているデータを返す.
		void*	GetData(){ return m_Data;}
		//保持しているデータを返す.
		void**	GetDataPointer(){ return &m_Data;}
		//データの削除.
		void	Release(TStringHashData** inNextPointer)
		{
			m_ReferenceCount--;
			if (m_ReferenceCount <= 0)
			{
				if (inNextPointer != NULL)
				{
					//一つ前のやつの Next の書き換え(危険なプログラムでごめんなさい).
					*inNextPointer = m_Next;
				}
				delete this;
			}
		}
		//これ以降のデータの問答無用で削除.
		void	ReleaseAll()
		{
			if ( m_Next != NULL ) m_Next->ReleaseAll();
			delete this;
		}
		//次の Next を取得.
		TStringHashData*	GetNext(){		return m_Next;	}
		TStringHashData**	GetNextPointer(){return &m_Next;}
	protected:
		//キーワード文字列
		char*	m_KeyWord;
		//キーワードの長さ
		int		m_KeyWordLen;
		//保持しているデータ
		void*	m_Data;
		//このデータを参照している数.
		int		m_ReferenceCount;
		//シノニムレコードの場合のリスト.
		TStringHashData*	m_Next;
	};


public:
	TStringHashTable();
	~TStringHashTable();
public:		//データ操作.

	//データを追加する.
	bool Add(const char * inKeyWord , void* inData , bool inForce=false);
	//データを検索する.
	void* Get(const char * inKeyWord );
	//データを検索する.
	void** GetPointer(const char * inKeyWord );
	//データを変更する
	bool Change(const char * inKeyWord , void* inData);
	//データを削除する
	bool Delete(const char * inKeyWord );
	//ハッシュテーブルの破棄
	bool Release();

protected:	//ハッシュ操作.
	//ハッシュした値を返す.
	unsigned int  GetDataPos(const char * inKeyWord , int inLen);
	
protected:
	TStringHashData*		m_HashTable[TSTRING_HASH_TABLE_MAX];
};
