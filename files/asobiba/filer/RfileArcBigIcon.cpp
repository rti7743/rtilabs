// RfileArcBigIcon.cpp: RfileArcBigIcon クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileArcBigIcon.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////



//登録します。
void RfileArcBigIcon::Attach(const char* inPath,const char* inName,DWORD inSize,DWORD inDate,WORD inRatio ,const char* inMode,const char* inAttribute)
{
	this->Path = inPath;
	this->Name = inName;
	this->Size = inSize;
	this->Date = inDate;
	this->Ratio = inRatio;
	this->Mode = inMode;

	if ( strcmp( inAttribute , "DIR" ) != 0)
	{//ファイルだったら...
		string ext = Rfiles::Filename2Ext(inName) ;
		this->FileModify = this->NormalLoadModifty(ext ,inName);
	}
	else
	{//ディレクトリだったら...
		this->FileModify = this->DirectoryLoadModifty();
	}

	ASSERT(this->FileModify);
}


//値の取得.
const string RfileArcBigIcon::getVars(__FILECOLUMN_TYPE inType)  const
{
	switch(inType)
	{
	case __FILECOLUMN_TYPE_NAME:			//名前
		return Name;
	case __FILECOLUMN_TYPE_VIRTUALPATH:		//仮想パス(圧縮ファイルなどの　圧縮ファイルの先頭を / とするパス)
		return Path;
	case __FILECOLUMN_TYPE_EXT:				//タイプ(拡張子に関連付けられている名前)
		return FileModify->getFileType();
	case __FILECOLUMN_TYPE_SIZE:			//サイズ
		return "";
	case __FILECOLUMN_TYPE_DATE:			//更新日付
		return "";
	case __FILECOLUMN_TYPE_ARCMODE:			//圧縮モード
		return "";
	case __FILECOLUMN_TYPE_RATIO:			//圧縮レート(圧縮率)
		return "";
	}
	return "";
}

//イメージの取得.
const int RfileArcBigIcon::getImageIndex()  const
{
	return FileModify->getIndex();
}


//普通なファイル修飾子の取得.
FileModifyData* RfileArcBigIcon::NormalLoadModifty(string inExt , string inFullFilename )
{
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	FileModifyData* p = fmc->Search( inExt );
	if (!p)
	{
		//ないので取得.
		SHFILEINFO  shfi = {0};
		SHGetFileInfo( ('.' + inExt).c_str() , FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES | SHGFI_DISPLAYNAME | SHGFI_TYPENAME | SHGFI_ICON);

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

//ディレクトリのファイル修飾子の取得.
FileModifyData* RfileArcBigIcon::DirectoryLoadModifty()
{
	//とりあえず、ふつーのディレクトリアイコンを返します。
	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	return fmc->SearchSystem(FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFOLDER);
}
