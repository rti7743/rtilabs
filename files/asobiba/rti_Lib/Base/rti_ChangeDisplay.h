#ifndef ___ChangeDisplayH
#define ___ChangeDisplayH

class TChangeDisplay
{
private:
    UINT RestorationWidth  ;
    UINT RestorationHeight ;
    UINT RestorationBitsPerPal ;
    bool InitializeFlag ;
    bool ChangeSuccess ;
    bool Init();
public:
    TChangeDisplay();
    ~TChangeDisplay();
    bool Change(UINT Width, UINT Height, UINT BitsPerPal);
    bool Restoration();
};


#endif
