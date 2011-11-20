#include "StartWithExeFile.h"
#ifdef BITMAPLIB_LIFE

#ifndef ___BITMAPLIBH
#define ___BITMAPLIBH
#include <windows.h>
#include <.\Graphics\rti_graphics.h>    //グラフィックスのボス
#include <vector>
 	
class TBmpChar
{
private:
    HWND hwnd;
    std::vector<BMD> BmdList;
public: 
    TBmpChar(HWND whwnd){hwnd = whwnd;};
    TBmpChar(){hwnd = GetActiveWindow();};
	~TBmpChar();
    
    //ファイルから読み込む
	BMD Make(const char *filename);
    //すべて破棄
	void Clear();
    //指定したヤツをはき
	void Breaker(struct BMP_DATA_TABLE *puf);
    //新規作成
    BMD New(int sx,int sy);
    //コピー
    BMD Copy(BMD bmd);
    //
    BMD At(int no){return BmdList[no];};
};


extern TBmpChar *BmpChar;

#endif

#endif //LIFE END
