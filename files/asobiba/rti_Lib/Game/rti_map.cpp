#include "StartWithExeFile.h"
#ifdef BASE_MAP


#include <.\Game\rti_map.h>
#include <.\Base\rti_error_message.h>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Base\rti_debug.h>
#include <stdio.h>
/*

	リティ's Map Liblary R.T.I (C) copylight since 1999...

*/



/************************************************************************************/
//マップロード
/************************************************************************************/
bool TMap::Read(int no)
{
	FILE *fp;
	char heder[sizeof(MAP_HEDER)];
	char filename[256];
	int filenamesize;

	//ファイル名作成.
	sprintf(filename,MAP_FILENAME,no);

	MapNo = no;
	ChipFileName = NULL;
	Map = NULL;
	Chip = NULL;

    try
    {
	    //ファイルを読み込みます
	    fp = fopen(filename,"rb");
	    if (fp == NULL)
	    {
            throw FileNotOpen("TMap::Read",filename);
    	}
    	//ヘッダを読み込み検査
    	fread(heder,sizeof(MAP_HEDER),sizeof(char),fp); //ヘッダ
	    if (lstrcmp(heder,MAP_HEDER) != 0)
	    {
            throw FriendlyNameError("TMap::Read",filename,"ヘッダがおかしいにょ","正常なファイルに差し替えてください");
    	}
    	filenamesize = fgetc(fp); //BMPのファイル名の長さ
    	//ファイル名格納
    	ChipFileName = (char*) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,filenamesize +1);
    	if (ChipFileName == NULL) 
    	{
            throw FriendlyNameError("TMap::Read",filename,"BMPファイル名を格納するメモリ確保に失敗にょ","ファイルがおかしいか、メモリが足りません");
	    }
	    fread(ChipFileName, filenamesize,sizeof(char),fp); //BMPファイル名

        //使用するマップチップBMPファイルを定義する
	    Chip = BmpChar->Make(ChipFileName);
	    if (Chip == NULL) 
	    {
            throw FriendlyNameError("TMap::Read",filename,"指定された BMPが読めません.","ファイルがおかしいか、MAP_CHIPファイルがありません.");
    	}
        int UseMemroy = sizeof(short) * (MapW * MapH)+1;
        //X Y を読み込む
    	fread(&(MapW),1,sizeof(short),fp); //Xサイズ
    	fread(&(MapH),1,sizeof(short),fp); //Yサイズ
        //マップチップ用のメモリ確保
    	Map = (short*) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, UseMemroy);
    	if (Map == NULL) 
    	{
            throw MemoryDeficient("TMap::Read","MAP領域作成",UseMemroy);
    	}
    }
    catch(...)
    {
        FriendlyNameError("TMap::Read",filename,"このマップファイル読み込みは断念されました.","");
    	Destroy();
	    if (fp != NULL)	fclose(fp);
	    return false;
    }
    //マップを読み込む
    fread(Map,(MapW * MapH),sizeof(short),fp);
	fclose(fp);
	return true;

}


/************************************************************************************/
//マップデータの破棄
/************************************************************************************/
void TMap::Destroy()
{
	if (ChipFileName != NULL)		GlobalFree(ChipFileName);
	if (Map != NULL)				GlobalFree(Map);
	if (Chip != NULL)				BmpChar->Breaker(Chip); 
	ChipFileName = NULL;
	Map = NULL;
	Chip = NULL;
}


/***********************************************************/
//マップの外に飛び出さないように... 飛び出していたら true
/***********************************************************/
bool TMap::Clipping(int x,int y,int w,int h)
{
    return (x <= 0-w || x >= MAP_STOP_XSCROLL ||
					y <= 0-h || y >= MAP_STOP_YSCROLL );
}

/***********************************************************/
//私は表示されていないのかな? されてない true
/***********************************************************/
bool TMap::IamNotDisplay(short x,short y,short faj)
{
    x -= Nowx;
    y -= Nowy;
    return (x < -faj || x > SCREENXSIZE+faj || y < -faj || y > SCREENYSIZE+faj);
}


