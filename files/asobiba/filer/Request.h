// Request.h: Request クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REQUEST_H__AD9CDAF7_D6D4_4521_BE75_B5E3D4A6ADE4__INCLUDED_)
#define AFX_REQUEST_H__AD9CDAF7_D6D4_4521_BE75_B5E3D4A6ADE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Request  ;

//ワーカースレッドパターンで仕事を表すスレッド.
class Request  
{
public:
	Request()
	{
	}
	virtual ~Request()
	{
	}

	virtual void Execute() = 0;
};

#endif // !defined(AFX_REQUEST_H__AD9CDAF7_D6D4_4521_BE75_B5E3D4A6ADE4__INCLUDED_)
