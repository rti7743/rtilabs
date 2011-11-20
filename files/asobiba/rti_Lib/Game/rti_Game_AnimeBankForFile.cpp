#include "StartWithExeFile.h"
#ifdef GAME_ANIME_LIFE

#include <.\Game\rti_Game_AnimeBankForFile.h>


TGameAnimeBankForFile GameAnimeBankForFile;

#define DEFINE_ANIME_BMD      "DEFINE_ANIME_BMD"        //定義開始
#define DEFINE_ANIME_PATH     "DEFINE_ANIME_PATH"       //パス
#define DEFINE_ANIME_RECT     "DEFINE_ANIME_RECT"       //その絵のどこを使うのか.
#define DEFINE_DDESHAPE       "DEFINE_DDESHAPE"         //絵の共有
#define DEFINE_ANIME_END      "DEFINE_ANIME_END"        //定義終了
/*
    アニメーションデータの定義.
DEFINE_ANIME_BMD                //必須
DEFINE_ANIME_PATH   char*       //必須
DEFINE_ANIME_RECT   int int int int //X,Y,W,H
DEFINE_ANIME_RECT   int int int int //X,Y,W,H
DEFINE_ANIME_RECT   int int int int //X,Y,W,H
DEFINE_ANIME_RECT   int int int int //X,Y,W,H

  //RECT をアニメパターン分作成する. 1 ～ 5.

*/
#include <.\Base\rti_data_list.h>
#include <.\Base\rti_error_message.h>
/*******************************************************************/
//アニメーションデータをファイルから読み込む
/*******************************************************************/
bool TGameAnimeBankForFile::Make(char *filename)
{
    TCommandList TCL;
    char *CL[] = 
    {      
        DEFINE_ANIME_BMD,DEFINE_ANIME_PATH,DEFINE_ANIME_RECT,DEFINE_ANIME_END, NULL,
    };
    TCL.set(CL);

    int  AnimeSrcNum;   //アニメソースの現在使っている数.
    BMD  UseBmd;        //絵
    Anime_Src_struct AS[5];     //アニメソース.

    FILE *fp;           //ファイルポインタ
    int x,y,w,h;        //四方
    int ddshape_no;     //絵のデータ共有の場合
    char *NextChar;
    char buf[MAX_PATH]; //ファイルを読み込むときに使うバッファ
    char bmd_filename[MAX_PATH];

    fp = fopen(filename,"rb");
    if (!fp)
    {
        FileNotOpen("TGameAnimeBankForFile::Make",filename);
        return false;
    }

    while( !feof(fp) )
    {
        if ( !fgets(buf,256,fp) ) break;
        NextChar = buf;
        switch (TCL.search(buf,&NextChar) )
        {
        case -1:    //エラー
            break;
        case 0:     //DEFINE_ANIME_BMD
            //登録個数のクリア.
            AnimeSrcNum = 0;
            break;
        case 1:
            sscanf(NextChar,"  %s    %d",bmd_filename,&ddshape_no);
            if (stricmp(bmd_filename,DEFINE_DDESHAPE) == 0)
            {//絵の共有
                UseBmd = AnimeBankBmpChar->At(ddshape_no);
            }
            else
            {
                UseBmd = AnimeBankBmpChar->Make(bmd_filename);
                if ( ! UseBmd)
                {
                    MsgError3("ファイル %s で、絵 %s の読み込みに失敗しました!",filename,bmd_filename);
                    return false;    
                }
            }
            break;
        case 2:
            if (AnimeSrcNum > 4) break;
            sscanf(NextChar,"  %d  %d  %d  %d",&x,&y,&w,&h);
            //マイナスの幅高さは、画像の最大幅とする.
            if (w < 0)  w = UseBmd->w;
            if (h < 0)  h = UseBmd->h;
            //登録
            AS[AnimeSrcNum].x = x;            AS[AnimeSrcNum].y = y;
            AS[AnimeSrcNum].w = w;            AS[AnimeSrcNum].h = h;
            AS[AnimeSrcNum].bmd = UseBmd;
            AnimeSrcNum++;
            break;
        case 3:     //DEFINE_ANIME_END
            //前回のデータの登録をやります.
            switch(AnimeSrcNum)
            {
            case 1: New(NULL,&AS[0],NULL,NULL,NULL,NULL);        break;
            case 2: New(NULL,&AS[0],&AS[1],NULL,NULL,NULL);      break;
            case 3: New(NULL,&AS[0],&AS[1],&AS[2],NULL,NULL);    break;
            case 4: New(NULL,&AS[0],&AS[1],&AS[2],&AS[3],NULL);  break;
            case 5: New(NULL,&AS[0],&AS[1],&AS[2],&AS[3],&AS[4]);break;
            }
        }
    }
    fclose(fp);
    return true;
}

#endif //LIFE END
