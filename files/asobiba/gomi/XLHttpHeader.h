// XLHttpHeader.h: XLHttpHeader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLHTTPHEADER_H__3569AE9A_5235_4741_B71B_1FB2ABE4CD70__INCLUDED_)
#define AFX_XLHTTPHEADER_H__3569AE9A_5235_4741_B71B_1FB2ABE4CD70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XLHttpHeader  
{
public:
	XLHttpHeader();
	virtual ~XLHttpHeader();

	bool Parse(const char * inHeader , int inHeaderLen);

	string getRequestMethod() const
	{
		return this->getFirstParam(0);
	}
	string getRequestPath() const
	{
		return this->getFirstParam(1);
	}
	string getRequestProtocol() const
	{
		return this->getFirstParam(2);
	}

	string getResponsProtocol() const
	{
		return this->getFirstParam(0);	//HTTP/1.0
	}
	string getResponsResultCode() const
	{
		return this->getFirstParam(1);	//200
	}
	string getResponsResultState() const
	{
		return this->getFirstParam(2);	//OK
	}

	string getFirstParam(int inNumebr) const;

	void setFirstParam(int inNumebr, const string & inStr)
	{
		ASSERT(inNumebr >= 0 && inNumebr <= 2);
		this->FirstHeader[inNumebr] = inStr;
	}
	void setFirstParams( const string & inStr1, const string & inStr2, const string & inStr3)
	{
		this->FirstHeader[0] = inStr1;
		this->FirstHeader[1] = inStr2;
		this->FirstHeader[2] = inStr3;
	}

	void setAt(const string inKey , const string & inValue);
	string getAt(const string inKey) const;
	string Build() const;

	static void test();

private:
	string	FirstHeader[3];
	XLStringMap	Header;
};

#endif // !defined(AFX_XLHTTPHEADER_H__3569AE9A_5235_4741_B71B_1FB2ABE4CD70__INCLUDED_)
