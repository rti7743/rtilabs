// RCreateFile.cpp: RCreateFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RCreateFile.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
void RStream::test()
{
	{
		RCreateFile file;
		file.Open("test/rstreamtest.txt" , GENERIC_READ);
		StreamCheckerForReturnCode returnCheck;
		
		char buffer[4];

		RStream stream;
		stream.Create( &file , 11);
		int readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("abc") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("bes") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("t!") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("tot") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("oro") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("") == buffer);
	}
	{
		RCreateFile file;
		file.Open("test/rstreamtest.txt" , GENERIC_READ);
		StreamCheckerForReturnCode returnCheck;
		
		char buffer[11];

		RStream stream;
		stream.Create( &file , 11);
		int readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("abc") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("best!") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("totoro") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("") == buffer);
	}
	{
		RCreateFile file;
		file.Open("test/rstreamtest.txt" , GENERIC_READ);
		StreamCheckerForReturnCode returnCheck;
		
		char buffer[11];

		RStream stream;
		stream.Create( &file );
		int readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("abc") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("best!") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("totoro") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("") == buffer);
	}
}


int RStream::Read(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException)
{
	ASSERT(BufferLength >= inBufferSize);

	//まず、前回読みすぎたところに終端があるかチェック.
	if (! inStreamChecker->Check(Buffer , 0 , BufferUse) )
	{//ある.
		int breakPoint = inStreamChecker->getSeparater();

		//とりざらにコピー.
		int copy = breakPoint > inBufferSize ? inBufferSize : breakPoint;
		memcpy(outBuffer , Buffer , copy);
		//読み込みすぎた分は先頭に回す
		int skip = copy + (inStreamChecker->getNext() - inStreamChecker->getSeparater() ) + 1;
		memmove(Buffer , Buffer + skip  , BufferUse - skip );
		BufferUse = BufferUse - skip;

		return copy;
	}
	//取り皿以上の量をプールしている?
	if (BufferUse > inBufferSize)
	{
		//とりざらにコピー.
		int copy = inBufferSize;
		memcpy(outBuffer , Buffer , copy);
		//読み込みすぎた分は先頭に回す
		memmove(Buffer , Buffer + copy , BufferUse - copy);
		BufferUse = BufferUse - copy;

		return copy;
	}
	int sended;
	for(; BufferUse < BufferLength ; BufferUse += sended)
	{
//		TRACE("//RStream::Read  読み込みます." );
		//読み込み
		sended = Stream->Read(Buffer+BufferUse ,BufferLength - BufferUse);
//		TRACE("//RStream::Read  %d 読み込みました." , sended);
		if ( sended == 0 )
		{//読み込み終了
			break;
		}
		else if (sended < 0)
		{//異常事態
			throw RException(EXCEPTIONTRACE + "読み込み時にエラーが発生しました");
		}
		//今読み込んだ中に終了条件があるかチェック.
		if ( ! inStreamChecker->Check(Buffer , BufferUse , BufferUse+sended) )
		{	//チェッカーによる停止.
			BufferUse += sended;

			int breakPoint = inStreamChecker->getSeparater();

			//とりざらにコピー.
			int copy = breakPoint > inBufferSize ? inBufferSize : breakPoint;
			memcpy(outBuffer , Buffer , copy);
			//読み込みすぎた分は先頭に回す
			int skip = copy + (inStreamChecker->getNext() - inStreamChecker->getSeparater() ) + 1;
			memmove(Buffer , Buffer + skip  , BufferUse - skip );
			BufferUse = BufferUse - skip;

			return copy;
		}
	}

	//取り皿にコピー
	int copy = BufferUse > inBufferSize ? inBufferSize : BufferUse;
	memcpy(outBuffer , Buffer , copy);
	//読み込みすぎた分は先頭に回す
	memmove(Buffer , Buffer + copy , BufferUse - copy);
	BufferUse = BufferUse - copy;

	return copy;

}
