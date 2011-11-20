// RStringRingBuffer.cpp: RStringRingBuffer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RStringRingBuffer.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

void RStringRingBuffer::Push(const char* inStr , int inSize) throw(RException)
{
	if (inSize <= 0)
	{
		return ;
	}

	if ( this->NowPos + inSize > this->BufferSize )
	{
		if (inSize > this->OneSize)
		{
			throw RException(EXCEPTIONTRACE + "バッファ以上のデータが渡されました" );
		}
		
		//新しい文字列を入れてもこれだけ残せる.
		int nokori = this->OneSize - inSize ;
		//残せる部分を先頭に持ってくる.
		memmove(this->Buffer , this->Buffer + (this->NowPos - nokori ), nokori);
		//そいつの後ろに新しい文字列をコピー
		memcpy(this->Buffer + nokori , inStr , inSize);
		ASSERT(nokori + inSize <= this->OneSize);
		this->Buffer[nokori + inSize] = '\0';
		this->NowPos = nokori + inSize;
	}
	else
	{
		memcpy(this->Buffer + this->NowPos , inStr , inSize);
		this->Buffer[this->NowPos + inSize] = '\0';
		this->NowPos += inSize;
	}
}
const string RStringRingBuffer::getBuffer() const
{
	if (this->NowPos > this->OneSize)
	{
		return string( this->Buffer ,this->NowPos - this->OneSize , this->OneSize);
	}
	else
	{
		return string( this->Buffer , 0 , this->NowPos);
	}
}

void RStringRingBuffer::test()
{
	string r;
	RStringRingBuffer p;
	p.Create(10);

	string t = "totoro";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "totoro" ) ;

	t = "3";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "totoro3" ) ;

	t = "mahoro";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "oro3mahoro" ) ;

	t = "minawa";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "horominawa" ) ;

	t = "meido";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "inawameido" ) ;

	t = "0123456789";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "0123456789" ) ;
}
