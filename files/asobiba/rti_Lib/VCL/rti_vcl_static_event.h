/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef STATICTEXT_LIFE

#ifndef ___TSTATIC_EVENTH
#define ___TSTATIC_EVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//リストボックスのイベントクラス
//リストボックスから作るコンポーネントはこれを継承させます.
//EDIT

class TStaticEvent	: public TBaseObject
{
public:

	TStaticEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
