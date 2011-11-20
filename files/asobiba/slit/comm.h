//便利なライブラリ.
#if !defined(_COMM__INCLUDED_)
#define _COMM__INCLUDED_

#ifdef NDEBUG
    #undef _STLP_DEBUG
#else
    #pragma warning(disable:4786)
    #pragma warning(disable:4290)
    #define _STLP_DEBUG 1
#endif

//STL
	#pragma warning (disable: 4786) 
	#pragma warning (disable: 4503) 
	#pragma warning (disable: 4996)		//VC2005うざすぎ
	#pragma warning (disable: 4290)		//VC2005うざすぎ
	#include <string>
	#include <map>
	#include <vector>
	#include <set>
	#include <deque>
	#include <algorithm>
	#include <utility>
	#include <functional>
	#include <new>
	#include <memory>
	using namespace std;

//WINDOWS
	#include <windows.h>
	#include <commctrl.h>
	#include <wingdi.h>
	#include <stdlib.h>
//C
	#include <stdlib.h>
	#include <stdio.h>
	#include <time.h>

#ifdef _DEBUG
//デバッグ
	#include <cstdlib>
	#ifndef _WIN32_WCE
		#include <crtdbg.h>
	#endif
#endif
	//ATL
//	#include <atlbase.h>


#ifdef _WIN32_WCE
	#include <stdarg.h>
	#include <aygshell.h>
	#include <sipapi.h>
#else
	#include <process.h>
//C
	#include <sstream>
#endif



	// 2つのDWORDを __int64 にする.
	#define INT64CONVERT(high,low)	( ( ((unsigned __int64)high) << 32) + (unsigned __int64)low)
	//TRUE を true に
	#define Btob(x)	((x) != FALSE)
	//true を TRUE に
	#define btoB(x)	((DWORD) x)

	template <class _T >class newVector : public vector<_T*>
	{
	public:
		virtual ~newVector()
		{
			clear();
		}

		void clear()
		{
			iterator  i = begin();
			iterator  e = end();

			for( ; i != e; i++)
			{
				delete *i;
			}
			vector<_T*>::clear();
		}
	};

	//Unicode <-> SJIS の変換に使うためのバッファ管理
	template <class _T >class _myAutoPtr
	{
	public:
		_myAutoPtr()
		{
			this->Core = NULL;
		}
		~_myAutoPtr()
		{
			Delete();
		}
		_T* Alloc(int inSize)
		{
			Delete();
			this->Core = new _T[inSize];

			return this->Core;
		}
		void Delete()
		{
			delete [] this->Core;
			this->Core = NULL;
		}

		const _T* getCore() const
		{
			return Core;
		}
	private:
		_T* Core;
	};
	static unsigned short* _atou_convert(const char* inSrc,unsigned short* outBuffer , int inDestSize)
	{
		outBuffer[0] = 0;
		MultiByteToWideChar(CP_ACP,0,inSrc,-1,outBuffer,inDestSize);

		return outBuffer;
	}
	static char* _utoa_convert(const unsigned short* inSrc, char* outBuffer , int inDestSize)
	{
		outBuffer[0] = 0;
		WideCharToMultiByte(CP_ACP,0,inSrc,-1,outBuffer,inDestSize , NULL,NULL);

		return outBuffer;
	}

	#define MY_USES_CONVERSION int _convert;  _myAutoPtr<char> _utoa_str ; _myAutoPtr<unsigned short> _atou_str

	#define _A2W(lpa) (\
		(lpa == NULL) ? NULL : ( _convert = (strlen(lpa)+1) , _atou_convert(lpa, _atou_str.Alloc(_convert * 2) ,_convert * 2) ) )
	#define _W2A(lpa) (\
		(lpa == NULL) ? NULL : ( _convert = (lstrlen(lpa)+1) , _utoa_convert(lpa, _utoa_str.Alloc(_convert * 2),_convert * 2) ) )


