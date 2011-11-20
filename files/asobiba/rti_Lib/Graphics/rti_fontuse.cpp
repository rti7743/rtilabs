#include "StartWithExeFile.h"
#ifdef FONT_USE_LIFE

/*****************************************************************/
/*****************************************************************/
//グラフィッカルフォント　多少重いなり
//              Thank you For Botchy 様
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_debug.h>
#include <.\Graphics\rti_fontuse.h>
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_error_message.h>
#include <.\Comm\rti_Comm.h>

/*****************************************************************/
//コンストラクタ
/*****************************************************************/
TFontUse::TFontUse(HWND hwnd)
{
	FontInfo=(PLOGFONT)GlobalAlloc(GPTR,sizeof(LOGFONT));
	NewFont = NULL;
    FontWorkBufAlloc(hwnd);
}

/*****************************************************************/
//ワークエリア確保
/*****************************************************************/
bool TFontUse::FontWorkBufAlloc(HWND hwnd)
{
    int x,y;

    x = Sprite->w;
    y = Sprite->h;
	Fontpuf = NULL; Font_memDC = NULL;
	try
	{
		//新規に作成します
	    Fontpuf = new (struct BMP_DATA_TABLE) ;//メモリ確保
		if (!Fontpuf) throw (FriendlyError("TFontUse::FontWorkBufAlloc","メモリ確保","","") );
		Fontpuf->w = x;
		Fontpuf->h = y;
		if ( !CommDraw->GraphicsAlloc(Fontpuf) )
			 throw (FriendlyError("TFontUse::FontWorkBufAlloc","新規領域確保","","") );
		//DC 確保.
		Font_memDC = CommDraw->GetDC(Fontpuf);
	}
	catch(...)
	{
		delete Fontpuf;
		return false;
	}
	return true;
}


/*****************************************************************/
//デストラクタ
/*****************************************************************/
TFontUse::~TFontUse()
{
	//もし、前に使っているならば前のやつをぶっころす
	FontBreaker();
	GlobalFree((LOCALHANDLE)FontInfo);

	//DC 破棄.
	CommDraw->DeleteDC(Fontpuf,Font_memDC);
	CommDraw->GraphicsFree(Fontpuf);
	delete Fontpuf;
}

/*****************************************************************/
//フォントセット
/*****************************************************************/
void TFontUse::FontSet(char* FontName,int Size,int Round,
				   BOOL Bold,BOOL Italic,BOOL Line,BOOL Strike)
{
	//もし、前に使っているならば前のやつをぶっころす
	FontBreaker();

	lstrcpy(FontInfo->lfFaceName,FontName);

    NowBold = Bold;
    FontInfo->lfHeight=Size;
	FontInfo->lfWeight=(Bold==FALSE)*FW_NORMAL+(Bold==TRUE)*FW_BOLD;
	FontInfo->lfEscapement=Round*10;
	FontInfo->lfWidth=0;
	FontInfo->lfItalic=Italic;
	FontInfo->lfUnderline=Line;
    FontInfo->lfStrikeOut = Strike;
	FontInfo->lfCharSet=SHIFTJIS_CHARSET;

	NewFont=CreateFontIndirect(FontInfo);

	OldFont=(HFONT)SelectObject(Font_memDC,NewFont);
}

/*****************************************************************/
//フォント破棄
/*****************************************************************/
void TFontUse::FontBreaker()
{
	if (NewFont!=NULL)
	{ //もし、前に使っているならば前のやつをぶっころす
		OldFont=(HFONT)SelectObject(Font_memDC,OldFont);
		DeleteObject(NewFont);
		NewFont = NULL;
	}

}

/*****************************************************************/
//フォントの色のセット
/*****************************************************************/
void TFontUse::FontColorSet(unsigned long TextColor,unsigned long BackColor,
				   BOOL Mode)
{
	SetTextColor(Font_memDC,TextColor);
	SetBkColor(Font_memDC,BackColor);

	if (Mode) SetBkMode(Font_memDC,TRANSPARENT);
}


