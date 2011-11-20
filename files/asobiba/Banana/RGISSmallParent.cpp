// RGISSmallParent.cpp: RGISSmallParent クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallParent.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallParent::RGISSmallParent()
{
	Block = 0;
	Data = NULL;
	UserDataBuffer = NULL;
}

RGISSmallParent::~RGISSmallParent()
{
	delete [] Data;
	delete [] UserDataBuffer;
}

//データのセット
void RGISSmallParent::setData(unsigned long inBlock ,char* ioBuffer , char * ioUserDataBuffer) 
{
	ASSERT(Data == NULL);
	ASSERT(Block == 0);

	this->Block = inBlock;
	this->Data = ioBuffer;
	this->UserDataBuffer = ioUserDataBuffer;
	this->BlockStart = RGISSmallType::BlockToPoint(0,0, inBlock);
}

//ユーザーデータのリロード.
void RGISSmallParent::UserDataReload(char* ioUserDataBuffer) 
{
	delete [] this->UserDataBuffer;
	this->UserDataBuffer = ioUserDataBuffer;
}


//シンボルの解決
void RGISSmallParent::Parse(unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	switch(inSimbol)
	{
	case 'D':
		Douro.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'T':
		Tetudo.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'C':
		Chimei.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'K':
		Kawa.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'U':
		Koukyou.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'S':
		SuiikiKai.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'N':
		NameData.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'M':
		AddressName.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'A':
		Address.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'H':
		Koudo.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'B':
		Tatemono.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'O':
		Zyouti.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	case 'Z':
		UserData.Create(Block , inDataSize ,inDataCount ,inBuffer);
		break;
	}
}

//描画
void RGISSmallParent::Draw(RGISSmallDraw *inDraw ,const RECT & inClip)
{

	//線と塗りつぶし.
	Koudo.Draw1(&BlockStart , inDraw);
	SuiikiKai.Draw1(&BlockStart , inDraw);
	{
		//クリップを作成し、適応.
		inDraw->setPaintClip(&inClip);

		Koudo.Draw2(&BlockStart , inDraw);
		SuiikiKai.Draw2(&BlockStart , inDraw);
	}

	//線のみ
	Douro.Draw1(&BlockStart , inDraw );
	Tetudo.Draw1(&BlockStart , inDraw);
	Kawa.Draw1(&BlockStart , inDraw);
	Tatemono.Draw1(&BlockStart , inDraw);
	Zyouti.Draw1(&BlockStart , inDraw);

	//文字列
	Douro.Draw3(&BlockStart , inDraw , &NameData);
	Tetudo.Draw3(&BlockStart , inDraw , &NameData);
	Kawa.Draw3(&BlockStart , inDraw , &NameData);

	//文字列
	Chimei.Draw3(&BlockStart , inDraw , &NameData);
	Koukyou.Draw3(&BlockStart , inDraw , &NameData);
	Chimei.Draw3(&BlockStart , inDraw , &NameData);
	Koukyou.Draw3(&BlockStart , inDraw , &NameData);
	Address.Draw3(&BlockStart , inDraw , &AddressName);
	Tatemono.Draw3(&BlockStart , inDraw , &NameData);

	//ユーザデータ
	UserData.Draw3(&BlockStart , inDraw);
//	static iaaa = 0;
//	inDraw->DebugSave("test/test" + num2str(iaaa++) + ".bmp");

}

