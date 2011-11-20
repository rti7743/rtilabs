// RFixedColumn.h: RFixedColumn クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFIXEDCOLUMN_H__C8AA28B9_2589_4141_BCF4_266E497ED479__INCLUDED_)
#define AFX_RFIXEDCOLUMN_H__C8AA28B9_2589_4141_BCF4_266E497ED479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RFixedColumn ;

#include "comm.h"


enum __FILECOLUMN_TYPE
{
	__FILECOLUMN_TYPE_MARK,			//マーク
	__FILECOLUMN_TYPE_ICON,			//アイコン
	__FILECOLUMN_TYPE_NAME,			//名前
	__FILECOLUMN_TYPE_VIRTUALPATH,	//仮想パス(圧縮ファイルなどの　圧縮ファイルの先頭を / とするパス)
	__FILECOLUMN_TYPE_EXT,			//タイプ(拡張子に関連付けられている名前)
	__FILECOLUMN_TYPE_SIZE,			//サイズ
	__FILECOLUMN_TYPE_DATE,			//更新日付
	__FILECOLUMN_TYPE_ATTRIB,		//属性
	__FILECOLUMN_TYPE_FREE,			//空き領域
	__FILECOLUMN_TYPE_COMMENT,		//コメント
	__FILECOLUMN_TYPE_ARCMODE,		//圧縮モード
	__FILECOLUMN_TYPE_RATIO,		//圧縮レート(圧縮率)
	__FILECOLUMN_TYPE_END,			//番兵.

};


class RFixedColumn  
{
public:
	RFixedColumn()
	{
		Type = __FILECOLUMN_TYPE_END;
		Name = "";
		WidthParsent = 0;
	}
	RFixedColumn(__FILECOLUMN_TYPE inType,string inName,double inWidthParsent)
	{
		Type = inType;
		Name = inName;
		WidthParsent = inWidthParsent;
	}
	virtual ~RFixedColumn()
	{
	}

	__FILECOLUMN_TYPE getType()	const {	return Type;	}
	string getName()	const {	return Name;	}
	double getWidthParsent()	const {	return WidthParsent;	}

private:
	//種類
	__FILECOLUMN_TYPE	Type ;
	//名前
	string				Name;
	//幅 窓の幅を 1 としたときの縮尺率.
	double				WidthParsent;
};

#endif // !defined(AFX_RFIXEDCOLUMN_H__C8AA28B9_2589_4141_BCF4_266E497ED479__INCLUDED_)
