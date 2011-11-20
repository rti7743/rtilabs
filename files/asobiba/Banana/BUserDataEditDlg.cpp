// BUserDataEditDlg.cpp: BUserDataEditDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BUserDataEditDlg.h"
#include "WindowsMain.h"
#include "RStdioMemoryWrite.h"
#include "resrc1.h"
#include "RGISSmallGlobalToBlock.h"
#include "BUserDataIconDlg.h"
#include "BConf.h"
#include "RMessageBox.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BUserDataEditDlg::BUserDataEditDlg()
{

}

BUserDataEditDlg::~BUserDataEditDlg()
{

}


void BUserDataEditDlg::Create(BBanana	*	poolBanana , const RGISSmallShareImage* poolShareImage) throw(RWin32Exception)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();
	RDialog::Create(winMain->getInstance() ,
					winMain->getWindowHandle() , 
					"IDD_USERDATAEDIT_DIALOG");

	Name.MountWindow( this->GetDlgItem(IDC_NAME_EDIT) );
	Comment.MountWindow( this->GetDlgItem(IDC_COMMENT_EDIT) );
	Catego.MountWindow( this->GetDlgItem(IDC_CATEGO_COMBO) );
	SubCatego.MountWindow( this->GetDlgItem(IDC_SUBCATEGO_COMBO) );

	try
	{
		CategoFileRead();
	}
	catch(RException e)
	{
		RMessageBox::Message( e.getMessage() );
	}

	//カテゴリコンボボックスの初期化.
	newVector<BCategoList>::const_iterator i = List.begin();
	newVector<BCategoList>::const_iterator e = List.end();
	for( ; i != e ; i ++)
	{
		Catego.AddItem( (*i)->getItem()->getName() );
	}
	Catego.Select(0);
	SelectSubCatego();
	IconNO = 0;

	this->Banana = poolBanana;
	this->ShareImage = poolShareImage;
}

void BUserDataEditDlg::OnCancel()
{
	this->Hide();
}

//ユーザ定義データの書き込み
void BUserDataEditDlg::WriteData() throw(RException)
{
	const string name = Name.getText();
	const string comment = Comment.getText();

	RGISSmallUserDataWrite attribute;

	const BCategoList * sel = getNowSelectCatego();
	if (sel == NULL)
	{
		throw RException(EXCEPTIONTRACE + "カテゴリを選択してください");
	}
	int categoNo = sel->getItem()->getNo();
	if (categoNo < 0 || categoNo > 255)
	{
		throw RException(EXCEPTIONTRACE + "カテゴリの範囲は 0 ～ 255 までです。 catego.txt を見直してください");
	}
	const BCategoListItem* item = getNowSelectSubCatego(sel);
	if (item == NULL)
	{
		throw RException(EXCEPTIONTRACE + "サブカテゴリを選択してください");
	}
	int subCategoNo = item->getNo();
	if (subCategoNo < 0 || subCategoNo > 255)
	{
		throw RException(EXCEPTIONTRACE + "サブカテゴリの範囲は 0 ～ 255 までです。 catego.txt を見直してください");
	}
	if (name.size() >= 255)
	{
		throw RException(EXCEPTIONTRACE + "名前は 254 文字以内にしてください");
	}
	if (comment.size() >= 255)
	{
		throw RException(EXCEPTIONTRACE + "コメントは 254 文字以内にしてください");
	}
	RGISSmallGlobalToBlock g2b;
	g2b.Create(TargetPoint.x , TargetPoint.y);

	int size = 0;
	//ヘッダ
	size += attribute.Create(NULL , g2b.getPoint() );
	//名前属性.
	size += attribute.Attribute(NULL , USERDATA_NAME , name);
	//コメント
	size += attribute.Attribute(NULL , USERDATA_COMMENT , comment);
	//アイコン
	size += attribute.Attribute(NULL , USERDATA_ICON , (unsigned char)IconNO);
	//カテゴリ
	size += attribute.Attribute(NULL , USERDATA_CATEGO , (unsigned char)categoNo);
	//サブカテゴリ
	size += attribute.Attribute(NULL , USERDATA_SUB_CATEGO , (unsigned char)subCategoNo);
	//終端
	size += attribute.Null(NULL);

	char * data = new char[size];
	size = 0;

	//ヘッダ
	size += attribute.Create(data + size , g2b.getPoint() );
	//名前属性.
	size += attribute.Attribute(data + size , USERDATA_NAME , name);
	//コメント
	size += attribute.Attribute(data + size , USERDATA_COMMENT , comment);
	//アイコン
	size += attribute.Attribute(data + size , USERDATA_ICON , (unsigned char)IconNO);
	//カテゴリ
	size += attribute.Attribute(data + size , USERDATA_CATEGO , (unsigned char)categoNo);
	//サブカテゴリ
	size += attribute.Attribute(data + size , USERDATA_SUB_CATEGO , (unsigned char)subCategoNo);
	//終端
	size += attribute.Null(data + size);

	try
	{
		if (Serial == -1)
		{
			Banana->UserDataModify(&TargetPoint , NULL , 0 , data , size , 1);
		}
		else
		{
			Banana->UserDataModify(&TargetPoint , &Serial , 1 , data , size , 1);
		}
	}
	catch(RException e)
	{
		delete [] data;
		throw;
	}

	delete [] data;
}

