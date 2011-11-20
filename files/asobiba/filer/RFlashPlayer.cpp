// RFlashPlayer.cpp: RFlashPlayer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RFlashPlayer.h"
#include "Rfiles.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RFlashPlayer::RFlashPlayer()
{

}

RFlashPlayer::~RFlashPlayer()
{

}


//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RFlashPlayer::Create(HINSTANCE inINstance,HWND isParentWindow) throw()
{
	RActiveXWindow::Create(inINstance,"ShockwaveFlash.ShockwaveFlash.1" , ShockwaveFlashObjects::IID_IShockwaveFlash, (void**)&ShockwaveFlash,isParentWindow,0, 0,WS_EX_STATICEDGE | WS_EX_ACCEPTFILES);
	puts("Control is OK");
}

//ファイルを開く
void RFlashPlayer::Open(const string inFilename )	throw() 
{
	USES_CONVERSION;
	BSTR filename = T2OLE( inFilename.c_str() );
	ShockwaveFlash->LoadMovie( 0 , filename );
}

//再生する.
void RFlashPlayer::Play() throw() 
{
	ShockwaveFlash->Play();
}


//停止する.
void RFlashPlayer::Stop() throw() 
{
	ShockwaveFlash->Stop();
}


//一時停止 / 解除
void RFlashPlayer::Pause(bool inON)	throw() 
{
	if (inON)	ShockwaveFlash->Play();
	else		ShockwaveFlash->Stop();
}

//ファイルクローズ
void RFlashPlayer::Close()	throw() 
{

}


//ボリューム
int RFlashPlayer::getVolum() const throw() 
{
	return 0;
}

void RFlashPlayer::setVolum(int inVolum) throw() 
{
}

//再生位置
int  RFlashPlayer::getPos() const throw() 
{
	long pos,all;
	ShockwaveFlash->get_TotalFrames(&all);
	ShockwaveFlash->CurrentFrame(&pos);

	printf("%f / %f \n", pos , all);

	return CHANGE_SCALE( pos , all , 255) ;
}


void RFlashPlayer::setPos(int inPos) throw() 
{
	long pos,all;
	ShockwaveFlash->get_TotalFrames(&all);
	pos = REV_CHANGE_SCALE( inPos , all , 255) ;

	ShockwaveFlash->GotoFrame(pos);
}


//再生中?
bool RFlashPlayer::IsPlay() const 
{
	VARIANT_BOOL	b;
	ShockwaveFlash->get_Playing(&b);
	return b == VARIANT_TRUE;
}


//一時停止中?
bool RFlashPlayer::IsPause() const 
{
	return true;
}


//サポートしてます?
bool RFlashPlayer::IsSupport(const string inFilename)
{
	//とりあえず、拡張子判定.
	string ext = Rfiles::Filename2Ext(inFilename);
	return  (ext == "SWF");
}

//タイトルの取得
string RFlashPlayer::getTitle() const 
{
	return "";
}
