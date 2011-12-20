#pragma once

#include <string>
#include <typeinfo>
//
//戻り値にエラーを絡ませられるようにしてみたライブラリ
//
//
namespace xreturn
{

//エラーコードと std::string もいけるようにした例外クラス
//xreturn::error として使ってね。
class error : public std::exception {
protected:
	std::string message; //
	int code;			 //std::exceptionがなぜか保持してくれないので自分で保持する.
public:

	error()
	{
	}
	error(const std::string & message , int code)
		: std::exception()
	{
		this->message = message;
		this->code = code;
	}
	virtual ~error() throw()
	{
	}

	virtual int getErrorCode() const
	{
		return this->code;
	}
	virtual std::string getErrorMessage() const
	{
		return this->message;
	}
	virtual const char* what() const throw()
	{
		return this->message.c_str();
	}
};


//テンプレートの型で関数作りたいときに同じ型だと衝突して死ぬので回避する.
class dummy_error2 : public xreturn::error{};
class dummy_error3 : public xreturn::error{};
class dummy_error4 : public xreturn::error{};
class dummy_error5 : public xreturn::error{};
class dummy_error6 : public xreturn::error{};
class dummy_error7 : public xreturn::error{};
class dummy_error8 : public xreturn::error{};

//例外から派生している型を格納する
class any_error_base
{
public:
	virtual void throwException() const = 0;
	virtual const xreturn::error& get() const = 0;
	virtual const std::type_info& type() const = 0;

	virtual ~any_error_base(){}
};

template<typename ERRORTYPE>
class any_error : public any_error_base
{
	ERRORTYPE error;
public:
	any_error(const ERRORTYPE & e) : any_error_base()
	{
		this->error = e;
	}
	virtual ~any_error()
	{
	}
	virtual void throwException() const
	{
		throw this->error;
	}
	virtual const xreturn::error& get() const
	{
		return this->error;
	}
	virtual const std::type_info& type() const
	{
		return typeid(ERRORTYPE);
	}
	const ERRORTYPE& getError() const
	{
		return &this->error;
	}
};

//エラーをラップする構造
template <typename TYPE
	,typename ERRORTYPE1 = xreturn::error
	,typename ERRORTYPE2 = dummy_error2
	,typename ERRORTYPE3 = dummy_error3
	,typename ERRORTYPE4 = dummy_error4
	,typename ERRORTYPE5 = dummy_error5
	,typename ERRORTYPE6 = dummy_error6
	,typename ERRORTYPE7 = dummy_error7
	,typename ERRORTYPE8 = dummy_error8
>
class r
{
private:
	TYPE value;
	any_error_base*	error;
	mutable bool    nigirichubushi; //すまぬ・・・ const auto に対応するために、 mutableにさせてくれ。
public:

