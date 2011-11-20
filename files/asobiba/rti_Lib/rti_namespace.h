#ifndef ___RTI_NAMESPACEH
#define ___RTI_NAMESPACEH

/*
    クラスにするまでもない関数とかは、  namespace で押し込みます(汗)
*/

//グラフィックス関係.
#define _GRAPHICS_BEGIN namespace RtiGraphics{
#define _GRAPHICS_END   };
#define USEING_GRAPHICS using namespace RtiGraphics;
//基本入出力関係
#define _BIO_BEGIN namespace RtiBio{
#define _BIO_END   };
#define USEING_BIO using namespace RtiBio;

#endif

