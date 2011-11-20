// MVControl.h: MVControl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVCONTROL_H__5D9AD80B_6FD4_477B_A992_8AE3A43A00CC__INCLUDED_)
#define AFX_MVCONTROL_H__5D9AD80B_6FD4_477B_A992_8AE3A43A00CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MVFileList.h"
#include "MVThreadManager.h"
#include "RWaitCursor.h"

class MVControl  
{
public:
	MVControl();
	virtual ~MVControl();

	/**
	 * ChangeOption:	画面の幅と高さ
	 *
	 * @param inImageConvertParam	変換に使用するパラメータ
	 * @return void 
	 */
	void ChangeOption(const MVImagePreConvert* inImageConvertParam)
	{
		this->ImageConvertParam = *inImageConvertParam;

		this->Manager.ChangeOption( & this->ImageConvertParam );
	}

	/**
	 * Update:			本を開く
	 *
	 * @param inBookFilename	book.zip ファイル
	 * @param inPage			ページ
	 * @return void 
	 */
	void Update( const string & inBookFilename , int inPage) throw (RException)
	{
		if (inBookFilename.empty() ) return;

		this->Manager.Update(inBookFilename);
		GotoPage(inPage);
	}

	/**
	 * GotoPage:			指定ページにジャンプ
	 *
	 * @param inPage 
	 * @return void 
	 */
	void GotoPage(int inPage) throw (RException);

	/**
	 * getPageCount:		総ページ数
	 *
	 * @return int 
	 */
	int getPageCount()
	{
		return this->Manager.getPageCount();
	}

	/**
	 * getNowPage:		今のページ数
	 *
	 * @return int 
	 */
	int  getNowPage() const
	{
		return this->Page;
	}

	/**
	 * Scroll:		ページをスクロールする
	 *
	 * @param inUp	ture	上
	 *				false	下
	 * @return void 
	 */
	void Scroll(bool inUp) throw (RException);

	/**
	 * Draw:				描画
	 *
	 * @param ioHDC 
	 * @return void 
	 */
	void Draw(HDC ioHDC) ;


private:
	MVThreadManager		Manager;
	const MVPageData*			PageData;

//	int			ScrollWidth;
//	int			Width;
//	int			Height;
	int			Page;
//	bool		ScreenType;

	int			ScrollXY;

	MVImagePreConvert	ImageConvertParam;
};

#endif // !defined(AFX_MVCONTROL_H__5D9AD80B_6FD4_477B_A992_8AE3A43A00CC__INCLUDED_)
