#include <iostream>
#include <string>

//
//戻り値にエラーを絡ませられるようにしてみたライブラリ
//
//
namespace xreturn
{

//エラーを上げたい時に使う
class error
{
	std::string    errormessage;
	int            errorcode;
public:
	error()
	{
		this->errormessage = "";
		this->errorcode = 0;
	}
	error(const std::string & errormessage , int errorcode = -1)
	{
		this->errormessage = errormessage;
		this->errorcode = errorcode;
	}
	std::string getErrorMessage() const
	{
		return this->errormessage;
	}
	int getErrorCode() const
	{
		return this->errorcode;
	}
};

enum enum_errortype{
	 errortype_no_error
	,errortype_error
	// 複雑になるからやめよう  ,errortype_ecetpyion 
};

//エラーをラップする構造
template <typename TYPE,typename EXCEPTIONTYPE = std::exception>
class r
{
private:
	TYPE value;
	enum_errortype errortype;
	std::string    errormessage;
	int            errorcode;
public:
	typedef typename TYPE __TYPE;
	typedef typename EXCEPTIONTYPE __EXCEPTIONTYPE;

	r()
	{
		this->errormessage = "";
		this->errorcode = -1;
		this->errortype = errortype_error;
	}
	virtual ~r()
	{
	}

	//boolにキャストする場合
	operator bool()
	{
		return this->isOK();
	}
	operator TYPE()
	{
		if (! this->isOK() )
		{
			//なんで std::exceptionって std::string を引数に取らないんだろうネ。
			throw EXCEPTIONTYPE(this->errormessage.c_str() , this->errorcode);
		}
		return this->value;
	}
	r<TYPE,EXCEPTIONTYPE>& operator =(TYPE a)
	{
		this->errormessage = "";
		this->errorcode = 0;
		this->errortype = errortype_no_error;
		return *this;
	}
	r(TYPE a)
	{
		this->errortype = errortype_no_error;
		this->value = a;
	}

	r(error a)
	{
		this->errortype = errortype_error;
		this->errormessage = a.getErrorMessage();
		this->errorcode = a.getErrorCode();
	}

	bool isOK() const
	{
		return this->errortype == errortype_no_error;
	}
	bool isError() const
	{
		return ! this->isOK();
	}
	const char* getErrorMessage() const
	{
		return this->errormessage.c_str();
	}
	std::string getErrorMessageString() const
	{
		return this->errormessage;
	}
	int getErrorCode() const
	{
		return this->errorcode;
	}
};


//エラーをラップする構造
//bool専用
template <typename EXCEPTIONTYPE>
class r<bool,EXCEPTIONTYPE>
{
private:
	bool value;
	enum_errortype errortype;
	std::string    errormessage;
	int            errorcode;
public:
	typedef typename bool __TYPE;
	typedef typename EXCEPTIONTYPE __EXCEPTIONTYPE;

	r()
	{
		this->errormessage = "";
		this->errorcode = -1;
		this->errortype = errortype_error;
	}
	virtual ~r()
	{
	}

	//boolにキャストする場合
	operator bool()
	{
		if (! this->isOK() )
		{
			//なんで std::exceptionって std::string を引数に取らないんだろうネ。
			throw EXCEPTIONTYPE(this->errormessage.c_str() , this->errorcode);
		}
		return this->value;
	}
	r<bool,EXCEPTIONTYPE>& operator =(bool a)
	{
		this->errormessage = "";
		this->errorcode = 0;
		this->errortype = errortype_no_error;
		return *this;
	}
	r(bool a)
	{
		this->errortype = errortype_no_error;
		this->value = a;
	}

	r(error a)
	{
		this->errortype = errortype_error;
		this->errormessage = a.getErrorMessage();
		this->errorcode = a.getErrorCode();
	}

