// FModPlay.h: FModPlay クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMODPLAY_H__42FA9301_CBCD_40DE_ACE7_5BD7BD397F39__INCLUDED_)
#define AFX_FMODPLAY_H__42FA9301_CBCD_40DE_ACE7_5BD7BD397F39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "fmod.h"

class FModPlay  
{
public:
	FModPlay()
	{
		Stream	=	NULL;
		Sptr	=	NULL;
		Channel	=	-1;

		FModCore = FMod::getInstance();
		FModCore->New();
	}
	virtual ~FModPlay()
	{
		if ( Stream != NULL )
		{
			Close();
		}

		FModCore->Delete();
	}


	//ファイルを開く
	void Open(const string inFilename )	throw()
	{
		if (Stream != NULL)
		{
			Close();
		}

		Stream = FModCore->Open(inFilename );
	}
	//再生する.
	void Play(FModCallable *inCallBack) throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"再生するファイルが開かれていません");
		}
		if (Channel >= 0)
		{
			throw RFModException(0,"チャネル %d は、すでに再生を行っています",Channel);
		}

		Channel = FModCore->Play(Stream , inCallBack);
	}

	//停止する.
	void Stop() throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"チャネル%d は再生をおこなっていません",Channel);
		}

		FModCore->Stop(Channel);
		Channel = -1;
	}

	//一時停止 / 解除
	void Pause(bool inON)	throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"チャネル%d は再生をおこなっていません",Channel);
		}

		FModCore->Pause(Channel , inON);
	}

	//ファイルクローズ
	void Close()	throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"このストリームでは再生を行っていません");
		}

		FModCore->Close(Stream);
	}

	//ボリューム
	int getVolum() const throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"チャネル%d は再生をおこなっていません",Channel);
		}

		return FModCore->getVolum(Channel);
	}
	void setVolum(int inVolum) throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"チャネル%d は再生をおこなっていません",Channel);
		}

		FModCore->setVolum(Channel,inVolum);
	}

	//再生位置
	int  getPos() const throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"このストリームでは再生を行っていません");
		}

		return FModCore->getPos(Stream);
	}
	void setPos(int inPos) throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"このストリームでは再生を行っていません");
		}

		FModCore->setPos(Stream,inPos);
	}
	int  getLength() const throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"このストリームでは再生を行っていません");
		}

		return FModCore->getLength(Stream);
	}

	//再生中?
	bool IsPlay() const
	{
		if (Channel < 0)
		{
			throw RFModException(0,"チャネル%d は再生をおこなっていません",Channel);
		}

		return FModCore->IsPlay(Channel);
	}
	//一時停止中?
	bool IsPause() const
	{
		if (Channel < 0)
		{
			throw RFModException(0,"チャネル%d は再生をおこなっていません",Channel);
		}

		return FModCore->IsPause(Channel);
	}

private:
	FMod * FModCore;

	FSOUND_STREAM	*Stream;
	FSOUND_SAMPLE	*Sptr;
	int				Channel;
};

#endif // !defined(AFX_FMODPLAY_H__42FA9301_CBCD_40DE_ACE7_5BD7BD397F39__INCLUDED_)
