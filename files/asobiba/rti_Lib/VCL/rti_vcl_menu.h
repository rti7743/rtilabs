#include "StartWithExeFile.h"
#ifdef MENU_LIFE

#ifndef ___MENUH
#define ___MENUH
#include <.\VCL\rti_vcl_object.h>

class TMenu ;

class TMenu : public TObject
{
private:
	//新規作成(すべてこのルーチンで発生させなくてはならない)
	void CreateMenuObject(TObject* hOwner,char *Caption, DWORD dwStyle);
    void DestroyComponentMenu();
    void PopUpCancle(TObject *Owner);
protected:
	/******************************************/
	/*                メソッド                */
	/******************************************/
	//オブジェクトの削除
	void DestroyComponent();
	//オブジェクトの初期化
    void InitComponent(){};
public:
	/******************************************/
	/*                メソッド                */
	/******************************************/
    TMenu::TMenu(TMenu* whOwner,char *Caption,DWORD wdwStyle);
	~TMenu(); //コンストラクタ

    void WmCommandCome(WPARAM wParam);

    void SetId(UINT k){Id = k;};
    SimpleEvent MenuClick;
};

#endif

#endif //LIFE END
