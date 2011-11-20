// MVFileList.cpp: MVFileList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVFileList.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MVFileList::MVFileList()
{

}

MVFileList::~MVFileList()
{

}


void MVFileList::Update(const string & inZIPname) throw(RException)
{
	struct LocalFileHeaderDef				//local file header signature     4 bytes  (0x04034b50)
	{
		unsigned short version;				//version needed to extract       2 bytes
		unsigned short purposeFlg;			//general purpose bit flag        2 bytes
		unsigned short compressionMethod;	//compression method              2 bytes
		unsigned short lastModFileTime;		//last mod file time              2 bytes
		unsigned short lastModFileDate;		//last mod file date              2 bytes
		unsigned long  crc32;				//crc-32                          4 bytes
		unsigned long  compressedSize;		//compressed size                 4 bytes
		unsigned long  uncompressedSize;	//uncompressed size               4 bytes
		unsigned short filenameLength;		//file name length                2 bytes
		unsigned short extraFieldLength;	//extra field length              2 bytes
		//file name (variable size)
		//extra field (variable size)
		//DATA

	};
	struct ArchiveExtraDataRecordDef		//archive extra data signature    4 bytes  (0x08064b50)
	{
		unsigned short extraFieldLength;	//extra field length              2 bytes
		//extra field data                (variable size)
	};

	struct CentralDirectoryStructureDef		//central file header signature   4 bytes  (0x02014b50)
	{
		unsigned short versionMadeBy;		//version made by                 2 bytes
		unsigned short version;				//version needed to extract       2 bytes
		unsigned short purposeFlg;			//general purpose bit flag        2 bytes
		unsigned short compressionMethod;	//compression method              2 bytes
		unsigned short lastModFileTime;		//last mod file time              2 bytes
		unsigned short lastModFileDate;		//last mod file date              2 bytes
		unsigned long  crc32;				//crc-32                          4 bytes
		unsigned long  compressedSize;		//compressed size                 4 bytes
		unsigned long  uncompressedSize;	//uncompressed size               4 bytes
		unsigned short filenameLength;		//file name length                2 bytes
		unsigned short extraFieldLength;	//extra field length              2 bytes

		unsigned short fileCommentLength;	//file comment length             2 bytes
		unsigned short diskNumberStart;		//disk number start               2 bytes
		unsigned short internalFileAttributes;//internal file attributes        2 bytes
		unsigned long  externalFileAttributes;//external file attributes        4 bytes
		unsigned long  relativeOffsetOfLocalHeader;//relative offset of local header 4 bytes
		//file name (variable size)
		//extra field (variable size)
		//file comment (variable size)
	};

	struct DigitalSignatureDef				//header signature                4 bytes  (0x05054b50)
	{
		unsigned short sizeOfData;			//size of data                    2 bytes
		//signature data (variable size)
	};

	struct EndOfCcentralDirectoryRecord				//end of central dir signature    4 bytes  (0x06054b50)
	{
		unsigned short numberOfThisDisk;			//number of this disk             2 bytes
		unsigned short startOfTheCentralDirectory;	//version needed to extract       2 bytes
		unsigned short centralDirectoryOnThisDisk;	//central directory on this disk  2 bytes
		unsigned short theCentralDirectory;			//the central directory           2 bytes
		unsigned long  sizeOfTheCentralDirectory;	//size of the central directory   4 bytes
		unsigned long  theStartingDiskNumber;		//the starting disk number        4 bytes
		unsigned short zipFileCommentLength;		//.ZIP file comment length        2 bytes
		//.ZIP file comment       (variable size)
	};


	PKChain.clear();

	
	File.Open(inZIPname , "rb");
	int size = File.getSize();

	//ZIPファイルをパースします。。。
	//どーして zip はこんなにパースしにくいですか? lzhのマシ。
	//作った香具師は氏ね
	char signature[4];
	while( size > File.ftell() )
	{
		this->File.fread(signature , 4);
		if ( ! (signature[0] == 'P' && signature[1] == 'K') )
		{
			throw RException(EXCEPTIONTRACE + inZIPname + "の読み込みに失敗 " + 
				num2str( this->File.ftell() )  + "バイト目のヘッダがPKではありません");
		}
		if (signature[2] == 0x03 && signature[3] == 0x04)
		{
			//local file header signature
			LocalFileHeaderDef header;
			this->File.fread( &header.version , sizeof(header.version) );
			this->File.fread( &header.purposeFlg , sizeof(header.purposeFlg) );
			this->File.fread( &header.compressionMethod , sizeof(header.compressionMethod) );
			this->File.fread( &header.lastModFileTime , sizeof(header.lastModFileTime) );
			this->File.fread( &header.lastModFileDate , sizeof(header.lastModFileDate) );
			this->File.fread( &header.crc32 , sizeof(header.crc32) );
			this->File.fread( &header.compressedSize , sizeof(header.compressedSize) );
			this->File.fread( &header.uncompressedSize , sizeof(header.uncompressedSize) );
			this->File.fread( &header.filenameLength , sizeof(header.filenameLength) );
			this->File.fread( &header.extraFieldLength , sizeof(header.extraFieldLength) );

			File.fseek(header.filenameLength + 	header.extraFieldLength , SEEK_CUR);

			if (header.compressionMethod == 8 || header.compressionMethod == 0)
			{
				if (header.uncompressedSize > 0)
				{
					ZipPos * pos = new ZipPos;
					pos->tell = File.ftell();
					pos->compressedSize = header.compressedSize;
					pos->uncompressedSize = header.uncompressedSize;
					pos->compressionMethod = header.compressionMethod;
					PKChain.push_back(pos);
				}
			}
			File.fseek(header.compressedSize , SEEK_CUR);
		}
		else if (signature[2] == 0x06 && signature[3] == 0x08)
		{
			//Archive extra data record
			ArchiveExtraDataRecordDef header;
			this->File.fread( &header.extraFieldLength , sizeof(header.extraFieldLength) );

			File.fseek(header.extraFieldLength , SEEK_CUR);
		}
		else if (signature[2] == 0x01 && signature[3] == 0x02)
		{
			//central file header signature   
			CentralDirectoryStructureDef header;
			this->File.fread( &header.versionMadeBy , sizeof(header.versionMadeBy) );
			this->File.fread( &header.version , sizeof(header.version) );
			this->File.fread( &header.purposeFlg , sizeof(header.purposeFlg) );
			this->File.fread( &header.compressionMethod , sizeof(header.compressionMethod) );
			this->File.fread( &header.lastModFileTime , sizeof(header.lastModFileTime) );
			this->File.fread( &header.lastModFileDate , sizeof(header.lastModFileDate) );
			this->File.fread( &header.crc32 , sizeof(header.crc32) );
			this->File.fread( &header.compressedSize , sizeof(header.compressedSize) );
			this->File.fread( &header.uncompressedSize , sizeof(header.uncompressedSize) );
			this->File.fread( &header.filenameLength , sizeof(header.filenameLength) );
			this->File.fread( &header.extraFieldLength , sizeof(header.extraFieldLength) );
			this->File.fread( &header.fileCommentLength , sizeof(header.fileCommentLength) );
			this->File.fread( &header.diskNumberStart , sizeof(header.diskNumberStart) );
			this->File.fread( &header.internalFileAttributes , sizeof(header.internalFileAttributes) );
			this->File.fread( &header.externalFileAttributes , sizeof(header.externalFileAttributes) );
			this->File.fread( &header.relativeOffsetOfLocalHeader , sizeof(header.relativeOffsetOfLocalHeader) );

			File.fseek(header.filenameLength + 	header.extraFieldLength + header.fileCommentLength, SEEK_CUR);
		}
		else if (signature[2] == 0x05 && signature[3] == 0x05)
		{
			//central file header signature   
			DigitalSignatureDef header;
			this->File.fread( &header.sizeOfData , sizeof(header.sizeOfData) );

			File.fseek(header.sizeOfData , SEEK_CUR);
		}
		else if (signature[2] == 0x05 && signature[3] == 0x06)
		{
			//End of central directory record:
			EndOfCcentralDirectoryRecord header;
			this->File.fread( &header.numberOfThisDisk , sizeof(header.numberOfThisDisk) );
			this->File.fread( &header.startOfTheCentralDirectory , sizeof(header.startOfTheCentralDirectory) );
			this->File.fread( &header.centralDirectoryOnThisDisk , sizeof(header.centralDirectoryOnThisDisk) );
			this->File.fread( &header.theCentralDirectory , sizeof(header.theCentralDirectory) );
			this->File.fread( &header.sizeOfTheCentralDirectory , sizeof(header.sizeOfTheCentralDirectory) );
			this->File.fread( &header.theStartingDiskNumber , sizeof(header.theStartingDiskNumber) );
			this->File.fread( &header.zipFileCommentLength , sizeof(header.zipFileCommentLength) );

			File.fseek(header.zipFileCommentLength , SEEK_CUR);
		}
		else
		{
			throw RException(EXCEPTIONTRACE + inZIPname + "の読み込みに失敗 " + 
				num2str( this->File.ftell() )  + "バイト目のヘッダがPKですが、サポート外です");
		}
	}
}



//ページの情報を取得します.
void MVFileList::getInfo(int inIndex , ZipReadInfo* outInfo) throw(RException)
{
	ASSERT(inIndex >= 0);
	ASSERT(inIndex < getCount() );

	const ZipPos * pos = PKChain[inIndex] ;


	outInfo->buffer = new char[pos->compressedSize];
	outInfo->compressedSize = pos->compressedSize;
	outInfo->uncompressedSize = pos->uncompressedSize;
	outInfo->compressionMethod = pos->compressionMethod;

	//所定の位置まで戻して...
	this->File.fseek( pos->tell );
	//読み込み
	File.fread( outInfo->buffer , pos->compressedSize);
}

void MVFileList::test()
{
	//テキストファイルで構成した zip が読めますか?
	{
		MVFileList mvFileList;
		mvFileList.Update("test/sample0.zip");

	}
	//bmpファイルで構成した zip が読めますか?
	{
		MVFileList mvFileList;
		mvFileList.Update("test/sample1.zip");

		ASSERT(mvFileList.getCount() == 3);

		
	}

}
