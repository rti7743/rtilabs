#ifndef	___LIM3D_POLYGONCARETAKER__H	//重複防止
#define ___LIM3D_POLYGONCARETAKER__H

//#include <./Lim3D/rti_ClassCaretaker.h>
#include <./Lim3D/rti_SuperList.h>
#include <./Lim3D/rti_Lim3DPolygon.h>
#include <./base/rti_debug.h>

class TLim3DModel;

class TLim3DPolygonCaretaker : public __TSuperListServer<TLim3DPolygon>
{
public:
	TLim3DPolygonCaretaker(){m_ParenceModel = NULL;}
	~TLim3DPolygonCaretaker(){}

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
	TLim3DModel* m_ParenceModel;	//親のモデル.
private:
};

#include <./Lim3D/rti_Lim3DModel.h>

#endif				//重複防止