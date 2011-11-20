// RFunctionScript.h: RFunctionScript クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFUNCTIONSCRIPT_H__049A8A30_B2CA_413B_870D_A6D73C1C3EE8__INCLUDED_)
#define AFX_RFUNCTIONSCRIPT_H__049A8A30_B2CA_413B_870D_A6D73C1C3EE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RConv.h"
#include "Rfiles.h"
#include "StringList.h"
#include "RParseException.h"

//一行解析
//構文解析のソースは汚くなるねー それって俺がへっぽこだから ムム。 
class Sentence
{
public:
	string Parse(string inSentence) throw(RParseException);
private:
	string Run(string inRunnable,string inStdOut) throw(RParseException);

};





//スクリプト解析
//マクロは「あふ」をパクることに決定(w
class RFunctionScript  
{
public:
	RFunctionScript();
	virtual ~RFunctionScript();

	string Interpreter(const string inScript) const throw (RParseException);

private:

	//マクロ展開
	string MacroReplace(const string inScript) const throw(RParseException);

	//ショートパスに変換.
	string ConvertSortFilename(const string inLong) const
	{
		char buf[MAX_PATH];
		GetShortPathName(inLong.c_str() , buf , MAX_PATH);
		return buf;
	}
	//aaaa.txt -> aaaa
	string getFilenameOnly(const string inFilename) const
	{
		const string ext = Rfiles::Filename2Ext( inFilename );
		return inFilename.substr(0,inFilename.size() - ext.size() - 1);
	}
	//aaaa.txt -> txt
	string getExtOly(const string inFilename) const
	{
		return Rfiles::Filename2Ext( inFilename );
	}

};

#endif // !defined(AFX_RFUNCTIONSCRIPT_H__049A8A30_B2CA_413B_870D_A6D73C1C3EE8__INCLUDED_)
