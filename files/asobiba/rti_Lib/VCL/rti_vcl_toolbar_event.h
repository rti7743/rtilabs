/**********************************************************/
// ツールバーのイベントとか
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef TOOLBAR_LIFE

#ifndef ___TTOOLBAR_EVENTH
#define ___TTOOLBAR_EVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
#include <vector>

struct _ToolBarUsingStruct
{
    int idCommand;
    SimpleEvent CallRtn;
    int         UsingBmpNo;
};
typedef std::vector<_ToolBarUsingStruct*>::iterator ToolBarUsingIT;

class TToolBarEvent	: public TBaseObject
{
    bool FindCommandToButton(WPARAM wParam);
protected:
    //ボタンの個数
    int ButtonCounter ;
    //自分の所有しているボタンリスト
    std::vector<_ToolBarUsingStruct*> VToolBarUsing;

public:
	TToolBarEvent();
	~TToolBarEvent();
	void WmCommandCome(WPARAM wParam);

};

#endif

#endif
