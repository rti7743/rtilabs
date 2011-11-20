// RPackSuper.h: RPackSuper クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RPACKSUPER_H__4A989A1D_111A_4862_9574_52417852088C__INCLUDED_)
#define AFX_RPACKSUPER_H__4A989A1D_111A_4862_9574_52417852088C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


class RPackSuper
{
public:
	/**
	 * RPack:		コンストラクタ
	 *
	 */
	RPackSuper()
	{
		this->Memory = NULL;
		this->Len = 0;
		this->ForcesBuffer = NULL;
		this->ForcesBufferLen = 0;
	}
	/**
	 * RPack:			初期値つきコンストラクタ
	 *
	 * @param inMem		格納するメモリ
	 * @param inLen		メモリの長さ
	 */
	RPackSuper(const void* inMem,int inLen)
	{
		this->Memory = NULL;
		this->Len = 0;
		this->ForcesBuffer = NULL;
		this->ForcesBufferLen = 0;

		Write(inMem , inLen);
	}
	~RPackSuper()
	{
		delete [] this->Memory;
	}
	/**
	 * Write:			新規に格納する(既存のメモリは解放されます)
	 *
	 * @param inStr		文字列
	 */
	void Write(const char* inStr)
	{
		this->Write( (const void*)inStr , strlen(inStr) );
	}

	/**
	 * Write:			新規に格納する(既存のメモリは解放されます)
	 *
	 * @param inMem		格納するメモリ
	 * @param inLen		メモリの長さ
	 */
	void Write(const void* inMem,int inLen)
	{
		ASSERT(inMem != NULL);
		ASSERT(inLen >= 0);

		delete [] Memory;
		if ( this->ForcesBufferLen < inLen)
		{
			this->Memory = new char[inLen];
			memcpy(this->Memory,inMem , inLen);
		}
		else
		{
			Memory = NULL;
//			memcpy(this->ForcesBuffer,inMem , inLen);
			char * p = this->ForcesBuffer;
			_asm
			{
				cld
				mov ecx,inLen
				mov esi,inMem
				add ecx,3
				mov edi,p
				shr ecx,2
				rep movsd;
			}
		}
		this->Len = inLen;
	}

	/**
	 * Read:			新規に格納する(既存のメモリは解放されます)
	 *
	 * @param outMem	読み出すメモリ
	 * @param inLen		メモリの長さ
	 * @return int		読み出せたサイズ.
	 */
	int Read(char* outMem,int inLen) const
	{
		ASSERT(outMem != NULL);
		ASSERT(inLen >= 0);

		//補正.
		if (inLen > this->Len) inLen = this->Len ;

		memcpy(outMem , this->Memory , inLen);
		return inLen;
	}

	/**
	 * getMemory:	格納しているメモリへ直参照
	 *
	 * @return const void*	読み出しOnlyなメモリ.
	 */
	const void*	getMemory() const
	{
		return this->Memory != NULL ? this->Memory : this->ForcesBuffer;
	}

	/**
	 * getSize:		格納しているメモリサイズ
	 *
	 * @return const int	サイズ.
	 */
	int	getSize() const
	{
		return this->Len;
	}

	void Mount(char* inForcesBuffer , int inBufferLen)
	{
		ForcesBuffer = inForcesBuffer;
		ForcesBufferLen = inBufferLen;
	}

private:
	//外では足りなくなったときのための自前ヒープ
	char*	Memory;
	int		Len;

	//優先して使う外のバッファ
	char*	ForcesBuffer;
	int		ForcesBufferLen;
};

typedef RPackSuper	RPack;

#endif // !defined(AFX_RPACKSUPER_H__4A989A1D_111A_4862_9574_52417852088C__INCLUDED_)
