// RGISCVCut.cpp: RGISCVCut クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RDebug.h"

const int NEXT_LINE_BLOCK	= 4096;
//const int NEXT_LINE_BLOCK	= 3200 * 1000 * 10;



//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISCVCut::RGISCVCut()
{

}

RGISCVCut::~RGISCVCut()
{

}

//書き込み
void RGISCVCut::Write(const string & inDir , const RGISSlp * inSlp,
					  const GISINTArray* inArray ,RGISCVCutDataIml * ioData)
{
	PointSet * p = new PointSet[65535];

	int i;

	//CVの数の取得...
	unsigned short count = inArray->getCount();

	UPOINT lastStep = {0};
	unsigned short trueCVcount = 0;
	//とりあえず、すべてのデータをブロックごとに切り分けてみます。
	for(i = 0 ;  i < count ; i++)
	{
		int pt = inArray->getPoint(i) ;
		if (pt != 0 )
		{
			//精度を /100 にした結果、隣り合う点がまったく同じ場所に配置されている無駄な個所があります。
			//それを排除してデータサイズを小さくします.
			UPOINT getpoint = inSlp->getPoint(pt - 1);

//			TRACE("%I64u, %I64u\n" , getpoint.x , getpoint.y);

			if (!(lastStep.x == getpoint.x && lastStep.y == getpoint.y ))
			{
				lastStep = getpoint;
				p[trueCVcount].po = getpoint;
				p[trueCVcount].lastBlock = selectBlock( &p[trueCVcount].po );
				trueCVcount++;
			}
		}
	}
	if (trueCVcount != 0) 
	{
		WriteMain(inDir ,  p , trueCVcount , ioData);
	}

	delete [] p;
}

