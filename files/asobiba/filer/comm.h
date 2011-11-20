//便利なライブラリ.
#if !defined(_COMM__INCLUDED_)
#define _COMM__INCLUDED_


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
	#include <windows.h>
	#include <commctrl.h>
	#include <process.h>
//ATL
	#include <atlbase.h>
	extern CComModule _Module;
	#include <atlcom.h>
//C
	#include <stdlib.h>
	#include <stdio.h>
	//DEBUG
	#include <cstdlib>
	#include <crtdbg.h>

	// 2つのDWORDを __int64 にする.
	#define INT64CONVERT(high,low)	( ( ((unsigned __int64)high) << 32) + (unsigned __int64)low)
	//TRUE を true に
	#define Btob(x)	((x) != FALSE)

#endif

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
//	#define ASSERT(f) /* */
//	#define TRACE	/* */
	#define ASSERT(f) \
		if (!(f) ) \
		{ \
			printf("ASSERT! %s %d\n", __FILE__, __LINE__); \
		} 
	#define TRACE	printf
#endif

#endif //_COMM__INCLUDED_
