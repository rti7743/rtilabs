// YSettingTXT.cpp: YSettingTXT クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YSettingTXT.h"
#include "RConfigFile.h"
#include "YHttpdConf.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YSettingTXT::YSettingTXT()
{

}

YSettingTXT::~YSettingTXT()
{

}

void YSettingTXT::Load(const string inBoardName)
{
	
	RConfigFile	Setting;

	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "/SETTING.TXT";
	//設定をロードするのですよー.
	Setting.Load(filename);

	/*
		BBS_TITLE		板の名前

		例:
			BBS_TITLE=格闘技＠2ch掲示板
		詳細
			掲示板の名前です。
			ここで設定した名前が index.html などに表示されます。
		省略時
			名無し板
	*/
	BBS_TITLE = Setting.LoadOrDefault("BBS_TITLE" , string("名無し板") );
	/*
		BBS_NONAME_NAME	ディフォルトの名前

		例:
			BBS_NONAME_NAME=名無しの格闘家
		詳細
			掲示板の名前です。
			ここで設定した名前が index.html などに表示されます。
		省略時
			名無しさん＠おなかいっぱい
	*/
	BBS_NONAME_NAME = Setting.LoadOrDefault("BBS_NONAME_NAME" , string("名無しさん＠おなかいっぱい") );
	/*
		BBS_PROXY_CHECK	PROXY制限

		例:
			BBS_PROXY_CHECK=checked
			BBS_PROXY_CHECK=
		詳細
			プロキシチェックをするかどうかです。
			checked にするとプロキシチェックになります。
			何も書かない場合はプロキシチェックは行わないということです。
		省略時
			チェックなし
	*/
	BBS_PROXY_CHECK = Setting.LoadOrDefault("BBS_PROXY_CHECK" , SettingTxtBool(false) );
	/*
		BBS_NO_ID	任意ID

		例:
			BBS_NO_ID=checked
			BBS_NO_ID=
		詳細
			任意IDをするかどうかです。
			checked にすると任意IDになります。
			何も書かない場合は任意IDは行わないということです。
		省略時
			任意IDなし
	*/
	BBS_NO_ID = Setting.LoadOrDefault("BBS_NO_ID" , SettingTxtBool(false) );
	/*
		BBS_FORCE_ID	強制ID

		例:
			BBS_FORCE_ID=checked
			BBS_FORCE_ID=
		詳細
			強制IDをするかどうかです。
			checked にすると強制IDになります。
			何も書かない場合は強制IDは行わないということです。

			このパラメータはBBS_NO_ID より強いので、 両方指定された場合は、
			BBS_FORCE_IDが優先されます。
		省略時
			任意IDなし
	*/
	BBS_FORCE_ID = Setting.LoadOrDefault("BBS_FORCE_ID" , SettingTxtBool(false) );
	/*
		BBS_DELETE_NAME	削除時の名前

		例:
			BBS_DELETE_NAME=あぼーん
		詳細
			削除時に使用される名前です。

		省略時
			あぼーん
	*/
	BBS_DELETE_NAME = Setting.LoadOrDefault("BBS_DELETE_NAME" , string("あぼーん") );
	/*
		BBS_SUBJECT_COUNT	タイトルの長さ

		例:
			BBS_SUBJECT_COUNT=60
		詳細
			タイトルに使用できる文字列の長さです。
			>>1 さんがつけるやつね。

		省略時
			60
	*/
	BBS_SUBJECT_COUNT = Setting.LoadOrDefault("BBS_SUBJECT_COUNT" , 60 );
	/*
		BBS_NAME_COUNT	名前の長さ

		例:
			BBS_NAME_COUNT=30
		詳細
			名前の長さです。
			通常書き込み時につける名前ですね。
			大抵は 名無しさん なので関係ないですが(w

		省略時
			30
	*/
	BBS_NAME_COUNT = Setting.LoadOrDefault("BBS_NAME_COUNT" , 30 );
	/*
		BBS_MAIL_COUNT	メールアドレスの長さ

		例:
			BBS_MAIL_COUNT=30
		詳細
			メールアドレスの長さです。
			通常書き込み時につけるメールアドレスですね。
			大抵は age/sage なので関係ないですが(w

		省略時
			30
	*/
	BBS_MAIL_COUNT = Setting.LoadOrDefault("BBS_MAIL_COUNT" , 30 );
	/*
		BBS_MESSAGE_COUNT	メッセージのパイト数

		例:
			BBS_MESSAGE_COUNT=2048
		詳細
			メールアドレスの長さです。
			通常書き込み時につけるメールアドレスですね。
			大抵は age/sage なので関係ないですが(w

		省略時
			2048
	*/
	BBS_MESSAGE_COUNT = Setting.LoadOrDefault("BBS_MESSAGE_COUNT" , 2048 );
	/*
		SUBBBS_CGI_ON	新規スレッド立てる時の確認あり

		例:
			SUBBBS_CGI_ON=checked
			SUBBBS_CGI_ON=
		詳細
			新規スレッド立てる時の確認をするかどうかです。
			checked にすると新規スレッド立てる時の確認になります。
			何も書かない場合は新規スレッド立てる時の確認は行わないということです。

		省略時
			なし
	*/
	SUBBBS_CGI_ON = Setting.LoadOrDefault("SUBBBS_CGI_ON" , SettingTxtBool(false) );
	/*
		NANASHI_CHECK	無名の禁止

		例:
			NANASHI_CHECK=checked
			NANASHI_CHECK=
		詳細
			名前の省略を許さないかどうかです。
			checked にすると名前の省略を許さなくなります。
			何も書かない場合は名前の省略を許し、BBS_NONAME_NAME で設定した名前になります。

		省略時
			なし
	*/
	NANASHI_CHECK = Setting.LoadOrDefault("NANASHI_CHECK" , SettingTxtBool(false) );
	/*
		BBS_NOTJP_FILTER	.jp以外のホストからの書込み禁止

		例:
			BBS_NOTJP_FILTER=checked
			BBS_NOTJP_FILTER=
		詳細
			.jp(JPNICが管理する)IP以外からの書込み禁止を禁止します。
			checked にすると書込み禁止禁止として動作します。
			何も書かない場合は規制は発動されません。

		省略時
			なし
	*/
	BBS_NOTJP_FILTER = Setting.LoadOrDefault("BBS_NOTJP_FILTER" , SettingTxtBool(false) );

	/*
		SAKUBBS	削除依頼掲示板の名前 (ここが削除依頼板の場合は元板の名前)

		例:
			SAKUBBS=animesaku
		詳細
			この掲示板に対する削除依頼板の名前です。
			ここが削除依頼板の場合は、元板の名前を書いてください。

		省略時
			省略は基本的に許されません!!!
	*/
	SAKUBBS = Setting.LoadOrDefault("SAKUBBS" , string("DANGER") );
	/*
		IS_SAKUBBS	ここが削除依頼板なのかどうか

		例:
			IS_SAKUBBS=checked
			IS_SAKUBBS=
		詳細
			ここが削除依頼板であるかどうかです。
			checked にすると削除依頼板として動作します。
			何も書かない場合は通常の板として動作します。

		省略時
			なし
	*/
	IS_SAKUBBS = Setting.LoadOrDefault("IS_SAKUBBS" , SettingTxtBool(false) );
	/*
		DELETE_WAIT	削除までプールする秒数

		例:
			DELETE_WAIT=86400
		詳細
			削除依頼がたされてから、削除するまでの秒数です。
			ディフォルトは一日です。

		省略時
			86400
	*/
	DELETE_WAIT = Setting.LoadOrDefault("DELETE_WAIT" , 86400 );
	/*
		BBS_DISP_IP	IPの表示

		例:
			BBS_DISP_IP=checked
			BBS_DISP_IP=
		詳細
			IPを表示するかどうかです。
			checked にするとIP表示として動作します。
			何も書かない場合はIPを表示しません。
			削除依頼板では 表示することをお勧めします。

		省略時
			なし
	*/
	BBS_DISP_IP = Setting.LoadOrDefault("BBS_DISP_IP" , SettingTxtBool(false) );
}

