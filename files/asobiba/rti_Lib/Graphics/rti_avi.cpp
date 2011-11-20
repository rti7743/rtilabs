#include "StartWithExeFile.h"
#ifdef AVI_LIFE
/*****************************************************************/
//AVi 再生ルーチン
/*****************************************************************/

#include <.\Graphics\rti_avi.h>
#include <.\Base\rti_debug.h>

/*****************************************************************/
//コンストラクタ
/*****************************************************************/
TMyAVI::TMyAVI()
{
    Gpstm = NULL;
    GBmpLib = NULL;
    GAviBmd = NULL;
	AVIFileInit();
}
/*****************************************************************/
//デストラクタ
/*****************************************************************/
TMyAVI::~TMyAVI()
{
    Close();
	AVIFileExit();
}
/*****************************************************************/
//ファイルオープンですわ
/*****************************************************************/
bool TMyAVI::Open(char *filename,TBmpChar *BmpLib)
{
	AVIFILEINFO fi;
	AVISTREAMINFO si;
	DWORD dwStream;
	WORD wVideo=~0;
	PAVISTREAM ptmp;

    GBmpLib = BmpLib;

	if (AVIFileOpen(&Gpavi,_T(filename),OF_READ | OF_SHARE_DENY_NONE,NULL)!=0)
		return false;
	if (AVIFileInfo(Gpavi,&fi,sizeof(AVIFILEINFO))!=0)
		return false;
	for (dwStream=0;dwStream<fi.dwStreams;dwStream++) {
		if (AVIFileGetStream(Gpavi,&ptmp,0,dwStream)!=0)
    		return false;
		if (AVIStreamInfo(ptmp,&si,sizeof(AVISTREAMINFO))!=0)
			return false;
		switch (si.fccType) {
			case streamtypeVIDEO:
				if (Gpstm==NULL || si.wPriority<wVideo) {
					if (Gpstm!=NULL)
						AVIStreamRelease(Gpstm);
					Gpstm=ptmp;
					wVideo=si.wPriority;
				}
				continue;
		}
		AVIStreamRelease(ptmp);
	}
	if (Gpstm==NULL)
		return false;

    //空の領域作成
    GAviBmd = GBmpLib->New(fi.dwWidth,fi.dwHeight);
    if (!GAviBmd) return false;

	if ((Gpfrm=AVIStreamGetFrameOpen(Gpstm,&GAviBmd->bm_header))==NULL)
    {
        GBmpLib->Breaker(GAviBmd);
        GAviBmd = NULL;
		return false;
    }

	GlStart=AVIStreamStart(Gpstm);
	GlEnd=AVIStreamLength(Gpstm)+GlStart;

    return true;
}
/*****************************************************************/
//ファイルクローズですわ
/*****************************************************************/
bool TMyAVI::Close()
{
    if (GAviBmd) GBmpLib->Breaker(GAviBmd);

	if (AVIStreamGetFrameClose(Gpfrm)!=0)
		return false;
	AVIStreamRelease(Gpstm);
	AVIFileRelease(Gpavi);
    return true;
}

/*****************************************************************/
//指定したフレームの絵をゲット
/*****************************************************************/
bool TMyAVI::Get(int fream)
{
	LPBITMAPINFOHEADER pbmih;
    if (fream < GlStart || fream >= GlEnd) return false;
 
	if ((pbmih=(LPBITMAPINFOHEADER)
        AVIStreamGetFrame(Gpfrm,fream))==NULL)
		return false;
//    CopyMemory(GAviBmd->bm,pbmih+13
//                              ,GAviBmd->bm_header.biSizeImage);
    CopyMemory(GAviBmd->bm,pbmih+1
                              ,GAviBmd->bm_header.biSizeImage);
    return true;
}

#endif
