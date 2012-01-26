// XLHttpHeader.cpp: XLHttpHeader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "XLHttpHeader.h"
#include "XLStream.h"
#include "XLStringUtil.h"
#include "XLStreamAdaptorMemory.h"
#include "XLStreamFindAlgorithmSimpleToken.h"
#include "XLStreamFindAlgorithmReturn.h"

USE_TEST(XLHttpHeader)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XLHttpHeader::XLHttpHeader()
{

}

XLHttpHeader::~XLHttpHeader()
{

}

bool XLHttpHeader::Parse(const char * inHeader , int inHeaderLen)
{
	XLStreamAdaptorMemory ad;
	ad.Mount(inHeader , inHeaderLen);
	
	XLStream stream;
	stream.Mount(&ad);

	//最初のヘッダー
	{
		{
			XLStreamFindAlgorithmSimpleToken token;
			this->FirstHeader[0] = stream.Read(&token);
			if (this->FirstHeader[0].empty() ) return false;
		}
		{
			XLStreamFindAlgorithmSimpleToken token;
			this->FirstHeader[1] = stream.Read(&token);
			if (this->FirstHeader[1].empty() ) return false;
		}
		{
			XLStreamFindAlgorithmReturn singleReturn;
			this->FirstHeader[2] = stream.Read(&singleReturn);
			if (this->FirstHeader[2].empty() ) return false;
		}
	}
	
	//それ以降のヘッダー
	while(1)
	{
		XLStreamFindAlgorithmReturn singleReturn;
		string line = stream.Read(&singleReturn);
		if ( line.empty() )
		{
			break;
		}

		int sep = line.find(':');
		if (sep < 0)
		{
			return false;
		}
		const string key = line.substr(0 ,sep);
		const string value = XLStringUtil::chop( line.substr(sep + 1) );
		Header[key] = value;
	}
	return true;
}

void XLHttpHeader::setAt(const string inKey , const string & inValue)
{
	this->Header[inKey] = inValue;
}

string XLHttpHeader::getAt(const string inKey) const
{
	XLStringMap::const_iterator i = this->Header.find(inKey);
	return i->second;
}

string XLHttpHeader::Build() const
{
	//最初のライン.
	string ret = this->FirstHeader[0]  + " " + this->FirstHeader[1]  + " " + this->FirstHeader[2] + "\r\n";
	//それ以降.
	XLStringMap::const_iterator i = this->Header.begin();
	XLStringMap::const_iterator e = this->Header.end();

	for( ; i != e ; i ++)
	{
		ret += i->first + ": " + i->second + "\r\n";
	}
	ret += "\r\n";

	return ret;
}


string XLHttpHeader::getFirstParam(int inNumebr) const
{
	ASSERT(inNumebr < 3);
	return this->FirstHeader[inNumebr];
}


void XLHttpHeader::test()
{
	{
		XLHttpHeader t;
		string th = 
				"HTTP/1.1 200 Document follows\r\n"
				"MIME-Version: 1.0\r\n"
				"Server: AnWeb/1.42n\r\n"
				"Date: Sat, 12 Mar 2005 05:45:20 GMT\r\n"
				"Content-Type: text/html\r\n"
				"Transfer-Encoding: chunked\r\n"
				"\r\n";
		bool r;
		ASSERT( ( r = t.Parse(th.c_str() , th.size() - 1) ) == true );

		string rr;
		ASSERT( ( rr = t.getResponsProtocol() ) == "HTTP/1.1" );
		ASSERT( ( rr = t.getResponsResultCode() ) == "200" );
		ASSERT( ( rr = t.getResponsResultState() ) == "Document follows" );
		ASSERT( ( rr = t.getAt("Date") ) == "Sat, 12 Mar 2005 05:45:20 GMT" );
		ASSERT( ( rr = t.getAt("MIME-Version") ) == "1.0" );
		ASSERT( ( rr = t.getAt("Content-Type") ) == "text/html" );
		ASSERT( ( rr = t.getAt("Transfer-Encoding") ) == "chunked" );
	}
}

