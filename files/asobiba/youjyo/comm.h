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


#if WIN32

//STL
	#pragma warning (disable: 4786) 
	#pragma warning (disable: 4503) 
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
	#include <Winsock2.h>
	#include <mswsock.h>
	#include <windows.h>
	#include <commctrl.h>
	#include <process.h>
//C
	#include <stdlib.h>
	#include <stdio.h>
	#include <time.h>
	#include <sstream>

//DEBUG
	#include <cstdlib>
	#include <crtdbg.h>

	// 2つのDWORDを __int64 にする.
	#define INT64CONVERT(high,low)	( ( ((unsigned __int64)high) << 32) + (unsigned __int64)low)
	//TRUE を true に
	#define Btob(x)	((x) != FALSE)


#endif

inline static std::string num2str(int num)
{
	char buf[20];
	return itoa(num , buf , 10);
}
inline static std::string num2str(unsigned int num)
{
	char buf[20];
	return itoa(num , buf , 10);
}
inline static std::string num2str(unsigned long num)
{
	char buf[20];
	return itoa(num , buf , 10);
}

#define EXCEPTIONTRACE	(string("throws::") + string(__FILE__) + num2str(__LINE__) + string(":") + string(" "))

#if _DEBUG
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
	#define TRACE printf
#else
	#define ASSERT(f) /* */
//	#define TRACE	/* */
/*
	#define ASSERT(f) \
		if (!(f) ) \
		{ \
			printf("ASSERT! %s %d\n", __FILE__, __LINE__); \
		} 
*/
	 #define TRACE	printf
#endif


typedef map<string,string>	RStringMap;
typedef string		RBuffer;

//ファイルの存在確認.
#define FILE_EXIST(name)	(0xFFFFFFFF != GetFileAttributes(name))

typedef unsigned long DATNUMBER;
const int THREAD_WORK_AREA		=	800 * 1024;	//800kB


#endif //_COMM__INCLUDED_
