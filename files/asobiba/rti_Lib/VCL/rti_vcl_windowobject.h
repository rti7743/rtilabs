
#ifndef ___WINDOWOBJECTH
#define ___WINDOWOBJECTH
#include <.\VCL\rti_vcl_object.h>

class TWinObject ;

class TWinObject : public TObject
{
private:
	//サブクラス化をする
	void CreateSubClass(WNDPROC WndPorc);
	//サブクラス化を解除する
	void DestroySubClass();
	//サブクラス化のための WinProcの保存
    //VCL を使う以上すべてのウィンドはサブクラス化されます
	FARPROC True_StaticProc;
protected:
	/******************************************/
	/*                メソッド                */
	/******************************************/
	//新規作成(すべてこのルーチンで発生させなくてはならない)
	bool CreateObject(TObject* hOwner,char *Caption,
        DWORD dwStyle,int x,int y,int w,int h,bool NULLid,WNDPROC WndPorc);
	//新規クラス作成
    bool CreateOlignalClass(WNDPROC WndPorc);
	//オブジェクトの削除
	void DestroyComponent();
	//オブジェクトの初期化
	void InitComponent();
	/******************************************/
	/*            プロパティ                  */
	/******************************************/
    char ClassName[20];  //クラスの名前
	//WM_COMMAND処理
    FARPROC GetDefProc(){return True_StaticProc;};

public:
	/******************************************/
	/*                メソッド                */
	/******************************************/
	TWinObject(); //コンストラクタ

};

#endif
