#ifndef	___LIM3D_VECCORETAKER_H	//重複防止
#define ___LIM3D_VECCORETAKER_H

#include <./Lim3D/rti_Caretaker.h>
#include <./Lim3D/rti_Lim3DDefine.h>

class TLim3DModel;

class	TLim3DVecCaretaker : public _TCaretaker<VecWorldEye , XYZFixed>
{
public:

	TLim3DVecCaretaker(){ m_ParenceModel = NULL;}
	~TLim3DVecCaretaker(){}

	//inVecCaretakerのコピーをとる.
	void CopyForm(TLim3DVecCaretaker * inVecCaretaker);

	//親モデルを教える.
	bool Mount_form_Model(TLim3DModel * Model)
	{
		if (m_ParenceModel != NULL)	return false;	//先客がいる.
		m_ParenceModel = Model;
		return true;
	}
protected:
	//データコピー
	virtual	void	CopyData(VecWorldEye* DestData , XYZFixed* SrcCaretation , int Many)
	{	
		for(int i=0; i < Many ; i++)
		{
			DestData[i].Vec = SrcCaretation[i];
		}
	}
	//新規にメモリを追加等をしたために、メモリブロックが変化した場合に呼ばれる.
	//しかし、何が悲しくて、メモリアロケーションルーチンかくねん >> 俺. ばかだねー.
	//注意: OldUseSize は 個数です.
	virtual	void	ReAllocMemory(VecWorldEye* OldMemoryAddress , 
								  VecWorldEye* NewMemoryAddress ,
								  int OldUseSize)	;
	
	//この頂点データを管理しているモデルクラス
	TLim3DModel* m_ParenceModel;
private:
};

#include <./Lim3D/rti_Lim3DModel.h>

#endif							//重複防止
