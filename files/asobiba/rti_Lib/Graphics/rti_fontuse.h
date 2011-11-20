#include "StartWithExeFile.h"
#ifdef FONT_USE_LIFE

#ifndef ___FONTUSEH
#define ___FONTUSEH

#include <windows.h>
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス

class TFontUse
{
protected:
    BMD Fontpuf;
	HDC Font_memDC;
	BITMAPINFOHEADER Fontbm_header;
private:
	PLOGFONT	FontInfo;
	HFONT		NewFont;
	HFONT		OldFont;
	HBITMAP Font_hBit;
    BOOL NowBold;
    bool FontWorkBufAlloc(HWND hwnd);
    void Disp(int x,int y,int ex,int ey,int *sx,int *sy,
                            int para,char* Str);
    int my_strstr(const char *p1,const char *p2);
    void FontProccesing(int tagno,char *AdderStye);
    int AdderStyrGet(char *Now,char *AdderStye);
    virtual void SetMojiPoint(char *Str,RECT *rt){}; //ここではダミー 一文字表示に使う
    virtual void MojiDraw
        (BMD bmd,GraphicsProc Proc,int x,int y,int ex,int ey)
            {  Proc(Fontpuf,0,0,ex-x,ey-y,bmd,x,y); }; //文字出力
    virtual void MojiInit(){};
public:
	TFontUse(HWND hwnd);
    ~TFontUse();
	void FontSet(char* FontName,int Size,int Round,
				   BOOL Bold,BOOL Italic,BOOL Line,BOOL Strike);
	void FontBreaker();
	void FontColorSet(unsigned long TextColor,unsigned long BackColor,
				   BOOL Mode);
	void Text(BMD bmd,GraphicsProc Proc,int x,int y,int ex,int ey,
				int para,char* Format,...);
    void TextEx(BMD bmd,GraphicsProc Proc,int x,int y,int ex,int ey,int para,
					  char* Format,...);
};


typedef TFontUse* DFONT;

#endif


#endif //LIFE END
