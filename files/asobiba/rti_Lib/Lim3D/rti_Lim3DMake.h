#ifndef	___LIM3D_MODELMAKE__H	//重複防止
#define ___LIM3D_MODELMAKE__H

//#include <./Lim3D/rti_ClassCaretaker.h>
#include <./Lim3D/rti_SuperList.h>
#include <./Lim3D/rti_Lim3DModel.h>

class TLim3DMake : public __TSuperListServer<TLim3DModel>
{
public:

	//コンストラクタ
	TLim3DMake(){m_ParenceModel = NULL;}
	//親モデルを教える.
	bool Mount_form_Model(TLim3DModel* ParenceModel)
	{
		if (m_ParenceModel != NULL)	return false;	//先客がいる.
		m_ParenceModel = ParenceModel;
		return true;
	}
	//親のモデル取得.
	TLim3DModel* GetParenceModel(){ return m_ParenceModel; }
protected:
	TLim3DModel* m_ParenceModel;			//親モデル.
private:
};

#endif				//重複防止