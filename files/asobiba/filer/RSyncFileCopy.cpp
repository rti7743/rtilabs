// RSyncFileCopy.cpp: RSyncFileCopy クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RSyncFileCopy.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RSyncFileCopy::RSyncFileCopy()
{
	for(int i = 0 ; i < BUFFER_COUNT ; i++)
	{
		Buffer[i] = new char[BUFFER_SIZE];
	}
}

RSyncFileCopy::~RSyncFileCopy()
{
	for(int i = 0 ; i < BUFFER_COUNT ; i++)
	{
		delete[] Buffer[i];
	}
}

void RSyncFileCopy::Run(string inReadPath , Rfile* ioRead,
						string inWritePath , Rfile* ioWrite) throw()
{
	return;

	struct POINT_STRUCT
	{
		int				BufferCount;
		unsigned long	ModBufferSize;
	};
	POINT_STRUCT	read = {0};
	POINT_STRUCT	write = {0};
	bool			read_end = false;
	unsigned long	read_mod_buffer = 0;
	string			errro;

	try
	{
		ioRead->Open ( inReadPath , __RFILEOPENTYPE_READ );
		ioWrite->Open ( inWritePath , __RFILEOPENTYPE_WRITE );

		ioRead->Read(Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
		//リードバッファを満タンにします。
		while(read.ModBufferSize < BUFFER_SIZE) 
		{
			ioRead->WaitFor();
			read.ModBufferSize += ioRead->GetReport();
			if (read.ModBufferSize >= BUFFER_SIZE) break;

			bool ret = ioRead->Read(Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
			if (!ret)
			{
				//読み込みで EOF まで読み込みました。
//				printf("-------Eof-----1\n");
				read_end = true;
				read_mod_buffer = read.ModBufferSize;
				//あとこれだけ書き込んだら終わりです。
				write.ModBufferSize = BUFFER_SIZE - read_mod_buffer;
				break;
			}
		}
		//読み込みがまだ続くなら、次の読み込みバッファにセット.
		if (!read_end)
		{
			read.ModBufferSize = read_mod_buffer;
			read.BufferCount = (read.BufferCount+1) % BUFFER_COUNT;
		}

		//完全非同期IO プログラム汚すぎ.
		while(1)
		{
			ioWrite->Write( Buffer[write.BufferCount] , BUFFER_SIZE - write.ModBufferSize);

			//読み込み終わりました?
			if ( ioRead->Poll() )
			{
				//んぢゃ、バッファ更新、と。
				read.ModBufferSize += ioRead->GetReport();

				//バッファをすべて消化しました?
				if (read.ModBufferSize >= BUFFER_SIZE)
				{
					//次のバッファへ
					read.BufferCount = (read.BufferCount+1) % BUFFER_COUNT;
					read.ModBufferSize = 0;
					//次のバッファは実は使われてたりして
					if (read.BufferCount == write.BufferCount) 
					{	//使っているぢゃん、どけ。
						while(write.ModBufferSize < BUFFER_SIZE) 
						{
							ioWrite->WaitFor();
							write.ModBufferSize += ioWrite->GetReport();
							if (write.ModBufferSize >= BUFFER_SIZE) break;

							ioWrite->Write(Buffer[write.BufferCount] , BUFFER_SIZE - write.ModBufferSize);
						}
						write.BufferCount = (write.BufferCount+1) % BUFFER_COUNT;
						write.ModBufferSize = 0;
					}
				}
			}
			//読み込み開始.
			if (!read_end)
			{
				bool ret = ioRead->Read( Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
				if (!ret)
				{
					//読み込みで EOF まで読み込みました。
//					printf("-------Eof-----2\n");
					read_end = true;
					read_mod_buffer = read.ModBufferSize;
				}
			}

			//書き込み終わりました?
			if ( ioWrite->Poll() )
			{
				//んぢゃ、バッファ更新、と。
				write.ModBufferSize += ioWrite->GetReport();

				//バッファをすべて消化しました?
				if (write.ModBufferSize >= BUFFER_SIZE)
				{
					if (!read_end)
					{//処理はまだ続いている
						//次のバッファへ
						write.BufferCount = (write.BufferCount+1) % BUFFER_COUNT;
						write.ModBufferSize = 0;
						//次のバッファは実は使われてたりして
						if (write.BufferCount == read.BufferCount) 
						{
							//使っているぢゃん、どけ。
							while(read.ModBufferSize < BUFFER_SIZE) 
							{
								ioRead->WaitFor();
								read.ModBufferSize += ioRead->GetReport();

								if (read.ModBufferSize >= BUFFER_SIZE) break;

								bool ret = ioRead->Read(Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
								if (!ret)
								{
									//読み込みで EOF まで読み込みました。
//									printf("-------Eof-----3\n");
									read_end = true;
									read_mod_buffer = read.ModBufferSize;
									//あとこれだけ書き込んだら終わりです。
									write.ModBufferSize = BUFFER_SIZE - read_mod_buffer;
									break;
								}
							}
							//読み込みがまだ続くなら、次の読み込みバッファにセット.
							if (!read_end)
							{
								read.BufferCount = (read.BufferCount+1) % BUFFER_COUNT;
								read.ModBufferSize = 0;
							}
						}
					}
					else
					{
						//今のバッファは最後のバッファ?
						if (write.BufferCount == read.BufferCount) 
						{//全部終わった! while(1) をぬけるぞー!!
							break;
						}
						//次のバッファへ
						write.BufferCount = (write.BufferCount+1) % BUFFER_COUNT;
						write.ModBufferSize = 0;
						//新しいバッファは最後のバッファですか?
						if (write.BufferCount == read.BufferCount) 
						{	//端数を書き込み.
							write.ModBufferSize = BUFFER_SIZE - read_mod_buffer;
						}
					}
				}
			}
		}
		ioRead->Close ( );
		ioWrite->Close ( );
	}
	catch(RException e)
	{
		ioRead->Close ( );
		ioWrite->Close ( );
		throw RIOException( "コピー処理中にエラーが発生しました.\n %s" , e.getMessage() );
	}
}

#include "RfileLocalReport.h"
#include "RfilesLocal.h"

void RSyncFileCopy::Test()
{
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send1.jpg" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv1.jpg" , 0 , 0 ,0 );
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		FileCheck("c:\\test2\\copy_test_send1.jpg","c:\\test2\\copy_test_resv1.jpg");
	}
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send3.jpg" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv3.jpg" , 0 , 0 ,0 );
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		FileCheck("c:\\test2\\copy_test_send3.jpg","c:\\test2\\copy_test_resv3.jpg");
	}
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send4.mp3" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv4.mp3" , 0 , 0 ,0 );
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		FileCheck("c:\\test2\\copy_test_send4.mp3","c:\\test2\\copy_test_resv4.mp3");
	}
	/*
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send2.lzh" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv2.lzh" , 0 , 0 ,0 );
		DWORD timer = ::timeGetTime();
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		printf( "かかった時間 %ld ms \n" , ::timeGetTime() - timer);
		FileCheck("c:\\test2\\copy_test_send2.lzh","c:\\test2\\copy_test_resv2.lzh");
	}
	*/

}

void RSyncFileCopy::FileCheck(const char* inSrc,const char* inDest)
{
	FILE* src = fopen(inSrc , "rb" );
	ASSERT(src);
	FILE* dest = fopen(inDest , "rb" );
	ASSERT(dest);

	string error;

	char* srcBuffer = new char[BUFFER_SIZE];
	char* destBuffer = new char[BUFFER_SIZE];
	while(1)
	{
		int srclen = fread( srcBuffer , 1 , BUFFER_SIZE , src);
		int destlen = fread( destBuffer , 1 , BUFFER_SIZE , dest);

		if (srclen == 0 && destlen == 0) break;

		if (srclen != destlen)
		{
			fclose(src);			fclose(dest);
			//サイズ不一致
			ASSERT(0);
			break;
		}
		if ( memcmp(srcBuffer ,  destBuffer ,srclen) != 0)
		{
			fclose(src);			fclose(dest);
			//内容不一致
			ASSERT(0);
			break;
		}
	}
	fclose(src);			fclose(dest);
	delete []  srcBuffer ;
	delete [] destBuffer ;
}
