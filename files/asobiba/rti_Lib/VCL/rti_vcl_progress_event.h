/**********************************************************/
// リストボックスのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef PROGRESS_LIFE

#ifndef ___TPROGRESS_EVENTH
#define ___TPROGRESS_EVENTH
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//リストボックスのイベントクラス
//リストボックスから作るコンポーネントはこれを継承させます.
//EDIT

class TProgressEvent	: public TBaseObject
{
public:
    //これで、プログレスバーの範囲を決めます。 デフォルトでは０から１００までとなっています。 
    void SetRange(WORD nMinRange,WORD nMaxRange)
    {SendMessage(hWnd,PBM_SETRANGE, 0L , MAKELPARAM(nMinRange, nMaxRange));};

    //これで、１回の増分を指定します。 
    void SetStep(int nStepInc) 
    {SendMessage(hWnd,PBM_SETSTEP, (WPARAM) nStepInc ,0L);};

    //これを送信すると１ステップ分だけ進みます。 満タンになったときはまた、０から始まります。
    void StepIt() 
    {SendMessage(hWnd,PBM_STEPIT, 0L ,0L);};


	TProgressEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
