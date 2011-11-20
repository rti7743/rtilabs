#include <windows.h>
#include <winnls32.h>

static bool StatusIME;

void ChangeIME(bool flg)
{
	WINNLSEnableIME(NULL,(BOOL)flg);
	StatusIME=flg;
}

bool GetIME()
{
    return StatusIME;
}