#ifdef _WIN32_WCE
	inline static string num2str(int num)
	{
		char buf[20];
		return _itoa(num , buf , 10);
	}
#else
	#ifndef _T
		#define _T(x)	(x)
	#endif
	inline static string num2str(int num)
	{
		char buf[20];
		return itoa(num , buf , 10);
	}
	inline static string num2str(unsigned int num)
	{
		char buf[20];
		return ultoa(num , buf , 10);
	}
	inline static string num2str(unsigned long num)
	{
		char buf[20];
		return ultoa(num , buf , 10);
	}
#endif
	static string PrintFormat(const char* Format,...)
	{
		char buffer[1024];
		_vsnprintf(buffer,1024,Format,(char*)(&Format+1));

		return buffer;
	}


#define EXCEPTIONTRACE	(string("throws::") + string(__FILE__) + ":" + num2str(__LINE__) + string(":") + string(" "))


typedef vector<char>		RBuffer;
typedef map<string,string>	RStringMap;
typedef deque<string>		RStringList;
typedef vector<string>		RStringVector;
typedef vector<int>			RArray;

//ファイルの存在確認.
#define FILE_EXIST(name)	(0xFFFFFFFF != GetFileAttributes(name))
//2次元テーブルの数回数の取得
#define TABLE_COUNT_OF(table)	(sizeof((table)) / sizeof((table)[0]))

#ifdef _WIN32_WCE
	#ifdef _DEBUG
		static void TRACE(const char* Format,...)
		{
			char buffer[1024];
			_vsnprintf(buffer,1024,Format,(char*)(&Format+1));


			MY_USES_CONVERSION;
			OutputDebugString( _A2W(buffer) );
		}
		static void TRACE(const string & inStr)
		{
			TRACE(inStr.c_str());
		}
	#else
		#define TRACE	/* */
	#endif
#else
	#ifdef _DEBUG

		static void TRACE(const char* Format,...)
		{
			char buffer[1024];
			_vsnprintf(buffer,1024,Format,(char*)(&Format+1));
			OutputDebugString( buffer );
		}
		static void TRACE(const string & inStr)
		{
			TRACE(inStr.c_str());
		}

		#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
		//windowsだったら... ASSERT のパクリ
		#define ASSERT(f) \
			if (!(f) ) \
			{ \
				MSG msg;	\
				BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);	\
				if (bQuit)	PostQuitMessage(msg.wParam);	\
				__asm { int 3 }; \
			} 
	#else
		#define ASSERT(f) 
		#define TRACE	
	/*
		#define ASSERT(f) \
			if (!(f) ) \
			{ \
				printf("ASSERT! %s %d\n", __FILE__, __LINE__); \
			} 
		#define TRACE	
	*/
	#endif
#endif


struct SPOINT
{
	unsigned short x;
	unsigned short y;
};

struct LPOINT
{
	unsigned long x;
	unsigned long y;
};
struct LRECT
{
	unsigned long left;
	unsigned long top;
	unsigned long right;
	unsigned long bottom;
};


struct UPOINT
{
//	unsigned __int64 x;
//	unsigned __int64 y;
	__int64 x;
	__int64 y;
};

static UPOINT LtoU(const LPOINT & lpoint)
{
	UPOINT xy;
	xy.x = lpoint.x;
	xy.y = lpoint.y;
	return xy;
}

struct BPOINT
{
	__int64 x;
	__int64 y;
};

struct URECT
{
	unsigned __int64 left;
	unsigned __int64 top;
	unsigned __int64 right;
	unsigned __int64 bottom;
};

struct DoublePoint
{
	double x,y;
};
struct Double3Point
{
	double x,y,z;
};

//ディグリーからラジアンに変換
#define RRR(i) ( ((double)i) * 3.141592 / 128.f)
//ラジアンからディグリーに変換
#define DDD(i) ( ((double)i) * 128.f / 3.141592 )

#endif //_COMM__INCLUDED_
