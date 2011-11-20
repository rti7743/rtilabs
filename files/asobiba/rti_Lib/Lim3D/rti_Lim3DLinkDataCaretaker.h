#ifndef	___LIM3D_LINKDATACARETAKER__H	//èdï°ñhé~
#define ___LIM3D_LINKDATACARETAKER__H

#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_Caretaker.h>

//í∏ì_ÉfÅ[É^ä«óù.
class TLim3DLinkDataCaretaker : _TCaretaker<PolygonDataEx , PolygonDataEx>
{
	public:
	protected:
		//ë„ì¸.
		virtual	void Storage(PolygonDataEx *writeObject , 
					const  PolygonDataEx *inputData) 
		{
			*writeObject = *inputData;
		}
	private:
};

#endif			//èdï°ñhé~
