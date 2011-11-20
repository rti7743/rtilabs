#ifndef ___SYSTEMINFO_H
#define ___SYSTEMINFO_H

class TSystemInfo
{
    bool MMX;   //MMX ‚ðŽg‚¦‚é‚©‚Ç‚¤‚©.
public:
    TSystemInfo();
    bool GetMMMX(){return MMX;};
};


extern TSystemInfo SystemInfo;

#endif
