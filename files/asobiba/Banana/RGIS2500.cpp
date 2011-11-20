// RGIS2500.cpp: RGIS2500 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGIS2500.h"
#include "RDiskUtil.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGIS2500::RGIS2500()
{
	Count = 0;
	Points= NULL;
}

RGIS2500::~RGIS2500()
{
	Delete();
}

void RGIS2500::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

//Keyを文字列として返します. Keyは必ず 5桁の文字列になる.
string RGIS2500::KeyToString(int inKey) const
{
	char buffer[MAX_PATH];
	sprintf(buffer , "%05d" , inKey);

	return buffer;
}



//01100 等のディレクトリの中に分断されて入っているエリアの数を取得します.
int RGIS2500::getAreaCount(const string & inPath ) const throw(RNotFoundException , RException) 
{
	WIN32_FIND_DATA data;
	const string path = inPath + "\\*.*";
	HANDLE handle = ::FindFirstFile(path.c_str() , &data );
	if (handle == INVALID_HANDLE_VALUE )
	{
		throw RNotFoundException(path + "検索の失敗しました");
	}

	int count = 0;
	do
	{
		if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  && 
			!(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) ) )
		{
			count ++;
		}
	}
	while( ::FindNextFile(handle , &data) );

	::FindClose(handle);
	return count;
}

//実際に読み込み
void RGIS2500::Read(const string & inPath ,int inScale) throw(RException)
{
	WIN32_FIND_DATA data;
	const string path = inPath + "\\*.*";
	HANDLE handle = ::FindFirstFile(path.c_str() , &data );
	if (handle == INVALID_HANDLE_VALUE )
	{
		throw RNotFoundException(path + "検索の失敗しました");
	}

	int count = 0;
	do
	{
		if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  && 
			!(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) ) )
		{
			ReadInPart( &this->Points[count] , inPath + "\\" + data.cFileName,inScale);
			count ++;

			if (count > this->Count)
			{
				throw RException(EXCEPTIONTRACE + "ディレクトリの数が、調べたときより増加してます");
			}
		}
	}
	while( ::FindNextFile(handle , &data) );

	::FindClose(handle);
}

//ひとつのエリアを読み込んで見ます。
void RGIS2500::ReadInPart(RGIS2500OneArea* ioArea , const string & inPath , int inScale) throw(RException)
{
	WIN32_FIND_DATA data;
	const string path = inPath + "\\*.*";
	HANDLE handle = ::FindFirstFile(path.c_str() , &data );
	if (handle == INVALID_HANDLE_VALUE )
	{
		throw RNotFoundException(path + "検索の失敗しました");
	}

	string pgnFilename , atrFilename;
	do
	{
		if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  && 
			!(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) ) )
		{
			string arcPath = inPath + "\\" + data.cFileName ;
			string arcFilename = arcPath + "\\" + data.cFileName + ".arc";
			if ( stricmp( data.cFileName , "TATEMONO" ) == 0)
			{
				pgnFilename = arcPath + "\\" + "tatemono.pgn";
				atrFilename = arcPath + "\\" + "tatemono.atr";
				if ( RDiskUtil::Exist( pgnFilename ) )
				{
					//アークの読み取り
					ioArea->Tatemono.Arc.Create( arcFilename , inScale);
					//ポリゴン情報の読み取り
					ioArea->Tatemono.Pgn.Create( pgnFilename , inScale);
					//属性の読み込み
					ioArea->Tatemono.Atr.Create( atrFilename , inScale);
				}
			}
			else if ( stricmp( data.cFileName , "OTHERS" ) == 0)
			{
				pgnFilename = arcPath + "\\" + "zyouti.pgn";
				atrFilename = arcPath + "\\" + "zyouti.atr";
				if ( RDiskUtil::Exist( pgnFilename ) )
				{
					//アークの読み取り
					ioArea->Zyouti.Arc.Create( arcFilename , inScale);
					//ポリゴン情報の読み取り
					ioArea->Zyouti.Pgn.Create( pgnFilename , inScale);
					//属性の読み込み
					ioArea->Zyouti.Atr.Create( atrFilename , inScale);
				}
				pgnFilename = arcPath + "\\" + "eki.pnt";
				atrFilename = arcPath + "\\" + "eki.atr";
				if ( RDiskUtil::Exist( pgnFilename ) )
				{
					//アークの読み取り
					ioArea->Eki.Arc.Create( arcFilename , inScale);
					//ポリゴン情報の読み取り
					ioArea->Eki.Pnt.Create( pgnFilename , inScale);
					//属性の読み込み
					ioArea->Eki.Atr.Create( atrFilename , inScale);
				}
			}
		}
	}
	while( ::FindNextFile(handle , &data) );

	::FindClose(handle);
}



void RGIS2500::SmallPack(const string inPath,const string inTo,const string inNumString 
						 , MessageGIS2500Info * gis2500Info, RGISNameDatabase * ioNameDatabase, int inScale)
{
	Delete();

	printf(" 2500:read /");

	//読み込み
	string path = inPath + "/2500/" + inNumString ;
	if (! RDiskUtil::Exist( path ) )
	{
		TRACE(path + "はないので省略します");
		return ;
	}


	try
	{
		//領域確保
		this->Count = getAreaCount(path);
		this->Points = new RGIS2500OneArea[this->Count];

		//実際に読み込み
		Read(path , inScale);
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
		throw ;
	}

	printf(" write /");
	//書き込み
	path = inTo + "/temp/"  ;

	for(int i = 0 ; i < this->Count ; i++)
	{
		RGIS2500OneArea* area = &this->Points[i];

		area->Tatemono.Pgn.SmallPack( &area->Tatemono.Arc , &area->Tatemono.Atr , path  , "TA" , ioNameDatabase ,&gis2500Info->Tatemono);
		printf(".");
		area->Zyouti.Pgn.SmallPack( &area->Zyouti.Arc , &area->Zyouti.Atr ,path  , "ZO" , ioNameDatabase , &gis2500Info->Tatemono);
		printf(".");
		area->Eki.Pnt.SmallPackNoWrite( &area->Eki.Arc , &area->Eki.Atr , &gis2500Info->Eki);
		printf(".");
	}
	printf("\n");
}

JoinTempFormatScript RGIS2500::PgnJoinScript(const string & inDirectoryName , unsigned char inBlockStart) const
{
	JoinTempFormatScript ret ;
	ret.TempFileDirectoryName = inDirectoryName;
	ret.BlockStartSimbol = inBlockStart;
	ret.DataSize = sizeof(unsigned short);	//NameIndex
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeMultiPoint;
	ret.WithNode = false;

	return ret;
}

//結合スクリプトの取得.
int RGIS2500::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const
{

	ASSERT(inSize >= 7);
	ioScripts[0] = PgnJoinScript("TA" , 'B');
	ioScripts[1] = PgnJoinScript("ZO" , 'O');

	return 2;
}

//結合スクリプトの取得.
int RGIS2500::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const
{
	return 0;
}