//削除
void BUserDataEditDlg::DeleteData() throw(RException)
{
	if (Serial == -1)
	{
		return ;
	}
	else
	{
		Banana->UserDataModify(&TargetPoint , &Serial , 1 , "" , 0 , 0);
	}
}

//ウィンドウプロシージャ.
LRESULT BUserDataEditDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDOK:	//書き込み
				try
				{
					WriteData();
				}
				catch(RException e)
				{
					RMessageBox::Message(e.getMessage() , "エラー");
					return FALSE;
				}

				OnCancel();
				break;
			case IDCANCEL:	//ｷｬﾝｾﾙ
				OnCancel();
				break;
			case ID_DEL:	//削除
				try
				{
					DeleteData();
				}
				catch(RException e)
				{
					RMessageBox::Message(e.getMessage() , "エラー");
					return FALSE;
				}
				OnCancel();
				break;
			case  IDC_CATEGO_COMBO:	//カテゴリコンボボックスが変更されたとき...
				if (HIWORD(inWParam) == CBN_SELCHANGE)
				{
					SelectSubCatego();
				}
				break;
			case IDC_CHANGE_ICON_BUTTON:
				OnIconChangeButtom();
				break;
			}
		}
		break;
	case WM_PAINT:
		OnPaint();
		break;
	}
	return FALSE;
}

//アイコン変更ボタンが押された.
void BUserDataEditDlg::OnIconChangeButtom() throw(RWin32Exception)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();

	BUserDataIconDlg dataIconDlg;
	dataIconDlg.Create( this->ShareImage , this->IconNO ,
		this->Name.getText() , this->Catego.getText() , this->SubCatego.getText() );
	int ret = dataIconDlg.DoModel(winMain->getInstance() , this->getWindowHandle() , "IDD_USERDATAICON_DIALOG");
	if (ret)
	{
		//選択されたアイコンをセット
		this->IconNO = dataIconDlg.getNowIconNumber();

		//画面を更新.
		InvalidateRect( this->getWindowHandle() ,  NULL , FALSE);
	}


}

void BUserDataEditDlg::OnPaint()
{
	RECT rc;
	this->getControlClientRect(this->GetDlgItem(IDC_USERDATA_ICON) , &rc);

    HDC hdc;
    PAINTSTRUCT paint;

	WindowsMain * winMain = WindowsMain::getMainSystem();
	HWND window = this->getWindowHandle();
    hdc = BeginPaint( window , &paint);
	::BitBlt(hdc  , rc.left , rc.top , 16 , 16 ,
		ShareImage->getIconImage()->getConstDC() , 16 * IconNO , 0 , SRCCOPY);
    EndPaint(window, &paint);
}


