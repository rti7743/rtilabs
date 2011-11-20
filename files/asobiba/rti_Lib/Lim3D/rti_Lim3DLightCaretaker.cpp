#include <./Lim3D/rti_Lim3DLightCaretaker.h>
#include <./Graphics/rti_graphics.h>
#include <./Lim3D/rti_Lim3DMaster.h>

//‚»‚ÌF‚ð‹‚ß‚é.
DWORD TLim3DLightCaretaker::GetColor
	(DWORD inRGB,POLYGON inPoly,XYZFixed const *inPos, TLim3DMaterial* inMaterial)
{
	DWORD				theRet = inRGB;
	//Žqƒ‚ƒfƒ‹‚ðŠG‰æ‚µ‚Ü‚·.
	TLim3DLight*		theLight ;

	for( theLight = FindFirst()  ;	theLight != NULL ; theLight = FindNext() )
	{
		RtiGraphics::AddColor( &theRet , theLight->Lighting( inPoly, inPos ,inMaterial) );
	}
	return theRet;
}

