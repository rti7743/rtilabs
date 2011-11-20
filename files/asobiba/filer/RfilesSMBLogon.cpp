// RfilesSMBLogon.cpp: RfilesSMBLogon クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesSMBLogon.h"
#include "MultiString.h"
#include "ROutOfRangeException.h"

//列挙できるネットワークリソースの最大値
const int MAX_NETWORKRESOURCE	=	1024;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//初期化できなかった場合は RNotSupportException を投げる.
RfilesSMBLogon::RfilesSMBLogon(__RFILETYPE inRfileType , const string inPath) 
{
	RfileType = inRfileType;
	Path = inPath;

	switch(RfileType)
	{
	case __RFILETYPE_SMBLOGON_REPORT:
	case __RFILETYPE_SMBLOGON_BIGICON:
		break;
	default:
		throw RNotSupportException("RfilesSMBLogon::指定された ファイル形式 %d は サポートしていません。",RfileType);
		break;
	}
}
//final
RfilesSMBLogon::~RfilesSMBLogon()
{
}

//一覧取得
void RfilesSMBLogon::ls()
{
	RTable * rt = getWritebleTable();
	rt->clear();
	__FILECOLUMN_TYPE sortkey = rt->GetSortKey();

	string Provider ,RemoteName;

	NETRESOURCE netResouce = {0};
	netResouce.dwScope = RESOURCE_GLOBALNET;
	netResouce.dwType = RESOURCETYPE_ANY;
	netResouce.dwUsage = RESOURCEUSAGE_CONTAINER;

	NETRESOURCE *nr = &netResouce;

	try
	{
		if (Path.substr(0,6) == "smb://")
		{
			int dilimiter[MAX_DIMLIMITERES] ;
			int maxDilimiter = MultiString::setPathDelimiteres(Path.c_str() , '\\' , dilimiter ,MAX_DIMLIMITERES);
			
			ASSERT(maxDilimiter <= MAX_DIMLIMITERES);
			switch(maxDilimiter)
			{
			case 1:	// smb://\\  //サービスプロバイダの取得.
				nr = NULL;
				break;
			case 2:	// smb://\\Microsoft Windows Network\\		//ドメインの検索.
				Provider = Path.substr( dilimiter[0]+1 , dilimiter[1]-(dilimiter[0]+1) );
				netResouce.lpProvider = (char*) Provider.c_str();
				netResouce.lpRemoteName = NULL;
				break;
			case 3:	// smb://\\Microsoft Windows Network\\DOMAIN\\		//マシンの検索
				Provider = Path.substr( dilimiter[0]+1 , dilimiter[1]-(dilimiter[0]+1) );
				netResouce.lpProvider = (char*) Provider.c_str();
				RemoteName = Path.substr( dilimiter[1]+1 , dilimiter[2]-(dilimiter[1]+1) );
				netResouce.lpRemoteName = (char*) RemoteName.c_str();
				break;
			default:
				throw ROutOfRangeException("ネットワークリソースのパスの解釈に問題があります");
			}
			netResouce.dwDisplayType = RESOURCEDISPLAYTYPE_DOMAIN;
		}
		else
		{	// \\\\マシン名 の表記

			//最後の \\ を削る.
			RemoteName = Path.substr( 0 , Path.size() - 1);

			netResouce.dwType = RESOURCETYPE_ANY;
			netResouce.dwDisplayType = RESOURCEDISPLAYTYPE_SERVER;
			netResouce.dwUsage = RESOURCEUSAGE_ALL;
			netResouce.lpProvider = "Microsoft Windows Network";
			netResouce.lpRemoteName = (char*) RemoteName.c_str();
		}

		//列挙の開始.
		HANDLE hEnum;
		DWORD ret=WNetOpenEnum(RESOURCE_GLOBALNET,RESOURCETYPE_ANY,0, nr,&hEnum);
		if(ret!=NO_ERROR)
		{
			throw RWin32Exception("ネットワークリソースの取得に失敗しました.");
		}

		NETRESOURCE Array[MAX_NETWORKRESOURCE];
		DWORD Entries = 0xFFFFFFFF;
		DWORD ArraySize = ArraySize=sizeof(Array);
		ret=WNetEnumResource(hEnum,&Entries,Array,&ArraySize);
		if(ret==NO_ERROR)
		{
			DWORD i;
			for(i=0;i<Entries;++i)
			{
				if(Array[i].dwUsage & RESOURCEUSAGE_CONTAINER || Array[i].dwUsage & RESOURCEUSAGE_CONNECTABLE)
				{ // リソースを取得しました。
					RfileNetResourceBigIcon* p = (RfileNetResourceBigIcon*) RfileFactory::New(RfileType);

					string name;
					name = Array[i].lpRemoteName;
					if (Array[i].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER)
					{	//サーバー \\マシン名 とかとでるので先頭の \\ を削除
						if (name[0] == '\\' ) name = name.substr(2);
					}
					else if (Array[i].dwDisplayType == RESOURCEDISPLAYTYPE_SHARE)
					{	//共有名が \\マシン名\\共有 となるので \\マシン名\\ まで削除
						name = name.substr( MultiString::getLastDirectoryPos(name)+1 );
					}

					p->Attach(name.c_str() ,Array[i].lpComment , Array[i].dwDisplayType );
					p->SortAttach( sortkey );
					rt->add( p );
				}
			}
		}
		else if(ret!=ERROR_NO_MORE_ITEMS)
		{
			WNetCloseEnum(hEnum);
			throw RWin32Exception("ネットワークリソースの列挙中にエラーが発生しました.");
		}

		WNetCloseEnum(hEnum);
	}
	catch(RException e)
	{
		TRACE("取得中に RException が発生しました 理由:%s\n" , e.getMessage() );
	}

	rt->Sort();
	rt->createAllList( );
}
//一覧取得
void RfilesSMBLogon::ls(string inFilter)
{
	//区別します?
	ls();
}

//削除
void RfilesSMBLogon::rm(string inFilename)
{
}
//リネーム
void RfilesSMBLogon::rename(string inSrcFilename,string inDestFilename)
{
}
//種類を返す.
__RFILETYPE		RfilesSMBLogon::getRfileType()	const
{
	return RfileType;
}
//パスの取得
string RfilesSMBLogon::getPath() const 
{
	return Path;
}
//パスの設定
void		RfilesSMBLogon::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTableを作成します。
//ls を使う前に呼んでください。
//作成できない __FILECOLUMN_TYPE が渡されたら例外 RNotSupportExceptionを返します。
void RfilesSMBLogon::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	switch(inKey)
	{
	case __FILECOLUMN_TYPE_NAME:
		setTable( new RTableEasy(inKey,IsAscending) );
		break;
	case __FILECOLUMN_TYPE_COMMENT:
		setTable( new RTableMuchDuplication<string>(inKey,IsAscending) );
		break;
	default:
		throw RNotSupportException("指定されたタイプのカラム %d はサポートされていません",inKey);
		break;
	}
	return;
}

//テスト.
void RfilesSMBLogon::test()
{
}
