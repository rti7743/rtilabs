#include "StartWithExeFile.h"
#ifdef BITMAPLIB_LIFE

/*****************************************************************/
/*****************************************************************/
/*    ハイパービットマップ　ライブラリ         */
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <vector>
#include <algorithm>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_debug.h>
#include <.\Comm\rti_Comm.h>

TBmpChar *BmpChar;

/**********************************************************/
// キャラクターを作成する
/**********************************************************/
struct BMP_DATA_TABLE* TBmpChar::Make(const char *filename)
{
	struct BMP_DATA_TABLE *puf;
    //とりあえず、初期化.
    puf = NULL;
    try
    {
	    //新規に作成します
        puf = new (struct BMP_DATA_TABLE) ;//メモリ確保
        if (!puf) throw (MemoryDeficient("TBmpChar::Make","BMD領域確保",sizeof(BMP_DATA_TABLE) ) );
    
        if ( !CommDraw->GraphicsMake(puf,filename) )
			throw (FriendlyError("TBmpChar::Make","グラフィックス読み込み","ファイル読み込みに失敗",""));

        //リストに追加
        BmdList.push_back(puf);
    }
    catch(...)
    {
        delete puf; //NULL を delete しても大丈夫.
	   	return NULL;
    }
	return puf;
}


/**********************************************************/
// デストラクタ
/**********************************************************/
TBmpChar::~TBmpChar()
{
	Clear();
}

/**********************************************************/
// すべてのキャラクターを破棄する
/**********************************************************/
void TBmpChar::Clear()
{
    std::vector<BMD>::iterator it;

    //すべてのリストにアクセスして解放する.
    for(it = BmdList.begin() ; it != BmdList.end() ; it ++)
    {
		CommDraw->GraphicsFree((*it));
        delete (*it);
    }
    //リストのすべてのデータの破棄
    BmdList.clear();
}

/**********************************************************/
// 任意のキャラクターを破棄する
/**********************************************************/
void TBmpChar::Breaker(struct BMP_DATA_TABLE *puf)
{
    std::vector<BMD>::iterator it =
        std::find(BmdList.begin(),BmdList.end(),puf);
    if(it != BmdList.end())
    {
	        CommDraw->GraphicsFree(puf);
        	delete puf;
            BmdList.erase(it);
    }
}

/**********************************************************/
// なにも表示されないキャラクターを作る
/**********************************************************/
BMD TBmpChar::New(int sx,int sy)
{
    int x,y;
	struct BMP_DATA_TABLE *puf;
    //とりあえず、初期化.
	//サイズが 0 だったら Sprite の大きさ
    if (!sx) x = Sprite->w;
    else     x = sx;
    if (!sy) y = Sprite->h;
    else     y = sy;

    puf = NULL;
    try
    {
    	//新規に作成します
        puf = new (struct BMP_DATA_TABLE) ;//メモリ確保
        if (!puf) throw (MemoryDeficient("TBmpChar::New","BMD領域確保",sizeof(BMP_DATA_TABLE) ) );

		puf->w = x ;		puf->h = y ;
        if (!CommDraw->GraphicsAlloc(puf) )
			throw (FriendlyError("TBmpChar::New","新規領域作成","新規領域を確保するメモリが足りません","メモリを空けてください"));
        //リストに追加
        BmdList.push_back(puf);
    }
    catch(...)
    {
        delete puf; //NULL を delete しても大丈夫.
	   	return NULL;
    }
	return puf;
}

/**********************************************************/
// コピーを作る
/**********************************************************/
BMD TBmpChar::Copy(BMD bmd)
{
    BMD nbmd = New(bmd->w,bmd->h);
    if (nbmd == NULL)  return NULL;

    memcpy(nbmd->bm,bmd->bm,(nbmd->w*nbmd->h)*3);

	return nbmd;
}



#endif //LIFE END
