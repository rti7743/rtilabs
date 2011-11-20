/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef STATUS_LIFE

#ifndef ___TSTATUSHXX_EVENTH
#define ___TSTATUSHXX_EVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//リストボックスのイベントクラス
//リストボックスから作るコンポーネントはこれを継承させます.
//EDIT

class TStatusEvent	: public TBaseObject
{
public:

	TStatusEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
