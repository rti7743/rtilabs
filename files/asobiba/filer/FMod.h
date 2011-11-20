// FMod.h: FMod クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMOD_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_)
#define AFX_FMOD_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "fmod_define.h"
#include "RAutoLibraryWithException.h"
#include "RDoNotInitaliseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotCloseException.h"
#include "RArchiveException.h"
#include "REnum.h"
#include "RFModException.h"




#include "FModCallable.h"

class FMod  
{
private:
	FMod();		//signleton
public:
	virtual ~FMod();

	//メインシステムの取得 singleton
	static FMod*	getInstance();

	//getInstance したあとで必ず呼び出してください
	//もし、初期化されていなかったら初期化する.
	void New() throw()
	{
		if ( RefCounter == 0) 
		{
			//実態の作成.
			Create();
			//初期化.
			setOutput(FSOUND_OUTPUT_DSOUND);
			setDrivers(1);
			Init(44100, 32, 0);
		}
		RefCounter++;
	}
	//New() をしたクラスが 消去されるときに呼び出してください.
	void Delete()
	{
		RefCounter--;
		if (RefCounter <= 0)
		{
			Close();
			AutoLib.Destory();
		}
	}


	//出力先一覧取得
	RENUM getNumOutputs()
	{
		RENUM p;
		p.push_back( RENUMPAIR("Direct Sound",1) );
		p.push_back( RENUMPAIR("Windows Multimedia Waveout",2) );
		p.push_back( RENUMPAIR("ASIO",3) );
		return p;
	}
	//出力先取得
	int getOutput()
	{
		return FSOUND_GetOutput();
	}
	//出力先決定
	void setOutput(int inIndex) throw()
	{
		if (! FSOUND_SetOutput(inIndex) )
		{
			RaiseException();
		}
	}

	//ドライバ一覧取得
	RENUM getNumDrivers()
	{
		RENUM p;
		for (int i=0; i < FSOUND_GetNumDrivers(); i++) 
		{
			p.push_back( RENUMPAIR( string( (char*)FSOUND_GetDriverName(i)),i+1) );
		}
		return p;
	}
	//ドライバ取得
	int getDrivers()
	{
		return FSOUND_GetDriver();
	}
	//ドライバ決定
	void setDrivers(int inIndex) throw()
	{
		if (! FSOUND_SetDriver(inIndex) )
		{
			RaiseException();
		}
	}

	//音源情報の初期化
	void Init(int mixrate, int maxsoftwarechannels, unsigned int flags) const throw()
	{
		if (!FSOUND_Init(mixrate, maxsoftwarechannels, flags	))
		{
			RaiseException();
		}
	}
	//音源情報のクローズ
	void Close() const
	{
		FSOUND_Close();
	}




	//ファイルを開く
	FSOUND_STREAM* Open(const string inFilename)	throw()
	{
		FSOUND_STREAM* stream = 
			FSOUND_Stream_OpenFile(inFilename.c_str() , FSOUND_NORMAL | FSOUND_LOOP_NORMAL | FSOUND_MPEGACCURATE, 0);
		if (stream == NULL)
		{
			RaiseException();	//エラーなので例外
		}
		return stream;
	}

	//再生する.
	int Play(FSOUND_STREAM* inStream , FModCallable *inCallBack) throw()
	{
		int channel = FSOUND_Stream_PlayEx(FSOUND_FREE, inStream, NULL, TRUE);
		FSOUND_SetPaused(channel, FALSE);

		return channel;
	}
	//停止する.
	void Stop(int inChannel) throw()
	{
		FSOUND_StopSound(inChannel);
	}

	//一時停止 / 解除
	void Pause(int inChannel,bool inON)	throw()
	{
		FSOUND_SetPaused(inChannel, inON );
	}

	//ファイルクローズ
	void Close(FSOUND_STREAM* inStream )	throw()
	{
		FSOUND_Stream_Close(inStream);
	}

	//ボリューム
	int getVolum(int inChannel) const throw()
	{
		return FSOUND_GetVolume(inChannel);
	}
	void setVolum(int inChannel,int inVolum) throw()
	{
		FSOUND_SetVolume(inChannel,	inVolum);
	}

	//再生位置
	unsigned int  getPos(FSOUND_STREAM* inStream) const throw()
	{
		return FSOUND_Stream_GetPosition(inStream);
	}
	void setPos(FSOUND_STREAM* inStream,unsigned int inPos) throw ()
	{
		FSOUND_Stream_SetPosition(inStream,inPos);
	}
	unsigned int  getLength(FSOUND_STREAM* inStream) const throw()
	{
		return FSOUND_Stream_GetLength(inStream);
	}

	//再生中?
	bool IsPlay(int inChannel) const
	{
		return (FSOUND_IsPlaying(inChannel) != 0);
	}
	//一時停止中?
	bool IsPause(int inChannel) const
	{
		return (FSOUND_GetPaused(inChannel) != 0);
	}
private:

	void RaiseException() const throw();

//お礼にたっぷり DLL からインポートするにゃん
	typedef	void (WINAPI *FSOUND_CLOSE)			(VOID);
	FSOUND_CLOSE	FSOUND_Close;

	typedef	signed char (WINAPI *FSOUND_STREAM_CLOSE)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_CLOSE	FSOUND_Stream_Close;

	typedef	int (WINAPI *FSOUND_STREAM_GETLENGTHMS)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETLENGTHMS	FSOUND_Stream_GetLengthMs;

	typedef	int (WINAPI *FSOUND_STREAM_GETTIME)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETTIME	FSOUND_Stream_GetTime;

