// RGISSuiikiKai.cpp: RGISSuiikiKai クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSuiikiKai.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RDiskUtil.h"
#include "RGISCVCut.h"
#include "RGISCvLight.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RGISSuiikiKai::RGISSuiikiKai()
{
	Count = 0;
	Points= NULL;

}

RGISSuiikiKai::~RGISSuiikiKai()
{
	Delete();
}


void RGISSuiikiKai::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISSuiikiKai::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "SK(ID{SK%d})");
	if (this->Count <= 0) return ;

	this->Points = new SuiikiKaiData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	int cvID;
	char cvBuffer[10240];

	string buffer;
	unsigned int i;
	//未使用フラグを立てる.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		cvBuffer[0] = '\0';

		int id;
		int rsize = sscanf(buffer.c_str() , "SK(ID{SK%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1から始まるため.
		if (id >= (int)Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の "  + num2str(i) + "行目のIDが変です!");
		}
		SuiikiKaiData* p = &Points[id];

		const char * toOption = strstr(buffer.c_str() , "SR{");
		rsize = sscanf( toOption , "SR{%2s}",
				p->SR.ptr()  );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		const char * cvStart = strstr(buffer.c_str() , "CV(");
		if (cvStart == NULL)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のデータの形式が辺です!");
		}
		rsize = sscanf(cvStart , "CV(ID{CV%d}){%10240[^}]}",				&cvID , cvBuffer);
		if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		p->CV.Create(cvBuffer);
		p->Use = true;
	}
}


void RGISSuiikiKai::Draw(const RGISDraw * inDraw) const
{
}

void RGISSuiikiKai::SmallPack(const string & inDir,const RGISSlp * inSlp,
						  const RGISNameDatabase* inNameData)  const throw(RException)
{
	string dir = inDir + "SK/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NX/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Writeの中から呼びだれる頂点データのヘッダーを書き込むルーチン.
	class RGISCVCutSuiikiKaiData  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//名前データベース
		string Dir;								//名前ファイルを作成する場所.

	public:

		unsigned char SR;				//状態				1

		RGISCVCutSuiikiKaiData(const RGISNameDatabase* inNameData , const string inDir)
		{
			this->NameData = inNameData;
			this->Dir = inDir;
		}

		//これが呼び出される.
		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			//main
			ioFile->fwrite(&SR , sizeof(SR));
		}
	};

	RGISCVCutSuiikiKaiData	data(inNameData , namedir);
	RGISCvLight light;
	RGISCVCut cut;
	for(unsigned int n = 0 ; n < Count ; n++)
	{
		const SuiikiKaiData* p = &Points[n];
		if (p->Use)	
		{
			data.SR =  p->SR.OnePack();

			GISINTArray lightCV;
			if ( light.DoLight( inSlp , &p->CV , &lightCV) )
			{
				cut.Write(dir , inSlp , &lightCV , &data );
			}
			else
			{
				cut.Write(dir , inSlp , &p->CV , &data );
			}
		}
	}
}

JoinTempFormatScript RGISSuiikiKai::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "SK";
	ret.BlockStartSimbol = 'S';
	ret.DataSize = sizeof(unsigned char);	//SR
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeMultiPoint;
	ret.WithNode = false;

	return ret;
}

void RGISSuiikiKai::test()
{
	try
	{
		//国土地理院の了承がないとテストデータでも侵害になるかもしれないので、
		//テストルーチンは動かしません
		/*
		RGISSuiikiKai dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.sk",&ndb);

		unsigned long r;
		const SuiikiKaiData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 2);
		rr = dk.getPoint(0);

		ASSERT( (r = rr->CV.getPoint(0)) == 18541);
		ASSERT( (r = rr->CV.getPoint(1)) == 18539);
		ASSERT( (r = rr->CV.getPoint(2)) == 18538);
		ASSERT( (rrr = rr->SR) == "51");
		*/
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
