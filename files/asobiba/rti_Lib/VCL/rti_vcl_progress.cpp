/**********************************************************/
// プログラレスバーの土台
/**********************************************************/
#include "StartWithExeFile.h"

#ifdef PROGRESS_LIFE

#include <.\VCL\rti_vcl_progress.h>

TProgress::TProgress(TObject* hOwner,char *Caption,DWORD dwStyle,
					 int x,int y,int w,int h,WNDPROC UserWndp)
{
    lstrcpy(ClassName,"PROGRESS_CLASS");
    if (! CreateObject(hOwner,Caption,dwStyle,x,y,w,h,false,UserWndp) )
    {   //作成失敗
        Kind = OBJECT_KIND_BAD;
        return ;
    }
    //自分のオーナーに自分が作成されたことを伝える
    OwnerAdd(hOwner,this);

    Kind            = OBJECT_KIND_PROGRESS;
}

TProgress::~TProgress()
{
	DestroyComponent();
}

void TProgress::WmCommandCome(WPARAM wParam)
{
	switch(HIWORD(wParam))
	{
	}
}


/**********************************************************/
/* 基本ルーチン */
/**********************************************************/
LRESULT TProgress::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
    }
    return TBaseObject::UserDisPacth(hwnd,message,wParam,lParam);
}

#endif  //LIFE END
