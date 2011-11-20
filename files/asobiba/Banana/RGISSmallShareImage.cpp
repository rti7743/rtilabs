// RGISSmallShareImage.cpp: RGISSmallShareImage クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallShareImage.h"
#include "RResorceImageLoad.h"
#include "BConf.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallShareImage::RGISSmallShareImage()
{

}

RGISSmallShareImage::~RGISSmallShareImage()
{

}

void RGISSmallShareImage::Create()
{
	//地図上で使用するイメージのロード.
	RResorceImageLoad ril;
	ril.LoadFile(&IconImage , BConf::getConf()->getCurrentDirectory() + "tool_bit.bmp");
}
