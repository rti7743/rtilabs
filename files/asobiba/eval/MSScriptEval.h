// MSScriptEval.h: MSScriptEval クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSSCRIPTEVAL_H__FAE5EBDB_DDD4_44A5_9F77_1F2D6059C36B__INCLUDED_)
#define AFX_MSSCRIPTEVAL_H__FAE5EBDB_DDD4_44A5_9F77_1F2D6059C36B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//ScriptControl を利用して jscript evalを呼び出す
//参考
//http://dobon.net/vb/dotnet/programing/eval.html
//http://www.codeproject.com/KB/COM/scriptdemo.aspx?display=PrintAll&fid=4239&df=90&mpp=25&noise=3&sort=Position&view=Quick&select=1250951
class MSScriptEval  
{
public:
	MSScriptEval();
	virtual ~MSScriptEval();

	//式の実行
	double Eval(const string & inString);

	//エラーかどうか.
	bool IsError() const;
	//エラーメッセージの取得
	string getErrorMessage() const
	{
		return this->ErrorMessage;
	}
	//エラーの位置
	int getErrorPosstion() const
	{
		return this->ErrorPosstion;
	}

	//テスト
	static void test();

private:
	//エラーメッセージ
	string ErrorMessage;
	//エラーの場所
	int ErrorPosstion;
};

#endif // !defined(AFX_MSSCRIPTEVAL_H__FAE5EBDB_DDD4_44A5_9F77_1F2D6059C36B__INCLUDED_)