	r()
	{
		this->error = nullptr;
		this->nigirichubushi = false;
	}
	virtual ~r()
	{
		if (this->nigirichubushi)
		{
			//非常に悩ましいが、あえてデストラクタで例外を投げる。
			//エラーをチェックしないで握りつぶすようなコードで例外処理を行うにはこれしか方法がない。。。
			if ( this->isError() )
			{
				//deleteで消し終わってから、throw exception したいんだけど、 投げる例外が消したいメモリ空間にあるわけで・・・
				//これはあもりにもジャンクコードすぎるのでなんとかしたいんだけど。
				if ( typeid(ERRORTYPE1) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE1>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
				if ( typeid(ERRORTYPE2) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE2>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
				if ( typeid(ERRORTYPE3) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE3>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
				if ( typeid(ERRORTYPE4) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE4>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
				if ( typeid(ERRORTYPE5) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE5>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
				if ( typeid(ERRORTYPE6) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE6>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
				if ( typeid(ERRORTYPE7) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE7>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
				if ( typeid(ERRORTYPE8) == this->error->type() )
				{
					auto e = *(dynamic_cast< any_error<ERRORTYPE8>* >(this->error));
					delete this->error;					this->error = nullptr;
					e.throwException();
				}
			}
		}

		delete this->error;
		this->error = nullptr;
	}

//boolにキャストする場合
//これで実装すると、 xreturn::r<bool> の時に衝突するから operator で逃げる.
//	operator bool() 
//	{
//		this->nigirichubushi = false;
//		return this->isOK();
//	}
	bool operator !() const
	{
		return this->isError();
	}
 	bool operator ==(bool t) const
	{
		if ( typeid(TYPE) == typeid(bool) )
		{//xreturn::r<bool>の場合は判別不能なので普通にキャスト operator TYPE() に飛ばす.
			return (*this);
		}
		return (t == true && this->isOK()) || (t == false && this->isOK());
	}

	operator TYPE() const
	{
		this->nigirichubushi = false;
		if ( this->isError() )
		{
			this->error->throwException(); //ここで例外を投げる
		}
		return this->value;
	}
	r<TYPE>& operator =(const TYPE & a)
	{
		this->value = a;
		this->error = nullptr;
		this->nigirichubushi = false;
		a.nigirichubushi = false;

		return *this;
	}
	r(const TYPE& a)
	{
		this->value = a;
		this->error = nullptr;
		this->nigirichubushi = false;
	}
	r(const ERRORTYPE1& e)
	{
		this->error = new any_error<ERRORTYPE1>(e);
		this->nigirichubushi = true;
	}
	r(const ERRORTYPE2& e)
	{
		this->error = new any_error<ERRORTYPE2>(e);
		this->nigirichubushi = true;
	}
	r(const ERRORTYPE3& e)
	{
		this->error = new any_error<ERRORTYPE3>(e);
		this->nigirichubushi = true;
	}
	r(const ERRORTYPE4& e)
	{
		this->error = new any_error<ERRORTYPE4>(e);
		this->nigirichubushi = true;
	}
	r(const ERRORTYPE5& e)
	{
		this->error = new any_error<ERRORTYPE5>(e);
		this->nigirichubushi = true;
	}
	r(const ERRORTYPE6& e)
	{
		this->error = new any_error<ERRORTYPE6>(e);
		this->nigirichubushi = true;
	}
	r(const ERRORTYPE7& e)
	{
		this->error = new any_error<ERRORTYPE7>(e);
		this->nigirichubushi = true;
	}
	r(const ERRORTYPE8& e)
	{
		this->error = new any_error<ERRORTYPE8>(e);
		this->nigirichubushi = true;
	}

	bool isOK()  const
	{
		this->nigirichubushi = false;   //すまぬ・・・ const auto に対応するために、 mutableにさせてくれ。
		return this->error == nullptr;
	}
	bool isError()  const
	{
		this->nigirichubushi = false; 
		return this->error != nullptr;
	}
	TYPE check() const
	{
		this->nigirichubushi = false; 
		return (*this);
	}
	const any_error_base* getError() const
	{
		//ASSERT(this->error != nullptr);
		this->nigirichubushi = false; 
		return this->error;
	}
	int getErrorCode() const
	{
		return this->getError()->get().getErrorCode();
	}
	std::string getErrorMessage() const
	{
		return this->getError()->get().getErrorMessage();
	}

	//例外を投げる.
	void throwException() const
	{
		this->nigirichubushi = false; 
		this->error->throwException();
	}


	//現状の型情報を取得する
	const std::type_info& type() const
	{
		//成功している場合は、保持している型をそのまま
		if ( this->isOK() )
		{
			return typeid(TYPE);
		}
		//エラーの場合は、エラー型をいただく.
		return this->getError()->type();
	}
};





//おまけ
#ifdef _WIN32
//windowsエラーメッセージを xreturn の中に組み込む感じのやつ
class windowsError : public xreturn::error
{
public:
	windowsError()
	{
		DWORD lasterror = ::GetLastError();
		this->code = (int)lasterror;
		this->appendMessage(lasterror);
	}
	windowsError(DWORD lasterror)
	{
		this->code = (int)lasterror;
		this->appendMessage(lasterror);
	}
	windowsError(const std::string & message,DWORD lasterror) 
	{
		this->code = (int)lasterror;
		this->message = message;

		this->appendMessage(lasterror);
	}
	virtual ~windowsError()
	{
	}
protected:
	void appendMessage(DWORD lasterror)
	{
		void* msgBuf;
		::FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			lasterror,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
			(LPTSTR) &msgBuf,
			0,
			NULL 
		);

		this->message = this->message + " " + (TCHAR*)msgBuf;

		::LocalFree( msgBuf );
	}
};
#endif //_WIN32

#ifdef errno
//errno を xreturn 風に
class errnoError : public xreturn::error
{
public:
	errnoError()
	{
		DWORD lasterror = errno;
		this->code = (int)lasterror;
		this->appendMessage(lasterror);
	}
	errnoError(int lasterror)
	{
		this->code = lasterror;
		this->appendMessage(lasterror);
	}
	errnoError(const std::string & message,int lasterror) 
	{
		this->code = lasterror;
		this->message = message;

		this->appendMessage(lasterror);
	}
	virtual ~errnoError()
	{
	}
protected:
	void appendMessage(int lasterror)
	{
#ifdef __STDC_WANT_SECURE_LIB__
		char buffer[1024];
		strerror_s( buffer , sizeof(buffer)-1, lasterror);
		buffer[sizeof(buffer)-1] = '\0';

		this->message = this->message + " " + buffer;
#else
		this->message = this->message + " " + strerror( lasterror );
#endif
	}
};
#endif //errno

}; //namespace xreturn

