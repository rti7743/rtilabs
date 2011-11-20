#include "StartWithExeFile.h"
#ifdef AVI_LIFE

#ifndef ___AVIH
#define ___AVIH
#pragma comment(lib,"vfw32.lib ")

/*****************************************************************/
/*****************************************************************/
//AVI Player
//              Thank you For C MAGAZIN
/*****************************************************************/
/*****************************************************************/

#include <windows.h>
#include <vfw.h>
#include <tchar.h>
#include <.\Graphics\rti_bitmaplib.h>

class TMyAVI
{
	long GlStart,GlEnd; //スタート位置と終わりの位置
	PAVIFILE Gpavi;     
	PAVISTREAM Gpstm;
	PGETFRAME Gpfrm;
    BMD       GAviBmd;  //これに絵が納められる
    TBmpChar  *GBmpLib;
public:
    TMyAVI();
    ~TMyAVI();
    bool Open(char *filename,TBmpChar *BmpLib);
    bool Close();
    bool Get(int fream);
    //絵を取得
    BMD GetAviBmd(){return GAviBmd;};
};

typedef TMyAVI* AVD;

#endif

#endif

