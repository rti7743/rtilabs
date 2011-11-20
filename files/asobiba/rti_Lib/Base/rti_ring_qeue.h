#ifndef ___RING_QUEUEH
#define ___RING_QUEUEH

/**************************************************************************************/
//リングキュー STL コンパチになったかな?
//			(C) R.T.I
/**************************************************************************************/
#include <stdio.h>

template<class _T>

class ringqueue256
{
	//リングキューバッファ
	_T RingQueueBuffer[256];
	//255 ～ 0 までを自動ループ ( Oo-y~~ )
	unsigned char Top ;

public:
	ringqueue256(){Top = 0;};
	//要素を追加.
	void push_back(_T* p){RingQueueBuffer[Top++] = *p;} ;
	//要素を削除.
	void pop_back(){Top--; } ;
	//先頭の要素を返す
//	_T begin(){return &RingQueueBuffer[0];};
	//最後の要素を返す.
//	_T end(){return &RingQueueBuffer[Top];};
	//サイズを返す.
	int size(){return Top;};
	//空っぽかどうか返す.
	bool empty(){ return (size == 0); };
	//最初の値を返す.
	_T* front(){return &RingQueueBuffer[0]; } ;
	//最後の値を取得して、最後の値 --
	_T* pop(){return &RingQueueBuffer[--Top]; } ;
	//最後から指定されたやつ番目のデータ
	_T* back(int a = 0){return &RingQueueBuffer[ unsigned char(Top-1-a)]; } ;
	//クリア
	void clear(){Top = 0;	memset(RingQueueBuffer,0,256);};
	//現在の終端の取得.
	unsigned char getTop(){return Top;}
	//要素を取得.
	void getes(_T* p,unsigned char len , unsigned char pos )
	{	for( unsigned char i = 0 ; i < len ; i++ , p++ ) *p = RingQueueBuffer[ (unsigned char)(i + pos) ];	}

};


#endif
