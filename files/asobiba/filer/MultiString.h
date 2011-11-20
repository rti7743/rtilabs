// MultiString.h: MultiString クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTISTRING_H__C85B369E_25FB_4283_96F4_E21538F1F00F__INCLUDED_)
#define AFX_MULTISTRING_H__C85B369E_25FB_4283_96F4_E21538F1F00F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


#define iskanji1st(c) (((c) >= 0x81 && (c) <= 0x9f) || ((c) >= 0xe0 && (c) <= 0xfc))
#define iskanji2nd(c) ((c) >= 0x40 && (c) <= 0xfc && (c) != 0x7f)

class MultiString  
{
public:
	MultiString();
	virtual ~MultiString();

	static bool isKanji(unsigned char in1st,unsigned char in2nd)
	{
		return (iskanji1st(in1st) && iskanji2nd(in2nd));
	}


	//パスの最後を補完する必要があるかどうか
	// 最後が \\ でない場合は true
	static bool IsPathToComplement(const string inPath , const char inPathChar = '\\')
	{
		const char* buf = inPath.c_str();

		bool kanji = false;
		for(int i = 0; buf[i] != '\0' ; i++)
		{
			unsigned char c = (unsigned char) buf[i];
			if (iskanji1st(c))
			{
				c = (unsigned char) buf[i+1 ];
				if (iskanji2nd(c))
				{
					c = (unsigned char) buf[i+2 ];
					if (c == '\0')
					{
						kanji = true;
						break;
					}
					// 2 byte skip
					i++;
				}
			}
		}
		//漢字で終わっているか、 最後が \\ でない場合は最後に \\ を補完します。
		return (kanji || buf[i-1] != inPathChar) ;
	}

	static int getLastDirectoryPos(const string inPath , const char inPathChar = '\\')
	{
		const char* buf = inPath.c_str();
		int lastpos = 0; 
		for(int i = 0; buf[i] != '\0' ; i++)
		{
			unsigned char c = (unsigned char) buf[i];
			if (iskanji1st(c))
			{
				c = (unsigned char) buf[i+1];
				if (iskanji2nd(c))
				{
					// 2 byte skip
					i++;
				}
			}
			else
			{
				if (c == inPathChar)
				{
					c = (unsigned char) buf[i+1];
					if (c == '\0')
					{
						break;
					}
					else
					{
						lastpos = i ;
					}
				}
			}
		}
		return lastpos  ;
	}

	static int setPathDelimiteres(const string inPath , const char inPathChar , int* outPathDelimiteres , int inSize)
	{
		const char* buf = inPath.c_str();
		int count = 0;
		for(int i = 0; buf[i] != '\0' ; i++)
		{
			unsigned char c = (unsigned char) buf[i];
			if (iskanji1st(c))
			{
				c = (unsigned char) buf[i+1];
				if (iskanji2nd(c))
				{
					// 2 byte skip
					i++;
				}
			}
			else
			{
				if (c == inPathChar)
				{
					if ( count >= inSize)			break;

					outPathDelimiteres[count++] = i;
				}
			}
		}
		outPathDelimiteres[count] = '\0';
		
		return count;
	}

	//深さを知る
	static int setPathDepth(const string inPath , const char inPathChar = '\\')
	{
		const char* buf = inPath.c_str();
		int count = 0;
		for(int i = 0; buf[i] != '\0' ; i++)
		{
			unsigned char c = (unsigned char) buf[i];
			if (iskanji1st(c))
			{
				c = (unsigned char) buf[i+1];
				if (iskanji2nd(c))
				{
					// 2 byte skip
					i++;
				}
			}
			else
			{
				if (c == inPathChar)
				{
					count++;
				}
			}
		}
		return count;
	}
};

#endif // !defined(AFX_MULTISTRING_H__C85B369E_25FB_4283_96F4_E21538F1F00F__INCLUDED_)
