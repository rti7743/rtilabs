// RGISSmallMaster.cpp: RGISSmallMaster クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallMaster.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallMaster::RGISSmallMaster()
{
	NowPoint.x = 51286263;
	NowPoint.y = 4279274433;
	Width = 320;
	Height = 240;
	Scale = 3;
	Deg = 0;
}

RGISSmallMaster::~RGISSmallMaster()
{

}

void RGISSmallMaster::Create(const string &inMapFilename  , const string & inUserDataPath,
							 RDIBBitmap * poolDlib,RDIBBitmap * poolCaption, const RGISSmallShareImage* poolShareImage,
							 const RGISSmallPenFile	* inPenFile) throw(RException)
{
	this->DrawObject.Create(poolDlib, poolDlib->getWidth() ,  poolDlib->getHeight() , poolShareImage);
	this->Width = poolDlib->getWidth();
	this->Height = poolDlib->getHeight();
	this->PenFile = inPenFile;

	this->GISSmallCahce.Create(inMapFilename,inUserDataPath);
	this->Caption.Create(poolCaption);
}


void RGISSmallMaster::Draw()
{
	//地図本体の描画.
	DrawObject.Black( BBananaConfig::getInstance()->getColor(COLOR_MIZU)  );
	GISSmallEffect.InitDisplay(&NowPoint , Width,Height , this->Scale,this->Deg );
	GISSmallCahce.New(&DrawObject , &GISSmallEffect );

	NoCacheDraw();

}

void RGISSmallMaster::NoCacheDraw()
{
	RGISSmallDraw draw(&DrawObject , &GISSmallEffect);
	PenFile->Draw(&draw);

	//画面の中心に + を書く.
	DrawObject.CenterCross();


	//キャプションの描画.
	Caption.Black( BBananaConfig::getInstance()->getColor(COLOR_CAPTION)  );
	Caption.Houi( Deg  );
	Caption.Zahyou( &NowPoint  );
	Caption.ScaleLine( this->Scale );
}


//この場所を画面の中心に持ってきます。
void RGISSmallMaster::LookAt(const LPOINT & inPoint)
{
	NowPoint = inPoint;
}




void RGISSmallMaster::MoveAt(int inX,int inY)
{
	RAngle d = RevAngle(this->Deg);

	const RMathAngleFixed* ang = RMathAngleFixed::getMathAngleFixed();
	this->NowPoint.x +=  (FtoI((inX) *ang->Cos(d) - (inY) *ang->Sin(d) ) ) << this->Scale;
	this->NowPoint.y +=  (FtoI((inX) *ang->Sin(d) + (inY) *ang->Cos(d) ) ) << this->Scale;

}


void RGISSmallMaster::FindEki(const string &inName , RGISSmallFindEki * ioFind)
{
	ioFind->Create(inName , &GISSmallCahce);
	GISSmallCahce.Find(ioFind);
}

void RGISSmallMaster::FindAddress(unsigned long inCode , const string &inName , unsigned long inBanchi, RGISSmallFindAddress * ioFind)
{
	ioFind->Create(inCode , inName , inBanchi , &GISSmallCahce);
	GISSmallCahce.Find(ioFind);
}

void RGISSmallMaster::AllCityToCodeTable(RGISSmallFindCityToNumberConvert * ioFind)
{
	ioFind->Create();
	GISSmallCahce.Find(ioFind);
}
