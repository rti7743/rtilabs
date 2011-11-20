// Filer.h: Filer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILER_H__46A320B9_82F1_4C97_99ED_0761EFF98A91__INCLUDED_)
#define AFX_FILER_H__46A320B9_82F1_4C97_99ED_0761EFF98A91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Filer;
#include "comm.h"
#include "FilerSub.h"
#include "RTaskView.h"

//メインクラス.
class Filer
{
private:
	RTaskView*	TaskView;

private:
	Filer();
public:
	virtual ~Filer();

	//singleton
	static Filer* getFiler();
	//TaskViewの取得.
	RTaskView* getTaskView() const
	{
		ASSERT(TaskView != NULL);
		return TaskView;
	}
	//左右のウィンドウを取得.
	FilerSub* getFilerSub(bool IsLeft) const
	{
		if (IsLeft)	return Left;
		return		Right;
	}
	//左側にフォーカスがありますか?
	bool IsLeftFocus() const;

	//ウィンドウがリサイズされたときに呼ばれる
	void OnWindowResize();
	//キーが押されたとき
	void OnKeyDown(unsigned char inKey);
	//開始処理
	void OnCreate();
	//終了処理
	void OnDestory();
	//フォーカスをもつウィンドウが移動したときに呼ばれる.
	void OnChangeFocus(bool isLeftWindow);
	//有効なウィンドウかどうか調べる.
	bool IsValidWindow(const Window* inWin);

	//フォーカスが変わったことを通知.
	void OnChangeFocus();

	
private:
	//左のウィンドウ.
	FilerSub*	Left;
	//右のウィンドウ.
	FilerSub*	Right;
	//左ウィンドウがフォーカスを持っている.
	bool		LeftWindowFocus;

};

#endif // !defined(AFX_FILER_H__46A320B9_82F1_4C97_99ED_0761EFF98A91__INCLUDED_)
