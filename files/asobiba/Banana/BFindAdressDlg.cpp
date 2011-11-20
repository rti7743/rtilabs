// BFindAdressDlg.cpp: BFindAdressDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "BFindAdressDlg.h"
#include "WindowsMain.h"
#include "BBanana.h"
#include "resrc1.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BFindAdressDlg::BFindAdressDlg()
{

}

BFindAdressDlg::~BFindAdressDlg()
{

}

void BFindAdressDlg::Create(BBanana	*	poolBanana) throw(RWin32Exception)
{
	RDialog::Create(WindowsMain::getMainSystem()->getInstance() ,
					WindowsMain::getMainSystem()->getWindowHandle() , 
					"IDD_ADDRESS_FIND_DIALOG");

	Word.MountWindow( this->GetDlgItem(IDC_WORD_EDIT) );
	ListView.MountWindow( this->GetDlgItem(IDC_LIST) );
	ListView.InsertColumn(0 , 0 , "" , 320, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM , LVCFMT_LEFT);

	this->Banana = poolBanana;
}

void BFindAdressDlg::OnCancel()
{
	this->Hide();
}

void BFindAdressDlg::OnSearch()
{
	string word = Word.getText();

	if ( word.empty() )
	{
		//文字列がありません
		TRACE( "検索文字列を入れてください" );
		return ;
	}

	RGISSmallFindCityToNumberConvert cityToNumberConvert;
	Banana->getGisMaster()->AllCityToCodeTable(&cityToNumberConvert);

	BAddressParse parse;
	if ( ! parse.Parse(word , cityToNumberConvert.getTable() , cityToNumberConvert.getTableCount()   ) )
	{
		TRACE( "住所をパースできません" );
		return ;
	}

	FindAddress.Delete();
	Banana->getGisMaster()->FindAddress(parse.getCode() , parse.getMachiCyoume() , parse.getBanchi() , &FindAddress);

	ListView.DeleteItemAll();

	int count = FindAddress.getCount();
	for(int i = 0;  i < count ; i++)
	{
		const RGISSmallFindObject * findObject = FindAddress.getPoint(i);
		int itemIndex = ListView.GetItemCount();
		ListView.SetItem(itemIndex  , 0 , findObject->getName() );
	}
}

bool BFindAdressDlg::OnDoubleClick()
{
	unsigned int select = ListView.getNowSelected();
	if (FindAddress.getCount() < select)
	{
		return false;	//範囲外選択.
	}
	const RGISSmallFindObject * findObject = FindAddress.getPoint(select);
	Banana->LookAt( findObject->getPos() );

#ifdef _WIN32_WCE
	//CE の場合は窓を消したほうかわかりやすい
	this->Hide();
#endif
	return true;
}

bool BFindAdressDlg::OnKeyDown(unsigned char inKey)
{
	switch(inKey)
	{
	case VK_ESCAPE:
		OnCancel();
		break;
	case VK_RETURN:
		OnSearch();
		break;
	}
	return true;
}

//ウィンドウプロシージャ.
LRESULT BFindAdressDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_SIPDOWN ;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDOK:	//検索
				OnSearch();
				break;
			case IDCANCEL:	//ｷｬﾝｾﾙ
				OnCancel();
				break;
			}
		}
		break;
	case WM_KEYDOWN:
		OnKeyDown( (unsigned char)inWParam );
		break;
	case WM_NOTIFY:
		{
			LPNMHDR nmhdr = (LPNMHDR)inLParam;
			if (nmhdr && nmhdr->hwndFrom == ListView.getWindowHandle() )
			{
				switch(nmhdr->code)
				{
				case NM_CLICK:	//ダブルクリックされたとき
					{
						if (this->OnDoubleClick() )
						{
							return true;
						}
					}
					break;
				case LVN_KEYDOWN:
					{
					#ifndef _WIN32_WCE
						if (this->OnKeyDown( ((LPNMKEY)inLParam)->nVKey) )
					#else
						if (this->OnKeyDown( ((LPNMKEY)inLParam)->wVKey) )
					#endif
						{
							return true;
						}
					}
				}
			}
			else return false;
		}
		break;
	}
	return FALSE;
}

