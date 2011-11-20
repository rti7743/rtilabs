// RCommKeyDown.h: RCommKeyDown クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCOMMKEYDOWN_H__A6234C14_4867_4D3F_BE2D_AAD26489C94A__INCLUDED_)
#define AFX_RCOMMKEYDOWN_H__A6234C14_4867_4D3F_BE2D_AAD26489C94A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "StringList.h"
#include "RFunctionScript.h"

//高速化のためにちょっと汚い手段をとります。
//KEY_DATA の上位 8バイトは仮想キーコード、 下位 8バイトはSHIFTキーなどと同時押しの場合に ONになるビットです。
typedef unsigned short	KEY_DATA;
#define MAKE_KEY_DATA(key,flg)		((((key)&0xff) << 8) | ((flg)&0xff))

enum	MODIFY_KEY
{
	MODIFY_KEY_NONE		=	0,
	MODIFY_KEY_SHIFT	=	1,
	MODIFY_KEY_CTRL		=	2,
	MODIFY_KEY_ALT		=	4,
	MODIFY_KEY_DBCLICK	=	0xff,
};

class RCommKeyDown  
{
public:
	RCommKeyDown();
	virtual ~RCommKeyDown();

	bool OnKeyDown(unsigned char inKeyCode ,MODIFY_KEY inModifyKey ) const throw (RParseException)
	{
		StringMap::const_iterator it = Keys.find( MAKE_KEY_DATA(inKeyCode,inModifyKey) );
		if (it == Keys.end())	return false;	//そんなファンクションないです。

		RFunctionScript	rfs;
		rfs.Interpreter( (*it).second );


		return true;
	}
	void SetKey(unsigned char inKeyCode ,MODIFY_KEY inModifyKey , string Script)
	{
		Keys[ MAKE_KEY_DATA(inKeyCode,inModifyKey)  ] = Script;
	}



private:
	//キーを押したときに発動するコマンドたち.
	StringMap	Keys;
};

#endif // !defined(AFX_RCOMMKEYDOWN_H__A6234C14_4867_4D3F_BE2D_AAD26489C94A__INCLUDED_)
