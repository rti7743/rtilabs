
#include <StartWithExeFile.h>
#ifdef ERROR_MESSAGE_LIFE

#ifndef ___ERROR_MESSAGEH
#define ___ERROR_MESSAGEH
	void MsgError3(char* Format,...);
	int MsgOr3(char* Format,...);
    void FriendlyError
        (const char *FunktionName,const char *Inpor,const char *Reason,const char *Support);
    void FriendlyNameError
        (const char *FunktionName,const char *Name,const char *Reason,const char *Support);
    void FileNotOpen(const char *FunktionName,const char *Filename);
    void MemoryDeficient(const char *FunktionName,const char *Inpor,int Size);
    void ProgramError
        (const char *FunktionName,const char *Inpor,const char *Reason);

#define MSGOUT(wcaption,str2) \
	MessageBox(NULL,wcaption,str2,MB_OK | MB_TASKMODAL)

#endif

#endif //LIEF END
