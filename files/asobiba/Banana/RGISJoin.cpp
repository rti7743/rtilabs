// RGISJoin.cpp: RGISJoin クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISJoin.h"
#include "RStdioFile.h"
#include "RConv.h"
#include "RStdioMemory.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISJoin::RGISJoin()
{

}

RGISJoin::~RGISJoin()
{

}

//結合処理
void RGISJoin::Join(const string & inDir , const string &inMapArcFilename ,
					const JoinTempFormatScript* inScript, int inSize,
					const JoinHeadFormatScript* inHead , int inHeadSize)
{
	//ZLIB準備!
	Zlib.Create();

	//すべてのファイル名リストの取得.
	RStringVector List;
	GetAllFilenameList(&List , inDir,inScript,inSize);

	int n;

	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};

	unsigned long indexSize = List.size();
	IndexArea * index = new IndexArea[ indexSize ];

	{
		RStringVector::iterator i = List.begin();
		RStringVector::iterator e = List.end();
		for(n = 0 ; i != e ; i++ , n++)
		{
			index[n].Block = ::_atoi64( (*i).c_str() );
			index[n].Possition = 0;
		}
	}
	//では書き込み開始と。
	RStdioFile master;
	master.Open(inMapArcFilename , "wb" );

	//まず、シンボル
	string simbol = "KMAP";
	master.fwrite( simbol.c_str() , simbol.size() );
	//バージョン
	unsigned char Version = 1;
	master.fwrite( &Version , sizeof(Version) );

	//indexの数 を書き込みます。
	master.fwrite( &indexSize , sizeof(indexSize) );

	unsigned long indexPos = master.ftell();
	//とりあえず、適当に index を書き込みます。 (あとで正しい値を書きます)
	master.fwrite( index , sizeof(IndexArea) * indexSize);

	//ヘッダの書き込み.
	WriteHeader(&master ,inDir, inHead , inHeadSize );


	//indeにかかれているデータを書いていきます。
	for(n = 0 ; n <  List.size() ; n++)
	{
		//このブロックの開始位置を求めて..
		index[n].Possition = master.ftell();

	
		//圧縮するためにデータを入れるテンポラリ.
		RStdioFile zfile;
		string zPath = inDir + "/temp/zipping";
		zfile.Open(zPath , "wb");

		//最適化するために利用するファイル.
		RStdioFile optimizfile;
		string optimizPath = inDir + "/temp/optimiz";
		optimizfile.Open(optimizPath , "w+b");

		for(int k = 0 ; k < inSize ; k++)
		{
			const JoinTempFormatScript * script = &inScript[k];
			string path = inDir + "/temp/" + script->TempFileDirectoryName + "/" + num2str(index[n].Block) ;
			path = RConv::Replace(path , "/" , "\\");

			printf("ターゲット %s\n" , path.c_str() );
			
			try
			{
				RStdioFile file;
				file.Open(path , "rb");

				unsigned long dataSize = file.getSize();
				//まず件数を数えます
				unsigned long count = this->GetDataCount( &file , script , dataSize);

				//共有データを前に集めて最適化する.
				dataSize = Optimize(&file , &optimizfile , script , count);

				//シンボルの書き込み
				zfile.fwrite( &script->BlockStartSimbol , sizeof(unsigned char) );
				//サイズの書き込み
				zfile.fwrite( &dataSize , sizeof(dataSize) );
				//件数の書き込み
				zfile.fwrite( &count , sizeof(count) );
				//自身を丸ごとコピー
				FileAppend( &zfile , &optimizfile , dataSize);
			}
			catch(RException e)
			{
				printf("結合処理中にエラー : %s\n" , e.getMessage() );
				continue;
			}
		}
		zfile.Close();
		zfile.Open(zPath , "rb");

		//圧縮して、それを master に追加します.
		ZFileAppend(&master , &zfile);
	}

	//目次を更新します。
	//目次に戻って、上書きなのですよー
	master.fseek(indexPos);
	master.fwrite( index , sizeof(IndexArea) * indexSize);

	delete [] index;
}


