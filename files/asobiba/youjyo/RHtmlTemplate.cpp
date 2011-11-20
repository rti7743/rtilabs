// RHtmlTemplate.cpp: RHtmlTemplate クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RHtmlTemplate.h"
#include <sys/stat.h>

#define RTEMPLATE_LOOP_START	"<!--?LS:"
#define RTEMPLATE_TAG_END		"-->"
#define RTEMPLATE_LOOP_END		"<!--?LE:"

#define RTEMPLATE_VALUE_START	"<!--?V:"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RHtmlTemplate::RHtmlTemplate()
{
	this->Perent = NULL;
}

RHtmlTemplate::~RHtmlTemplate()
{
	CleanUp();
}


void RHtmlTemplate::Load(const string inFilename) throw(RIOCannotOpenException,RParseException)
{
	CleanUp();

	struct _stat statbuf;
	if ( _stat( inFilename.c_str() ,  &statbuf) == -1)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "stat の取得に失敗" + inFilename);
	}
	int size = statbuf.st_size;
	char * buf =  new char[size + 1];
	
	FILE * fp = fopen( inFilename.c_str() ,"rb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "DATファイルが開けませんでした" + inFilename);
	}
	size = fread( buf ,sizeof(char), size ,fp);
	fclose(fp);

	buf[size] = '\0';

	this->Parse( buf ,size , 0);
	delete [] buf;
}

