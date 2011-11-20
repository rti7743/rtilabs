// RNiseOle.cpp: RNiseOle クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RNiseOle.h"
#include "MultiString.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RNiseOle::RNiseOle()
{
	TargetWindow = NULL;
}

RNiseOle::~RNiseOle()
{
	//残っているようであれば強制終了
	if (::WaitForSingleObject(Procinfo.hProcess, 0) == WAIT_TIMEOUT)
	{
		TerminateProcess(Procinfo.hProcess, 0);
	}
	//開放.
	::CloseHandle(Procinfo.hProcess);
}

//起動
//例外 RNoComplateException RNoComplateException
void RNiseOle::Exec(HINSTANCE inINstance,HWND isParentWindow ,const string inFilename) throw()
{
	//起動する構文の作成.
	string executableCommand = FindExecutable(inFilename);
	string executableCommandLine;
	//EXEをNiseOLEする馬鹿のために
	if ( stricmp(executableCommand.c_str() , inFilename.c_str() ) != 0)
	{
		executableCommandLine = " \"" +  inFilename + "\"";
	}

	ZeroMemory(&Procinfo,sizeof(Procinfo));

	//起動するまで待つ
	//参考 http://hp.vector.co.jp/authors/VA016117/waitidle.html
	STARTUPINFO	stinfo;

	stinfo.cb=sizeof(stinfo);
	stinfo.lpReserved=NULL;
	stinfo.lpDesktop=NULL;
	stinfo.lpTitle=NULL;
	stinfo.dwFlags=0;
	stinfo.cbReserved2=0;
	stinfo.lpReserved2=NULL;
	stinfo.wShowWindow = SW_HIDE;

	char commandLine[MAX_PATH * 2];
	strcpy( commandLine , executableCommandLine.c_str() );
	//カレントディレクトリは 起動するプログラムの直下です。
	const int lastpos = MultiString::getLastDirectoryPos(executableCommand);
	string currentDir = executableCommand.substr( 0 , lastpos ) ;

	if ( ! CreateProcess(executableCommand.c_str() , commandLine ,NULL,NULL,FALSE,0,NULL,currentDir.c_str() ,&stinfo,&Procinfo) )
	{
		throw RWin32Exception("実行することができませんでした\n" + executableCommand);
	}
	if ( WaitForInputIdle(Procinfo.hProcess,10000))
	{
		throw RWin32Exception("起動できませんでした\n" + executableCommand);
	}

	//起動させたアプリケーションのウィンドウの取得
	//参考 http://techtips.belution.com/ja/vc/0024/
	TargetWindow = NULL;
	EnumThreadWindows(Procinfo.dwThreadId, &RNiseOle::FindWindowProc, (LPARAM)&TargetWindow);
	if (!TargetWindow)
	{
		throw RNoComplateException("起動させたウィンドウが見つかりません");
	}

	//Kinderのソースを参考にしました。
	//親の変更
	SetParent(TargetWindow , isParentWindow);
	//スタイルの変更
	DWORD Style;
	Style = GetWindowLong(TargetWindow,GWL_EXSTYLE);
	Style = Style | WS_EX_MDICHILD;
	SetWindowLong(TargetWindow,GWL_EXSTYLE,Style);

	Style = GetWindowLong(TargetWindow,GWL_STYLE);
	Style = Style & (~WS_CAPTION);
	Style = Style & (~WS_THICKFRAME);
	SetWindowLong(TargetWindow,GWL_STYLE,Style);

	//RWindow に登録.
	MountWindow(TargetWindow);

}

//拡張子に対応した実行ファイル名の取得
string RNiseOle::FindExecutable(const string inFilename) const
{
	//取得した実行するコマンド名
    char  szExeFilePath[MAX_PATH];
    //  拡張子に対応した実行ファイル名の取得
    ::FindExecutable( inFilename.c_str(), NULL, szExeFilePath );

	return szExeFilePath;
}

// EnumWindowsProcコールバック関数
BOOL CALLBACK RNiseOle::FindWindowProc(HWND hwnd, LPARAM lParam)
{
    HWND* pphWnd = (HWND*)lParam;
    *pphWnd = hwnd;
    return FALSE; 
}


//タイトルを取得する
string RNiseOle::getTitle() const 
{
	char buf[MAX_PATH];
	::GetWindowText(TargetWindow , buf , MAX_PATH);

	return buf;
}

