// RMediaPlayer.cpp: RMediaPlayer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMediaPlayer.h"
#include "Rfiles.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMediaPlayer::RMediaPlayer()
{

}

RMediaPlayer::~RMediaPlayer()
{

}

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RMediaPlayer::Create(HINSTANCE inINstance,HWND isParentWindow) throw()
{
	RActiveXWindow::Create(inINstance, "MediaPlayer.MediaPlayer.1" ,MediaPlayer::IID_IMediaPlayer2,(void**) &MediaPlayer,isParentWindow,0,0 ,WS_EX_STATICEDGE | WS_EX_ACCEPTFILES);

	puts("Control is OK");
}

//ファイルを開く
void RMediaPlayer::Open(const string inFilename )	throw() 
{
	USES_CONVERSION;
	MediaPlayer->put_AllowScan(VARIANT_TRUE);

	MediaPlayer->put_ClickToPlay(VARIANT_FALSE);
	MediaPlayer->put_ShowTracker(VARIANT_FALSE);
	MediaPlayer->put_ShowPositionControls(VARIANT_FALSE);
	MediaPlayer->put_ShowControls(VARIANT_FALSE);

	BSTR filename = T2OLE( inFilename.c_str() );
	MediaPlayer->put_FileName( filename );
}

//再生する.
void RMediaPlayer::Play() throw() 
{
	MediaPlayer->Play();
}


//停止する.
void RMediaPlayer::Stop() throw() 
{
	MediaPlayer->Stop();
}


//一時停止 / 解除
void RMediaPlayer::Pause(bool inON)	throw() 
{
	MediaPlayer->Pause(); 
}

//ファイルクローズ
void RMediaPlayer::Close()	throw() 
{
}


//ボリューム
int RMediaPlayer::getVolum() const throw() 
{
	long ret;
	MediaPlayer->get_Volume(&ret);
	ret = 255 - CHANGE_SCALE( abs(ret) , 2000 , 255) ;
	printf("getvolumn ret is %d\n",ret);
	return ret;
}

void RMediaPlayer::setVolum(int inVolum) throw() 
{
	long ret;
	ret = REV_CHANGE_SCALE( inVolum , 2000 , 255) - 2000;
	printf("setvolumn is %d\n",ret);
	MediaPlayer->put_Volume(ret);
}

//再生位置
int  RMediaPlayer::getPos() const throw() 
{
	double pos,all;
	MediaPlayer->get_CurrentPosition(&pos);
	MediaPlayer->get_Duration(&all);

	printf("%f / %f \n", pos , all);

	return CHANGE_SCALE( pos , all , 255) ;
}


void RMediaPlayer::setPos(int inPos) throw() 
{
	double pos,all;
	MediaPlayer->get_Duration(&all);
	pos = REV_CHANGE_SCALE( inPos , all , 255) ;

	MediaPlayer->put_CurrentPosition(pos);
}


//再生中?
bool RMediaPlayer::IsPlay() const 
{
	return true;
}


//一時停止中?
bool RMediaPlayer::IsPause() const 
{
	return true;
}





//サポートしてます?
bool RMediaPlayer::IsSupport(const string inFilename)
{
	//とりあえず、拡張子判定.
	string ext = Rfiles::Filename2Ext(inFilename);
	return  (ext == "AVI" || ext == "MPG" || ext == "MP3" ||	ext == "WMA" || ext == "ASF" );
}

//タイトルを取得する
string RMediaPlayer::getTitle() const 
{
	BSTR title;
	MediaPlayer->GetMediaInfoString( MediaPlayer::mpClipTitle ,&title);

	USES_CONVERSION;

	return OLE2CT(title);
}