//カテゴリーファイルの読み込み.
void BUserDataEditDlg::CategoFileRead() throw(RException)
{
	RStdioFile file;
	file.Open( BConf::getConf()->getCurrentDirectory() + "catego.txt" , "rb");

	char nameBuffer[256];
	int  no;

	List.clear();


	char spaceBuffer[1024];

	BCategoList * lastUse = NULL;

	for(int nowLine = 0 ;  !file.IsEof() ; nowLine ++ )
	{
		string line = file.ReadLine();
		//空行または、コメントは無視.
		if ( line.empty() || line[0] == '#' || line == "\r\n") continue;

		if ( line[0] == '@' )
		{
//			int r = sscanf(line.c_str() , "@%256s %d", nameBuffer , &no);
			int r = sscanf(line.c_str() , "@%256s%1024[ ]%d", nameBuffer , spaceBuffer , &no);
			if (r != 3) 
			{
				throw RException(EXCEPTIONTRACE + "catego.txt ファイルの" + num2str(nowLine) + "行目の読み込みに失敗しました" );
			}
			lastUse = new BCategoList(nameBuffer , no) ;
			List.push_back( lastUse );
		}
		else
		{
			//カテゴリがないのにサブカテゴリはありえない.
			if (lastUse == NULL) continue;

			int r = sscanf(line.c_str() , "%1024[ ]%256s%1024[ ]%d", spaceBuffer , nameBuffer , spaceBuffer , &no);
			if (r != 4) 
			{
				throw RException(EXCEPTIONTRACE + "catego.txt ファイルの" + num2str(nowLine) + "行目の読み込みに失敗しました" );
			}
			lastUse->Insert(nameBuffer , no) ;
		}
	}
}

//選択されたカテゴリのサブカテゴリを SubCatego にセットします1
void BUserDataEditDlg::SelectSubCatego()
{
	SubCatego.DeleteItemAll();

	//カテゴリから選択されたカテゴリの検索.
	const BCategoList* sel = getNowSelectCatego();
	if ( sel == NULL ) return ;

	VecBCategoListItems::const_iterator i2 = sel->getList()->begin();
	VecBCategoListItems::const_iterator e2 = sel->getList()->end();
	for( ; i2 != e2 ; i2 ++)
	{
		SubCatego.AddItem( (*i2)->getName() );
	}
	SubCatego.Select(0);
}

//現在選択されているカテゴリを保持している BCategoList を取得
const BCategoList* BUserDataEditDlg::getNowSelectCatego()
{
	const string name = Catego.GetItem( Catego.getNowSelected() );	
	//カテゴリから選択されたカテゴリの検索.
	newVector<BCategoList>::const_iterator i = List.begin();
	newVector<BCategoList>::const_iterator e = List.end();
	for( ; i != e ; i ++)
	{
		if ( (*i)->getItem()->getName() == name) return *i;
	}
	return NULL;
}
//現在選択されているサブカテゴリを保持している BCategoListItem を取得
const BCategoListItem* BUserDataEditDlg::getNowSelectSubCatego(const BCategoList* inSelectCatego)
{
	const string name = SubCatego.GetItem( SubCatego.getNowSelected() );	
	//カテゴリから選択されたカテゴリの検索.
	VecBCategoListItems::const_iterator i = inSelectCatego->getList()->begin();
	VecBCategoListItems::const_iterator e = inSelectCatego->getList()->end();
	for( ; i != e ; i ++)
	{
		if ( (*i)->getName() == name) return *i;
	}
	return NULL;
}

void BUserDataEditDlg::Show(const LPOINT & inTargetPoint)
{
	TargetPoint = inTargetPoint;
	Serial = -1;

	//新規作成のなので削除ボタンを押せなくする.
	::EnableWindow( this->GetDlgItem(ID_DEL)  , FALSE);

	RWindowBase::Show();
}

void BUserDataEditDlg::Show(const RGISSmallUserDataRead * inUserData)
{
	TargetPoint = *inUserData->getXY();

	Catego.Select( inUserData->FindAttributeC(USERDATA_CATEGO)  - 1);
	SubCatego.Select( inUserData->FindAttributeC(USERDATA_SUB_CATEGO) - 1);
	IconNO = inUserData->FindAttributeC(USERDATA_ICON);
	Serial = inUserData->getSerial();

	const char * p;
	p = inUserData->FindAttributeStr(USERDATA_NAME);
	if (p)		Name.setText( p );
	else		Name.setText( "" );

	p = inUserData->FindAttributeStr(USERDATA_COMMENT);
	if (p)		Comment.setText( p );
	else		Comment.setText( "" );

	//編集,削除のなので削除ボタンを押せるようにする.
	::EnableWindow( this->GetDlgItem(ID_DEL)  , TRUE);

	RWindowBase::Show();
}
