
#include <.\Base\rti_joystick.h>
#include <.\Base\rti_key.h>

#define COMM_INPUT_MAX_DISCERNMENT 64	//識別子の数.
#define COMM_INPUT_MAX_REGISTRY 64		//その登録場所が 識別できる最大のキーの数.

//識別デバイス
enum DEVDISCERNMENT
{
	DEV_NONE,			//空っぽ.
	DEV_KEYBORD,		//キーボード
	DEV_JOYSTICK1,		//ジョイステック1
	DEV_JOYSTICK2,		//ジョイステック2
	DEV_JOYSTICK3,		//ジョイステック3
	DEV_JOYSTICK4,		//ジョイステック4
	DEV_MOUSE,			//マウス
};	
struct _CommKeyRegistryStruct
{
	DEVDISCERNMENT DevDiscernment;	//デバイス識別子
	DWORD Key;						//キー
};

/************************************************************************/
//統合インターフェース
/************************************************************************/
class TGameCommInput
{
	JSC JoyStickControl[4];
	_CommKeyRegistryStruct CommKeyRegistryStruct[COMM_INPUT_MAX_DISCERNMENT][COMM_INPUT_MAX_REGISTRY];
public:
	//	=================  メソッド =====================
	TGameCommInput();
	~TGameCommInput();
	//指定したキーを RegistryNum 番目のキーとして登録.
	bool SetKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum);
	//指定したキーを RegistryNum より削除
	bool DestroyKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum);
	//指定したキーを更新.
	bool UpdateKey(DEVDISCERNMENT DevDiscernment,DWORD OldKey,DWORD NewKey,int RegistryNum);
	//指定したキーが入っているかチェック
	bool Check(int RegistryNum);
	//指定したキーが入っているかチェック(連打していないとだめバージョン)
	bool CheckOne(int RegistryNum);
};
