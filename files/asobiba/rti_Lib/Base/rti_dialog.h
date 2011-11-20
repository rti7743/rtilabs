#include <StartWithExeFile.h>
#ifdef DIALOG_LIFE

#ifndef ___DIALOGH
#define ___DIALOGH
#include <windows.h>
class TDialog
{
public:

	static bool Open(HWND hWnd,char *Title,char *Path,LPCSTR Filter,char *RetFullFilename,char *FileTile);
	static bool Save(HWND hWnd,char *Title,char *Path,LPCSTR Filter,char *RetFullFilename,char *FileTile);
    static bool TDialog::Path(HWND hWnd,char *Title,char *RetPath);
};

#endif

#endif //LIFE END