//書き込み
void RGISCVCut::Write(const string & inDir , 
					  const RGIS2500PointArray* inArray ,RGISCVCutDataIml * ioData)
{
	PointSet * p = new PointSet[65535];

	int i;

	//CVの数の取得...
	unsigned short count = inArray->getCount();

	UPOINT lastStep = {0};
	unsigned short trueCVcount = 0;
	//とりあえず、すべてのデータをブロックごとに切り分けてみます。
	for(i = 0 ;  i < count ; i++)
	{
		//精度を /100 にした結果、隣り合う点がまったく同じ場所に配置されている無駄な個所があります。
		//それを排除してデータサイズを小さくします.
		UPOINT getpoint= inArray->getPoint(i) ;


//			TRACE("%I64u, %I64u\n" , getpoint.x , getpoint.y);

		if (!(lastStep.x == getpoint.x && lastStep.y == getpoint.y ))
		{
			lastStep = getpoint;
			p[trueCVcount].po = getpoint;
			p[trueCVcount].lastBlock = selectBlock( &p[trueCVcount].po );
			trueCVcount++;
		}
	}
	if (trueCVcount != 0) 
	{
		WriteMain(inDir ,  p , trueCVcount , ioData);
	}

	delete [] p;
}
//書き込み
void RGISCVCut::WriteMain(const string & inDir ,
						   PointSet* p ,unsigned int trueCVcount , RGISCVCutDataIml * ioData)
{
	UPOINT * writePointBuffer = new UPOINT[65535];

	RStdioFile file;
	int i;
	int breakPoint = 0;
	for(i = 0 ;  i < trueCVcount-1 ; i++)
	{
		if (p[i].lastBlock != p[i + 1].lastBlock)
		{
			//前回と違うブロックに属している香具師を発見!

			//とりあえず、直前までの成果を書き込みます。
			ASSERT((i + 1 + 1 - breakPoint) <= 65535);
			int cvCount = i + 1 + 1 - breakPoint;		//途中で点を分割するので + 1 している.

			for(int n = breakPoint ; n < i+1 ; n++)
			{
				writePointBuffer[ n - breakPoint ] = p[n].po;
			}

			//で、今回の点のために分割点を求めます。
			UPOINT newPoint = getMidpoint( &p[i].po , &p[i + 1].po );
			writePointBuffer[ n - breakPoint ]  = newPoint;

			//書き込み.
			file.Open(inDir + num2str(p[breakPoint].lastBlock) , "a+b");
			PointWrite256( &file , p[breakPoint].lastBlock , writePointBuffer , cvCount , ioData);
//			ioData->OnWriteHeader(&file , p[breakPoint].lastBlock , writePointBuffer , cvCount);	//付属データの書き込み
//			file.fwrite( &cvCount , sizeof(cvCount) );	
//			PointWrite( &file, writePointBuffer , cvCount);

			//相手側の点からみた分割点を求めます。
			newPoint = getMidpoint( &p[i + 1].po , &p[i].po );	

			int de = selectBlock( &newPoint );
			ASSERT( de ==  p[i + 1].lastBlock);

			//隣り合わないブロックの場合は第3ブロックを経由しているわけでそれの処理.
			if (! isTexanRude(p[i].lastBlock , p[i + 1].lastBlock) )
			{
				PointSet start,goal;
				start = p[i];
				goal = p[i + 1];
				while(1)
				{
					PointSet therd[2];
					therd[0].po = getTexanRudeMidpoint(&start.po , &goal.po);
					therd[0].lastBlock = selectBlock( &therd[0].po );
					therd[1].po = getMidpoint(&therd[0].po , &goal.po);
					therd[1].lastBlock = selectBlock( &therd[0].po );

					//当然、こいつらは同一ブロックにいるわけで...
					ASSERT( therd[0].lastBlock == therd[1].lastBlock );
					//で、当然、[0] と p[i] は隣のブロックで
					ASSERT( isTexanRude(therd[0].lastBlock , start.lastBlock) );
					//もちろん、[1] と p[i + 1] は隣のブロック
//					ASSERT( isTexanRude(therd[1].lastBlock , goal.lastBlock) );

					//新しい点を書き込む.
					cvCount = 2;		//入るところと出るところね

					writePointBuffer[0] = therd[0].po;
					writePointBuffer[1] = therd[1].po;

					file.Open(inDir + num2str(therd[0].lastBlock) , "a+b");
					PointWrite256( &file , therd[0].lastBlock , writePointBuffer , cvCount , ioData);
//					ioData->OnWriteHeader(&file , therd[0].lastBlock , writePointBuffer , cvCount);	//付属データの書き込み
//					file.fwrite( &cvCount , sizeof(cvCount) );
//					PointWrite( &file, writePointBuffer , cvCount);

					//もちろん、[1] と p[i + 1] は隣のブロックになったらやめ.
					if (isTexanRude(therd[1].lastBlock , goal.lastBlock))
					{
						break;
					}
					//第 N のブロックを経由している長いやつはまだ処理を続ける.
					start = therd[0];
				}
			}

			//現在の po はもう書き込んだので破棄します.
			p[i].po = newPoint;
			p[i].lastBlock = p[i + 1].lastBlock;
				
			breakPoint = i ;
		}
	}
	//最後の一個
	if ( i != breakPoint)
	{
		int cvCount = i + 1 - breakPoint;
		for(int n = breakPoint ; n < i +1; n++)
		{
			writePointBuffer[ n - breakPoint ] = p[n].po;
		}

		file.Open(inDir + num2str(p[breakPoint].lastBlock) , "a+b");
		PointWrite256( &file , p[breakPoint].lastBlock , writePointBuffer , cvCount , ioData);
	}


	delete [] writePointBuffer;
}

void RGISCVCut::PointWrite256(RStdioFile *iofile , unsigned long inBlock , const UPOINT * inPtArray ,unsigned int inCount, RGISCVCutDataIml * ioData)
{
	UPOINT writePointBuffer[256 + 1];	

	writePointBuffer[0] = inPtArray[0];

	int writePoint = 1;
	for(int i = 1 ; i < inCount ; i++)
	{
		writePointBuffer[writePoint] = inPtArray[i];

		while(1)
		{
			if (abs(writePointBuffer[writePoint - 1].x - writePointBuffer[writePoint].x)<=127 && 
				abs(writePointBuffer[writePoint - 1].y - writePointBuffer[writePoint].y)<=127 )
			{
				break;
			}
			UPOINT t = LineHoseiChar(&writePointBuffer[writePoint - 1] ,  &writePointBuffer[writePoint]);
			ASSERT(abs(writePointBuffer[writePoint - 1].x - t.x)<=127 && 
				   abs(writePointBuffer[writePoint - 1].y - t.y)<=127 );

			//今の値を一つ横にずらして、保管した値を入れる補完を作ります.
			writePointBuffer[writePoint + 1] = writePointBuffer[writePoint];
			//補完あたいを入れます.
			writePointBuffer[writePoint] = t;
			//一つ進めます.
			writePoint++;

			if (writePoint >= 255)
			{
				unsigned char cvCount = 255;
				ioData->OnWriteHeader(iofile , inBlock , writePointBuffer , cvCount);	//付属データの書き込み
				iofile->fwrite( &cvCount , sizeof(cvCount) );	//最後まで含めるため + 1 している.
				PointWrite( iofile, writePointBuffer , cvCount);

				writePointBuffer[0] = writePointBuffer[writePoint - 1];
				writePointBuffer[1] = writePointBuffer[writePoint];
				writePoint = 1;
			}
		}
		writePoint++;
		if (writePoint >= 255)
		{
			unsigned char cvCount = 255;
			ioData->OnWriteHeader(iofile , inBlock , writePointBuffer , cvCount);	//付属データの書き込み
			iofile->fwrite( &cvCount , sizeof(cvCount) );	//最後まで含めるため + 1 している.
			PointWrite( iofile, writePointBuffer , cvCount);

			writePointBuffer[0] = writePointBuffer[writePoint - 1];
			writePointBuffer[1] = writePointBuffer[writePoint];
			writePoint = 1;
		}
	}

	//結果を書き込みます。
	unsigned char cvCount = writePoint ;
	ioData->OnWriteHeader(iofile, inBlock , writePointBuffer , cvCount);	//付属データの書き込み
	iofile->fwrite( &cvCount , sizeof(cvCount) );	//最後まで含めるため + 1 している.
	PointWrite( iofile, writePointBuffer , cvCount);
}

