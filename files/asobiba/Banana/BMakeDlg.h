// BMakeDlg.h: BMakeDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMAKEDLG_H__6E9E7082_9BD2_4181_9403_1014F44B7123__INCLUDED_)
#define AFX_BMAKEDLG_H__6E9E7082_9BD2_4181_9403_1014F44B7123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RDialog.h"
#include "REdit.h"
#include "RListView.h"
#include "RException.h"
#include "RWin32Exception.h"

class BMakeDlg  : public RDialog  
{
public:
	BMakeDlg();
	virtual ~BMakeDlg();

	void PreCreate() throw(RWin32Exception);
	virtual LRESULT DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam);

	const string getMapName() const;

private:
	void OnCancel();
	void SmallPack(const string inForm , const string & inTo,
		const string & inKmap , const RStringList & inTargetList) throw(RException);
	void Make() ;
	void Scan() throw(RException);

	//行政名を取得します.
	string ParseGyouseiName(const string & inGyoseiDFilename,unsigned long inCode) const throw(RException)  ;

	REdit				AreaSrc;
	RListView			AreaList;
	REdit				Temp;
	REdit				Map;
	REdit				Example;

	string				MapFilename;

};
#endif // !defined(AFX_BMAKEDLG_H__6E9E7082_9BD2_4181_9403_1014F44B7123__INCLUDED_)
