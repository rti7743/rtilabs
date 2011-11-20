
#include <./rti_define.h>
#include <./Graphics/rti_graphics.h>
#include <windows.h>

#ifndef ___RTI_GRAPHICS_TRIANGLE_DRAWH
#define ___RTI_GRAPHICS_TRIANGLE_DRAWH


#define __TRIANGLE_DRAW_LEFT	0
#define __TRIANGLE_DRAW_RIGHT	1

	//ベタ
    struct _XYZP
    {
		Fixed x,y,z;
    };
	//テクスチャ
    struct _XYZTP
    {
		Fixed x,y,z;
        short tx,ty;        //テクスチャ	
    };
	//ベタ グロー.
    struct _XYZCTP
    {
		Fixed x,y,z;
        short cr,cg,cb;     //色
    };
	//テクスチャ グロー.
    struct _XYZCTGP
    {
		Fixed x,y,z;
        short cr,cg,cb;     //色
        short tx,ty;        //テクスチャ
    };

	//ベタに必要なその他のデータ.
	struct _BetaData
	{
		int		rgb;
		BMD		OutBmd;
		LPZBUFFER ZBuffer;
	};

	//テクスチャに必要なその他のデータ.
	struct _TextureData
	{
		BMD		OutBmd;
		BMD		TextureBmd;
		LPZBUFFER ZBuffer;
	};


	//ベタ
    struct _WorkAreaP
    {
        Fixed StepX,StepZ;
    };
	//テクスチャー.
    struct _WorkAreaTP
    {
        Fixed StepX,StepZ,StepTX,StepTY;
    };
	//ベタ　グロー.
    struct _WorkAreaGP
    {
        Fixed StepX,StepZ,StepCR,StepCG,StepCB;
    };
	//ベタ　テクスチャ.
    struct _WorkAreaTGP
    {
        Fixed StepX,StepZ,StepCr,StepCG,StepCB,StepTX,StepTY;
    };

	struct _NN_WorkAreaP
	{
		int			Y;
		_WorkAreaP	Data[2]; // L R
	};


//単純四角形絵画.
void TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData);


#endif