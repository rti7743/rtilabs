#ifndef __COMM_INCLUDE__
#define __COMM_INCLUDE__

//STL
	#pragma warning (disable: 4786) 
	#pragma warning (disable: 4503) 
	#include <deque>
	#include <vector>
	#include <map>
	#include <string>
	#include <set>
	#include <algorithm>
	#include <functional>
	#include <utility>
	#include <new>
	#include <memory>
	using namespace std;

//WINDOWS
	#include <windows.h>
//	#include <winsock.h>

//C
	#include <stdlib.h>
	#include <stdio.h>
	#include <time.h>

//WINCE
#ifdef _WIN32_WCE
	#include <stdarg.h>
	#include <aygshell.h>
	#include <sipapi.h>
#endif


//支援
#ifdef _WIN32_WCE
	inline static string num2str(int num)
	{
		char buf[20];
		return _itoa(num , buf , 10);
	}
#else
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
#endif

static string format(const char* Format,...)
{
	char buffer[1024];
	_vsnprintf(buffer,1024,Format,(char*)(&Format+1));

	return buffer;
}

//便利な型
typedef vector<char>		XLBuffer;
typedef map<string,string>	XLStringMap;
typedef deque<string>		XLStringList;
typedef vector<int>			XLArray;

//2次元テーブルの数回数の取得
#define TABLE_COUNT_OF(table)	(sizeof((table)) / sizeof((table)[0]))

//TRUE を true に
#define Btob(x)	((x) != FALSE)
//true を TRUE に
#define btoB(x)	((DWORD) x)


//デバッグ支援
#ifdef _DEBUG
		//指定したクラスのテストを強制実行.
		#define USE_TEST(X)	\
			class X##testGo\
			{\
			public:\
				X##testGo()\
				{\
					X::test() ;\
				}\
			};\
			static X##testGo testGo;


#else
		#define USE_TEST
#endif

#define ETRACE	(format("throws::%s:%d " , __FILE__ , __LINE__) )



#endif //__COMM_INCLUDE__