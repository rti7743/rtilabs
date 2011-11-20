// RHtmlTemplate.h: RHtmlTemplate クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTMLTEMPLATE_H__D00CDA57_C017_4322_AF00_78222B5F7B19__INCLUDED_)
#define AFX_RHTMLTEMPLATE_H__D00CDA57_C017_4322_AF00_78222B5F7B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"
#include "RIOCannotOpenException.h"


/**
 * class RHtmlTemplateContent:		.tpl をパースした結果　インチキオートマトンの単位
 *
 */
class RHtmlTemplateContent
{
public:
	enum TemplateFlg
	{
		StaticContent,
		DynamicContent,
	};
	//本当は 継承でやりたいんだけど、 virtual の処理はインラインが効かないので、フラグを使います。
	RHtmlTemplateContent(TemplateFlg inFlg , const string inUseData)
	{
		this->Flg = inFlg;
		this->UseData = inUseData;
	}
	//データの取得.
	inline const string getConect(const RStringMap * inReplaceStringMap) const
	{
		if (this->Flg == StaticContent)
		{	//そのままのデータ.
			return this->UseData;
		}
		else
		{	//置換して返す.
			RStringMap::const_iterator i = inReplaceStringMap->find(this->UseData);
			if (i == inReplaceStringMap->end() ) return "";

			return (*i).second;
		}
	}
	//データの長さの取得.
	inline int getConectLength(const RStringMap * inReplaceStringMap) const
	{
		if (this->Flg == StaticContent)
		{	//そのままのデータ.
			return this->UseData.size();
		}
		else
		{	//置換して返す.
			RStringMap::const_iterator i = inReplaceStringMap->find(this->UseData);
			if (i == inReplaceStringMap->end() ) return 0;

			return (*i).second.size();
		}
	}

	//デバッグ用の Dump.
	string Dump() const
	{
		return this->UseData;
	}
private:
	TemplateFlg		Flg;
	string			UseData;
};

/**
 * class RHtmlTemplate:		デザイン
 *
 */
class RHtmlTemplate  
{
public:
	RHtmlTemplate();
	virtual ~RHtmlTemplate();

	//.tpl を読み込む
	void Load(const string inFilename) throw(RIOCannotOpenException,RParseException);
	//子供の場合.
	void Load(const RHtmlTemplate* inPerent ,const string inContect, int inStartLine) throw(RParseException);

	//ループの取得
	RHtmlTemplate* Select(const string inPartName) const
	{
		RHtmlTemplateMap::const_iterator i = Loop.find(inPartName);
		if (i == Loop.end() ) return NULL;
		return (*i).second;
	}
	/**
	 * Replace:						置換を行う
	 *
	 * @param inReplaceStringMap	置換する文字列のマップ
	 * @return string				結果 
	 */
	const string Replace(const RStringMap * inReplaceStringMap) const;

	void Dump() const;
	static void test();

private:
	/**
	 * Parse:						内容を解析し、静的、ループ、置換 の項目別にリスト構造に変換します。
	 *								パースは汚くなりますね(w
	 *
	 * @param inTargetBuffer		解析される値 最後に \0 が入っていることは保証される
	 * @param inSize				解析される値のサイズ
	 * @param inStartLine			.tpl ファイルでのinTargetBufferがある行数 エラー行を出力するときに使う
	 * @return void					
	 * @exception	RParseException	構文解析エラー
	 */
	void Parse(const char* inTargetBuffer , int inSize, int inStartLine) throw(RParseException);
	/**
	 * ParseValue:					内容を解析し、静的、置換、の項目別にリスト構造に変換します。
	 *								パースは汚くなりますね(w
	 *
	 * @param inTargetBuffer		解析される値 最後に \0 が入っていることは保証される
	 * @param inSize				解析される値のサイズ
	 * @param inStartLine			.tpl ファイルでのinTargetBufferがある行数 エラー行を出力するときに使う
	 * @return void					
	 * @exception	RParseException	構文解析エラー
	 */
	void ParseValue(const char* inTargetBuffer,int inSize , int inStartLine) throw(RParseException);

	/**
	 * getline:				行数を数える
	 *
	 * @param inStart		ここからはじまって
	 * @param inTarget		ここまで
	 * @return int			行数
	 */
	int RHtmlTemplate::getline(const char * inStart , const char* inEnd) const;

	/**
	 * CleanUp:				new で確保したものの解放
	 *
	 * @return void 
	 */
	void CleanUp();
private:
	typedef map<string,RHtmlTemplate*>	RHtmlTemplateMap;
	typedef deque<RHtmlTemplateContent*>	RHtmlTemplateValueMap;

	//自分が保持するループ.
	RHtmlTemplateMap		Loop;
	//自分が保持する置換場所の index. 高速化を図る.
	RHtmlTemplateValueMap	ReplaceIndex;

	const RHtmlTemplate* Perent;
};

#endif // !defined(AFX_RHTMLTEMPLATE_H__D00CDA57_C017_4322_AF00_78222B5F7B19__INCLUDED_)
