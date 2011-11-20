#include "StartWithExeFile.h"
#ifdef SAVE_MENU_LIST

#include <.\Graphics\rti_graphics.h>

#define LIST_MENU_MAX   20
class TListMenu
{
    char          Menu_Count;
    char          Selected;
    BMD           Fream;        //メニューの枠
    BMD           Cursor;       //カーソル
    BMD           Card[LIST_MENU_MAX];  //メニューの内容
public:
    TListMenu();
	int GetSelect(){return Selected;};
    void Set(BMD fream,BMD cursor);
    int Add(BMD bmd);
    bool Del(short num);
    void AllDel();
    void MenuUd(bool ud);
    void Draw(GraphicsProc Proc,int x,int y);

};



#endif 
