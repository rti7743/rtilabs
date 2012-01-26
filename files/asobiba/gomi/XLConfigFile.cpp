// XLConfigFile.cpp: XLConfigFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "XLConfigFile.h"
#include "XLStringUtil.h"

USE_TEST(XLConfigFile)


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XLConfigFile::XLConfigFile()
{

}

XLConfigFile::~XLConfigFile()
{

}

/**
 * Load:				設定ファイルを読み込む.
 *
 * @param inFilename	ファイル名
 * @return bool 
 */
bool XLConfigFile::Load(const string & inFilename)
{
	FILE * fp = fopen( inFilename.c_str() ,"rb");
	if (fp == NULL)
	{
		return false;
	}
	char buf[MAX_PATH];

	while( !feof(fp) )
	{
		if ( fgets(buf , MAX_PATH , fp) == NULL ) break;

		if ( buf[0] == '\0' || buf[0] == '#' ) continue;

		const char * eq = strchr( buf , '=');
		if (eq == NULL) continue;

		string key = XLStringUtil::chop(  string(buf , 0 , eq - buf) ) ;
		string value = XLStringUtil::chop( string(eq + 1) ) ;

		Setting[key] = value;
	}
	fclose(fp);

	return true;
}

/**
 * Save:				書き込み
 *
 * @param inFilename	ファイル名
 * @return bool 
 */
bool XLConfigFile::Save(const string & inFilename)
{
	//ダミーを作って書き込み、終了後リネームという簡単なルーチンだが、 wince では
	//不安定になる fopen のせい?
	//よくわからないので、 ifdef (逃げ
#ifndef _WIN32_WCE
	XLStringMap	localSetting = Setting;

	string tmpfilename = inFilename + ".tmp";
	FILE * wfp = fopen( tmpfilename.c_str() ,"wb");
	if (wfp == NULL)
	{
		return false;
	}
	FILE * fp = fopen( inFilename.c_str() ,"rb");
	if (fp != NULL)
	{
			char buf[MAX_PATH];

			while( !feof(fp) )
			{
				if ( fgets(buf , MAX_PATH , fp) == NULL ) break;


				if ( buf[0] == '\0' || buf[0] == '#' )
				{
					fprintf(wfp , "%s" , buf);
					continue;
				}

				const char * eq = strchr( buf , '=');
				if (eq == NULL)
				{
					fprintf(wfp , "%s" , buf);
					continue;
				}

				string key = XLStringUtil::chop( string(buf , 0 , eq - buf) ) ;
				string value = XLStringUtil::chop( string(eq + 1) ) ;

				//このデータは読み込んでいますか?
				XLStringMap::iterator i = localSetting.find(key);
				if (i == localSetting.end() )
				{
					//ない場合は削除
					continue;
				}
				if (value == (*i).second)
				{
					fprintf(wfp , "%s" , buf);
					localSetting.erase(i);
					continue;
				}

				//書き換え.
				fprintf(wfp , "%s=%s\r\n" , (*i).first.c_str()  , (*i).second.c_str() );

				localSetting.erase(i);
			}
	}
	//まだ書き込んで値は追加.
	{
		for(XLStringMap::iterator i = localSetting.begin() ; i != localSetting.end() ; i++)
		{
			fprintf(wfp , "%s=%s\r\n" , (*i).first.c_str()  , (*i).second.c_str() );
		}
		fflush(wfp);
	}
	if (fp) fclose(fp);
	if (wfp) fclose(wfp);

	CopyFile( tmpfilename.c_str() , inFilename.c_str() , FALSE);
	DeleteFile( tmpfilename.c_str() );

#else

	FILE * fp = fopen( inFilename.c_str() ,"wb");
	if (fp != NULL)
	{
		for(XLStringMap::iterator i = Setting.begin() ; i != Setting.end() ; i++)
		{
			fprintf(fp , "%s=%s\r\n" , (*i).first.c_str()  , (*i).second.c_str() );
		}
		fclose(fp);
	}
#endif

	return true;
}


void XLConfigFile::test()
{
	//書き換えテスト
	{
		XLConfigFile	rcf;
		rcf.Load("test/conftest/test2.txt");

		string ret;
		ASSERT( (ret = rcf.Setting["abc"]) == "10");
		ASSERT( (ret = rcf.Setting["xyz"]) == "20");
		ASSERT( (ret = rcf.Setting["sister"]) == "kaho");
		ASSERT( (ret = rcf.Setting["meid"]) == "mahoro");

		rcf.Setting["meid"] = "minawa";
		rcf.Save("test/conftest/test2.txt");

		rcf.Load("test/conftest/test2.txt");
		ASSERT( (ret = rcf.Setting["abc"]) == "10");
		ASSERT( (ret = rcf.Setting["xyz"]) == "20");
		ASSERT( (ret = rcf.Setting["sister"]) == "kaho");
		ASSERT( (ret = rcf.Setting["meid"]) == "minawa");

		rcf.Setting["meid"] = "mahoro";
		rcf.Save("test/conftest/test2.txt");
	}
	//ロードテスト
	{
		XLConfigFile	rcf;
		rcf.Load("test/conftest/test1.txt");

		string ret;
		ASSERT( (ret = rcf.Setting["abc"]) == "10");
		ASSERT( (ret = rcf.Setting["xyz"]) == "20");
		ASSERT( (ret = rcf.Setting["sister"]) == "kaho");
		ASSERT( (ret = rcf.Setting["meid"]) == "mahoro");
	}
}
