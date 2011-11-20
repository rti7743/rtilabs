// BMakeDlg.cpp: BMakeDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMakeDlg.h"
#include "WindowsMain.h"
#include "RDialogUtil.h"
#include "resrc1.h"
#include "RDiskUtil.h"
#include "RFileUtil.h"
#include "RConv.h"
#include "RGISGyoseiD.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


BMakeDlg::BMakeDlg()
{

}

BMakeDlg::~BMakeDlg()
{

}


void BMakeDlg::PreCreate() throw(RWin32Exception)
{
	Example.MountWindow( this->GetDlgItem(IDC_EXAMPLE_EDIT) );
	AreaSrc.MountWindow( this->GetDlgItem(IDC_AREASRC_EDIT) );
	AreaList.MountWindow( this->GetDlgItem(IDC_AREALIST) );
	Temp.MountWindow( this->GetDlgItem(IDC_TEMP_EDIT) );
	Map.MountWindow( this->GetDlgItem(IDC_MAP_EDIT) );

	AreaList.InsertColumn(0 , 0 , "コード" , 50, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM , LVCFMT_LEFT);
	AreaList.InsertColumn(0 , 0 , "町名" , 220, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM , LVCFMT_LEFT);


	Example.setText("map                  ここのディレクトリを (2) にセット\r\n"
					" |-25000             1/25000の地図\r\n"
					"     |---01101\r\n"
					"           |---01101.slm\r\n"
					"           |---01101.slp\r\n"
					"           .............\r\n"
					"     |---01102\r\n"
					"     |---01103\r\n"
					"     .............\r\n"
					" |-2500              1/2500の地図\r\n"
					"     |---01101\r\n"
					"           |---12NB592\r\n"
					"           |---12NB594\r\n"
					"           .............\r\n"
					"     |---01102\r\n"
					"     |---01103\r\n"
					"     .............\r\n"
					" |-address           街区レベル位置参照情報\r\n"
					"     |---01101JGD.csv\r\n"
					"     |---01102JGD.csv\r\n"
					"     |---01103JGD.csv\r\n"
					"     .............\r\n"
					);
}

void BMakeDlg::OnCancel()
{
	this->EndDialog(0);
}

const string BMakeDlg::getMapName() const
{
	return MapFilename;
}



void BMakeDlg::Scan() throw(RException)
{
	RStringList			numberList;

	//ディレクトリスキャン
	numberList.clear();
	AreaList.DeleteItemAll();
	{
		int listItemIndex = 0;
		WIN32_FIND_DATA data;
		const string path = AreaSrc.getText() + "\\25000";
		const string findpath = path + "\\*.*";
		HANDLE handle = ::FindFirstFile(findpath.c_str() , &data );
		if (handle == INVALID_HANDLE_VALUE )
		{
			return ;
		}
		do
		{
			if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  && 
				!(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) ) )
			{
					string read = path + "\\" + data.cFileName + "\\" + data.cFileName + ".slm";
					if ( RDiskUtil::Exist(read) )
					{
						//行政の名前の読み込み.
						read = path + "\\" + data.cFileName + "\\" + data.cFileName + "GD.sal";
						try
						{
							const string name = ParseGyouseiName(read , atoi(data.cFileName) );
							AreaList.SetItem(listItemIndex  , 0 , name.c_str() );	//名前
							AreaList.SetItem(listItemIndex  , 1 , data.cFileName );	//数字

							printf("リストに追加 %d 番目 : コード:%d:%s\r\n" ,listItemIndex, atoi(data.cFileName) ,  name.c_str() );

							listItemIndex++;
						}
						catch(RException & )
						{
							::FindClose(handle);
							throw ;
						}
					}
			}
		}
		while( ::FindNextFile(handle , &data) );
		::FindClose(handle);
	}
}


//行政名を取得します.
string BMakeDlg::ParseGyouseiName(const string & inGyoseiDFilename,unsigned long inCode) const throw(RException) 
{
	RGISGyoseiD gyoseiD;
	gyoseiD.Create(inGyoseiDFilename , inCode);
	int gyoseiCount = gyoseiD.getCount();
	if (gyoseiCount <= 0)
	{
		throw RException("行政データ:%s に一行も有効な行がありません" , inGyoseiDFilename.c_str() );
	}
	for(int i = 0 ; i < gyoseiCount ; i ++)
	{
		const GyoseiDData* gyoseiDData = gyoseiD.getPoint(i);
		if (! gyoseiDData->Name.empty() )
		{
			return gyoseiDData->Name;
		}
	}
	throw RException("行政データ:%s に有効な行はありましたが、みんな名前が空です", inGyoseiDFilename.c_str());
}

