// blowfish.h     interface file for blowfish.cpp
// _THE BLOWFISH ENCRYPTION ALGORITHM_
// by Bruce Schneier
// Revised code--3/20/94
// Converted to C++ class 5/96, Jim Conger

// このソースは、上記のソースを rti が改造して使用しています。
//ライセンスはオリジナルのソースのライセンスに従います。

#include "comm.h"


// choose a byte order for your hardware

// DCBA - little endian - intel
union ORDER_DCBA {
  DWORD dword;
  BYTE byte [4];
  struct {
	unsigned int byte3:8;
	unsigned int byte2:8;
	unsigned int byte1:8;
	unsigned int byte0:8;
  } w;
};

// ABCD - big endian - motorola
union ORDER_ABCD {
  DWORD dword;
  BYTE byte [4];
  struct {
	unsigned int byte0:8;
	unsigned int byte1:8;
	unsigned int byte2:8;
	unsigned int byte3:8;
  } w;
};

// BADC - vax
union ORDER_BADC {
  DWORD dword;
  BYTE byte [4];
  struct {
	unsigned int byte1:8;
	unsigned int byte0:8;
	unsigned int byte3:8;
	unsigned int byte2:8;
  }w;
};


//intel入ってる.
#define AWORD	ORDER_DCBA

class RBlowFish
{
private:
	DWORD 		* PArray ;
	DWORD		(* SBoxes)[256];
	void 		Blowfish_encipher (DWORD *xl, DWORD *xr) ;
	void 		Blowfish_decipher (DWORD *xl, DWORD *xr) ;
public:
	RBlowFish () 
	{
	 	PArray = new DWORD [18] ;
 		SBoxes = new DWORD [4][256] ;
	}
	~RBlowFish () 
	{
		delete PArray ;
		delete [] SBoxes ;
	}
	void 		Initialize (BYTE * key, int keybytes) ;
	// get output length, which must be even MOD 8
	DWORD		GetOutputLength (DWORD lInputLong) ;

	// Encode pIntput into pOutput.  Input length in lSize.  Returned value
	// is length of output which will be even MOD 8 bytes.  Inputbuffer and
	// output buffer can be the same, but be sure buffer length is even MOD 8.
	DWORD		Encode (BYTE * pInput, BYTE * pOutput, DWORD lSize) ;
	// Decode pIntput into pOutput.  Input length in lSize.  Inputbuffer and
	// output buffer can be the same, but be sure buffer length is even MOD 8.
	void		Decode (BYTE * pInput, BYTE * pOutput, DWORD lSize) ;

	
	//rti  テスト.
	static void test();
} ;
