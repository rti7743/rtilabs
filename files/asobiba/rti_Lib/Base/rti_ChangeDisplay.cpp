/***********************************************************************/
//指定解像度に変更するライブラリ
//    Olginal Pascal Src By 疋田雅也　さん
//    C++                By R.T.I
//     パスカルから移植しました (^^;;
//     しかし、 NT では動かないらしい.
//     まっ、オイラはそんなもんつかわないからいいげとね(ぉぉぉ
/***********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_ChangeDisplay.h>
#include <.\Base\rti_debug.h>

/***********************************************************************/
//コンストラクタ
/***********************************************************************/
TChangeDisplay::TChangeDisplay()
{
    InitializeFlag  = false;
    ChangeSuccess   = false;
}

/***********************************************************************/
//デストラクタ
/***********************************************************************/
TChangeDisplay::~TChangeDisplay()
{
}

/***********************************************************************/
//初期化 成功 true  /  失敗 false
/***********************************************************************/
bool TChangeDisplay::Init()
{
    HKEY  key;
    unsigned char rbuf[20];
    DWORD size = sizeof(rbuf);
    DWORD size2 = sizeof(rbuf);

    //レジストリから現在の解像度と色数を読みとり保存しておきます.
    //元に戻すことができるようにするためだねっ
    RestorationBitsPerPal = 0;
    if (RegOpenKey( HKEY_CURRENT_CONFIG,"Display\\Settings",  &key ) 
              == ERROR_SUCCESS)
    {
         //色数ゲットだぜー
         if (RegQueryValueEx
            (key, "BitsPerPixel", NULL, NULL,rbuf, &size) ==ERROR_SUCCESS)
         {
               RestorationBitsPerPal = atoi((char*)rbuf);

               //解像度ゲットだぜー
               if (RegQueryValueEx
                   (key, "Resolution", NULL, NULL,
                            rbuf, &size2) ==ERROR_SUCCESS)
               {
                   sscanf((char*)rbuf,"%d,%d",&RestorationWidth,&RestorationHeight);
                   InitializeFlag = true;
               }
         }
         //なんじのあるべき姿に戻れ(^^;;
         RegCloseKey( key );
    }
    return InitializeFlag;
}

/***********************************************************************/
//チェンジ  成功 true  /  失敗 false
/***********************************************************************/
bool TChangeDisplay::Change(UINT Width, UINT Height, UINT BitsPerPal)
{
    int i;
    bool CanChange ;
    DEVMODEA dm,dms ;

    if (InitializeFlag == false) 
    {  //まだ初期化されていないので初期化しにいく.
        if (!Init()) return false;
    }

    //カラー数が書いてなかったらディフォを使う.
    if (BitsPerPal == 0 ) BitsPerPal = RestorationBitsPerPal;
    if (BitsPerPal == 0 ) return false;

    //チェンジできる解像度をサーチします.
    CanChange = false;
    for (i = 0;EnumDisplaySettings(NULL, i, &dm) ; i++)
    {
        if ( (dm.dmPelsWidth == Width)
                 && (dm.dmPelsHeight == Height)
                 && (dm.dmBitsPerPel == BitsPerPal) )
        {
            //チェンジできるかテスト
             if (ChangeDisplaySettings(&dm, CDS_TEST) 
                         == DISP_CHANGE_SUCCESSFUL )
             {
                 dms = dm;
                 CanChange = true;
             }
        }
    }
    //変更できる解像度が見つからなかった場合、
    //念のため、直接指定された解像度に変更して良いのか直接テストしてみる (^^;

    if (! CanChange)
    {
        dm.dmPelsWidth = Width;
        dm.dmPelsHeight = Height;
	    if (BitsPerPal == 0 ) dm.dmBitsPerPel = RestorationBitsPerPal;
        else				  dm.dmBitsPerPel = BitsPerPal;
        //チェンジできるかテスト
        if (ChangeDisplaySettings(&dm, CDS_TEST) 
                         == DISP_CHANGE_SUCCESSFUL )
        {
                 dms = dm;
                 CanChange = true;
        }
    }

    //チェンジできる解像度が見つかったの ?
    if (CanChange)
    {   
        //チェンジしまーす
       ChangeDisplaySettings(&dms, 0);
       ChangeSuccess = true;
    }
    return CanChange;           
}

/***********************************************************************/
//元に戻す  成功 true  /  失敗 false
/***********************************************************************/
bool TChangeDisplay::Restoration()
{
    if ( (InitializeFlag == false)  )
    {  //初期化もされていないのに よぶんぢゃねー
       return false;
    }
    if (ChangeSuccess)
    {
         if ( Change(RestorationWidth,
             RestorationHeight,RestorationBitsPerPal) )
         {
              ChangeSuccess = false;
              return true;
         }
    }
    return false;
}
