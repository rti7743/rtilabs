// RBigArray2.h: RBigArray2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBIGARRAY2_H__08DE1735_8A4F_4997_909E_A20327332AF9__INCLUDED_)
#define AFX_RBIGARRAY2_H__08DE1735_8A4F_4997_909E_A20327332AF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class _T>
class RBigArray2
{
public:
	RBigArray2(int inX , int inY)
	{
		this->P = new _T*[inX];
		for(int i = 0 ; i < inX ; i++)
		{
			this->P[i] = new _T[inY];
		}
		this->X = inX;
	}
	~RBigArray2()
	{
		for(int i = 0 ; i < this->X ; i++)
		{
			delete [] this->P[i] ;
		}
		delete [] this->P;			
	}

	_T** get()
	{
		return this->P;
	}
private:
	int		X;
	_T**	P;
};

#endif // !defined(AFX_RBIGARRAY2_H__08DE1735_8A4F_4997_909E_A20327332AF9__INCLUDED_)