//ヘッダーを書き込みます。
void RGISJoin::WriteHeader(RStdioFile * ioMaster , const string & inDir ,
					const JoinHeadFormatScript* inHead , int inHeadSize)
{

	//ヘッダのサイズを適当に書き込みます。 (あとで正しい値を書きます)
	unsigned long headerPos = ioMaster->ftell();
	unsigned long headerHeaderSize = 0;
	ioMaster->fwrite( &headerHeaderSize , sizeof(headerHeaderSize));

	//次にヘッダ情報を書き込みます。
	for(int n = 0; n < inHeadSize ; n++)
	{
		const JoinHeadFormatScript * head = &inHead[n];

		string path = inDir + "/temp/" + head->TempFilename ;
		path = RConv::Replace(path , "/" , "\\");

		try
		{
			RStdioFile file;
			file.Open(path , "rb");

			unsigned long dataSize = file.getSize();
			unsigned long count = GetHeadDataCount( &file , head , dataSize);

			//シンボルの書き込み
			ioMaster->fwrite( &head->BlockStartSimbol , sizeof(unsigned char) );
			//サイズの書き込み
			ioMaster->fwrite( &dataSize , sizeof(dataSize) );
			//件数の書き込み
			ioMaster->fwrite( &count , sizeof(count) );
			//masterに自身を丸ごとコピー
			FileAppend( ioMaster , &file , dataSize);
		}
		catch(RException e)
		{
			printf("結合処理中にエラー : %s\n" , e.getMessage() );
			continue;
		}
			
	}
	//ヘッダの正しい値を書き込みます。
	headerHeaderSize = ioMaster->ftell() - headerPos;

	ioMaster->fseek(headerPos);
	ioMaster->fwrite( &headerHeaderSize , sizeof(headerHeaderSize));

	//終端に戻します。
	ioMaster->fseek(0,SEEK_END);
}


//masterに自身を丸ごとコピー
void RGISJoin::FileAppend( RStdioFile * ioMaster , RStdioFile * ioFile , unsigned long inSize)
{
	//先頭に戻して
	ioFile->fseek(0);

	char * buffer = new char[inSize];

	ioFile->fread(buffer , inSize);
	ioMaster->fwrite(buffer , inSize);

	delete [] buffer;
}


//masterに圧縮した自分自身をコピー
void RGISJoin::ZFileAppend( RStdioFile * ioMaster , RStdioFile * ioZFile)
{
	//圧縮して、それを master に追加します.
	unsigned long zsizeSrc = ioZFile->getSize();
	unsigned long zsizeDest = zsizeSrc * 1.5;	//まぁ、 1.5倍以上にはならないでしょうし(w
	//圧縮に必要なメモリを確保.
	unsigned char * src = new unsigned char[zsizeSrc];			//まぁ、そんなにでかくはならないから OnMemoryで大丈夫(w
	unsigned char * dest = new unsigned char[zsizeDest];		

	//すべて読む.
	ioZFile->fseek(0);
	ioZFile->fread(src , zsizeSrc);

	try
	{
		//圧縮!
		Zlib.Compress2(dest , &zsizeDest , src , zsizeSrc ,RZlib.BEST_COMPRESSION);

		printf("zip.. %d --> %d   (%d%%)\n" , zsizeSrc , zsizeDest , zsizeDest * 100 / zsizeSrc);

#ifdef _DEBUG
		//デバッグのときだけ有効...
		//本当にうまくいっているのか実際に解凍してチェックしてみる.
		try
		{
			//元に戻してみる.
			unsigned long orignalSize = zsizeSrc;
			unsigned char * orignal = new unsigned char[orignalSize];
			Zlib.UnCompress(orignal , &orignalSize , dest , zsizeDest );

			//サイズはいっしょ?
			ASSERT(orignalSize == zsizeSrc);
			ASSERT(memcmp(orignal , src , orignalSize) == 0 );

			delete [] orignal;
		}
		catch(RException e)
		{
			ASSERT(0);
		}
#endif

		//マスターにコピー.
		ioMaster->fwrite(&zsizeSrc , sizeof(zsizeSrc) );		//まず圧縮前の容量
		ioMaster->fwrite(dest , zsizeDest);						//次に実態.
	}
	catch(RException e)
	{
		printf("圧縮中にエラー : %s\n" , e.getMessage() );
		ASSERT(0);
	}

	//メモリ開放.
	delete [] dest;
	delete [] src;
}