	typedef	int (WINAPI *FSOUND_STREAM_GETLENGTH)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETLENGTH	FSOUND_Stream_GetLength;

	typedef	unsigned int (WINAPI *FSOUND_STREAM_GETPOSITION)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETPOSITION	FSOUND_Stream_GetPosition;

	typedef	signed char (WINAPI *FSOUND_STREAM_SETTIME)			(FSOUND_STREAM *stream, int ms);
	FSOUND_STREAM_SETTIME	FSOUND_Stream_SetTime;

	typedef	int (WINAPI *FSOUND_SETPAUSED)			(int channel, signed char paused);
	FSOUND_SETPAUSED	FSOUND_SetPaused;

	typedef	signed char (WINAPI *FSOUND_STREAM_PLAYEX)			(int channel, FSOUND_STREAM *stream, FSOUND_DSPUNIT *dsp, signed char startpaused);
	FSOUND_STREAM_PLAYEX	FSOUND_Stream_PlayEx;

	typedef	char* (WINAPI *FSOUND_SAMPLE_GETNAME)			(FSOUND_STREAM *stream);
	FSOUND_SAMPLE_GETNAME	FSOUND_Sample_GetName;

	typedef	signed char (WINAPI *FSOUND_SAMPLE_GETDEFAULTS)			(FSOUND_SAMPLE *sptr, int *deffreq, int *defvol, int *defpan, int *defpri);
	FSOUND_SAMPLE_GETDEFAULTS	FSOUND_Sample_GetDefaults;

	typedef	FSOUND_SAMPLE * (WINAPI *FSOUND_SAMPLE_GETSAMPLE)			(FSOUND_STREAM *stream);
	FSOUND_SAMPLE_GETSAMPLE	FSOUND_Stream_GetSample;

	typedef	signed char (WINAPI *FSOUND_SAMPLE_SETSYNCHCALLBACK)			(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);
	FSOUND_SAMPLE_SETSYNCHCALLBACK	FSOUND_Stream_SetSynchCallback;

	typedef	signed char (WINAPI *FSOUND_SAMPLE_SETENDCALLBACK)			(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);
	FSOUND_SAMPLE_SETENDCALLBACK	FSOUND_Stream_SetEndCallback;

	typedef	int (WINAPI *FSOUND_GETERROR)			(VOID);
	FSOUND_GETERROR	FSOUND_GetError;

	typedef	int (WINAPI *FSOUND_GETDRIVER)			(VOID);
	FSOUND_GETDRIVER FSOUND_GetDriver;

	typedef	signed char (WINAPI *FSOUND_SETDRIVER)			(int driver);
	FSOUND_SETDRIVER FSOUND_SetDriver;

	typedef	signed char * (WINAPI *FSOUND_SETDRIVERNAME)			(int id);
	FSOUND_SETDRIVERNAME FSOUND_GetDriverName;

	typedef int (WINAPI *FSOUND_GETNUMDRIVERS)			(VOID);
	FSOUND_GETNUMDRIVERS FSOUND_GetNumDrivers;

	typedef int (WINAPI *FSOUND_GETOUTPUT)			(VOID);
	FSOUND_GETOUTPUT FSOUND_GetOutput;

	typedef signed char (WINAPI *FSOUND_SETOUTPUT)			(int outputtype);
	FSOUND_SETOUTPUT	FSOUND_SetOutput;

	typedef float (WINAPI *FSOUND_GETVERSION)			(VOID);
	FSOUND_GETVERSION	FSOUND_GetVersion;

	typedef float (WINAPI *FSOUND_INIT)			(int mixrate, int maxsoftwarechannels, unsigned int flags);
	FSOUND_INIT	FSOUND_Init;

	typedef FSOUND_STREAM* (WINAPI *FSOUND_STREAM_OPENFILE)			(const char *filename, unsigned int mode, int memlength);
	FSOUND_STREAM_OPENFILE	FSOUND_Stream_OpenFile;

	typedef signed char (WINAPI *FSOUND_STOPSOUND)			(int channel);
	FSOUND_STOPSOUND	FSOUND_StopSound;

	typedef int (WINAPI *FSOUND_GETPAN)			(int channel);
	FSOUND_GETPAN	FSOUND_GetPan;

	typedef int (WINAPI *FSOUND_GETVOLUME)			(int channel);
	FSOUND_GETVOLUME	FSOUND_GetVolume;

	typedef signed char (WINAPI *FSOUND_SETVOLUME)			(int channel,int vol);
	FSOUND_SETVOLUME	FSOUND_SetVolume;

	typedef signed char (WINAPI *FSOUND_SETPOS)			(int channel,int pan);
	FSOUND_SETPOS		FSOUND_SetPan;

	typedef	signed char (WINAPI *FSOUND_STREAM_SETPOSITION)			(FSOUND_STREAM *stream, unsigned int position);
	FSOUND_STREAM_SETPOSITION	FSOUND_Stream_SetPosition;

	typedef int (WINAPI *FSOUND_ISPLAYING)			(int channel);
	FSOUND_ISPLAYING	FSOUND_IsPlaying;

	typedef signed char (WINAPI *FSOUND_GETPAUSED)			(int channel);
	FSOUND_GETPAUSED	FSOUND_GetPaused;
private:
	//!初期化できなかったら RDoNotInitaliseException
	//!関数が読み込めなかったら RDoNotInitaliseException
	void Create() throw() ;
private:
	RAutoLibraryWithException	AutoLib;
	int					RefCounter;

};

#endif // !defined(AFX_FMOD_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_)
