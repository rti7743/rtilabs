// RRealPlayer.cpp: RRealPlayer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RRealPlayer.h"
#include "Rfiles.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RRealPlayer::RRealPlayer()
{

}

RRealPlayer::~RRealPlayer()
{

}

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RRealPlayer::Create(HINSTANCE inINstance,HWND isParentWindow) throw()
{
	RActiveXWindow::Create(inINstance, "rmocx.RealPlayer G2 Control.1" ,RealAudioObjects::IID_IRealAudio,(void**) &RealAudio,isParentWindow,0,0 ,WS_EX_STATICEDGE | WS_EX_ACCEPTFILES);

	puts("Control is K");
}

//ファイルを開く
void RRealPlayer::Open(const string inFilename )	throw() 
{
	USES_CONVERSION;
	CComVariant varg[4];
	try
	{
		BSTR bstr;
		bstr = T2OLE( inFilename.c_str() );
		RealAudio->SetSource( bstr );

		//これをやらないと文字通り Audioだけしかでない
		//参考... http://www.jp.realnetworks.com/resources/sdk/vbplay.html
		bstr = T2OLE("one");
		RealAudio->put_Console(bstr);
		bstr = T2OLE("ImageWindow");
		RealAudio->put_Controls(bstr);

		RealAudio->SetOriginalSize();
	}
	catch(RException e)
	{
		printf("例外:\n %s " , e.getMessage() );
	}
}

//再生する.
void RRealPlayer::Play() throw() 
{
	RealAudio->DoPlay();
}


//停止する.
void RRealPlayer::Stop() throw() 
{
	RealAudio->DoStop();
}


//一時停止 / 解除
void RRealPlayer::Pause(bool inON)	throw() 
{
	RealAudio->DoPause();
}

//ファイルクローズ
void RRealPlayer::Close()	throw() 
{
}


//ボリューム
int RRealPlayer::getVolum() const throw() 
{
	short vol;
	RealAudio->GetVolume(&vol);
	return CHANGE_SCALE(vol ,100 , 255 );
}

void RRealPlayer::setVolum(int inVolum) throw() 
{
	RealAudio->SetVolume( REV_CHANGE_SCALE(inVolum ,100 , 255 ) );
}

//再生位置
int  RRealPlayer::getPos() const throw() 
{
	long pos,len;
	RealAudio->GetPosition(&pos);
	RealAudio->GetLength(&len);

	if (len == 0) return 0;

	return CHANGE_SCALE(pos , len , 255);
}


void RRealPlayer::setPos(int inPos) throw() 
{
	long pos,len;
	RealAudio->GetLength(&len);
	
	pos = REV_CHANGE_SCALE(inPos , len , 255);

	RealAudio->SetPosition(pos);
}


//再生中?
bool RRealPlayer::IsPlay() const 
{
	return true;
}


//一時停止中?
bool RRealPlayer::IsPause() const 
{
	return true;
}




//サポートしてます?
bool RRealPlayer::IsSupport(const string inFilename)
{
	//とりあえず、拡張子判定.
	string ext = Rfiles::Filename2Ext(inFilename);
	return  (ext == "RA" || ext == "RM" );
}

//タイトルを取得する
string RRealPlayer::getTitle() const 
{
	BSTR title;
	RealAudio->GetTitle(&title);

	USES_CONVERSION;

	return OLE2CT(title);
}