//子供の場合.
void RHtmlTemplate::Load(const RHtmlTemplate* inPerent ,const string inContect, int inStartLine) throw(RParseException)
{
	CleanUp();

	this->Perent = inPerent;

	this->Parse( inContect.c_str() ,inContect.size() , inStartLine);
}


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
void RHtmlTemplate::Parse(const char* inTargetBuffer , int inSize, int inStartLine) throw(RParseException)
{
	const char * p = inTargetBuffer;
	const char * territory = inTargetBuffer + inSize;	
	const char* oldpos = p;
	while(1)
	{
		oldpos = p;

		//ループタグの検索
		//例 <!--?LS:Kahotan-->
		const char* loopstart = strstr(p , RTEMPLATE_LOOP_START);
		if (loopstart == NULL) break;
		p=loopstart + sizeof(RTEMPLATE_LOOP_START) - 1;

		const char* looptagend = strstr(p , RTEMPLATE_TAG_END);
		if (looptagend == NULL)
		{
			throw RParseException("%d行目:ループ開始タグが閉じていません" , getline(inTargetBuffer , loopstart) + inStartLine);
		}
		// p ～ looptagend までがタグの名前.
		string tagname = string(p , 0 , looptagend - p);

		p=looptagend + sizeof(RTEMPLATE_LOOP_START) - 1;

		//例 <!--?LE:Kahotan-->
		//このタグをサーチしる.
		const string LoopEndTagName = string(RTEMPLATE_LOOP_END) + tagname + RTEMPLATE_TAG_END;
		//ループタグの終わりを検索.
		const char* loopend = strstr(p , LoopEndTagName.c_str());
		if (loopend == NULL) 
		{
			throw RParseException("%d行目:ループ開始はあるのに終了がありません" , getline(inTargetBuffer , loopstart) + inStartLine);
		}

		//登録したいのですが、実はそのタグが激しくガイシュツではないか調べる.
		if ( this->Select(tagname) != NULL)
		{
			throw RParseException("%d行目～%d行目:ループ開始,終了のタグ(%s)は激しくガイシュツです" , 
				getline(inTargetBuffer , loopstart) + inStartLine, 
				getline(inTargetBuffer , loopend) + inStartLine, 
				tagname.c_str() );
		}

		//ループの前にある置換タグをすべて取得します。
		this->ParseValue(oldpos , loopstart - oldpos , getline(inTargetBuffer , loopstart) );
		//このループを置換タグとして登録します。
		ReplaceIndex.push_back( new RHtmlTemplateContent(RHtmlTemplateContent::DynamicContent , tagname) );
		//ループ自体を登録します。
		const char* tagContentStart = looptagend + sizeof(RTEMPLATE_TAG_END)- 1;
		RHtmlTemplate * childTemplate = new RHtmlTemplate();
		Loop[tagname] =  childTemplate;
		childTemplate->Load(this , string(tagContentStart , 0 , loopend - tagContentStart) , getline(inTargetBuffer , loopstart) );

		p = loopend + LoopEndTagName.size();
	}
	//残りの領域の調査.
	this->ParseValue(oldpos , territory - oldpos , getline(inTargetBuffer , oldpos) );
}

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
void RHtmlTemplate::ParseValue(const char* inTargetBuffer,int inSize , int inStartLine) throw(RParseException)
{
	const char * p = inTargetBuffer;
	const char * territory = inTargetBuffer + inSize;	
	const char* oldpos = p;
	while(1)
	{
		oldpos = p;

		//置換タグの検索
		//例 <!--?V:Kahotan-->
		const char* valuestart = strstr(p , RTEMPLATE_VALUE_START);
		if (valuestart == NULL) break;			//見つからない
		if (valuestart >= territory) break;		//担当範囲外

		p=valuestart + sizeof(RTEMPLATE_VALUE_START)- 1;

		//置換タグの終了をサーチ
		const char* valueend = strstr(p , RTEMPLATE_TAG_END);
		if (valueend == NULL)
		{
			throw RParseException("%d行目:置換タグが閉じていません" , getline(inTargetBuffer , valuestart) + inStartLine );
		}
		if (valueend >= territory)
		{
			throw RParseException("%d行目:置換タグがおかしいことになっています" , getline(inTargetBuffer , valuestart) + inStartLine );
		}
		//oldpos ～ valuestart までが静的コンテンツです。
		if (valuestart-oldpos > 0)
		{
			ReplaceIndex.push_back( 
				new RHtmlTemplateContent(RHtmlTemplateContent::StaticContent , string(oldpos , 0 , valuestart-oldpos) ) );
		}
		//p ～ valueend までがタグの名前です。
		string tagname = string(p , 0 , valueend - p);
		//そして、それが動的コンテンツです。
		ReplaceIndex.push_back( new RHtmlTemplateContent(RHtmlTemplateContent::DynamicContent , tagname) );

		p=valueend + sizeof(RTEMPLATE_TAG_END)- 1;
	}
	//残りはすべて静的コンテンツ.
	if (territory-oldpos > 0)
	{
		ReplaceIndex.push_back( 
			new RHtmlTemplateContent(RHtmlTemplateContent::StaticContent , string(oldpos , 0 , territory-oldpos) ) );
	}
}


/**
 * getline:				行数を数える
 *
 * @param inStart		ここからはじまって
 * @param inTarget		ここまで
 * @return int			行数
 */
int RHtmlTemplate::getline(const char * inStart , const char* inEnd) const
{
	int count = 0;
	const char* p = inStart;
	while( p < inEnd )
	{
		if (*p == '\n') count++;
		p++;
	}
	return count;
}

/**
 * CleanUp:				new で確保したものの解放
 *
 * @return void 
 */
void RHtmlTemplate::CleanUp()
{
	{
		for(RHtmlTemplateMap::iterator i = Loop.begin() ; i != Loop.end() ; ++i)
		{
			delete (*i).second;
		}
		Loop.clear();
	}
	{
		for(RHtmlTemplateValueMap::iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			delete (*i);
		}
		ReplaceIndex.clear();
	}
}

/**
 * Replace:						置換を行う
 *
 * @param inReplaceStringMap	置換する文字列のマップ
 * @return string				結果 
 */
const string RHtmlTemplate::Replace(const RStringMap * inReplaceStringMap) const
{
	int length = 0;
	{
		for(RHtmlTemplateValueMap::const_iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			const RHtmlTemplateContent* htc = (*i);
			length += htc->getConectLength(inReplaceStringMap);
		}
	}

	string ret ;
	ret.reserve(length);
	{
		for(RHtmlTemplateValueMap::const_iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			const RHtmlTemplateContent* htc = (*i);
			ret += htc->getConect(inReplaceStringMap);
		}
	}
	return ret;
}

