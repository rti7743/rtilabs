// SusieCallable.h: SusieCallable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIECALLABLE_H__F17020E0_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
#define AFX_SUSIECALLABLE_H__F17020E0_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Susie プラグインの進捗をチェキするやつ
class SusieCallable  
{
public:
	SusieCallable()
	{
	}
	virtual ~SusieCallable()
	{
	}
protected:
    //まず nNum==0 でコールされ、nNum==nDenom になるまで
    //定期的に呼ばれる。
    //戻値が 非0 の時、Plug-inは処理を中断する。
	virtual int SusieaCallBack(int inNum,int inDenom) 
	{
		return 0;
	}
public:
	//Susieプラグインの進捗をチェキするやつ
	static  int __stdcall ProgressCallback(int inNum,int inDenom,long inData)
	{
		return ((SusieCallable*)inData)->SusieaCallBack(inNum,inDenom);
	}

};

#endif // !defined(AFX_SUSIECALLABLE_H__F17020E0_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