//文字列フォーマzットで記録されているファイルの
//件数を数えます
unsigned long RGISJoin::StringCount(RStdioFile * ioFile , unsigned long inSize)
{
	//先頭に戻して
	ioFile->fseek(0);

	unsigned long count;

	//とりあえず、全部読み
	char* buffer = new char[inSize + 1];
	ioFile->fread(buffer , inSize);

	int addpoint = 0;
	for(count = 0 ;  ; count++)
	{
		const char * name = buffer + addpoint ;
		//次の文字へ.
		addpoint += strlen(name) + 1;	//+1は \0の分.
		if (addpoint >= inSize)
		{
			count ++;
			break;	//これ以上ないのです。
		}
	}
	delete [] buffer;

	return count;
}


//文字列フォーマzットで記録されているファイルの
//件数を数えます
unsigned long RGISJoin::FixedAndStringCount(RStdioFile * ioFile , unsigned long inSize , int inFixedSize)
{
	//先頭に戻して
	ioFile->fseek(0);

	unsigned long count;

	//とりあえず、全部読み
	char* buffer = new char[inSize + 1];
	ioFile->fread(buffer , inSize);

	int addpoint = inFixedSize;
	for(count = 0 ;  ; count++)
	{
		const char * name = buffer + addpoint ;
		//次の文字へ.
		addpoint += strlen(name) + 1 + inFixedSize;	//+1は \0の分.
		if (addpoint >= inSize)
		{
			count ++;
			break;	//これ以上ないのです。
		}
	}
	delete [] buffer;

	return count;
}

//件数を数えます
unsigned long RGISJoin::GetHeadDataCount(RStdioFile * ioFile , const JoinHeadFormatScript* inScript , unsigned long inSize) 
{
	unsigned long count;

	//先頭に戻して
	ioFile->fseek(0);

	if (inScript->HeadType == HeadTypeFixedAndString)
	{//文字列
		count = FixedAndStringCount(ioFile , inSize , inScript->DataSize);
	}
	else if (inScript->HeadType == HeadTypeFixed)
	{//固定データ 
		count = inSize / inScript->DataSize;
	}
	else
	{
		ASSERT(0);
	}
	return count;
}

//件数を数えます
unsigned long RGISJoin::GetDataCount(RStdioFile * ioFile , const JoinTempFormatScript* inScript , unsigned long inSize) 
{
	unsigned long count;

	//先頭に戻して
	ioFile->fseek(0);

	if (inScript->CVType == CVTypeString)
	{//文字列
		count = StringCount(ioFile , inSize);
	}
	else
	{//通常データ

//		unsigned char cvSize;
//		unsigned char ndSize;
		for(count = 0;  ioFile->ftell() < inSize ; count ++)
		{
			DataSkip(ioFile ,inScript );
		}
	}
	return count;
}

