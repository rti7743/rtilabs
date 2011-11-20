#include "StartWithExeFile.h"
#ifdef COMMAND_COMBO_MAP

#ifndef ___GAME_COMMAND_COMBOH
#define ___GAME_COMMAND_COMBOH

#include <.\Base\rti_ring_qeue.h>
#include <.\Base\rti_joystick.h>

struct _CommandKeyes
{
	DWORD PackKey;		//キー(パック)
	int	  Wait;			//受付時間 最終レコードは 0 である事.
};

class TCommandCombo	//格闘の名前ではない
{
	//パック キーデータだぴょん.
    ringqueue256<DWORD> PackLogKey;
	//ジョイスティック
	TJoyStick *JoyStick;
	//キーサーチ
	bool KeyCmp_SearchKey(_CommandKeyes *CKS);
public:
	TCommandCombo(){};
	//ジョイスティック登録
	void SetJoyStick(TJoyStick *j){JoyStick = j;};
	//キー登録のボス的存在.
	void Keyes();
	//キーを登録
	void AddKeys(DWORD C){ PackLogKey.push_back(&C); };
	//キーを取得
	DWORD GetKeys(){ return (* PackLogKey. pop() );     };
	//キー判定
	void KeyCmp();
};
#endif


#endif  //LIFE END

