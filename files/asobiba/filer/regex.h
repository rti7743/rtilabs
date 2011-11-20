#ifdef DEBUG
	#undef new								//デバッグ用の new を殺す

	#pragma warning (disable: 4786) 
	#pragma warning (disable: 4503) 
	#include <boost/regex.hpp>

	#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)		//殺したので再定義
#else
	#pragma warning (disable: 4786) 
	#pragma warning (disable: 4503) 
	#include <boost/regex.hpp>
#endif
