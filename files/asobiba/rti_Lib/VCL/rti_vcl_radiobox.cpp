/**********************************************************/
// チェックボックス
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef RADIOBOX_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_radiobox.h>
#include <.\VCL\rti_vcl_button.h>

TRadioBox::TRadioBox(TObject* hOwner,
	char *Caption,DWORD dwStyle,int x,int y,int w,int h,WNDPROC UserWndp)
    :TGroupBox(hOwner,Caption,dwStyle,x,y,w,h,UserWndp)
{
	ButtonPossion = 0;
    OnChange      = NULL;
}

TRadioBox::~TRadioBox()
{
} 


TRadioButton* TRadioBox::Add(char *Caption,DWORD dwStyle)
{
	RECT rect;
	SIZE s;
	HDC hdc ;
	int width;
	GetWindowRect( GethWnd(),&rect);

    hdc = GetDC(hWnd);
	GetTextExtentPoint32(hdc, Caption, lstrlen(Caption), &s);
    ReleaseDC( hWnd,hdc);
	width = rect.right - rect.left ;
	
	//強引なやり方っすね (^^;;;;
	return (new TRadioButton(this,
		Caption,dwStyle,
		10,ButtonPossion+= s.cy,  width-20,  s.cy) );
} 

/**********************************************************/
//連続配置が必要なコンポーネントのコントロールの再配置を促す
/**********************************************************/
void TRadioBox::RefControl()
{
	RECT rect;
	SIZE s;
	HDC hdc ;
	int width;
    ObjectIT it;
    char Caption[MAX_PATH];
    TBaseObject * TBO;

    ButtonPossion = 0; //ボタンの位置の初期化

    GetWindowRect( hWnd,&rect);
    hdc = GetDC(hWnd);

    width = rect.right - rect.left ;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            TBO = ((TBaseObject*)(*it));
            if ( TBO->GetKind() == OBJECT_KIND_RADIOBUTTON)
            { 
                TBO->GetCaption(Caption,MAX_PATH);
            	GetTextExtentPoint32(hdc, Caption , lstrlen(Caption), &s);
                TBO->Move(10,ButtonPossion+= s.cy);
            }
        }
    ReleaseDC( hWnd,hdc);
}

unsigned long TRadioBox::GetIndexMain(bool ByteReturn)
{
    unsigned long ret = 0;
    unsigned long counter = 0;
    ObjectIT it;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            if ((*it)->GetKind() == OBJECT_KIND_RADIOBUTTON)
            {
                //チェックボタンみっけ.
                if (((TRadioButton*)(*it))->Get()
		    										== BST_CHECKED)
                {//チェックされている.
                    ret = ret | (0x00000001 << counter);
                    break;  //ラジオボックスは排他だからこれでいい.                 
                }
                counter++;  //ラジオボタンカウントアップします
            }
        }

    if (ByteReturn)   return ret; //ビット列返し
    else              return  counter;//現在選択しているところを数値返し
}


/**********************************************************/
//ボタンをチェックします
/**********************************************************/
void TRadioBox::SetIndex(unsigned long bit)
{
    unsigned long ret = 0;
    int counter = 0;
    ObjectIT it;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            if ((*it)->GetKind() == OBJECT_KIND_RADIOBUTTON)
            {
                //チェックボタンみっけ.
                if (bit & (0x00000001 << counter) )
                { //そこのボタンはチェックする
                    RADIOBUTTON((*it))->Set();
                }
                else
                { //ボタンのチェックをおろす
                    RADIOBUTTON((*it))->ReSet();
                }
                counter++;  //チェックボタンカウントアップします
            }
        }
}

/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TRadioBox::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    TObject *Obj;
    switch(message)
    {
	case WM_COMMAND:
       if (OnChange) 
       {
           Obj = ((TObject*)GetWindowLong((HWND)lParam, GWL_USERDATA));
           if (!Obj) break;
           OnChange(this,Obj);
       }
       break;
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}


#endif //LIFE END
