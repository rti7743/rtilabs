// RfileNetResourceBigIcon.cpp: RfileNetResourceBigIcon クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileNetResourceBigIcon.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//登録します。
void RfileNetResourceBigIcon::Attach(const char* inName,const char* inComment,DWORD inDisplayType )
{
	this->Name = inName;
	if (inComment)	this->Comment = inComment;

	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	switch (  inDisplayType )
	{
	case RESOURCEDISPLAYTYPE_DOMAIN:		//ドメイン.
	case RESOURCEDISPLAYTYPE_NETWORK:
	case RESOURCEDISPLAYTYPE_SHAREADMIN:
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_DOMAIN );
		break;
	case RESOURCEDISPLAYTYPE_SERVER:		//サーバ.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_SERVER );
		break;
	case RESOURCEDISPLAYTYPE_SHARE:			//共有.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_SHAREPOINT_DISK );
		break;
	case RESOURCEDISPLAYTYPE_GROUP:			//ワークグループ.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_WORKGROUP );
		break;
	default:								//不明.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFILE );
		break;
	}

	ASSERT(this->FileModify);
}


//値の取得.
const string RfileNetResourceBigIcon::getVars(__FILECOLUMN_TYPE inType)  const
{
	switch(inType)
	{
	case __FILECOLUMN_TYPE_NAME:			//名前
		return Name;
	case __FILECOLUMN_TYPE_COMMENT:			//コメント
		return Comment;
	}
	return "";
}

//イメージの取得.
const int RfileNetResourceBigIcon::getImageIndex()  const
{
	return FileModify->getIndex();
}


