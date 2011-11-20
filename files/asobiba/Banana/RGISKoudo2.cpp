//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

#include "RGISKoudo2.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RDiskUtil.h"
#include "RGISCVCut.h"
#include "RGISNameDatabaseDisk.h"
#include "RGISCvLight.h"
#include "RDebug.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////




RGISKoudo2::RGISKoudo2()
{
	Count = 0;
	Points= NULL;

}

RGISKoudo2::~RGISKoudo2()
{
	Delete();
}


void RGISKoudo2::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISKoudo2::Create(const string & inSlpFilename,const string & inMHFilename,const RGISSlm * inMeshSlm) throw(RException)
{
	ASSERT(Points == NULL);

	//メッシュの slpの読み込み.
	MeshSlp.Create(inSlpFilename , inMeshSlm);
	MeshSlp.ConvertDisplayScal();


	this->Count = RGIS25000::GetMaxID(inMHFilename , "MH(ID{MH%d})");
	if (this->Count <= 0) return ;

	this->Points = new KoudoData2[this->Count];

	RStdioFile file;
	file.Open(inMHFilename , "rb");

	FILE * fp = file.getHandle();

	string buffer;
	int i;
	//未使用フラグを立てる.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		int id;
		int rsize = sscanf(buffer.c_str() , "MH(ID{MH%6d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inMHFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1から始まるため.
		if (id < 0)
		{
			throw RException(EXCEPTIONTRACE + inMHFilename + "の " + num2str(i) + "行目のIDが変です!");
		}
		KoudoData2* p = &Points[id];

		int dummy;
		unsigned int hk;
		unsigned int pm;
		const char * hkOption = strstr(buffer.c_str() , "HK{");
		rsize = sscanf(hkOption  , "HK{%d}",	 &hk  );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inMHFilename.c_str() , rsize , buffer.c_str()  ) );

		const char * ptStart = strstr(buffer.c_str() , "PT(");
		if (ptStart == NULL)
		{
			throw RException(inMHFilename + "の " + num2str(i) + "行目のデータのCV形式が辺です!");
		}
		rsize = sscanf(ptStart , "PT(ID{PM%d}){%6d}",	&dummy , &pm);
		if (rsize != 2) continue;	//PTがないデータは無視

		ASSERT(p->Use == false);

		p->Level = selectKoudoLevel(hk);
		p->Number = pm;
		p->Use = true;
	}
}

//標高をレベルに変換.
unsigned char RGISKoudo2::selectKoudoLevel(int inKM)
{
	if (inKM <   50) return 1;
	if (inKM <  200) return 2;
	if (inKM <  400) return 3;
	if (inKM <  600) return 4;
	if (inKM <  800) return 5;
	if (inKM < 1000) return 6;
	if (inKM < 1500) return 7;
	if (inKM < 2000) return 8;
	if (inKM < 2500) return 9;
	if (inKM < 3000) return 10;
	if (inKM < 4000) return 11;
	if (inKM < 5000) return 12;
	return 13;
}

void RGISKoudo2::Draw(const RGISDraw * inDraw) const
{
}

COLORREF RGISKoudo2::selectColor(GISType* inShubetsu) const
{
	return 0;
}
COLORREF RGISKoudo2::selectSize(GISType* inShubetsu) const
{
	return 0;
}

void RGISKoudo2::SmallPack(const string & inDir )  const throw(RException)
{
//	return ;

	string dir = inDir + "MH/";
	RDiskUtil::MakeDirectory(dir);

	RGISCVCutMesh cutMesh;
	for(int n = 0 ; n < Count ; n++)
	{
		const KoudoData2* p = &Points[n];

		ASSERT(p->Use);
		ASSERT(p->Level > 0);
		cutMesh.Write(dir , MeshSlp.getPoint(p->Number - 1) , p->Level );
	}
}

JoinTempFormatScript RGISKoudo2::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "MH";
	ret.BlockStartSimbol = 'H';
	ret.DataSize = 0;
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeByteData;
	ret.WithNode = false;

	return ret;
}


void RGISKoudo2::test()
{
/*
	RGISSlm slm;
	RGISKoudo mh;
	slm.Create("test/01101.slm" , 1);
	mh.Create("test/test.mh.slp","test/test.mh",&slm);
*/
}
