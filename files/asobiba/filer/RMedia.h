// RMedia.h: RMedia クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIA_H__D918FCBA_5D18_46B9_8DFE_AF9D3F8A0F84__INCLUDED_)
#define AFX_RMEDIA_H__D918FCBA_5D18_46B9_8DFE_AF9D3F8A0F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMediaViewPlayer.h"
#include "RException.h"

class RMedia  
{
public:
	RMedia();
	virtual ~RMedia();

	//読み込み
	bool Load(const string inFilename,HINSTANCE inINstance,HWND isParentWindow);

	//再生する.
	void Play()
	{
		try
		{
			Player->Play();	
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
		}
	}

	//停止する.
	void Stop() 
	{
		try
		{
			Player->Stop();
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
		}
	}

	//一時停止 / 解除
	void Play(bool inON)	
	{
		try
		{
			Player->Stop();
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
		}
	}

	//ボリューム
	int getVolum() const 
	{
		try
		{

			ASSERT(Player->getVolum () >= 0);
			ASSERT(Player->getVolum() <= 255);
			return Player->getVolum();
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
			return 0;
		}
	}

	void setVolum(int inVolum) 
	{
		if(inVolum < 0)	return;
		if(inVolum > 255)	return ;
		try
		{
			Player->setVolum(inVolum);
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
		}
	}

	//再生位置
	int  getPos() const 
	{
		try
		{
			ASSERT(Player->getPos() >= 0);
			ASSERT(Player->getPos() <= 255);
			return Player->getPos();
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
			return 0;
		}
	}

	void setPos(int inPos) 
	{
		if(inPos <= 0) return ;
		if(inPos > 255) return;
		try
		{
			Player->setPos(inPos);
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
		}
	}

	//再生中?
	bool IsPlay() const
	{
		try
		{
			Player->IsPlay();
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
		}
	}

	//一時停止中?
	bool IsPause() const
	{
		try
		{
			Player->IsPause();
		}
		catch(RException e)
		{
			printf( "エラー:\n%s \n" , e.getMessage() );
		}
	}

	//メディアウィンドの取得.
	const RWindow* getWindow()
	{
		return Player->getWindow();
	}

	//タイトルの取得
	string getTitle() const
	{
		return Player->getTitle();
	}

private:
	RMediaViewPlayer*	Player;
};

#endif // !defined(AFX_RMEDIA_H__D918FCBA_5D18_46B9_8DFE_AF9D3F8A0F84__INCLUDED_)