void RHtmlTemplate::Dump() const
{
	{
		printf("===================== RHtmlTemplate::Dump() ===========================\r\n");
		for(RHtmlTemplateValueMap::const_iterator i = ReplaceIndex.begin() ; i != ReplaceIndex.end() ; ++i)
		{
			printf("\r\n===================== part. ===========================\r\n");
			const RHtmlTemplateContent* htc = (*i);
			puts( htc->Dump().c_str() );
		}
		printf("===================== --------------------- ===========================\r\n");
	}
}

void RHtmlTemplate::test()
{
	try
	{
		string ret;

		RHtmlTemplate	t;
		t.Load("test4.tpl");

		RHtmlTemplate* threadindex = t.Select("YThreads");
		ASSERT (threadindex != NULL);

		const RHtmlTemplateContent* htc;

		RHtmlTemplateValueMap::const_iterator i = threadindex->ReplaceIndex.begin();
		htc = (*i);
		ASSERT((ret = htc->Dump()) == 
"\r\n"
"<TABLE cellSpacing=7 cellPadding=3 width=\"95%\" align=center bgColor=#efefef \r\n"
"border=1>\r\n"
"  <TBODY>\r\n"
"  <TR>\r\n"
"    <TD> \r\n"
"      <DL><A name=1></A> \r\n"
"        <DIV align=right><A href=\"http://pc.2ch.net/linux/#menu\">■</A><A \r\n"
"        href=\"http://pc.2ch.net/linux/#16\">▲</A><A \r\n"
"        href=\"http://pc.2ch.net/linux/#2\">▼</A></DIV>\r\n"
"        <B>【1:"
		);

	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}
	try
	{
		RHtmlTemplate	t;
		t.Load("test3.tpl");
		ASSERT(t.Loop.size() == 1);
		ASSERT(t.ReplaceIndex.size() == 1);

		RHtmlTemplate* threadindex = t.Select("YThreadIndex");
		ASSERT (threadindex != NULL);

		ASSERT(threadindex->Loop.size() == 0);
		ASSERT(threadindex->ReplaceIndex.size() == 7);

		RStringMap smap;
		smap["YThreadL50"] = "http://www.2ch.net/";
		smap["YThreadIndexNo"] = "ももーい";
		smap["YThreadIndexSubjectAndNo"] = "もっちー(；´Д｀)ﾊｧﾊｧ (1000)";
		string ret = threadindex->Replace(&smap);

		ASSERT(ret == "\r\n<A href=\"http://www.2ch.net/\" target=body>ももーい:もっちー(；´Д｀)ﾊｧﾊｧ (1000)</A>\r\n");
		smap["YThreadIndex"] = ret;

		ret = t.Replace(&smap);
		ASSERT(ret == "\r\n<A href=\"http://www.2ch.net/\" target=body>ももーい:もっちー(；´Д｀)ﾊｧﾊｧ (1000)</A>\r\n");
	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}

	try
	{
		RHtmlTemplate	t;
		t.Load("test2.tpl");
		ASSERT(t.Loop.size() == 0);
		ASSERT(t.ReplaceIndex.size() == 6);

		RStringMap smap;
		smap["YThreadIndexNo"] = "ももーい";
		string ret = t.Replace(&smap);

		ASSERT(ret == 
			"this is template test.\r\n"
			"ももーい\r\n"
			"this is template test2.\r\n"
			"aaaaaaももーい!!!\r\n"
			"ももーい");
	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}

	try
	{
		RHtmlTemplate	t;
		t.Load("test1.tpl");
		ASSERT(t.Loop.size() == 0);
		ASSERT(t.ReplaceIndex.size() == 1);
	}
	catch(RException e)
	{
		printf( e.getMessage() );
	}
}
