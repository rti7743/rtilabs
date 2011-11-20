#include "StartWithExeFile.h"
#ifdef GAME_ANIME_LIFE

#ifndef ___GAME_ANIME_BANK_FOR_FILEH
#define ___GAME_ANIME_BANK_FOR_FILEH

#include <.\Game\rti_Game_AnimeBank.h>
#include <.\Graphics\rti_bitmaplib.h>


class TGameAnimeBankForFile : public TGameAnimeBank
{
    TBmpChar *AnimeBankBmpChar;
public:
    TGameAnimeBankForFile(){ AnimeBankBmpChar = new TBmpChar();};
    ~TGameAnimeBankForFile(){delete AnimeBankBmpChar;};
    bool Make(char *filename);
};

//アニメデータのボス
extern TGameAnimeBankForFile GameAnimeBankForFile;

#endif

#endif //LIFE END
