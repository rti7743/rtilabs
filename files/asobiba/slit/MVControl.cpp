// MVControl.cpp: MVControl クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVControl.h"
#include "MVConfig.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MVControl::MVControl()
{
	this->Page = 0;
	this->PageData = NULL;
}

MVControl::~MVControl()
{
}

/**
 * GotoPage:			指定ページにジャンプ
 *
 * @param inPage 
 * @return void 
 */
void MVControl::GotoPage(int inPage) throw (RException)
{
//		RWaitCursor waitCursor;

	int a = getPageCount();
	if (inPage >= getPageCount()  || inPage < 0)
	{
		return ;
	}
	//ページをロード
	this->PageData = this->Manager.GotoPage(inPage);

	this->Page = inPage;
	this->ScrollXY = 0;
}


/**
 * Scroll:		ページをスクロールする
 *
 * @param inUp	ture	上
 *				false	下
 * @return void 
 */
void MVControl::Scroll(bool inUp) throw (RException)
{
	if (!  this->PageData )
	{
		//まだ、ページが読み込まれていない.
		return ;
	}

	int scrollStop = 0;
	switch ( this->ImageConvertParam.getScrollType() )
	{
	case MVConfig::YOKO:
	default:
		scrollStop = this->PageData->getWidth() - this->ImageConvertParam.getScreenWidth();
		break;
	case MVConfig::TATE:
		scrollStop = this->PageData->getHeight() - this->ImageConvertParam.getScreenHeight();
		break;

	}

	const int scrollCount = MVConfig::getInstance()->getScrollCount() ;
	const int scroolSize = abs( (scrollStop) / scrollCount) ;

	if (inUp)
	{
		if (this->ScrollXY <= 0)
		{
			//すでに一番上の場合、一つ前のページに戻す.
			GotoPage(getNowPage() - 1);
			this->ScrollXY = scroolSize * scrollCount ;
		}
		else
		{
			this->ScrollXY -= scroolSize;
			if (this->ScrollXY <= scrollCount)
			{//補正
				this->ScrollXY =0;
			}
		}
	}
	else
	{
		if (this->ScrollXY >= scrollStop )
		{
			//すでに一番下の場合、一つ次のページに進む.
			GotoPage(getNowPage() + 1);
		}
		else
		{
			this->ScrollXY += scroolSize;
			if (this->ScrollXY >= (scrollStop - scrollCount) )
			{//補正
				this->ScrollXY = scrollStop;
			}
		}
	}
}

/**
 * Draw:				描画
 *
 * @param ioHDC 
 * @return void 
 */
void MVControl::Draw(HDC ioHDC) 
{
	if (!  this->PageData )
	{
		//まだ、ページが読み込まれていない.
		return ;
	}

	switch ( this->ImageConvertParam.getScrollType() )
	{
	case MVConfig::YOKO:
	default:
		BitBlt(ioHDC , 0 , 0 , this->ImageConvertParam.getScreenWidth() , this->ImageConvertParam.getScreenHeight() ,
					this->PageData->getDC() , this->ScrollXY , 0 , SRCCOPY);
		break;
	case MVConfig::TATE:
		BitBlt(ioHDC , 0 , 0 , this->ImageConvertParam.getScreenWidth() , this->ImageConvertParam.getScreenHeight() ,
					this->PageData->getDC() , 0 , this->ScrollXY , SRCCOPY);
		break;

	}
}

