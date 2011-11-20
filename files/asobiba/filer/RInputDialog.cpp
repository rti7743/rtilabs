// RInputDialog.cpp: RInputDialog クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RInputDialog.h"
#include "WindowsMain.h"
#include "IMyEnumString.h"		//オートコンプリート用.
#include "resource.h"
#include "RDataLoad.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RInputDialog::RInputDialog() 
{
	AutoComplete = NULL;
	EnumStr = NULL;
}

RInputDialog::~RInputDialog()
{
	if (AutoComplete) AutoComplete->Release();
//	if (EnumStr)	EnumStr->Release();
}

string RInputDialog::DoModal(string inTitle , string inAutoCompleteName , string inDefualt)	throw(RWin32Exception,RNoComplateException)
{
	Title = inTitle;
	AutoCompleteName = inAutoCompleteName;
	Defualt = inDefualt;

	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	int ret =::DialogBoxParam(mainSystem->getInstance() ,"INPUTDIALOG", mainSystem->getWindowHandle() , (DLGPROC)DlgProcStatic ,(LPARAM) this);
	if ( ret == -1 )
	{
		throw RWin32Exception("入力ダイアログを作成できません");
	}
	//キャンセルされた場合
	if (ret == IDCANCEL)
	{
		throw RNoComplateException("入力ダイアログでキャンセルされました");
	}
	if (InputData.empty() )
	{
		throw RNoComplateException("入力ダイアログで NULL が入力されました");
	}

	return InputData;
}

//ウィンドウプロシージャ.
LRESULT RInputDialog::DlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		return OnInitDialog() != false;
	case WM_COMMAND:
		{
			int p = LOWORD(wparam);
			switch (LOWORD(wparam)) 
			{
			case IDOK:
				OnOK();
				break;
			case IDCANCEL:
				OnCancel();
				break;
			}
		}
		break;
	}
	return FALSE;
}

//OKをおされたときの処理.
void RInputDialog::OnOK()
{
	//エディットボックス入力されている値を  InputData に取得.
	HWND	EditWindowHandle = GetDlgItem(this->WindowHandle, IDC_INPUTEDIT);
	char	buf[MAX_PATH];
	GetWindowText(EditWindowHandle , buf , MAX_PATH); 
	InputData = buf;

	::EndDialog(this->WindowHandle , IDOK);
}

//キャンセルをおされたときの処理.
void RInputDialog::OnCancel()
{
	::EndDialog(this->WindowHandle , IDCANCEL);
}

bool RInputDialog::OnInitDialog()
{

	const WindowsMain * mainSystem = WindowsMain::getMainSystem();
	try
	{
		//タイトルにセット
		SetWindowText(this->WindowHandle , Title.c_str() );

		//エディットボックスにディフォルトをセット.
		HWND	EditWindowHandle = GetDlgItem(this->WindowHandle, IDC_INPUTEDIT);
		SetWindowText(EditWindowHandle , Defualt.c_str() );

		//補完システムを作成.
		EnumStr = new IMyEnumString(NULL, LoadComplete(AutoCompleteName) );

		// IAutoCompleteインターフェイスを作成
		if (CoCreateInstance(CLSID_AutoComplete, NULL, CLSCTX_INPROC_SERVER,
				IID_IAutoComplete, (void**)&AutoComplete) != S_OK)
		{
			throw RNotSupportException("CoCreateInstance に失敗しました.");
		}

		// エディットボックスを対象にする.
		if (AutoComplete->Init(EditWindowHandle, EnumStr, NULL, NULL) != S_OK)
		{
			throw RNotSupportException("登録できません.");
		}
//	IAutoComplete2* pAuto2 = NULL;
//	if (SUCCEEDED(pAutoComp->QueryInterface(IID_IAutoComplete2, (LPVOID*)&pAuto2))){
//		pAuto2->SetOptions(ACO_AUTOSUGGEST);
//		pAuto2->Release();
//	}
	}
	catch(RException e)
	{
		printf("オートコンプリート作成でエラーが発生しました:\n%s\n" , e.getMessage() );
		return false;
	}

	return true;
}

//コンプリートデータの読み込み.
WStringList RInputDialog::LoadComplete(string inName)
{
	string dataes;
	try
	{
		dataes = RDataLoad::Load("filer.dat" , "AutoComplete" , inName);
	}
	catch(RException e)
	{
		printf("コンプリートデータの取得に失敗\n%s\n",e.getMessage() );
		return WStringList();
	}

	USES_CONVERSION;
	WStringList ret;
	// \t 区切りなので..
	int findstart = 0;
	int hitpos = 0;
	while(1)
	{
		hitpos = dataes.find('\t' , findstart);
		if (hitpos == -1) break;
		string one = dataes.substr(findstart , hitpos - findstart);

		ret.push_back( T2COLE(one.c_str()) );

		findstart = hitpos + 1;
	}

	return ret;
}

//コンプリートデータの書き込み
void RInputDialog::SaveComplete(string inName , WStringList inTargetString)
{
	USES_CONVERSION;

	string writeString;
	for(int i = 0; i < inTargetString.size() ; i++)
	{
		 writeString += string( OLE2CT( inTargetString[i].c_str() ) ) + "\t";
	}
	try
	{
		RDataLoad::Save("filer.dat" , "AutoComplete" , inName,writeString);
	}
	catch(RException e)
	{
		printf("コンプリートデータの書き込みに失敗\n%s\n",e.getMessage() );
	}
}

