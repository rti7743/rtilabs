#include <.\Game\rti_GameCommInputDev.h>
#include <.\Base\rti_debug.h>

TGameCommInput::TGameCommInput()
{
	ZeroMemory(JoyStickControl,sizeof(JSC) * 4);
}
TGameCommInput::~TGameCommInput()
{
	//ジョイステックコントロールの破棄.
	for(int i = 0; i < 4 ; i++)
	{
		delete JoyStickControl[i];
	}
}



//指定したキーを RegistryNum 番目のキーとして登録.
bool TGameCommInput::SetKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum)
{
	//登録する場所 おかしくないかチェック.
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;

	switch(DevDiscernment)
	{
	case DEV_JOYSTICK1:	//ジョイステック
	case DEV_JOYSTICK2:	//ジョイステック
	case DEV_JOYSTICK3:	//ジョイステック
	case DEV_JOYSTICK4:	//ジョイステック
		{
			int joy_control_number = DevDiscernment - DEV_JOYSTICK1;
			if ( !JoyStickControl[ joy_control_number ] )
			{	//そのジョイスティックが宣言されていなかったら宣言する.
				JoyStickControl[ joy_control_number ]
						= new TJoyStick(JOYSTICKID1+joy_control_number,true);
			}
		}
		break;
	case DEV_KEYBORD:	//キーボードだだだ.
	case DEV_MOUSE:		//マウス
		break;
	default:
		return false;
	}
	//そのデータを登録する.
	int i;
	_CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		if (lpCommKeyRegistryStruct->DevDiscernment == DEV_NONE)
		{	//空っぽの場所発見!! 登録を開始する.
			lpCommKeyRegistryStruct->DevDiscernment = DevDiscernment;
			lpCommKeyRegistryStruct->Key            = Key;
			break;
		}
		lpCommKeyRegistryStruct ++;	//次のデータへ.
	}
	if (i == COMM_INPUT_MAX_REGISTRY ) return false; //キーは満タンです.
	return true;
}

//指定したキーを RegistryNum より削除
bool TGameCommInput::DestroyKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum)
{
	//登録する場所 おかしくないかチェック.
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;
	//そのデータを検索して削除する.
	int i;
	_CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		if (lpCommKeyRegistryStruct->DevDiscernment == DevDiscernment &&
												lpCommKeyRegistryStruct->Key == Key)
		{	//ターゲット発見.
			lpCommKeyRegistryStruct->DevDiscernment = DEV_NONE;
			lpCommKeyRegistryStruct->Key            = 0;
			break;
		}
		lpCommKeyRegistryStruct ++;	//次のデータへ.
	}
	if (i == COMM_INPUT_MAX_REGISTRY ) return false; //そんなキーありません.
	return true;
}

//指定したキーを更新.
bool TGameCommInput::UpdateKey(DEVDISCERNMENT DevDiscernment,DWORD OldKey,DWORD NewKey,int RegistryNum)
{
	//登録する場所 おかしくないかチェック.
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;
	//そのデータを検索して更新する.
	int i;
	_CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		if (lpCommKeyRegistryStruct->DevDiscernment == DevDiscernment &&
												lpCommKeyRegistryStruct->Key == OldKey)
		{	//ターゲット発見. 
			lpCommKeyRegistryStruct->Key            = NewKey;
			break;
		}
		lpCommKeyRegistryStruct ++;	//次のデータへ.
	}
	if (i == COMM_INPUT_MAX_REGISTRY ) return false; //そんなキーありません.
	return true;
}

//指定したキーが入っているかチェック
bool TGameCommInput::Check(int RegistryNum)
{
	//場所 おかしくないかチェック. (これくらいしてもいいよね.)
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;

	register int i;
	register _CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		switch(lpCommKeyRegistryStruct->DevDiscernment)
		{
		case DEV_KEYBORD:
			//キーボードチェック
			if ( KeyPush( (unsigned char)lpCommKeyRegistryStruct->Key) ) return true;
			break;
		case DEV_JOYSTICK1:
		case DEV_JOYSTICK2:
		case DEV_JOYSTICK3:
		case DEV_JOYSTICK4:
			{	//ジョイスティックチェック
				JSC jsc = JoyStickControl[lpCommKeyRegistryStruct->DevDiscernment - DEV_JOYSTICK1];
				if ( jsc->PadAlive())
				{
					jsc->JoyStickRef();
					if ( jsc->PackKeyGet() & lpCommKeyRegistryStruct->Key)	return true;
				}
			}
			break;
		default:
			return false;
		}
		lpCommKeyRegistryStruct ++;	//次のデータへ.
	}
	return false;
}

//指定したキーが入っているかチェック(連打していないとだめバージョン)
bool TGameCommInput::CheckOne(int RegistryNum)
{
	//場所 おかしくないかチェック. (これくらいしてもいいよね.)
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;

	register int i;
	register _CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		switch(lpCommKeyRegistryStruct->DevDiscernment)
		{
		case DEV_KEYBORD:
			//キーボードチェック
			if ( KeyPushOne( (unsigned char)lpCommKeyRegistryStruct->Key) ) return true;
			break;
		case DEV_JOYSTICK1:
		case DEV_JOYSTICK2:
		case DEV_JOYSTICK3:
		case DEV_JOYSTICK4:
            /*
			{	//ジョイスティックチェック
				JSC jsc = JoyStickControl[lpCommKeyRegistryStruct->DevDiscernment - DEV_JOYSTICK1];
				if (jsc->PadAlive())
				{
					jsc->JoyStickRef();
					if ( jsc->PackKeyGetOne() & lpCommKeyRegistryStruct->Key)	return true;
				}
			}
            */
			break;
		default:
			return false;
		}
		lpCommKeyRegistryStruct ++;	//次のデータへ.
	}
	return false;
}

