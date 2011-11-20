#ifndef	___CARETAKER__H	//重複防止
#define ___CARETAKER__H

/**********************************************************/
//コレクションテンプレートクラス (C)Copylight R.T.I since 2000...
/**********************************************************/
#include <windows.h>
#include <./Base/rti_debug.h>
#include <./rti_define.h>

//コレクターテンプレートクラス.
template<class _Caretation , class InputData = _Caretation >
class _TCaretaker
{
public:
	/*********************/
	//メソッド
	/*********************/
	//コンストラタ
	_TCaretaker()
	{
		m_Caretation = NULL;
		m_CaretationNubmer = 0;
		m_CaretationRealNumber = 0;
	}

	//デストラクタ
	~_TCaretaker()	{		Clear();	}

	//新規にデータをセットする.
	bool			Set(InputData* _c ,			
						int Many , bool SizeNotChange = true)
	{
		//件数がマイナスは没.
		if (Many <= 0) return false;

		//領域の確保
		Alloc(Many,SizeNotChange);
		//その領域にデータを転送します.
		CopyData( m_Caretation , _c , Many);
		//データの件数を更新する.
		m_CaretationNubmer = Many;
		return true;
	}
	//新規追加.
	bool			Add(InputData* _c ,			
						int Many , bool SizeNotChange = true)
	{
		//件数がマイナスは没.
		if (Many <= 0) return false;

		//現在の領域を維持しながら、新規領域確保.
		Alloc(m_CaretationNubmer+Many,SizeNotChange,false);
		//その領域にデータを転送します.
		CopyData( m_Caretation+m_CaretationNubmer , _c , Many);
		//データの件数を更新する.
		m_CaretationNubmer = m_CaretationNubmer+Many;
		return true;
	}

	//新規削除
	bool			Del(int Start , int Count = 1)
	{
		int i;
		//範囲外の削除は没.
		if ( Start+Count  <= m_CaretationNubmer) return false;
		//パラメータはマイナスは禁止.
		if ( Start < 0  || Count < 0) return false;
		
		//転送
		for( i = Start+Count ; i < m_CaretationNubmer ; i++)
		{
			m_Caretation[ i ] = m_Caretation[ i+Count ];
		}
		//データの件数を更新する.
		m_CaretationNubmer = m_CaretationNubmer-Count;
		return true;
	}
	
	//領域確保
	bool			Alloc(int Many , bool SizeNotChange = true , bool ClearAlloc = true)
	{
		if (m_Caretation == NULL)
		{	//初めての使用です.	領域を確保します.
			m_Caretation = (_Caretation*)::malloc(sizeof(_Caretation) * (Many + 1) );
			m_CaretationRealNumber = Many;	//実際確保している領域の更新.
		}
		else
		{	//m_Caretation に領域をすでに確保している場合.
			if (SizeNotChange == true)
			{	//ぴったし確保しなくてもいい.
				if (m_CaretationRealNumber < Many)
				{	//確保したい件数の方が、現在確保している領域より小さい場合.再確保.
					NewAlloc(Many ,SizeNotChange ,  ClearAlloc);
				}
			}
			else
			{	//ぴったりに確保しなくてはならない場合.
				if (m_CaretationRealNumber != Many)
				{	//現在確保している数と確保したい領域がいっしょではない場合、再確保.
					NewAlloc(Many ,SizeNotChange ,  ClearAlloc);
				}
			}
		}
		return true;
	}

	//領域消滅.
	void			Clear()						
	{
		if (m_Caretation != NULL)	::free(m_Caretation);
		m_Caretation = NULL;
		m_CaretationNubmer = 0;
		m_CaretationRealNumber = 0;
	}

	//破棄
	void			Release()	{	delete this;	}

	//サーチ.
	int				Search(_Caretation* const inSearchTarget , int inStartPos = 0)
	{
		int theI ;
		//念のため範囲チェック.
		if (inStartPos < 0 || inStartPos >= m_CaretationNubmer) return -1;
		//検索開始.
		for(theI = inStartPos; theI < m_CaretationNubmer ; theI ++)
		{
			if ( memcmp( &m_Caretation[theI]  , 
						inSearchTarget ,sizeof(_Caretation) ) == 0)
			{	//ミツカッタ!!
				return theI;
			}
		}
		//見つからない場合は、 -1.
		return -1;
	}

