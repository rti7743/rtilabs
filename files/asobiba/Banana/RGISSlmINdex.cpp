// RGISSlmINdex.cpp: RGISSlmINdex クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSlmINdex.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSlmINdex::RGISSlmINdex()
{
}

RGISSlmINdex::~RGISSlmINdex()
{
	Destroy();
}

//すべての slm ファイルを読み込む.
void RGISSlmINdex::Create(const string & inPath,const RStringList & inTargetList , int inScale) 
{

	Destroy();

	WIN32_FIND_DATA data;
	const string path = inPath + "\\*.*";
	HANDLE handle = ::FindFirstFile(path.c_str() , &data );
	if (handle == INVALID_HANDLE_VALUE )
	{
		return ;
	}
	do
	{
		if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  && 
			!(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) ) )
		{
				//作業対称に含まれていますか?
				if ( find(inTargetList.begin() , inTargetList.end() , data.cFileName ) == inTargetList.end() ) continue;

				string read = inPath + "\\" + data.cFileName + "\\" + data.cFileName + ".slm";

				RGISSlm* slm = new RGISSlm;
				try
				{
					slm->Create(read,inScale);
				}
				catch(RException e)
				{
					delete slm;
					TRACE(e.getMessage() );
					//無視.
					continue ;
				}
				Points.push_back(slm);
		}
	}
	while( ::FindNextFile(handle , &data) );

	::FindClose(handle);
	return ;
}

void RGISSlmINdex::Destroy()
{
	RGISSlms::iterator i = Points.begin();
	RGISSlms::iterator e = Points.end();

	for( ; i != e ; i++)
	{
		delete *i;
	}
	Points.clear();
}

