// RDiskUtil.cpp: RDiskUtil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RDiskUtil.h"
#include "RConv.h"
#include <imagehlp.h>

#pragma comment(lib,"imagehlp.lib")


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RDiskUtil::RDiskUtil()
{

}

RDiskUtil::~RDiskUtil()
{

}

//コピー(ディレクトリの場合以下全部)
void RDiskUtil::Copy(const string & inFilename,const string & inTo, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);
	char * to = RDiskUtil::getDoubleNULLFilename(inTo);


	SHFILEOPSTRUCT op;
	op.hwnd  = NULL;
	op.wFunc = FO_COPY;						//動作の種類
	op.pFrom = filename;				//対象のファイル
	op.pTo   = to;                        
	op.fFlags= appendFlg;         
	op.fAnyOperationsAborted = true; //キャンセルできるか？
	int ret = SHFileOperation(&op);

	delete [] filename;
	delete [] to;

	if ( ret != 0)
		throw RWin32Exception(ret , "コピーに失敗しました target:" + inFilename + " To:" + inTo);
}
//移動(ディレクトリの場合以下全部)
void RDiskUtil::Move(const string & inFilename,const string & inTo, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);
	char * to = RDiskUtil::getDoubleNULLFilename(inTo);

	SHFILEOPSTRUCT op;
	op.hwnd  = NULL;
	op.wFunc = FO_MOVE;							//動作の種類
	op.pFrom = filename;						//対象のファイル
	op.pTo   = to;                        
	op.fFlags= appendFlg;         
	op.fAnyOperationsAborted = true;			//キャンセルできるか？
	int ret = SHFileOperation(&op);

	delete [] filename;
	delete [] to;

	if ( ret != 0)
		throw RWin32Exception(ret , "移動に失敗しました target:" + inFilename + " To:" + inTo);
}
//削除(ディレクトリの場合以下全部)
void RDiskUtil::Delete(const string & inFilename, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);

	SHFILEOPSTRUCT op = {0};
	op.hwnd  = NULL;
	op.wFunc = FO_DELETE;				  //動作の種類
	op.pFrom = filename;				  //対象のファイル
	op.pTo   = NULL;                        
	op.fFlags= appendFlg;				  //Undoを許可するか？
	op.fAnyOperationsAborted = true;	  //キャンセルできるか？
	int ret = SHFileOperation(&op);
	delete [] filename;

	if ( ret != 0)
		throw RWin32Exception("削除に失敗しました target:" + inFilename);

}
//ゴミ箱へ
void RDiskUtil::Gomi(const string & inFilename, bool isSilent ) throw(RWin32Exception)
{
	FILEOP_FLAGS appendFlg = isSilent ? FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION: 0;

	char * filename = RDiskUtil::getDoubleNULLFilename(inFilename);

	SHFILEOPSTRUCT op;
	op.hwnd  = NULL;
	op.wFunc = FO_DELETE;						//動作の種類
	op.pFrom = filename;						//対象のファイル
	op.pTo   = NULL;                        
	op.fFlags= FOF_ALLOWUNDO | appendFlg;       //Undoを許可するか？
	op.fAnyOperationsAborted = true;			//キャンセルできるか？
	int ret = SHFileOperation(&op);

	delete [] filename;

	if ( ret != 0)
		throw RWin32Exception(ret , "ゴミ箱に移動に失敗しました target:" + inFilename );
}

//ファイル名は \0\0 で終わっている必要があるため付加する
char* RDiskUtil::getDoubleNULLFilename(const string & inStr)
{
	const string newStr = RConv::Replace(inStr , "/" , "\\");
	//ファイル名は \0\0 で終わっている必要があるため付加する.
	char * filename = new char[ newStr.size() + 2 ];
	strcpy(filename , newStr.c_str());
	filename[ newStr.size() + 1 ] = '\0';
	return filename;
}

//ディレクトリの作成
void RDiskUtil::MakeDirectory(const string & inDirectoryName)
{
	const string newStr = RConv::Replace(inDirectoryName , "/" , "\\");

	if (newStr.size() > 2 && newStr[1] == ':')
	{
		MakeSureDirectoryPathExists( newStr.c_str() );
	}
	else
	{
		::CreateDirectory(newStr.c_str() , NULL);
	}
}


//ファイルが存在するか?
bool RDiskUtil::Exist(const string & inFileName)
{
	const string newStr = RConv::Replace(inFileName , "/" , "\\");
	return (0xFFFFFFFF != GetFileAttributes(newStr.c_str() ));
}
