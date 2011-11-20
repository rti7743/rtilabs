// RMessageBox.cpp: RMessageBox クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RMessageBox.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RMessageBox::RMessageBox()
{

}

RMessageBox::~RMessageBox()
{

}

void RMessageBox::Message(const string &inMsg , const string &inTitle )
{
#ifndef _WIN32_WCE
	MessageBox(NULL , inMsg.c_str() , inTitle.c_str() , MB_OK | MB_ICONHAND);
#else
	MY_USES_CONVERSION;
	MessageBox(NULL , _A2W(inMsg.c_str()) , _A2W(inTitle.c_str()) , MB_OK | MB_ICONHAND);
#endif

}