	//データ取得.
	_Caretation*		At(int Num)	
	{
		if (Num < 0 || Num >= m_CaretationNubmer)	return NULL;
		return &m_Caretation[Num];
	}

	//[]演算子オーバーロード
	_Caretation*		operator[](int _P) const
	{
		//__RTI_CHECKER(_P >= 0 && _P < m_CaretationNubmer);
		return &(*(m_Caretation + _P)); 
	}

	//先頭のデータを取得.
	_Caretation*	GetStart()	{		return m_Caretation;	}
	//最後のデータを取得.
	_Caretation*	GetEnd()	{		return &m_Caretation[m_CaretationNubmer];	}

	//個数を取得.
	int				GetCount()	{	return m_CaretationNubmer;	}

	//データスワップ!    このメソッドは非常に危険です!
	void			SwapData(_Caretation*	inCaretation , int inNum , int inReal)
	{
		//現在のデータを破棄.
		if (m_Caretation != NULL) ::free( m_Caretation );
		//データを入れ替えます.
		m_Caretation = inCaretation;
		//データの個数も入れ替えます.
		m_CaretationNubmer     = inNum;
		m_CaretationRealNumber = inReal;
	}
	/*
	//指定したデータのコピーに自分がなる.
	void CopyForm(_TCaretaker* inSrc)
	{
		//まず、自分の領域をクリア.
		Clear();
		//新規に領域を確保します.
		Alloc( inSrc->GetCount() );
		//現在の設定をいただきます.
		memcpy( m_Caretation , inSrc->GetStart() , );
	}
	*/
protected:
	/*********************/
	//メソッド
	/*********************/
	//データコピー
	virtual	void	CopyData(_Caretation* DestData ,	
							InputData* SrcCaretation , int Many)
	{	//ただしこれは、 _Caretation と InputData の方が違うと大変なことになるので注意!!
		memcpy(DestData , SrcCaretation , sizeof(_Caretation) * Many);
	}
	//新規にメモリを追加等をしたために、メモリブロックが変化した場合に呼ばれる.
	//しかし、何が悲しくて、メモリアロケーションルーチンかくねん >> 俺. ばかだねー.
	//注意: OldUseSize は 個数です.
	virtual	void	ReAllocMemory(_Caretation* OldMemoryAddress , 
								  _Caretation* NewMemoryAddress ,
								  int OldUseSize)	
	{
	}
	//新規領域確保.
	bool			NewAlloc(int Many , bool SizeNotChange , bool ClearAlloc)
	{
		//昔のメモリのサイズ(個数でることに注意せよ).
		int				OldUseSize	= m_CaretationRealNumber ;

		if (ClearAlloc == true)
		{//今までの領域の保護はしないで新規確保.
			if (m_Caretation != NULL)
			{
				::free( m_Caretation );
			}
			m_Caretation = (_Caretation*)::malloc( sizeof(_Caretation) * (Many + 1) );
			m_CaretationRealNumber = Many;	//実際確保している領域の更新.
		}
		else
		{//今までの領域を維持して確保.
			_Caretation*	NewCaretation;
			//新規領域確保.
			NewCaretation = 
				(_Caretation*)::realloc( m_Caretation , sizeof(_Caretation) * (Many + 1) );
			if (NewCaretation != m_Caretation && NewCaretation != NULL)
			{
				//この移動で無効になったポインタを有効な場所へ移動させる.
				ReAllocMemory(m_Caretation , NewCaretation , min(OldUseSize , Many) );
			}
			//各種設定の更新.
			m_Caretation = NewCaretation;
			m_CaretationRealNumber = Many;	//実際確保している領域の更新.
		}

		return true;
	}

	/*********************/
	//メンバ
	/*********************/
	int				m_CaretationNubmer;			//↓の個数.
	_Caretation*	m_Caretation;				//コレクションするデータ(配列になる)
	int				m_CaretationRealNumber;		//↑で実際に確保している領域.
private:
};


#endif
