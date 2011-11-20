#ifndef	___LIM3D_CLASSCARETAKER__H	//重複防止
#define ___LIM3D_CLASSCARETAKER__H

#include <list>
/*
//コレクターテンプレートクラス.
template<class _Caretation>
class TClassCaretaker
{
public:
	//コンストラクタ
	TClassCaretaker(){ m_IsClear = false;}		
	//デストラクタ
	~TClassCaretaker(){Clear();}
	//新規データ作成.
	_Caretation*	New()
	{
		//領域確保
		_Caretation*	puf = new _Caretation;
		//領域確保失敗の場合...
		if (puf == NULL) return NULL;	
		//確保した領域に親を教える.
		if (Mount(puf) == false)
		{
			delete puf;
			return NULL;
		}
		//リストに追加.
		m_CaretationList.push_back(puf);
		//確保した領域を返す.
		return puf;
	}
	//ガページコレクションより削除(ふつーは使わないでください.)
	bool DeleteCaretation(_Caretation* const Target)
	{
		//現在クリア中により、リストをロックしています.
		if (m_IsClear==true)	return true;

		std::list<_Caretation*>::iterator it = 	m_CaretationList.begin();
		std::list<_Caretation*>::iterator end= 	m_CaretationList.end();
		for( ; it != end ; it++)	
		{
			if (*it == Target)
			{	//発見.
				m_CaretationList.erase(it);
				return true;
			}
		}
		return false;
	}
	//クリア
	void Clear()
	{
		//現在クリア中により、リストをロックしています.
		m_IsClear = true;

		std::list<_Caretation*>::iterator it = 	m_CaretationList.begin();
		std::list<_Caretation*>::iterator end= 	m_CaretationList.end();
		for( ; it != end ; it++)	
		{
			(*it)->Release();
		}
		m_CaretationList.clear();
		//リストに対するロックの解除.
		m_IsClear = false;

	}
	//破棄
	bool Release()
	{
		delete this;
		return true;
	}
	//コレクションの個数を取得.
	int GetCount(){return m_CaretationList.size(); }
	//コレクションデータを取得開始.
	_Caretation*	FindFirst()
	{
		m_ListIT = m_CaretationList.begin();
		if (m_ListIT == m_CaretationList.end() )	return NULL;
		return *m_ListIT;
	}
	//コレクションデータを取得継続.
	_Caretation*	FindNext()
	{
		m_ListIT ++;
		if (m_ListIT == m_CaretationList.end() )	return NULL;
		return *m_ListIT;
	}
	//指定場所のデータを取得.
	_Caretation* At(int No)
	{
		std::list<_Caretation*>::iterator	theCaretaker = m_CaretationList.begin();
		std::list<_Caretation*>::iterator	theEnd       = m_CaretationList.end();
		for( ; theCaretaker != theEnd ; theCaretaker++ )
		{
			if (No == 0) return *theCaretaker;
			No--;
		}
		//見つからない...
		return NULL;
	}
	bool Alloc(int Many)
	{
		m_CaretationList.resize(Many);
		return true;
	}
protected:
	//作成したクラスに自分が親だと伝える.
	virtual	bool Mount(_Caretation* AllocData) = 0;

	std::list<_Caretation*>				m_CaretationList;	//ガページコレクションしている内容.
	std::list<_Caretation*>::iterator	m_ListIT;			//検索に使うイテレータ
	bool								m_IsClear;			//Clearの中だったら true
private:
};
*/
#endif				//重複防止