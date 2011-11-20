#include <./Lim3D/rti_Lim3DVecCaretaker.h>

//新規にメモリを追加等をしたために、メモリブロックが変化した場合に呼ばれる.
//しかし、何が悲しくて、メモリアロケーションルーチンかくねん >> 俺. ばかだねー.
//注意: OldUseSize は 個数です.
void	TLim3DVecCaretaker::ReAllocMemory(VecWorldEye* OldMemoryAddress , 
								  VecWorldEye* NewMemoryAddress ,
								  int OldUseSize)
{
	m_ParenceModel->ReAllocMemoryVec(OldMemoryAddress , NewMemoryAddress , OldUseSize);
}
