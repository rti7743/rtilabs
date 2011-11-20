/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef TABCOONTROL_LIFE

#ifndef ___TTABCOONTROL_EVENTH
#define ___TTABCOONTROL_EVENTH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//リストボックスのイベントクラス
//リストボックスから作るコンポーネントはこれを継承させます.
//EDIT

class TTabControlObject	: public TBaseObject
{
public:
    //現在のページ数を取得する
    int GetPage(){return TabCtrl_GetCurSel(hWnd);};

	TTabControlObject();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
