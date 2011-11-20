// MSScriptEval.cpp: MSScriptEval クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Eval.h"
#include "MSScriptEval.h"
#include <atlbase.h>

//msscript エンジンの設定をぶっこく.
#import "msscript.ocx" no_namespace 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MSScriptEval::MSScriptEval()
{
	this->ErrorMessage = "";
	this->ErrorPosstion = 0;
}

MSScriptEval::~MSScriptEval()
{

}

//式の実行
double MSScriptEval::Eval(const string & inString)
{
	this->ErrorMessage = "";
	this->ErrorPosstion = 0;

	CComPtr<IScriptControl> scriptControl;
	//オブジェクトの構築
	HRESULT hr = scriptControl.CoCreateInstance(__uuidof(ScriptControl));
	if (FAILED(hr))
	{
		_com_issue_error(hr);
	}


	_variant_t v;
	try
	{
		//言語は、jscript を選択
		scriptControl->PutAllowUI( VARIANT_TRUE );
		scriptControl->PutLanguage(_bstr_t("jscript"));

		//式の実行
		v = scriptControl->Eval(_bstr_t(inString.c_str()));
	}
	catch(_com_error)
	{
		//実行時のエラー
		IScriptErrorPtr error = scriptControl->GetError();
		if (error != NULL)
		{
			this->ErrorMessage = (const char*) error->GetDescription();
			this->ErrorPosstion = (int) error->GetColumn() + 1;	//最初は1文字目にしたい
		}
		else
		{
			this->ErrorMessage = "未知のエラーが発生しました";
			this->ErrorPosstion = 0;
		}
	}
	return (double)v;
}

//エラーかどうか.
bool MSScriptEval::IsError() const
{
	//結構いい加減な実装ですね...
	return ( this->ErrorMessage != "");
}

//テスト
void MSScriptEval::test()
{
	{
		//正常系
		MSScriptEval mse;
		double rr;
		ASSERT((rr = mse.Eval("1+1")) == 2);
		ASSERT(!mse.IsError());
	}
	{
		//異常系
		MSScriptEval mse;
		mse.Eval("1+1abc");
		ASSERT(mse.IsError());
	}
}