/*

//最適化
DWORD RGISJoin::Optimize(RStdioFile *  ioReadFile , RStdioFile * ioWriteFile , const JoinTempFormatScript* inScript , int inCount) 
{
	//先頭に戻して
	ioReadFile->fseek(0);
	ioWriteFile->fseek(0);

	//今のところ CVTypeMultiPoint 形式以外は、この圧縮をすると
	//逆にでかくなってたしまうため何も行わない.
	if (inScript->CVType == CVTypeNone || inScript->CVType == CVTypeString || inScript->CVType == CVTypeByteData)
	{
		FileAppend(ioWriteFile , ioReadFile , ioReadFile->getSize() );
		return ioReadFile->getSize();
	}


	char * mainData = new char[inScript->DataSize];
	char * mainDataWork = new char[inScript->DataSize];

	char * copyWorkBuffer = new char[65535];

	//処理がすんでいるかどうか調べるためのテーブル.
	bool * checkTable =  new bool[inCount];
	memset(checkTable , 0 , sizeof(bool) * inCount);

	for(int ori = 0 ; ori < inCount ; ori ++)
	{
		DWORD starttell = ioReadFile->ftell();

		DWORD skip = DataSkip(ioReadFile ,inScript , mainData);

		ASSERT(ioReadFile->ftell() - skip == starttell);

		DWORD firstBlockEnd = ioReadFile->ftell() ;

		unsigned char identityDataCount = 0;

		//既に処理済.
		if ( checkTable[ori] ) continue;

		//共有ヘッダ部分
		ioWriteFile->fwrite(mainData , inScript->DataSize  );
		DWORD identityDataCountTell = ioWriteFile->ftell();
		//回数 0だと自分のみ
		ioWriteFile->fwrite(&identityDataCount , sizeof(identityDataCount)  );
		//自分のデータ部分の書き込み
		ioReadFile->fseek(starttell + inScript->DataSize);
		ioReadFile->fread(copyWorkBuffer , skip - inScript->DataSize);
		ioWriteFile->fwrite(copyWorkBuffer , skip - inScript->DataSize  );

		ASSERT(ioReadFile->ftell() == firstBlockEnd );

		checkTable[ori] = true;

		//このデータと同じデータはありますか?
		for(int des = ori + 1 ; des < inCount ; des ++ )
		{
			ASSERT(! ioReadFile->IsEof() );

			DWORD starttell2 = ioReadFile->ftell();
			DWORD skip2 = DataSkip(ioReadFile ,inScript , mainDataWork);
			ASSERT(ioReadFile->ftell() - skip2 == starttell2);
			DWORD firstBlockEnd2 = ioReadFile->ftell() ;

			//既に処理済.
			if ( checkTable[des] ) continue;

			//同じデータですか?
			if (memcmp(mainData ,mainDataWork , inScript->DataSize ) != 0)	continue;

			//では、共有しましょう.
			identityDataCount ++ ;

			//自分のデータ部分の書き込み
			ioReadFile->fseek(starttell2 + inScript->DataSize);
			ioReadFile->fread(copyWorkBuffer , skip2 - inScript->DataSize);
			ioWriteFile->fwrite(copyWorkBuffer , skip2 - inScript->DataSize  );

			ASSERT(ioReadFile->ftell() == firstBlockEnd2 );

			checkTable[des] = true;

			//まとめられる数の上限を超えたら終わり
			if (identityDataCount >= 255) break;
		}
		//identityDataCount に共有した値が入っているはずなので、
		//それを書き込みます.
		DWORD endPos = ioWriteFile->ftell();
		ioWriteFile->fseek(identityDataCountTell);
		ioWriteFile->fwrite(&identityDataCount , sizeof(identityDataCount)  );
		//最後尾に移動.
		ioWriteFile->fseek(endPos);

		//上の des のループで移動したことをなかったことにします.
		ioReadFile->fseek(firstBlockEnd);
	}

	DWORD starttell2 = ioReadFile->ftell();

	delete [] checkTable;
	delete [] copyWorkBuffer;
	delete [] mainDataWork;
	delete [] mainData;

	printf("optimize --> %s %d -> %d\r\n" ,inScript->TempFileDirectoryName.c_str() , 
		ioReadFile->getSize() , ioWriteFile->ftell() );
	return ioWriteFile->ftell();	//サイズを返す.

}
*/

