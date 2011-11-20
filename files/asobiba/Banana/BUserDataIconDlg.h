// BUserDataIconDlg.h: BUserDataIconDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUSERDATAICONDLG_H__D92E148E_E591_4CAD_9EEA_8DCB95FEB75D__INCLUDED_)
#define AFX_BUSERDATAICONDLG_H__D92E148E_E591_4CAD_9EEA_8DCB95FEB75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RDialog.h"
#include "RGISSmallShareImage.h"
#include "RScrollBar.h"
#include "Rconv.h"
#include "REdit.h"

class BToolBitText
{
private:
	string	Message;
	RStringList MatchList;
public:
	void Create(const string & inMessage , const string & inMatchList)
	{
		this->Message = inMessage;
		this->MatchList = RConv::Explode(inMatchList);
	}

	const string getMessage() const
	{
		return this->Message;
	}
	const RStringList* getMatchList() const
	{
		return &this->MatchList;
	}
};


class BUserDataIconDlg : public RDialog  
{
public:
	BUserDataIconDlg();
	virtual ~BUserDataIconDlg();

	void Create(const RGISSmallShareImage* poolShareImage, int inNowIconNumber,
		const string & inName , const string & inCatego , const string & inSubCatego);

	//現在選択されているアイコンを取得します.
	int getNowIconNumber() const
	{
		return NowIconNumber;
	}

	//ウィンドウプロシージャ.
	LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);
private:
	void OnInit();
	void OnPaint();
	void OnScroll(WPARAM inWParam);

	//説明の読み込み
	void Load();
	//自動選択.
	int AutoMatch(const string & inName , const string & inCatego , const string & inSubCatego) const;
	//リストの中の文字列に inName の部分が一致しているか検索.
	bool TextFind(const RStringList* inList , const string & inName) const;
	//指定されたアイコンの説明の取得
	const string getIconMessage(int inIconSelect) const;

private:
	const RGISSmallShareImage* PoolShareImage;		//使用する画像一覧.
	int			NowIconNumber;						//現在選択されているアイコン番号.
	RScrollBar	ScrollBar;
	REdit		IconMessageEdit;

	string Name;
	string Catego;
	string SubCatego;

	newVector<BToolBitText>	Text;

};

#endif // !defined(AFX_BUSERDATAICONDLG_H__D92E148E_E591_4CAD_9EEA_8DCB95FEB75D__INCLUDED_)