UPOINT RGISCVCut::LineHoseiChar(const UPOINT * inSrc , const UPOINT * inDest )
{
	UPOINT limit;
	if (inSrc->x > inDest->x)	limit.x = inSrc->x-127;
	else						limit.x = inSrc->x+127;
	if (inSrc->y > inDest->y)	limit.y = inSrc->y-127;
	else						limit.y = inSrc->y+127;

	UPOINT xy;
	if (inDest->y == inSrc->y)
	{
		//y 軸に等しい場合は別ルーチン.
		xy.y = inSrc->y;
		xy.x = limit.x;
	}
	else if (inDest->x == inSrc->x)
	{
		//x 軸に等しい場合は別ルーチン.
		xy.y = limit.y;
		xy.x = inSrc->x;
	}
	else
	{
		xy.y = (inDest->y - inSrc->y) * ( limit.x - inSrc->x) / (inDest->x - inSrc->x) + inSrc->y;
		xy.x = (limit.y - inSrc->y) * (inDest->x - inSrc->x)  / (inDest->y - inSrc->y) + inSrc->x;

		if (xy.y <= inSrc->y-127 || xy.y >= inSrc->y+127)
		{
			//xを採用.
			xy.y = limit.y;
		}
		else
		{
			//yを採用.
			xy.x = limit.x;
		}
	}
	ASSERT( (xy.y >= inSrc->y-127 && xy.y <= inSrc->y+127) && (xy.x >= inSrc->x-127 && xy.x <= inSrc->x+127) );

	return xy;
}

//点の書き込み
void RGISCVCut::Write(const string & inDir ,const UPOINT * inPoint,RGISCVCutDataIml * ioData)
{
	UPOINT writePointBuffer[1];
	unsigned long  block = selectBlock( inPoint );

	RStdioFile file;
	file.Open(inDir + num2str(block) , "a+b");
	writePointBuffer[0] = *inPoint;
	ioData->OnWriteHeader(&file , block , writePointBuffer , 1);	//付属データの書き込み
	PointWrite( &file, writePointBuffer , 1);
}


//点の書き込み
void RGISCVCut::Test2(const string & inDir ,unsigned long block , int inMeshKankaku)
{
	int oneLine = 65535/inMeshKankaku + 1;
	int feildSize = oneLine * oneLine;
	RMmap mem;


	mem.Create( inDir + num2str(block) , feildSize , OPEN_ALWAYS);
	unsigned char* mapMem = (unsigned char*)mem.getMemory();

		static no = 0;
		no ++;
		string out;
		out.reserve(6553500);
		out += "=======MH=============-\n";
		for (int y = 0 ; y < oneLine ; y++)
		{
			for (int x = 0 ; x < oneLine ; x++)
			{
				char buf[10];
				
				::sprintf(buf, "%2d" , (int)mapMem[x + y * oneLine] );
				out += (string) buf+ " ";
			}
			out += "\n";
		}
		out += "\n";
		RDebug::MemoryToFile("test/mh_log." + num2str(no) , out);
}


