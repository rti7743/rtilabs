// RMedia.cpp: RMedia クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMedia.h"

#include "RFModView.h"
#include "RMediaPlayer.h"
#include "RRealPlayer.h"
#include "RFlashPlayer.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMedia::RMedia()
{
	Player = NULL;
}

RMedia::~RMedia()
{
	delete Player;
}

//読み込み
bool RMedia::Load(const string inFilename,HINSTANCE inINstance,HWND isParentWindow)
{
	//すでに使用中の場合は破棄する.
	if (Player)
	{
		Player->Stop();
		Player->Close();
		delete Player;
	}

	//RealPlayer でいけるのですかー
	if ( RRealPlayer::IsSupport(inFilename) )
	{
		try
		{
			Player = new RRealPlayer;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "RRealPlayer エラー:\n%s \n" , e.getMessage() );
		}
	}

	//MS-MediaPlayer でいけるのですかー
	if ( RMediaPlayer::IsSupport(inFilename) )
	{
		try
		{
			Player = new RMediaPlayer;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "RMediaPlayer エラー:\n%s \n" , e.getMessage() );
		}
	}
	//MicroMedia Flashでいけるのですかー
	if ( RFlashPlayer::IsSupport(inFilename) )
	{
		try
		{
			Player = new RFlashPlayer;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "RFlashPlayer エラー:\n%s \n" , e.getMessage() );
		}
	}
	

	//fmod.dllでいけるのですかー
	if ( RFModView::IsSupport(inFilename) )
	{
		try
		{
			Player = new RFModView;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "fmod.dll エラー:\n%s \n" , e.getMessage() );
		}
	}
	return false;
}