//最適化
DWORD RGISJoin::Optimize(RStdioFile *  ioReadFile , RStdioFile * ioWriteFile , const JoinTempFormatScript* inScript , int inCount) 
{
	//先頭に戻して
	ioReadFile->fseek(0);
	ioWriteFile->fseek(0);

	//今のところ CVTypeMultiPoint 形式以外は、この圧縮をすると
	//逆にでかくなってたしまうため何も行わない.
	if (inScript->CVType == CVTypeNone || inScript->CVType == CVTypeString || inScript->CVType == CVTypeByteData)
	{
		FileAppend(ioWriteFile , ioReadFile , ioReadFile->getSize() );
		return ioReadFile->getSize();
	}

	//共有データを入れる場所.
	char		mainData[16];
	memset(mainData , 0 , 16);

	//共有されない個別のデータを読み取る作業バッファ.
	RBuffer unShareWriteBufferVector;
	unShareWriteBufferVector.reserve(65535);
	char * unShareWriteBuffer = &unShareWriteBufferVector[0];

	//共通にできそうなところをピックアップ.
	typedef newVector<OptimizeTemp> OptimizeDatabase;
	OptimizeDatabase db;

	for(int ori = 0 ; ori < inCount ; ori ++)
	{
		DWORD starttell = ioReadFile->ftell();

		DWORD skip = DataSkip(ioReadFile ,inScript , mainData);

		ASSERT(ioReadFile->ftell() - skip == starttell);

		OptimizeDatabase::iterator i = db.begin();
		OptimizeDatabase::iterator e = db.end();
		for( ; i != e ;  ++i)
		{
			if ( memcmp( (*i)->Share , mainData , 16) == 0)
			{
				//あるので既存のに追加.
				(*i)->Pos.push_back(starttell);
				break;
			}
		}
		if (i == e)
		{
			//ないので新規作成.
			OptimizeTemp* p = new OptimizeTemp;
			memcpy( (void*) (p->Share) , mainData , 16);
			p->Pos.push_back(starttell);

			db.push_back(p);
		}
	}
	for ( OptimizeDatabase::iterator opti = db.begin() ;  opti != db.end() ; ++opti)
	{
		const vector<DWORD> * sharePos = &((*opti)->Pos);
		int shareCount = sharePos->size();
		vector<DWORD>::const_iterator bi = sharePos->begin();

		ASSERT(shareCount >= 1);

		//共有部分は 255 づつしかかけないので、
		//それ以上共有している部分は、次の ブロックに書き込んでいきます.
		do
		{
			//共有部分.
			ioWriteFile->fwrite( (*opti)->Share , inScript->DataSize  );
			//共有個数
			const int temp = (shareCount < 257 ? shareCount : 256) - 1;
			ASSERT(temp >= 0 && temp <= 255);
			const unsigned char writeShareCount = temp;
			ioWriteFile->fwrite( &writeShareCount , sizeof(writeShareCount)  );

			for(int i = 0 ; i <= writeShareCount  ; ++bi , ++i)
			{
				DWORD starttell = *bi;
				//共有されない個別のデータを求めます.
				ioReadFile->fseek(starttell);
				DWORD skip = DataSkip(ioReadFile ,inScript , NULL);
				ioReadFile->fseek(starttell + inScript->DataSize);

				//共有されない個別のデータを読み取り、ファイルに書き込み.
				ioReadFile->fread(unShareWriteBuffer , skip - inScript->DataSize);
				ioWriteFile->fwrite(unShareWriteBuffer , skip - inScript->DataSize);
			}

			shareCount = shareCount - writeShareCount - 1;
			ASSERT(shareCount >= 0);
		}
		while(shareCount > 0);
	}

	printf("optimize --> %s %d -> %d\r\n" ,inScript->TempFileDirectoryName.c_str() , 
		ioReadFile->getSize() , ioWriteFile->ftell() );
	return ioWriteFile->ftell();	//サイズを返す.

}

DWORD RGISJoin::DataSkip(RStdioFile * ioFile , const JoinTempFormatScript* inScript , char * outMainData )
{
	unsigned char cvSize;
	unsigned char ndSize;

	DWORD tell = ioFile->ftell();

	if (outMainData == NULL)
	{
		//メインデータの読み飛ばし
		ioFile->fseek(inScript->DataSize , SEEK_CUR);
	}
	else
	{
		ioFile->fread(outMainData , inScript->DataSize);
	}

	//サブデータ (変動データ)の読み飛ばし.
	ioFile->fseek(inScript->DataSizeSub , SEEK_CUR);

	//ノードがある場合はノードデータの読み飛ばし.
	if (inScript->WithNode)
	{
		//ノードを読み飛ばす.
		ioFile->fread(&ndSize , sizeof(ndSize));
		//それだけの読み飛ばし.
		ioFile->fseek(sizeof(unsigned char) * ndSize,SEEK_CUR);
	}

	if (inScript->CVType == CVTypeOnePoint)
	{
		// x , y のデータの分の読み飛ばし.
		ioFile->fseek(sizeof(unsigned short) * 2,SEEK_CUR);
	}
	else if (inScript->CVType == CVTypeMultiPoint)
	{
		//CV長の読み込み
		ioFile->fread(&cvSize , sizeof(cvSize));

		ASSERT(cvSize > 0);

		//それだけの読み飛ばし.
		//まず unsigned short * 2 の先頭部分.
		ioFile->fseek(sizeof(unsigned short) * 2 ,SEEK_CUR);
		//残りは char *2 の差分データです.
		ioFile->fseek(sizeof(char) * 2 * (cvSize-1),SEEK_CUR);
	}
	else if (inScript->CVType == CVTypeByteData)
	{
		// 1byte のデータの分の読み飛ばし.
		ioFile->fseek(sizeof(unsigned char),SEEK_CUR);
	}

	return ioFile->ftell() - tell;
}

