// RFixedColumnArray.h: RFixedColumnArray クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFIXEDCOLUMNARRAY_H__6864D7BC_B7A4_44CE_81F7_1FF28E74CB4F__INCLUDED_)
#define AFX_RFIXEDCOLUMNARRAY_H__6864D7BC_B7A4_44CE_81F7_1FF28E74CB4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RFixedColumnArray;

#include "RFixedColumn.h"
#include "comm.h"

class RFixedColumnArray  
{
public:
	RFixedColumnArray()
	{
		this->Columnes = NULL;
	}
	virtual ~RFixedColumnArray()
	{
		delete [] this->Columnes;
	}

	//コピー.
	void setFixedCloumn(const RFixedColumn* inReadOnlyColumnes)
	{
		for( int len =0 ; inReadOnlyColumnes[len].getType()  != __FILECOLUMN_TYPE_END ; len++)
			;
		this->ColumnSize = len;
		delete [] this->Columnes;
		this->Columnes = new RFixedColumn[len + 1];

		for( int i =0 ; inReadOnlyColumnes[i].getType()  != __FILECOLUMN_TYPE_END ; i++)
		{
			this->Columnes[i] = inReadOnlyColumnes[i];
		}
		//番兵をコピー.
		this->Columnes[i] = inReadOnlyColumnes[i];

	}

	//指定したカラムの取得.
	const RFixedColumn* getColumn(int i) const
	{
		ASSERT(i < this->ColumnSize);
		return &this->Columnes[i];
	}
	//指定したカラムの取得.
	int getColumnSize() const
	{
		return this->ColumnSize;
	}

private:
	RFixedColumn*	Columnes;
	int				ColumnSize;
};

#endif // !defined(AFX_RFIXEDCOLUMNARRAY_H__6864D7BC_B7A4_44CE_81F7_1FF28E74CB4F__INCLUDED_)
