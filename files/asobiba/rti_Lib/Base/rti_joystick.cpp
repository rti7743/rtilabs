/*****************************************************************/
/*****************************************************************/
// 遊ぶ棒(^^;;  ジョイスティック
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef JOY_STICK_LIFE
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_joystick.h>

/*****************************************************************/
//コンストラクタ
/*****************************************************************/
TJoyStick::TJoyStick(unsigned int UsePad,bool Sailent)
{
	//パッドIDを保存
	PadId = UsePad;

	//ジョイスティックの戻り値を受け取るやつの初期設定
	JoyInfo.dwSize   = sizeof(JOYINFOEX);
	JoyInfo.dwFlags	 = JOY_RETURNX | JOY_RETURNY | JOY_RETURNBUTTONS;
	JoyKeyDown = 0;
	JoyInfo.dwButtons = 0;

	//ジョイスティックが使えるかどうか調べておく
	if (JoyStickInfo(Sailent)==JOYERR_NOERROR)
	{
		bPadAlive = true;
	}
    else
    {
		bPadAlive = false;
    }

}



/*****************************************************************/
//ジョイスティックの情報  Sailentがfalse のときは文句たれを代弁してくれます
/*****************************************************************/
MMRESULT TJoyStick::JoyStickInfo(bool Sailent)
{
	
	MMRESULT MMret = joyGetPosEx(PadId,&JoyInfo);

	if (!Sailent)
	{
		switch(MMret)
		{
		case MMSYSERR_NODRIVER:
			FriendlyError("TJoyStick::JoyStickInfo","ジョイスティック認識失敗",
				"ジョイスティック ドライバが存在しません","ドライバを入れてください");
			break;
		case JOYERR_PARMS:
			FriendlyError("TJoyStick::JoyStickInfo","ジョイスティック認識失敗",
				"指定されたジョイスティックID が無効です","ジョイスティック入れてますか?");
			break;
		case JOYERR_UNPLUGGED:
			FriendlyError("TJoyStick::JoyStickInfo","ジョイスティック認識失敗",
				"指定されたジョイスティックはシステムに接続されていません","さしなさい.");
			break;
		}
	}

	return MMret;
}


/*****************************************************************/
//ジョイスティック情報を更新
/*****************************************************************/
void TJoyStick::JoyStickRef()
{
	int x,y;

	//ボタンと方向の前回の値を保存
	JoyButtonDownOld = JoyInfo.dwButtons; //ボタン
	JoyKeyDownOld = JoyKeyDown;           //方向

	MMRESULT MMret = joyGetPosEx(PadId,&JoyInfo);
	x = JoyInfo.dwXpos;
	y = JoyInfo.dwYpos;

	//方向(privateメンバ)
	JoyKeyDown = 0;      
	if (x < JOY_POS_MIN) JoyKeyDown |= JOY_POS_LEFT; //右
	if (x > JOY_POS_MAX) JoyKeyDown |= JOY_POS_RIGHT; //左
	if (y < JOY_POS_MIN) JoyKeyDown |= JOY_POS_TOP; //上
	if (y > JOY_POS_MAX) JoyKeyDown |= JOY_POS_DOWN; //下

}

#endif