//点の書き込み
void RGISCVCut::PointWrite(RStdioFile *iofile , const UPOINT * inPtArray ,unsigned int inCount)
{
	unsigned short shortSize;
	shortSize = inPtArray[0].x % 65536 ; iofile->fwrite( &shortSize , sizeof(shortSize)  );
	shortSize = inPtArray[0].y % 65536 ; iofile->fwrite( &shortSize , sizeof(shortSize)  );

	char charSize;
	for(int i = 1; i < inCount ; i++)
	{
		ASSERT( abs((int)inPtArray[i].x - (int)inPtArray[i - 1].x) <= 127 );
		ASSERT( abs((int)inPtArray[i].y - (int)inPtArray[i - 1].y) <= 127 );

		charSize = (int)inPtArray[i].x - (int)inPtArray[i - 1].x;
		iofile->fwrite( &charSize , sizeof(charSize)  );
		charSize = (int)inPtArray[i].y - (int)inPtArray[i - 1].y;
		iofile->fwrite( &charSize , sizeof(charSize)  );
	}
}


//Src から Dest へ逝くときに境界線にぶつかるポイントの取得.
UPOINT RGISCVCut::getMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const
{
	BPOINT limit; 
	if (inSrc->x > inDest->x)	limit.x = 0;
	else						limit.x = 65535;
	if (inSrc->y > inDest->y)	limit.y = 0;
	else						limit.y = 65535;

	UPOINT scalUP;

	scalUP.x = (inSrc->x / 65536 * 65536);
	scalUP.y = (inSrc->y / 65536 * 65536);

	BPOINT src , dest;
	src.x = inSrc->x - scalUP.x;
	src.y = inSrc->y - scalUP.y;
	dest.x = inDest->x - scalUP.x;
	dest.y = inDest->y - scalUP.y;

	long x,y;
	if (inDest->y == inSrc->y)
	{
		//y 軸に等しい場合は別ルーチン.
		y = src.y;
		x = limit.x;
	}
	else if (inDest->x == inSrc->x)
	{
		//x 軸に等しい場合は別ルーチン.
		y = limit.y;
		x = src.x;
	}
	else
	{
		y = ((double)(dest.y - src.y) / (dest.x - src.x)) * (limit.x - src.x) + src.y;
//		x = (limit.y - src.y) / ((double)(dest.y - src.y) / (dest.x - src.x))  + src.x;
		x = (limit.y - src.y) * (double)(dest.x - src.x) / (dest.y - src.y) + src.x;

		if (y < 0 || y >= 65536)
		{
			//xを採用.
			y = limit.y;
		}
		else
		{
			//yを採用.
			x = limit.x;
		}
		ASSERT( (y >= 0 && y < 65536) && (x >= 0 && x < 65536) );
	}
	UPOINT ret;
	ret.x = x + scalUP.x;
	ret.y = y + scalUP.y;

	return ret;
}

//Src から Dest へ逝くときに境界線の次にあるブロックの取得
UPOINT RGISCVCut::getTexanRudeMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const
{
	BPOINT limit; 
	if (inSrc->x > inDest->x)	limit.x = -1;
	else						limit.x = 65536;
	if (inSrc->y > inDest->y)	limit.y = -1;
	else						limit.y = 65536;

	UPOINT scalUP;
	scalUP.x = (inSrc->x / 65536 * 65536);
	scalUP.y = (inSrc->y / 65536 * 65536);

	BPOINT src , dest;
	src.x = inSrc->x - scalUP.x;
	src.y = inSrc->y - scalUP.y;
	dest.x = inDest->x - scalUP.x;
	dest.y = inDest->y - scalUP.y;

	long x,y;
	if (inDest->y == inSrc->y)
	{
		//y 軸に等しい場合は別ルーチン.
		y = src.y;
		x = limit.x;
	}
	else if (inDest->x == inSrc->x)
	{
		//x 軸に等しい場合は別ルーチン.
		y = limit.y;
		x = src.x;
	}
	else
	{
		y = ((double)(dest.y - src.y) / (dest.x - src.x)) * (limit.x - src.x) + src.y;
//		x = (limit.y - src.y) / ((double)(dest.y - src.y) / (dest.x - src.x))  + src.x;
		x = (limit.y - src.y) * (double)(dest.x - src.x) / (dest.y - src.y) + src.x;

		if (y < 0 || y >= 65536)
		{
			//xを採用.
			y = limit.y;
		}
		else
		{
			//yを採用.
			x = limit.x;
		}
		ASSERT( (y >= -1 && y < 65537) && (x >= -1 && x < 65537) );
	}
	UPOINT ret;
	ret.x = x + scalUP.x;
	ret.y = y + scalUP.y;

	return ret;
}

