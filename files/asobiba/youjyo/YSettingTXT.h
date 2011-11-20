// YSettingTXT.h: YSettingTXT クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSETTINGTXT_H__22F20E01_266A_40C8_94AC_D0B644FEFD15__INCLUDED_)
#define AFX_YSETTINGTXT_H__22F20E01_266A_40C8_94AC_D0B644FEFD15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotOpenException.h"
	//SettingTxtでの bool型.
	class SettingTxtBool
	{
	public:
		SettingTxtBool(bool inB)
		{
			Check = inB;
		}
		SettingTxtBool(string inStr)
		{
			if ( inStr == "1" )	Check = true;
			else if ( inStr == "checked" )	Check = true;
			else		Check = false;
		}
		bool get() const
		{
			return Check;
		}

		operator bool()
		{
			return Check;
		}
	private:
		bool	Check;

	};
	

class YSettingTXT  
{
public:
	YSettingTXT();
	virtual ~YSettingTXT();

	void Load(const string inBoardName);


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
	string getBBS_TITLE() const{	return BBS_TITLE;	}
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
	string getBBS_NONAME_NAME() const{	return BBS_NONAME_NAME;	}
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
	bool getBBS_PROXY_CHECK() const{	return BBS_PROXY_CHECK;	}
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
	bool getBBS_NO_ID() const{	return BBS_NO_ID;	}
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
	bool getBBS_FORCE_ID() const{	return BBS_FORCE_ID;	}
	/*
		BBS_DELETE_NAME	削除時の名前

		例:
			BBS_DELETE_NAME=あぼーん
		詳細
			削除時に使用される名前です。

		省略時
			あぼーん
	*/
	string getBBS_DELETE_NAME() const{	return BBS_DELETE_NAME;	}
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
	int getBBS_SUBJECT_COUNT() const{	return BBS_SUBJECT_COUNT;	}
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
	int getBBS_NAME_COUNT() const{	return BBS_SUBJECT_COUNT;	}
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
	int getBBS_MAIL_COUNT() const{	return BBS_MAIL_COUNT;	}
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
	int getBBS_MESSAGE_COUNT() const{	return BBS_MESSAGE_COUNT;	}
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
	bool getSUBBBS_CGI_ON() const{	return SUBBBS_CGI_ON;	}
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
	bool getNANASHI_CHECK() const{	return NANASHI_CHECK;	}
	/*
		BBS_NOTJP_FILTER	.jp以外のホストからの書き込み禁止

		例:
			BBS_NOTJP_FILTER=checked
			BBS_NOTJP_FILTER=
		詳細
			.jp(JPNICが管理する)IP以外からの書き込みを禁止します。
			checked にすると書き込み禁止として動作します。
			何も書かない場合は規制は発動されません。

		省略時
			なし
	*/
	bool	getBBS_NOTJP_FILTER() const{	return BBS_NOTJP_FILTER;	}
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
	string getSAKUBBS() const{	return SAKUBBS;	}
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
	bool getIS_SAKUBBS() const{	return IS_SAKUBBS;	}
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
	int getDELETE_WAIT() const{	return DELETE_WAIT;	}
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
	bool	getBBS_DISP_IP() const{ return BBS_DISP_IP; }

private:
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
	string	BBS_TITLE;
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
	string	BBS_NONAME_NAME;
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
	bool	BBS_PROXY_CHECK;
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
	bool	BBS_NO_ID;
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
	bool	BBS_FORCE_ID;
	/*
		BBS_DELETE_NAME	削除時の名前

		例:
			BBS_DELETE_NAME=あぼーん
		詳細
			削除時に使用される名前です。

		省略時
			あぼーん
	*/
	string	BBS_DELETE_NAME;
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
	int		BBS_SUBJECT_COUNT;
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
	int		BBS_NAME_COUNT;
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
	int		BBS_MAIL_COUNT;
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
	int		BBS_MESSAGE_COUNT;
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
	bool	SUBBBS_CGI_ON;
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
	bool	NANASHI_CHECK;
	/*
		BBS_NOTJP_FILTER	.jp以外のホストからの書き込み禁止

		例:
			BBS_NOTJP_FILTER=checked
			BBS_NOTJP_FILTER=
		詳細
			.jp(JPNICが管理する)IP以外からの書き込みを禁止します。
			checked にすると書き込み禁止として動作します。
			何も書かない場合は規制は発動されません。

		省略時
			なし
	*/
	bool	BBS_NOTJP_FILTER;
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
	string	SAKUBBS;
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
	bool	IS_SAKUBBS;
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
	int		DELETE_WAIT;

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
	bool	BBS_DISP_IP;


/*
	宗教上の違いによりサポートません
	主に色関係はテンプレートを使いためサポートしません。

//../2ch.gif << バナー画像のURL
string	BBS_TITLE_PICTURE;
//#000000
string	BBS_TITLE_COLOR;
//http://www.2ch.net/info.html
//スレッド立てすぎです
int		BBS_THREAD_TATESUGI=64
string	BBS_TITLE_LINK;
string	BBS_BG_PICTURE=../ba.gif
string	BBS_BG_COLOR=#FFFFFF
string	BBS_MAKETHREAD_COLOR=#CCFFCC
string	BBS_MENU_COLOR=#CCFFCC
string	BBS_THREAD_COLOR=#EFEFEF
string	BBS_TEXT_COLOR=#000000
string	BBS_NAME_COLOR=forestgreen
string	BBS_LINK_COLOR=#0000FF
string	BBS_ALINK_COLOR=#FF0000
string	BBS_VLINK_COLOR=#660099
string	BBS_SUBJECT_COLOR=#FF0000
bool	BBS_PASSWORD_CHECK=
bool	BBS_NAMECOOKIE_CHECK;
bool	BBS_MAILCOOKIE_CHECK=checked

  意味がわかんないからサポートしません(w
string	BBS_SLIP=
string	BBS_UNICODE=change
bool	BBS_RAWIP_CHECK;
int		timecheck=1
int		timecount=10
int		timeclose=3
bool	BBS_NEWSUBJECT;
string	BBS_AD2
string	BBS_OVERSEA_THREAD=checked
string	BBS_OVERSEA_PROXY=checked

  	int		BBS_THREAD_NUMBER=20
	int		BBS_CONTENTS_NUMBER=10
	int		BBS_LINE_NUMBER=30
	int		BBS_MAX_MENU_THREAD=100
*/
};

#endif // !defined(AFX_YSETTINGTXT_H__22F20E01_266A_40C8_94AC_D0B644FEFD15__INCLUDED_)