	bool isOK() const
	{
		return this->errortype == errortype_no_error;
	}
	bool isError() const
	{
		return ! this->isOK();
	}
	const char* getErrorMessage() const
	{
		return this->errormessage.c_str();
	}
	std::string getErrorMessageString() const
	{
		return this->errormessage;
	}
	int getErrorCode() const
	{
		return this->errorcode;
	}
};


//////////////////////////////////////////////////////////
//小さな雪使い構文
//////////////////////////////////////////////////////////

//OK?
template<typename XRETURN> bool isOK(const XRETURN & a)
{
	return a.isOK();
}
//だめぽ?
template<typename XRETURN> bool isError(const XRETURN & a)
{
	return ! isOK(a);
}



//補足したエラーの内容で例外を上げて
template<typename __EXCEPTIONTYPE,typename XRETURN> 
void xthrow(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	//ASSERT( ! a.isOK() );
	if ( a.isOK() )
	{
		//OKなのに呼び出しやがった。どうしようかね・・・
		//ASSERTしたい。
		abort();
	}

	if ( ! a.isOK() )
	{
		//本当はexceptionを階層構造にしたいなー・・・・
		if (errormessage == "" && errorcode == -1) 
		{
			//ユーザが何も指定しないときは、
			//下位レイヤーが返した内容で例外を上げる.
			throw __EXCEPTIONTYPE(a.getErrorMessage() , a.getErrorCode() );
		}
		else
		{
			//ユーザーが指定した場合はその内容で例外を上げる
			throw __EXCEPTIONTYPE(errormessage.c_str() , errorcode);
		}
	}
}
//補足したエラーの内容で例外を上げて の投げる例外省略バージョン
template<typename XRETURN> 
void xthrow(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	xthrow< XRETURN::__EXCEPTIONTYPE,XRETURN>(a,errormessage,errorcode);
}


//エラーがあったら例外あげて、エラーじゃなかったら値頂戴
template<typename EXCEPTIONTYPE,typename XRETURN>XRETURN
	check(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	if ( ! a.isOK() )
	{
		xthrow<EXCEPTIONTYPE,XRETURN>(a,errormessage , errorcode);
	}
	return a;
}

//エラーがあったら例外あげて、エラーじゃなかったら値頂戴 の投げる例外省略バージョン
template<typename XRETURN>XRETURN
	check(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	return check<XRETURN::__EXCEPTIONTYPE,XRETURN>(a,errormessage , errorcode);
}

}; //namespace xreturn

///////
//こっから下は検証コード
////////


//オレオレ例外
class myexception
{
	std::string message;
	int code;
public:
	myexception( const std::string message , int code)
	{
		this->message = message;
		this->code = code;
	}
	virtual ~myexception()
	{
	}

	std::string what()
	{
		return this->message;
	}
};


xreturn::r<int> div10(int a)
{
	if (a == 0)
	{
		return xreturn::error("ゼロでわっちゃらめー",123);
	}
	return a / 10;
}

xreturn::r<bool,myexception> taechan(std::string fueruyo)
{
	if (fueruyo == "家族")
	{
		return xreturn::error("おい、やめろ",123);
	}
	if (fueruyo == "false")
	{
		return false;
	}
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//ふつーに使う
	int r1 = div10(100);
	std::cout << r1 << std::endl; 

	//エラーチェックして使う.
	if (!div10(0))
	{
		std::cout << "だめーぽ" << std::endl; 
	}

	//エラーチェックの内容を見る.
	auto r = div10(0);
	if (!r)
	{
		std::cout << "だめーぽ2 理由:" << r.getErrorMessage() << std::endl; 
	}

	//勇敢にもエラーチェックをしなかった場合キャストした瞬間に殺す.
	try
	{
		//エラーチェックなんてしないで使って<、
		//int にキャストした瞬間に殺される。
		int r99 = div10(0);
		std::cout << r99 << std::endl; 
	}
	catch(std::exception & )
	{
		std::cout << "例外99その1" << std::endl; 
	}


	//自分でエラーをハンドリングする場合
	try
	{
		auto r99 = div10(0);

		//エラーだったら例外を投げる
		if  ( isError(r99) )
		{
			//例外に変換する
			xreturn::xthrow(r99);
		}
	}
	catch(std::exception & )
	{
		std::cout << "例外99その2" << std::endl; 
	}


	//自分でエラーをハンドリングして、自前 exceptionにする場合
	try
	{
		auto r99 = div10(0);

		if  ( isError(r99) )
		{
			xreturn::xthrow<myexception>(r99);
		}
	}
	catch(myexception & e )	//std::exceptionではないことに注目.
	{
		std::cout << "例外99その3" << e.what() << std::endl; 
	}


	//エラーだったら、指定した例外を返して、
	//そうでなければ値を返して!
	try
	{
		int r99 = xreturn::check<myexception>(div10(0) , "エラーだったらこの文字列" , 9999);
	}
	catch(myexception & e )	//std::exceptionではないことに注目.
	{
		std::cout << "例外99その4" << e.what() << std::endl; 
	}

	//true が変えてくる場合
	if ( taechan("true") )
	{
		std::cout << "boolテスト その1" << std::endl; 
	}

	//false が変えてくる場合
	if ( ! taechan("false") )
	{
		std::cout << "boolテスト その2" << std::endl; 
	}

	//殺される場合
	try
	{
		if ( ! taechan("家族") )
		{
		}
	}
	catch(myexception & e )	//std::exceptionではないことに注目.
	{
		std::cout << "boolテスト。例外" << e.what() << std::endl; 
	}


	return 0;
}