void RGISJoin::GetAllFilenameList(RStringVector * inList , const string & inDir ,const JoinTempFormatScript* inScript, int inSize)
{
	for(int i = 0 ; i < inSize ; i++)
	{
		string path = inDir + "/temp/" + inScript[i].TempFileDirectoryName + "/*.*";
		path = RConv::Replace(path , "/" , "\\");
		
		WIN32_FIND_DATA data;
		HANDLE handle = ::FindFirstFile(path.c_str() , &data );
		if (handle == INVALID_HANDLE_VALUE )
		{
			continue ;
		}
		do
		{
			if ( !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				//まだ登録されていない名前だったら登録せよ
				RStringVector::iterator i = find(inList->begin() , inList->end() , data.cFileName );
				if (i == inList->end())		inList->push_back(data.cFileName);

			}
		}
		while( ::FindNextFile(handle , &data) );

		::FindClose(handle);
	}
	sort(inList->begin() , inList->end() );
}

//地図ファイルが壊れていないか整合性のチェック.
void RGISJoin::Check(const string & inMasterFilename) throw (RException)
{
	//ZLIB準備!
	Zlib.Create();

	RStdioFile file;
	file.Open(inMasterFilename , "rb" );

	char buffer[MAX_PATH];
	int i;

	unsigned long fileSize = file.getSize();

	//ヘッダーの読み込み
	file.fread( buffer , 4 );
	if (!(buffer[0] == 'K' && buffer[1] == 'M' && buffer[2] == 'A' && buffer[3] == 'P'))
	{
		throw RException(EXCEPTIONTRACE + "整合性チェックエラー:先頭にKMAPのプレフィックスがありません");
	}

	//バージョンの読み込み.
	unsigned char version;
	file.fread( &version , sizeof(version) );
	if (version != 1)
	{
		throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:未知のバージョンです %d" , (int)version) );
	}

	//index長の読み込み
	unsigned long indexCount = 0;
	file.fread( &indexCount , sizeof(indexCount) );

	//indexの読み込み.
	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};
	IndexArea* index = new IndexArea[indexCount];
	file.fread( index , sizeof(IndexArea) * indexCount);

	try
	{
		//各ブロックの構造チェック.
		unsigned char simbol;				// D などのデータシンボル
		unsigned long dataSize;				// このデータの長さ(bytes) ヘッダを除く.
		unsigned long dataCount;			// データのようその数
		for(i = 0 ; i < indexCount ; i++)
		{
			printf("番号 %d ブロック番号 %ld ブロック長 %ld \n" , i , index[i].Block , index[i].Possition);

			unsigned long blockStart = index[i].Possition;
			unsigned long blockSize ;
			if (i+1 == indexCount)
			{
				blockSize =  fileSize - blockStart;	//最後のブロックはファイル終端まで続いている.
			}
			else
			{
				blockSize =  index[i + 1].Possition - blockStart;
			}
			file.fseek( blockStart );

			//まず、圧縮データを解凍します.
			unsigned long fullSize;				//unzipすると成る正式な大きさ.
			file.fread( &fullSize , sizeof(fullSize) );

			unsigned char * dest = new unsigned char[fullSize];
			unsigned char * src = new unsigned char[blockSize];

			//ブロックサイズは、圧縮前サイズを記録している部分も含むため 引き算します
 			file.fread( src , blockSize  - sizeof(fullSize) );	
			try
			{
				//解凍の実行.
				unsigned long realSize = fullSize;
				Zlib.UnCompress(dest , &realSize , src , blockSize);
				ASSERT(realSize == fullSize);

				//まず、もう使わないソース領域の破棄.
				delete [] src;	src  = NULL;

				//解凍したデータの解析.
				RStdioMemory mem;
				mem.Open((char*)dest );
				while( mem.ftell() < realSize )
				{
					mem.fread( &simbol , sizeof(simbol) );
					mem.fread( &dataSize , sizeof(dataSize) );
					mem.fread( &dataCount , sizeof(dataCount) );

					printf("シンボル %c データ長 %ld データの数 %d \n" , simbol , dataSize , (int)dataCount);

					if ( ! ::isalpha(simbol) )
					{
						throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:アルファベット以外のシンボルを発見 %c  位置:%ld" , simbol , file.ftell() ) );
					}
					//読み飛ばし.
					mem.fseek( dataSize , SEEK_CUR);
				}
			}
			catch(RException e)
			{
				delete [] src;
				delete [] dest;

				ASSERT(0);
				throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:解凍中にエラーが発生しました ブロック番号 %ld ブロック開始位置:%ld : %s" ,
						index[i].Block , blockStart , e.getMessage() ) );
			}
			delete [] src;
			delete [] dest;

			//記録されているブロック長と実際の値が、ぴたりといっちしなければいけない
			if((file.ftell() - blockStart) != blockSize)
			{
				throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:目次が持っているブロック長[%ld] と 実際のブロック長[%ld]が合いません. ブロック番号 %ld ブロック開始位置:%ld" ,
					blockSize ,  (file.ftell() - blockStart)  , index[i].Block , blockStart) );
			}
		}
		if ( file.ftell() != fileSize )
		{
			throw RException(EXCEPTIONTRACE + "整合性チェックエラー:全部のブロックを検査してたどり着いた場所がファイルの終端ではありません");
		}
	}
	catch(RException e)
	{
		delete [] index;
		throw;
	}
	delete [] index;
}


