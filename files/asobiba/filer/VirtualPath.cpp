// VirtualPath.cpp: VirtualPath クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "VirtualPath.h"

#define iskanji1st(c) (((c) >= 0x81 && (c) <= 0x9f) || ((c) >= 0xe0 && (c) <= 0xfc))
#define iskanji2nd(c) ((c) >= 0x40 && (c) <= 0xfc && (c) != 0x7f)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

VirtualPath::VirtualPath(string inPath)
{
	const char* buf = inPath.c_str();
	int count = 0;
	for(int i = 0; buf[i] != '\0' ; i++)
	{
		unsigned char c = (unsigned char) buf[i];
		if (iskanji1st(c))
		{
			c = (unsigned char) buf[i+1];
			if (iskanji2nd(c))
			{
				// 2 byte skip
				i++;
			}
		}
		else
		{
			//仮想パスの区切り :: をサーチ.
			if (c == ':' && buf[i + 1] == ':' )
			{
				if ( count >= (MAX_NEXT_VIRTUAL_PATH-1) )			break;

				PathDilimiteres[count++] = i;

				// ::は 2バイトなので、1バイトスキップします。
				i++;
			}
		}
	}
	PathDilimiteres[count] = inPath.length();
	DilimiteresMax = count ;

	Path = inPath;

}

VirtualPath::~VirtualPath()
{

}

//テスト
void VirtualPath::test()
{
	VirtualPath		vp("c:\\test3\\a.lzh::はじめてのおるすばん/しおりたん（；´Д`）ﾊｧﾊｧ.zip::すくーるみじゅぎぷれい.png");
	ASSERT( vp.getPathDilimiterCount() == 2 );
	ASSERT( vp.getNativePath() == "c:\\test3\\a.lzh" );
	ASSERT( vp.getLastPath() == "すくーるみじゅぎぷれい.png" );
	ASSERT( vp.getAdjacentPath() == "はじめてのおるすばん/しおりたん（；´Д`）ﾊｧﾊｧ.zip" );
	ASSERT( vp.getSubPath(0) == "c:\\test3\\a.lzh" );
	ASSERT( vp.getSubPath(1) == "はじめてのおるすばん/しおりたん（；´Д`）ﾊｧﾊｧ.zip" );
	ASSERT( vp.getSubPath(2) == "すくーるみじゅぎぷれい.png" );

}