/*****************************************************************/
//単純な文字表示
/*****************************************************************/
void TFontUse::Text(BMD bmd,GraphicsProc Proc,int x,int y,int ex,int ey,int para,
					char* Format,...)
{
	char Buffer[256];
	int rx,ry;
	rx =ex-x;
	ry =ey-y;
	if (rx>Fontpuf->w || rx < 0) return;
	if (ry>Fontpuf->h || ry < 0) return;

    //文字列関係の初期化(上位の T1PUT )
    MojiInit();

	vsprintf(Buffer,Format,(char*)(&Format+1));

	//バッファのクリア
	BitBlt(Font_memDC,0,0,rx,ry,NULL,0,0,BLACKNESS);

    RECT rt;
    rt.top = 0;
    rt.bottom = ry;
    rt.right = rx;
    rt.left = 0;
    DrawText(Font_memDC, Buffer, -1, &rt, para);
    //一文字入力の場合　文字の出力位置を記録する.
    SetMojiPoint(Buffer,&rt);

	//DC 更新
	CommDraw->UpdataDC(Fontpuf,Font_memDC);

   //文字出力
   MojiDraw(bmd,Proc,x,y,ex,ey);
}

#define FONT_BIGGER      0
#define FONT_UN_BIGGER   1
#define FONT_SIZE_UP     2
#define FONT_SIZE_DOWN   3
#define FONT_ITALER      4
#define FONT_UN_ITALER   5
#define FONT_UNDER       6
#define FONT_UN_UNDER    7
#define FONT_STRIKE      8
#define FONT_UN_STRIKE   9
#define FONT_FONT_COLOR  10
#define FONT_FONT_NAME   11
#define FONT_FONT_SIZE   12
#define FONT_BACK_COLOR  13
#define FONT_BACK_MODE   14
#define FONT_SLOW        15
#define FONT_FARST       16
#define FONT_KEY         17
#define FONT_WAIT        18

#define ADDERSTYE_MAX 30
/*****************************************************************/
//高度文字表示
/*****************************************************************/
void TFontUse::TextEx(BMD bmd,GraphicsProc Proc,int x,int y,int ex,int ey,int para,
					  char* Format,...)
{
    static char *CommandLib[] =
    {
        {"B>"},{"/B>"},         //ふと文字
        {"BIG>"},               //一回り大きくする
        {"SMALL>"},             //一回り小さくする
        {"I>"},{"/I>"},         //イタリック
        {"U>"},{"/U>"},         //アンダーライン
        {"S>"},{"/S>"},         //取消線
        {"FONTCOLOR="},         //フォントカラー
        {"FONTNAME="},          //フォント名
        {"FONTSIZE="},          //フォントサイズ
        {"BACKCOLOR="},         //バックカラー
        {"BACKMODE="},          //バックを透明にする.
    };
    int p;
    int sx,sy;
    char Str[256];
    char DispStr[256];
    char *Now = Str;   //現在のフォーマットの位置
    char *CompStart = Str; //コンパイル済みバッファのスタート位置
    char AdderStye[ADDERSTYE_MAX];

    //文字列関係の初期化(上位の T1PUT )
    MojiInit();

    sx = sy = 0;
	//バッファのクリア
	BitBlt(Font_memDC,0,0,Fontpuf->w,Fontpuf->h,NULL,0,0,BLACKNESS);
	vsprintf(Str,Format,(char*)(&Format+1));

    //タグコマンド解析 & 出力.
    for(p=0; *Now ;)
    {
        if(*Now == '<')
        {
            //一致する命令を探す
            for (int i=0; i < sizeof(CommandLib)/sizeof(*CommandLib) ; i ++)
            { //すべての命令と比較
                if (my_strstr((Now+1),CommandLib[i])==0)
                { //一致した命令があった

                    //とりあえず絵画
                    if (p)
                    {
                        lstrcpy(DispStr,CompStart);
                        DispStr[p] = 0;
                        Disp(x,y,ex,ey,&sx,&sy,para,DispStr);
                    }
                    //文字ポインタを進める
                    Now = Now+lstrlen(CommandLib[i]);
                    //追加情報を得る
                    Now += AdderStyrGet(Now,AdderStye);
                    FontProccesing(i,AdderStye);

                    //また、ここから命令を解釈していきます.
                    CompStart = Now;
                    p=0;
                    break;
                }
            }
        }
        else
        {
            Now++,p++;
        }
    }
    if (CompStart != Now)
    {
        //残りを絵画
        Disp(x,y,ex,ey,&sx,&sy,para,CompStart);
   }
   //DC 更新
   CommDraw->UpdataDC(Fontpuf,Font_memDC);

   //文字出力
   MojiDraw(bmd,Proc,x,y,ex,ey);
}

