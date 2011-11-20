/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef SCROLLBAR_LIFE

#ifndef ___TSCROLLBAR_EVENTH
#define ___TSCROLLBAR_EVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//リストボックスのイベントクラス
//リストボックスから作るコンポーネントはこれを継承させます.
//EDIT

class TScrollEvent	: public TBaseObject
{
public:

	TScrollEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
