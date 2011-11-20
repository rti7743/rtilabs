// BUserDataEditDlg.h: BUserDataEditDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUSERDATAEDITDLG_H__DC463992_7DD9_40C1_9D16_781FDA0FBB81__INCLUDED_)
#define AFX_BUSERDATAEDITDLG_H__DC463992_7DD9_40C1_9D16_781FDA0FBB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "REdit.h"
#include "RComboBox.h"
#include "RDialog.h"
#include "RWin32Exception.h"
#include "RGISSmallUserData.h"	
#include "BConf.h"
#include "RGISSmallUserData.h"

class BBanana;


class BCategoListItem
{
	string	Name;
	int		No;
public:
	BCategoListItem()
	{
	}
	BCategoListItem(const string & inName, int inNo)
	{
		Create(inName , inNo);
	}
	void Create(const string & inName, int inNo)
	{
		Name = inName;
		No = inNo;
	}

	const string getName() const
	{
		return Name;
	}
	int getNo() const
	{
		return No;
	}
};

typedef newVector<BCategoListItem> VecBCategoListItems;

class BCategoList
{
	BCategoListItem	Label;
	VecBCategoListItems List;
public:
	BCategoList(const string & inName, int inNo)
	{
		Label.Create(inName , inNo);
	}
	//リストに追加
	void Insert(const string & inName, int inNo)
	{
		List.push_back(new BCategoListItem(inName , inNo) );
	}
	//リストそのものの取得.
	const VecBCategoListItems * getList() const
	{
		return &List;
	}
	const BCategoListItem* getItem() const
	{
		return &Label;
	}
};


class BUserDataEditDlg  : public RDialog
{
public:
	BUserDataEditDlg();
	virtual ~BUserDataEditDlg();

	void BUserDataEditDlg::Create(BBanana	*	poolBanana,const RGISSmallShareImage* poolShareImage) throw(RWin32Exception);
	void BUserDataEditDlg::OnCancel();

	void Show(const LPOINT & inTargetPoint);
	void Show(const RGISSmallUserDataRead * inUserData);

	//ウィンドウプロシージャ.
	LRESULT BUserDataEditDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	//カテゴリーファイルの読み込み.
	void BUserDataEditDlg::CategoFileRead() throw(RException);
	//選択されたカテゴリのサブカテゴリを SubCatego にセットします
	void BUserDataEditDlg::SelectSubCatego();
	//現在選択されているカテゴリを保持している BCategoList を取得
	const BCategoList* BUserDataEditDlg::getNowSelectCatego();
	//現在選択されているサブカテゴリを保持している BCategoListItem を取得
	const BCategoListItem* BUserDataEditDlg::getNowSelectSubCatego(const BCategoList* inSelectCatego);
	//ユーザ定義データの書き込み
	void WriteData() throw(RException);
	//削除
	void DeleteData() throw(RException);
	//アイコン変更ボタンが押された.
	void OnIconChangeButtom() throw(RWin32Exception);

	//描画
	void BUserDataEditDlg::OnPaint();
private:
	REdit	Name;
	REdit	Comment;
	RComboBox	Catego;
	RComboBox	SubCatego;
	unsigned char IconNO;

	//編集,削除するユーザーデータのシリアル (新規作成の場合は -1 )
	int		Serial;

	LPOINT	TargetPoint;

	newVector<BCategoList> List;

	BBanana * Banana;
	const RGISSmallShareImage* ShareImage;
};

#endif // !defined(AFX_BUSERDATAEDITDLG_H__DC463992_7DD9_40C1_9D16_781FDA0FBB81__INCLUDED_)
