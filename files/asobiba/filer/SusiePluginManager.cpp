// SusiePluginManager.cpp: SusiePluginManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "SusiePluginManager.h"
#include "Rfiles.h"
#include "DeleteObject.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SusiePluginManager::SusiePluginManager()
{
	PluginLoad("C:\\Program Files\\susie\\");
}

SusiePluginManager::~SusiePluginManager()
{
//	for_each( this->Plugines.begin() , this->Plugines.end() , DeleteObject )
	//確保したプラグインの消滅.
	for(PLUGINES::iterator p = this->Plugines.begin(); p != this->Plugines.end() ; ++p)
	{
		delete *p;
	}
}

//インスタンス取得
SusiePluginManager*	SusiePluginManager::getInstance()
{
	static SusiePluginManager spm;
	return  &spm;
}

//拡張子一覧の取得
SUSIE_EXT SusiePluginManager::getSupportExt()
{
	SUSIE_EXT ret;
	for(EXTMAPPER::iterator p = this->ExtMapper.begin(); p != this->ExtMapper.end() ; ++p)
	{
		ret.push_back((*p).first);
	}
	//重複を取り除く.
	ret.erase( unique(ret.begin() , ret.end()) , ret.end() );

	return ret;
}

//プラグインロード
string SusiePluginManager::PluginLoad(const string inPluginPath)
{
	string wildpath = inPluginPath + "*.spi";
	WIN32_FIND_DATA data;
	HANDLE p = ::FindFirstFile( wildpath.c_str() , &data);
	if (p != INVALID_HANDLE_VALUE)
	{
		do
		{
			//ディレクトリは無視
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			SusiePluginWapper* sp = NULL;
			try
			{
				sp = new SusiePluginWapper;
				sp->Create( inPluginPath + data.cFileName);
				AppendSupportBy(sp);
				this->Plugines.push_back(sp);
			}
			catch(RException e)
			{
				delete sp;
			}
		}
		while( ::FindNextFile( p , &data) );
		::FindClose(p);
	}

	return "";
}

//サポートしているプラグインきぼんぬ
//みつからない場合は RNotSupportException 
const SusiePluginWapper* SusiePluginManager::getSupportPlugin(const string inFilename,const string inExt) throw()
{
	pair<MULTIMAP_RANGE_ITERATOR,MULTIMAP_RANGE_ITERATOR> p = this->ExtMapper.equal_range(inExt);

	//2Kバイト読み込みます。
	const int THE2KBYTESIZE = 1024 * 2;
	char the2KByte[THE2KBYTESIZE] = {0};
	FILE *fp = fopen(inFilename.c_str() , "rb");
	if (fp == NULL) throw RNotSupportException("%s ファイルがオープンできません" , inFilename.c_str() );
	fread(the2KByte , sizeof(char) , THE2KBYTESIZE , fp);
	fclose(fp);

	for ( MULTIMAP_RANGE_ITERATOR i = p.first ; i != p.second ; ++i)
	{
		SusiePluginWapper* plugin = (*i).second;
		
		//サポートしてますか?
		if ( plugin->IsSupported( inFilename , the2KByte) )
		{
			return plugin;
		}
	}
	throw RNotSupportException("%s 形式は未サポートです" , inFilename.c_str() );
	return NULL;
}

//対応するプラグインをマッピング.
//エラー RSusieConferException
void SusiePluginManager::AppendSupportBy(SusiePluginWapper* inSusiePlugin) throw()
{
	int no = 0;
	string extes;
	string ext;
	do
	{
		//サポートしている形式の取得
		extes = inSusiePlugin->GetPluginSupportExt(no++);
		//ん? 空ぢゃん.
		if ( extes.empty() ) continue;
		//最後に番兵を入れる.
		extes += ';';
		// ; 区切りなので..
		int findstart = 0;
		int hitpos = 0;
		while(1)
		{
			hitpos = extes.find(';' , findstart);
			if (hitpos == -1) break;
			ext = extes.substr(findstart , hitpos - findstart);
			ext = Rfiles::Filename2Ext(ext);
			this->ExtMapper.insert(pair< string, SusiePluginWapper* >( ext, inSusiePlugin ));

			TRACE("Susie Mapping %s \n",ext.c_str());

			findstart = hitpos + 1;
		}
	}
	while(extes != "");
}