void BMakeDlg::Make() 
{
	RStringList numberList;

	//現在選択されているところをすべてリストに入れる.
	int index = -1;
	while((index = ListView_GetNextItem(AreaList.getWindowHandle(), index, LVNI_ALL | LVNI_SELECTED))!=-1)
	{
		if ( AreaList.GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED )
		{
			numberList.push_back( AreaList.GetItem(index , 1) );
		}
	}
	if (numberList.empty() )
	{
		RMessageBox::Message("(3)のエリアが一つも選択されていません");
		return ;
	}
	if (AreaSrc.getText().empty() )
	{
		RMessageBox::Message("(2)のディレクトリを指定してください");
		return ;
	}
	if (Temp.getText().empty() )
	{
		RMessageBox::Message("(4)のディレクトリを指定してください");
		return ;
	}
	if (Temp.getText().empty() )
	{
		RMessageBox::Message("(5)のファイル名を指定してください");
		return ;
	}

	try
	{
		//変換開始
		SmallPack(AreaSrc.getText() , Temp.getText() + "\\temp", Map.getText() , numberList );

		this->MapFilename = Map.getText();
	}
	catch(RException e)
	{
		RMessageBox::Message(string("") + "エラーが発生しました:\r\n" + e.getMessage() , "エラー");
		this->EndDialog(0);
	}
	catch(std::bad_alloc)
	{
		RMessageBox::Message(string("") + "BadAllocが発生しました" , "エラー");
		this->EndDialog(0);
	}
	catch(...)
	{
		RMessageBox::Message(string("") + "未知のエラーが発生しました" , "エラー");
		this->EndDialog(0);
	}

	//成功
	this->EndDialog(1);
}

void BMakeDlg::SmallPack(const string inForm , const string & inTo,
						 const string & inKmap , const RStringList & inTargetList) throw(RException)
{
	this->Hide();

	puts("SmallPack ................");
	RGISSlmINdex	SlmIndex;


	int inScale = 100;
	SlmIndex.Create(inForm + "\\25000", inTargetList , inScale);

	try
	{
		RDiskUtil::Delete(inTo);
	}
	catch(RException e)
	{
	}
	RDiskUtil::MakeDirectory(inTo + "\\");

	puts("件数を取得しています");

	int count = SlmIndex.getCount();

	for(int i = 0 ; i < count ; i++)
	{
		const RGISSlm* slm = SlmIndex.getPoint(i);
		int key = slm->getKey();

		printf("進捗 %d/%d Area(%d)\n" , i,count , key);

		RGIS * gis = new RGIS;
		try
		{
			gis->SmallPack(inForm ,inTo , key , inScale);

			TRACE("Convert!:%u\n",gis->getKey() );
		}
		catch(RException e)
		{
			TRACE(e.getMessage() );
			delete gis;

			throw RException (EXCEPTIONTRACE + "地図変換中のエリア("  + num2str(gis->getKey()) + ")でエラーが発生しました:"  + e.getMessage() );
		}
		delete gis;
	}

	puts("結合処理");

	//結合処理.
	JoinTempFormatScript script[SEGMENT_MAX];
	int segmentCount = RGIS::getJoinScript(&script[0] , SEGMENT_MAX);

	JoinHeadFormatScript head[SEGMENT_MAX];
	int segmentHeadCount = RGIS::getJoinHeadScript(&head[0] , SEGMENT_MAX);

	//結合処理.
	RGISJoin Join;
	Join.Join( inTo , inTo + "/kuma.map" , script , segmentCount , head , segmentHeadCount);

	try
	{
		puts("簡易チェック");
		//壊れていないかチェック.
		Join.Check(inTo + "/kuma.map");
	}
	catch(RException e)
	{
		ASSERT(0);
		throw RException( EXCEPTIONTRACE + "簡易チェック中にエラーが発生しました:" + e.getMessage() );
	}

	//成果物のコピー
	puts("作成した地図を所定の領域にコピーしています");
	RDiskUtil::Move(inTo + "/kuma.map" , inKmap );

	//作業領域のクリア
	puts("作業に使った領域を破棄しています");
	try
	{
		RDiskUtil::Delete(inTo);
	}
	catch(RException e)
	{
		ASSERT(0);
		throw RException( EXCEPTIONTRACE + "作業に使った領域を破棄中にエラーが発生しました:" + e.getMessage() );
	}
}


//ウィンドウプロシージャ.
LRESULT BMakeDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
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
		PreCreate();
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDCANCEL:	//ｷｬﾝｾﾙ
				OnCancel();
				break;
			case IDC_AREASRC_BUTTON:	//ソースパス
				{
					string name = RDialogUtil::SelectPath( this->getWindowHandle() , 
						"構築するデータが入ってるディレクトリを指定してください" , AreaSrc.getText() );
					AreaSrc.setText(name);
				}
				break;
			case IDC_TEMP_BUTTON:
				{
					string name = RDialogUtil::SelectPath( this->getWindowHandle() , 
						"作業用のディレクトリを指定してください" , Temp.getText() );
					Temp.setText(name);
				}
				break;
			case IDC_MAP_BUTTON:
				{
					string name = RDialogUtil::FileSaveDialog( this->getWindowHandle() , 
						"作成する .map ファイル名を選択してください" , Map.getText() , "kuma.map" , _T("mapファイル (*.map)\0*.map\0\0") );
					Map.setText(name);
				}
				break;
			case IDC_AREASRC_SCAN:	//Scanボタンを押したとき
				this->Hide();
				Scan();
				this->Show();
				break;
			case IDC_MAKE_BUTTON:	//Makeボタンを押したとき
				Make();
				break;
			}
		}
		break;
	}
	return FALSE;
}