//地図の詳しい情報を得る.
void RGISJoin::Info(const string & inMasterFilename) throw (RException)
{
	//ZLIB準備!
	Zlib.Create();

	RStdioFile file;
	file.Open(inMasterFilename , "rb" );

	char buffer[MAX_PATH];
	int i;

	unsigned long fileSize = file.getSize();

	//ヘッダーの読み込み
	file.fread( buffer , 4 );
	if (!(buffer[0] == 'K' && buffer[1] == 'M' && buffer[2] == 'A' && buffer[3] == 'P'))
	{
		throw RException(EXCEPTIONTRACE + "整合性チェックエラー:先頭にKMAPのプレフィックスがありません");
	}

	//バージョンの読み込み.
	unsigned char version;
	file.fread( &version , sizeof(version) );
	if (version != 1)
	{
		throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:未知のバージョンです %d" , (int)version) );
	}

	//index長の読み込み
	unsigned long indexCount = 0;
	file.fread( &indexCount , sizeof(indexCount) );

	//indexの読み込み.
	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};
	IndexArea* index = new IndexArea[indexCount];
	file.fread( index , sizeof(IndexArea) * indexCount);

	RStdioFile outlog;
	outlog.Open("mapinfo.csv" , "w+b");

	string l = PrintFormat("ブロック番号,ブロック全体長さ(byte),ブロックに占める割合,圧縮済みブロック全体長さ(byte),圧縮済みブロックに占める割合,シンボル,データ長(byte),データの数,1データの平均サイズ\n");
	outlog.fwrite(l.c_str() , l.size() );

	try
	{
		//各ブロックの構造チェック.
		unsigned char simbol;				// D などのデータシンボル
		unsigned long dataSize;				// このデータの長さ(bytes) ヘッダを除く.
		unsigned long dataCount;			// データのようその数
		unsigned long zipdataSize;			// このブロックを圧縮したときのサイズ.
		for(i = 0 ; i < indexCount ; i++)
		{
//			TRACE("番号 %d ブロック番号 %ld ブロック長 %ld \n" , i , index[i].Block , index[i].Possition);

			unsigned long blockStart = index[i].Possition;
			unsigned long blockSize ;
			if (i+1 == indexCount)
			{
				blockSize =  fileSize - blockStart;	//最後のブロックはファイル終端まで続いている.
			}
			else
			{
				blockSize =  index[i + 1].Possition - blockStart;
			}
			file.fseek( blockStart );

			//まず、圧縮データを解凍します.
			unsigned long fullSize;				//unzipすると成る正式な大きさ.
			file.fread( &fullSize , sizeof(fullSize) );

			unsigned char * dest = new unsigned char[fullSize];
			unsigned char * src = new unsigned char[blockSize];

			//ブロックサイズは、圧縮前サイズを記録している部分も含むため 引き算します
 			file.fread( src , blockSize  - sizeof(fullSize) );	
			try
			{
				//解凍の実行.
				unsigned long realSize = fullSize;
				Zlib.UnCompress(dest , &realSize , src , blockSize);
				ASSERT(realSize == fullSize);

				//まず、もう使わないソース領域の破棄.
				delete [] src;	src  = NULL;

				//解凍したデータの解析.
				RStdioMemory mem;
				mem.Open((char*)dest );
				while( mem.ftell() < realSize )
				{
					mem.fread( &simbol , sizeof(simbol) );
					mem.fread( &dataSize , sizeof(dataSize) );
					mem.fread( &dataCount , sizeof(dataCount) );
					{
						const char* nama = mem.ptr();
						zipdataSize = dataSize * 10;
						char * zipdata = new char[zipdataSize];
						Zlib.Compress2(  (unsigned char*)zipdata , &zipdataSize ,
							(const unsigned char*)nama , dataSize , RZlib.BEST_COMPRESSION);
						delete [] zipdata;
					}

					l = PrintFormat("%ld,%ld,%d%%,%ld,%d%%,%c,%ld,%ld,%d%%\n",
						index[i].Block,									//ブロック番号
						fullSize,										//ブロック全体長さ(byte)
						(int)(((double)dataSize)/fullSize * 100),		//ブロックに占める割合
						blockSize,										//圧縮済みブロック全体長さ(byte)
						(int)(((double)zipdataSize)/blockSize * 100),	//圧縮済みブロックに占める割合
						simbol,											//シンボル
						dataSize,										//データ長(byte)
						dataCount,										//データの数
						(int)((double)dataCount/dataSize * 100)			//1データの平均サイズ
						);
					outlog.fwrite(l.c_str() , l.size() );
//					TRACE("シンボル %c データ長 %ld データの数 %d \n" , simbol , dataSize , (int)dataCount);

					if ( ! ::isalpha(simbol) )
					{
						throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:アルファベット以外のシンボルを発見 %c  位置:%ld" , simbol , file.ftell() ) );
					}
					//読み飛ばし.
					mem.fseek( dataSize , SEEK_CUR);
				}
			}
			catch(RException e)
			{
				delete [] src;
				delete [] dest;

				ASSERT(0);
				throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:解凍中にエラーが発生しました ブロック番号 %ld ブロック開始位置:%ld : %s" ,
						index[i].Block , blockStart , e.getMessage() ) );
			}
			delete [] src;
			delete [] dest;

			//記録されているブロック長と実際の値が、ぴたりといっちしなければいけない
			if((file.ftell() - blockStart) != blockSize)
			{
				throw RException(EXCEPTIONTRACE + PrintFormat("整合性チェックエラー:目次が持っているブロック長[%ld] と 実際のブロック長[%ld]が合いません. ブロック番号 %ld ブロック開始位置:%ld" ,
					blockSize ,  (file.ftell() - blockStart)  , index[i].Block , blockStart) );
			}
		}
		if ( file.ftell() != fileSize )
		{
			throw RException(EXCEPTIONTRACE + "整合性チェックエラー:全部のブロックを検査してたどり着いた場所がファイルの終端ではありません");
		}
	}
	catch(RException e)
	{
		delete [] index;
		throw;
	}
	delete [] index;
}