//ブロックに分断
unsigned long RGISCVCut::selectBlock(const UPOINT * inPT) 
{
	return (inPT->y / 65536) * (NEXT_LINE_BLOCK) + (inPT->x /65536);
}

//隣り合うブロックか?
bool RGISCVCut::isTexanRude(unsigned long inA , unsigned long  inB) const
{
	if (inA-1 == inB) return true;
	if (inA+1 == inB) return true;
	if (inA-NEXT_LINE_BLOCK == inB) return true;
	if (inA+NEXT_LINE_BLOCK == inB) return true;

	return false;
}



UPOINT* RGISCVCut::DebugRead(const string & inDir ,unsigned short inBlock ,const string & inData  )
{
	RStdioFile file;
	file.Open(inDir + num2str(inBlock) , "r+b");

	//付属データの読み出し.
	char buffer[MAX_PATH];
	file.fread(buffer , inData.size() + 1);

	unsigned short cvCount;
	file.fread(&cvCount , sizeof(cvCount) );

	unsigned short sr;
	UPOINT * storePoint = new UPOINT[cvCount];

	for(int i = 0 ; i < cvCount ; i ++)
	{
		ASSERT(file.IsEof() == false );
		file.fread( &sr , sizeof(sr) );	storePoint[i].x = sr;
		ASSERT(file.IsEof() == false );
		file.fread( &sr , sizeof(sr) );	storePoint[i].y = sr;
	}

	return storePoint;
}

void RGISCVCut::test()
{

	{
		RGISCVCut cv;
		UPOINT src  = {50327201 , 4294967197};
		UPOINT dest = {50327791 , 4282104863};

		int xdiff = abs( dest.x - src.x );
		int ydiff = abs( dest.y - src.y );

		unsigned long x = dest.x ;
		unsigned long y = dest.y ;

		UPOINT xy = cv.LineHoseiChar(&src , &dest);
	}

	return ;
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 6357075;	src.y = 252968753;
		dest.x = 6356988;		dest.y = 252968970;
		UPOINT po = cv.getMidpoint(&src , &dest);

		ASSERT(po.x == 6356992 && po.y == 252968959);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 3275080;	src.y = 1012220;
		dest.x = 3282260;		dest.y = 1012220;
		int a = cv.selectBlock(&src);
		int b = cv.selectBlock(&dest);

		UPOINT po = cv.getMidpoint(&src , &dest);


		ASSERT(po.x == 3276799 && po.y == 1012220);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 5504870;	src.y = 1185930;
		dest.x = 5469540;		dest.y = 1114200;
		int a = cv.selectBlock(&src);
		int b = cv.selectBlock(&dest);

		UPOINT po = cv.getMidpoint(&src , &dest);


		ASSERT(po.x == 5501775 && po.y == 1179648);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 1834240;	src.y = 166750;
		dest.x = 1843600;		dest.y = 151950;
		int a = cv.selectBlock(&src);
		int b = cv.selectBlock(&dest);

		UPOINT po = cv.getMidpoint(&src , &dest);


		ASSERT(po.x == 1835007 && po.y == 165537);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 100000;	src.y = 100;
		dest.x = 10000;		dest.y = 100;
		UPOINT po = cv.getMidpoint(&src , &dest);

		ASSERT(po.x == 65536 && po.y == 100);
	}
	{
		RGISCVCut::PointSet src , dest;
		RGISCVCut cv;

		src.po.x = 5243183;		src.po.y = 252313284;
		dest.po.x = 5242777;	dest.po.y = 252313749;
		src.lastBlock = cv.selectBlock(&src.po);
		dest.lastBlock = cv.selectBlock(&dest.po);

		UPOINT po = cv.getMidpoint(&src.po , &dest.po);
		ASSERT(po.x == 5242907 && po.y == 252313599);

		//src と分割点は同一ブロック.
		ASSERT(src.lastBlock == cv.selectBlock(&po) );
		//src と dest はそもそも隣り合わない
		ASSERT( ! cv.isTexanRude(src.lastBlock , dest.lastBlock) );

		RGISCVCut::PointSet therd[2];
		therd[0].po = cv.getTexanRudeMidpoint(&src.po , &dest.po);
		therd[0].lastBlock = cv.selectBlock( &therd[0].po );
		therd[1].po = cv.getMidpoint(&therd[0].po , &dest.po);
		therd[1].lastBlock = cv.selectBlock( &therd[0].po );

		//当然、こいつらは同一ブロックにいるわけで...
		ASSERT( therd[0].lastBlock == therd[1].lastBlock );
		//で、当然、[0] と p[i] は隣のブロックで
		ASSERT( cv.isTexanRude(therd[0].lastBlock , src.lastBlock) );
		//もちろん、[1] と p[i + 1] は隣のブロック
		ASSERT( cv.isTexanRude(therd[1].lastBlock , dest.lastBlock) );
		//Y軸に突き抜けるので.
		ASSERT(therd[0].po.y == 252313599 + 1);
	}

	class RGISCVCutDataImlDummy  : public RGISCVCutDataIml
	{
	public:
		string WriteData;

		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			ioFile->fwrite(WriteData.c_str() , WriteData.size() + 1);
		}
	};

	try
	{
		RGISCVCut cv;
		RGISSlp slp;
		RGISSlm slm;
		GISINTArray array;
		slp.Create("test/cvtest.slp",&slm);
		array.Create("1,2,3,4,5");
		RDiskUtil::Delete("test/cv");
		RDiskUtil::MakeDirectory("test/cv");

		RGISCVCutDataImlDummy dummyData;
		dummyData.WriteData = "data";

		cv.Write( "test/cv/" , &slp , &array , &dummyData );

		char buffer[5];
		UPOINT* pointes = RGISCVCut::DebugRead("test/cv/" ,0 , dummyData.WriteData);
//		ASSERT(data == buffer);
		ASSERT(pointes[0].x == 10);
		ASSERT(pointes[0].y == 100);
		ASSERT(pointes[1].x == 100);
		ASSERT(pointes[1].y == 100);
		ASSERT(pointes[2].x == 1000);
		ASSERT(pointes[2].y == 100);
		ASSERT(pointes[3].x == 10000);
		ASSERT(pointes[3].y == 100);
		ASSERT(pointes[4].x == 65535);
		ASSERT(pointes[4].y == 100);
		delete [] pointes;

		pointes = RGISCVCut::DebugRead("test/cv/" ,1 , dummyData.WriteData );
//		ASSERT(data == buffer);
		ASSERT(pointes[0].x == 0);
		ASSERT(pointes[0].y == 100);
		ASSERT(pointes[1].x == 34464);
		ASSERT(pointes[1].y == 100);
		delete [] pointes;
	}
	catch(RException e)
	{
		ASSERT(0);
	}
}



