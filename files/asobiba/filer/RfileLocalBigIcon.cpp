// RfileLocalBigIcon.cpp: RfileLocalBigIcon クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileLocalBigIcon.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//登録します。
void RfileLocalBigIcon::Attach(const Rfiles* inRfiles ,const char* inName,__int64 inSize,__int64 inDate,DWORD inAttrib)
{
	this->Name = inName;
	this->Size = inSize;
	this->Date = inDate;
	this->Attrib = inAttrib;

	//ファイル修飾子の取得
	if ( IsDirectory() ) 
	{
		this->FileModify = this->DirectoryLoadModifty( inRfiles->getPath() + inName);
	}
	else
	{
		string ext = Rfiles::Filename2Ext(inName) ;

		if (ext == "EXE")
		{//特殊ファイル キャッシュしません.
			this->FileModify = this->SpecialLoadModifty(ext ,inRfiles->getPath() + inName,FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_EXEFILE);
		}
		else if (ext == "LNK" || ext == "ICO")
		{//特殊ファイル キャッシュしません.
			this->FileModify = this->SpecialLoadModifty(ext ,inRfiles->getPath() + inName,FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFILE);
		}
		else
		{//一般ファイル キャッシュします。
			this->FileModify = this->NormalLoadModifty(ext ,inRfiles->getPath() + inName);
		}
	}
	ASSERT(this->FileModify);
}


//値の取得.
const string RfileLocalBigIcon::getVars(__FILECOLUMN_TYPE inType) const
{
	switch(inType)
	{
	case __FILECOLUMN_TYPE_NAME:			//名前
		return Name;
	case __FILECOLUMN_TYPE_EXT:				//タイプ(拡張子に関連付けられている名前)
		return FileModify->getFileType();
	case __FILECOLUMN_TYPE_SIZE:			//サイズ
		return "";
	case __FILECOLUMN_TYPE_DATE:			//更新日付
		return "";
	case __FILECOLUMN_TYPE_ATTRIB:			//属性
		return "";
	}
	return "";
}

//イメージの取得.
const int RfileLocalBigIcon::getImageIndex() const
{
	return FileModify->getIndex();
}

//普通なファイル修飾子の取得.
FileModifyData* RfileLocalBigIcon::NormalLoadModifty(string inExt , string inFullFilename )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	FileModifyData* p = fmc->Search( inExt );
	if (!p)
	{
		//ないので取得.
		SHFILEINFO  shfi = {0};
		SHGetFileInfo( inFullFilename.c_str() , FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_TYPENAME);

		//取得失敗、システムの普通のファイルのアイコンを返します。
		if (shfi.hIcon == NULL) 
		{
			p = fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFILE);
		}
		else
		{//アイコン取得完了
			//ファイルタイプ取得できやんした?
			if (shfi.szTypeName[0] != '\0')
				p = fmc->Tearch(inExt , shfi.hIcon, shfi.szTypeName );
			else
				p = fmc->Tearch(inExt , shfi.hIcon,  inExt + "ファイル");
			DestroyIcon(shfi.hIcon);
		}
	}
	return p;
}

//特殊なファイル修飾子の取得.
FileModifyData* RfileLocalBigIcon::SpecialLoadModifty(string inExt , string inFullFilename ,
								FileModifyChacheLargeWin32::Modifty inDefaultIconIndex )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	//特殊ファイル キャッシュしません.
	SHFILEINFO  shfi = {0};
	SHGetFileInfo( inFullFilename.c_str() , FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_TYPENAME);
	//取得失敗、システムの普通のファイルのアイコンを返します。
	if (shfi.hIcon == NULL) 
	{
		return fmc->SearchSystem(inDefaultIconIndex);
	}
	else
	{//アイコン取得完了
		FileModifyData* p ;
		//特殊なアイコンなので非キャッシュとして登録します。
		//ファイルタイプ取得できやんした?
		if (shfi.szTypeName[0] != '\0')
			p = fmc->Tearch( shfi.hIcon, shfi.szTypeName );
		else
			p = fmc->Tearch( shfi.hIcon,  inExt + "ファイル");
		DestroyIcon(shfi.hIcon);
		return p;
	}
}

//ディレクトリのファイル修飾子の取得.
FileModifyData* RfileLocalBigIcon::DirectoryLoadModifty(string inFullFilename )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();

	SHFILEINFO  shfi = {0};
	SHGetFileInfo( inFullFilename.c_str(), FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_DISPLAYNAME );

	//取得失敗、システムの普通のディレクトリのアイコンを返します。
	if (shfi.hIcon == NULL) 
	{
		return fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
	}
	else
	{//アイコン取得完了
		FileModifyData* p ;
		if (shfi.dwAttributes & SFGAO_SHARE)
		{//共有ディレクトリ
			if ( FILE_EXIST((inFullFilename + "\\desktop.ini").c_str()) )
			{
				p = fmc->Tearch(shfi.hIcon, FileModifyChacheLargeWin32::FOLDER_STRING );
			}
			else
			{
				p = fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
			}
			//本当はmix とかごにょごにょする.
		}
		else
		{
			if ( FILE_EXIST((inFullFilename + "\\desktop.ini").c_str()) )
			{
				p = fmc->Tearch(shfi.hIcon, FileModifyChacheLargeWin32::FOLDER_STRING );
			}
			else
			{
				p = fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
			}
		}
		DestroyIcon(shfi.hIcon);
		return p;
	}
}