/**************************************************************/
//マップ 描く
/**************************************************************/
void TMap::MapDrawing(BMD bmd,int Dx,int Dy,int Sx,int Sy,int W,int H)
{
	short wchip;
	short *wmap;

	wmap = Map; //高速化...だと思う.

	//マップを絵画します
	short lx,ly,sx,sy;
	lx = Sx >> MAP_CHIP_POW_SIZE;
	ly = Sy >> MAP_CHIP_POW_SIZE;
	sx = Sx % MAP_CHIP_SIZE;
	sy = Sy % MAP_CHIP_SIZE;
	short x,y,ix,iy;
	for (y = 0 ,iy = ly ;	y <= H ;	    y +=MAP_CHIP_SIZE , iy++ )
	{
		for (x = 0,ix = lx ; x <= W ;	x +=MAP_CHIP_SIZE ,ix ++)
		{
            //マップチップ絵画
			wchip = wmap[MAP_NOW_POINT(ix,iy)];
			::DirectDraw(Chip
						,MAP_CHIPNO_TO_MAP_X(wchip) << MAP_CHIP_POW_SIZE
						,MAP_CHIPNO_TO_MAP_Y(wchip) << MAP_CHIP_POW_SIZE
						,MAP_CHIP_SIZE,MAP_CHIP_SIZE
						,bmd
						,Dx + x-sx,Dy + y-sy);
		}
	}
}

/**************************************************************/
//マップを絵画します
/**************************************************************/
void TMap::DrawMain(BMD bmd,int w,int h)
{
	MapDrawing(bmd,0,0,Nowx,Nowy,w,h);
}

/**************************************************************/
//主人公移動などによるマップスクロール
/**************************************************************/
void TMap::MapMasterMove(short nx,short ny,int speed)
{
	short lx,ly;  //スクリーン座標に変換
	lx = nx-Nowx;
	ly = ny-Nowy;
    //左右
    if (lx <=(SCREENXSIZE/2)-NOMOVEX)
		if (Nowx-speed >= 0) Nowx -=speed;
    if (lx >=(SCREENXSIZE/2)+NOMOVEX)
		if (Nowx+speed <= MAP_STOP_XSCROLL) Nowx +=speed;
    //上下
	if (ly <=(SCREENYSIZE/2)-NOMOVEY)
        if (Nowy-speed >= 0) Nowy -=speed;
	if (ly >=(SCREENYSIZE/2)+NOMOVEY)
        if (Nowy+speed <= MAP_STOP_YSCROLL) Nowy +=speed;
}


/**************************************************************/
//指定したポイントを画面の中心にする.
/**************************************************************/
void TMap::SetCeneter(short nx,short ny)
{
	int lx,ly;
	lx = -(SCREENXSIZEHARF-nx+Nowx);
	ly = -(SCREENYSIZEHARF-ny+Nowy);

	if (lx > 0)
	{
		if (Nowx+lx <= MAP_STOP_XSCROLL) Nowx +=lx;
        else Nowx = MAP_STOP_XSCROLL;
	}
	else
	{
        if (Nowx+lx >= 0)                Nowx +=lx;
        else Nowx = 0;
	}
	if (ly > 0)
	{
		if (Nowy+ly <= MAP_STOP_YSCROLL) Nowy +=ly;
        else Nowy = MAP_STOP_YSCROLL;
	}
	else
	{
        if (Nowy+ly >= 0)                Nowy +=ly;
        else Nowy = 0;
	}
}

/**************************************************************/
//そのポイントを通過できるかチェック
/**************************************************************/
bool TMap::If(short nx,short ny)
{
	//ここで問題になるのが、
	//マップチップをまたぐ場合である.
	//とりあえず、そのすべてのまたぐチップと判定をとる.
	//もし、一つでも移動できないチップがまたがれると、没になる.
	short lx,ly;
	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly  = ny >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx  = nx >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	lx = (nx+31) >> MAP_CHIP_POW_SIZE;
	ly = (ny+31) >> MAP_CHIP_POW_SIZE;
	if (! MAP_THIS_CHIP_THUR(Map[MAP_NOW_POINT(lx,ly)]) )	return false;

	return true;
}

#endif  //LIFE END