int TFontUse::AdderStyrGet(char *Now,char *AdderStye)
{
    if(*Now == '>') 
    {
        AdderStye[0] = 0;
        return 1; //追加スタイルなし
    }

    char *LpNow = Now+1;
    char *LpAdderStye = AdderStye;

    for(int i=0; i < ADDERSTYE_MAX ; i ++)
    {
        if (*LpNow == '>' || *LpNow == 0) 
        { //追加スタイル取得成功
            *LpAdderStye = 0;
            return (i+2);
        }
        *LpAdderStye = *LpNow;
        LpNow++;
        LpAdderStye++;
    }
//取得エラー　長すぎる
    AdderStye[0] = 0;
    return 0;

}

/*****************************************************************/
//フォント変更処理
/*****************************************************************/
void TFontUse::FontProccesing(int tagno,char *AdderStye)
{
    char  *FontName = FontInfo->lfFaceName;
    int   iSize      = FontInfo->lfHeight;
    int   iRotate    = 0;
    BOOL  fBold     = NowBold;
    BOOL  fItalic   = FontInfo->lfItalic;
    BOOL  fUnderline= FontInfo->lfUnderline;
    BOOL  fStrikeOut= FontInfo->lfStrikeOut;
    unsigned long  iTextColor = GetTextColor(Font_memDC);
    unsigned long  iBackColor = GetBkColor(Font_memDC);
    BOOL fMode                = GetBkMode(Font_memDC);

    switch(tagno)
    {
    case FONT_BIGGER: //大きい字にする.
        fBold = TRUE;
        break;
    case FONT_UN_BIGGER   :
        fBold = FALSE;
        break;
    case FONT_SIZE_UP     :
        iSize++;
        break;
    case FONT_SIZE_DOWN   :
        iSize--;
        break;
    case FONT_ITALER      :
        fItalic = TRUE;
        break;
    case FONT_UN_ITALER   :
        fItalic = FALSE;
        break;
    case FONT_UNDER       :
        fUnderline = TRUE;
        break;
    case FONT_UN_UNDER    :
        fUnderline = FALSE;
        break;
    case FONT_STRIKE      :
        fStrikeOut = TRUE;
        break;
    case FONT_UN_STRIKE   :
        fStrikeOut = FALSE;
        break;
    case FONT_FONT_COLOR  :
        iTextColor = atoi(AdderStye);
        break;
    case FONT_FONT_NAME   :
        FontName = AdderStye;
        break;
    case FONT_FONT_SIZE   :
        iSize = atoi(AdderStye);
        break;
    case FONT_BACK_COLOR  :
        iBackColor = atoi(AdderStye);
        break;
    case FONT_BACK_MODE   :
        break;
    case FONT_SLOW        :
        break;
    case FONT_FARST       :
        break;
    case FONT_KEY         :
        break;
    case FONT_WAIT        :
        break;
    }
    FontSet(FontName,iSize,iRotate,fBold,fItalic,fUnderline,fStrikeOut);
    FontColorSet(iTextColor,iBackColor,fMode);
}

/*****************************************************************/
//その文字列 p2 が p1 の現在位置から存在するかチェック
/*****************************************************************/
int TFontUse::my_strstr(const char *p1,const char *p2)
{
    char *w1,*w2;
    for(w1 = (char*)p1,w2 = (char*)p2;  *w2 ; w1++,w2++)
    {
        if (*w1 != *w2) return -1;
    }
    return 0;
}

/*****************************************************************/
//高度文字列表示で使う文字列表示
/*****************************************************************/
void TFontUse::Disp(int x,int y,int ex,int ey,int *sx,int *sy,
                    int para,char* Str)
{
	int rx,ry;
	rx =ex-x;
	ry =ey-y;
	if (rx>Fontpuf->w || rx < 0) return;
	if (ry>Fontpuf->h || ry < 0) return;

    RECT rt;
    rt.top = *sy;
    rt.bottom = ry+*sy;
    rt.right = rx+*sx;
    rt.left = *sx;

    //絵画する文字列の長さ
    int len = lstrlen(Str);
 
    SIZE s;
    GetTextExtentPoint32(Font_memDC, Str, len, &s);
    *sy += DrawText(Font_memDC, Str, -1, &rt, para) - s.cy;

    //一文字入力の場合　文字の出力位置を記録する.
    SetMojiPoint(Str,&rt);

    for(int seeklen = len;seeklen >= 0 ; seeklen--)
    {
        if (Str[seeklen] == '\n')
        {
            GetTextExtentPoint32(Font_memDC,
                    Str+seeklen+1, len-seeklen-1, &s);
            break;
        }
    }

    *sx += s.cx;
}


#endif //LIFE END