//メッシュ間隔
const int MESH_INTERVAL	=	256;
//一ラインにいくつメッシュがあるか?
const int MESH_LINE		=	(65536/MESH_INTERVAL);

RGISCVCutMesh::RGISCVCutMesh()
{
	this->LastBlock = ULONG_MAX;
}

RGISCVCutMesh::~RGISCVCutMesh()
{
	Mem.Flush(MESH_LINE  * MESH_LINE);
}

//点の書き込み
void RGISCVCutMesh::Write(const string & inDir  , const UPOINT & inPoint,unsigned char inLevel)
{
	ASSERT( (65536%MESH_INTERVAL) == 0);


	unsigned long  block = RGISCVCut::selectBlock( &inPoint );

	//前回と違うブロックだったら、
	//現在のブロックの内容をセーブし、新しいブロックを開く.
	if (block != this->LastBlock)
	{
		if (this->LastBlock != ULONG_MAX)
		{
			Mem.Flush(MESH_LINE  * MESH_LINE);
			Mem.Release();
		}

		Mem.Create( inDir + num2str(block) , MESH_LINE * MESH_LINE , OPEN_ALWAYS);
		this->LastBlock = block;
	}

	unsigned char* mapMem = (unsigned char*)Mem.getMemory();

	int x = inPoint.x % 65536 / MESH_INTERVAL;
	int y = inPoint.y % 65536 / MESH_INTERVAL;
	int targetPos =	( x) + ( y  * MESH_LINE );
	ASSERT(targetPos < (MESH_LINE * MESH_LINE) );

	mapMem[targetPos] = inLevel;

	//塗りのこしがないように、周りの領域で塗られていないところがあったら塗る.
	if (x > 0 )
	{
		targetPos = ( (x-1) ) + ( y * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}
	if (x < (MESH_LINE-1) )
	{
		targetPos = ( (x+1) ) + ( y * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}
	if (y > 0 )
	{
		targetPos = ( x ) + ( (y-1) * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}
	if (y < (MESH_LINE-1) )
	{
		targetPos = ( x ) + ( (y+1) * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}

	
}
