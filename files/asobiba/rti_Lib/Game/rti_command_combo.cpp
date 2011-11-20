#include "StartWithExeFile.h"
#ifdef COMMAND_COMBO_MAP

#include <.\Game\rti_command_combo.h>
#include <.\Base\rti_error_message.h>

/************************************************************************************/
//キー登録のボス的存在.
/************************************************************************************/
void TCommandCombo::Keyes()
{
	//ジョイスティック情報取得.
	JoyStick->JoyStickRef();
	//追加
	AddKeys( JoyStick->PackKeyGet() );
}

/************************************************************************************/
//コマンド入力判定
/************************************************************************************/
void TCommandCombo::KeyCmp()
{
	struct _CommandKeyes Hadou[]={
						{	JOY_PACK_BUTTOM_MASK|	JOY_PACK_A		,10} ,
						{	JOY_PACK_BUTTOM_MASK|	JOY_POS_RIGHT	,10} ,
						{	JOY_PACK_BUTTOM_MASK|	JOY_POS_RIGHT | JOY_POS_DOWN,10} ,
						{	JOY_PACK_STICK_MASK	|	JOY_POS_DOWN	,0} ,
	}; 
	if ( KeyCmp_SearchKey(Hadou) )
	{
		MsgOr3("Ok.");
	}
}

/************************************************************************************/
//キーサーチ
/************************************************************************************/
bool TCommandCombo::KeyCmp_SearchKey(_CommandKeyes *CKS)
{
	int count_key;	//現在そのキーをどこまで調べたか... と、いう番号
	int time;		//調べる時間
	int wait;		//受け付けのこり時間
	DWORD NowKey;		//リングキューから取り出した 入力されたやつ.
	DWORD CommandKey;
	for(time = 0, wait = 1 , count_key = 0 ; wait ; time++ ,wait --)
	{
		NowKey = *(PackLogKey.back(time));
		CommandKey = CKS[count_key].PackKey;
		if ((NowKey&JOY_PACK_BUTTOM_MASK) == (CommandKey&JOY_PACK_BUTTOM_MASK) ||
			(NowKey&JOY_PACK_STICK_MASK)  == (CommandKey&JOY_PACK_STICK_MASK)	)
		{	//探していたキーと一致した.
			wait = CKS[count_key].Wait;
			if (wait == 0) 
			{
				PackLogKey.clear();
				return true;	//成立
			}
			count_key ++;
			time --; //もう一回、調べる.(ボタンの同時押しもOK)
		}
	}
	return false;
}



#endif  //LIFE END
