#ifndef	___LIM3D_EYECARETAKER__H	//重複防止
#define ___LIM3D_EYECARETAKER__H

#include <./Lim3D/rti_ClassCaretaker.h>
#include <./Lim3D/rti_Lim3DEye.h>

class TLim3DEyeCaretaker : public TClassCaretaker<TLim3DEye>
{
public:
protected:
	//作成したクラスに自分が親だと伝える.(TClassCaretakerより)
	virtual	bool Mount(TLim3DEye* AllocData)
	{	//ダミー
		return true;
	}
private:
};

#endif				//重複防止
