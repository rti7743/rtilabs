// SolvablePathSMBLogon.cpp: SolvablePathSMBLogon クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SolvablePathSMBLogon.h"
#include "SolvablePathSpecialFolder.h"
#include "RConv.h"
#include "MultiString.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//そのパスを解決してみる
bool SolvablePathSMBLogon::DoSolvable(string inPath) 
{
	//先頭が file:/// だった場合、その部分をカット.
	if ( inPath.substr(0,8) == "file:///" )
	{
		inPath = inPath.substr(8);
		// / を \\ にする.
		replace( inPath.begin() , inPath.end() , '/','\\');
		//UrlEncode を取り除く.
		inPath = RConv::UrlEncode(inPath);
	}

	//先頭が \\ でなければダメ
	if (inPath.substr(0,2) != "\\\\")
	{
		//ただし smb:// の場合は、ドメインログインとして扱う. 独自仕様(w
		if (inPath.substr(0,6) != "smb://")
		{
			return false;	//ぼつ.
		}
		else
		{
			//最後が \\ でない場合は最後に \\ を補完します。
			if ( MultiString::IsPathToComplement(inPath) )
			{
				inPath += "\\";
			}
			//パスの深さが 3以下であることが必須   substr は 先頭の smb:// を除くため.
			//smb://\\Microsoft Windows Network\\DOMAIN\\ .

			int dilimiter[MAX_DIMLIMITERES] ;
			int maxDilimiter = MultiString::setPathDelimiteres(inPath.c_str() , '\\' , dilimiter ,MAX_DIMLIMITERES);
			if ( maxDilimiter >= 4)
			{
				// \\マシン名 として評価してみます.
				return DoSolvable( string("\\") + inPath.substr(dilimiter[2]) );
			}
		}
	}
	else
	{
		//最後が \\ でない場合は最後に \\ を補完します。
		if ( MultiString::IsPathToComplement(inPath) )
		{
			inPath += "\\";
		}

		//パスの深さが 1以下であることが必須   substr は 先頭の \\ を除くため.
		if ( MultiString::setPathDepth(inPath.substr(2) ) >= 2)
		{
			return false;	//ぼつ
		}
	}
	this->setPath(inPath);

	//OK
	return true;
}

//一つディレクトリを上がる
string SolvablePathSMBLogon::UpDirectory() const
{
	const string path = getPath() ;

	//先頭が \\ だったら...
	if (path.substr(0,2) == "\\\\")
	{	//共有の情報を取得します。

		//最後の \\ を削る.
		string remoteName = path.substr( 0 , path.size() - 1);

		NETRESOURCE srcNr;
		srcNr.lpProvider = "Microsoft Windows Network";
		srcNr.dwType = RESOURCETYPE_DISK;
		srcNr.lpRemoteName = (char*) remoteName.c_str();

		DWORD bufSize = MAX_PATH * 10;	//適当
		NETRESOURCE *destNr = (NETRESOURCE*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, bufSize);

		if ( WNetGetResourceParent( &srcNr , destNr , &bufSize) != NO_ERROR)
		{
			GlobalFree(destNr);
			//取得に失敗.. どうする? マイネットワークにでも戻すか?
			return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_NETWORK);
		}

		string ret = string("smb://\\") + destNr->lpProvider + string("\\") + destNr->lpRemoteName + string("\\");

		GlobalFree(destNr);

		return ret;
	}

	//ただし smb:// の場合は、ドメインログインとして扱う. 独自仕様(w
	//んで、smb:// 以上にあがる場合は マイネットワークに飛ばす.
	if (path.substr(0,6) == "smb://" && MultiString::setPathDepth(path) == 1)
	{
		return SolvablePathSpecialFolder::getSpecialFolder(CSIDL_NETWORK);
	}

	const int lastpos = MultiString::getLastDirectoryPos( path );
	return path.substr( 0 , lastpos + 1) ;
}

void SolvablePathSMBLogon::Test()
{
	SolvablePathSMBLogon sp;

	//パスが解決できるか?
//	ASSERT( sp.DoSolvable("c:\\") );
//	ASSERT( sp.getPath() == "c:\\" );

}


