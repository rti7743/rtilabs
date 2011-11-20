// RGIS25000.cpp: RGIS25000 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGIS25000.h"
#include "RStdioFile.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGIS25000::RGIS25000()
{

}

RGIS25000::~RGIS25000()
{

}

void RGIS25000::SmallPack(const string inPath,const string inTo,const string inNumString,
						  MessageGIS2500Info * gis2500Info, RGISNameDatabase * ioNameDatabase , int inScale)
{
	printf(" 25000:read /");

	//読み込み
	string path = inPath + "/25000/" + inNumString + "/" + inNumString;

	Slm.Create(path + ".slm" , inScale);
	Slp.Create(path + ".slp" , &Slm);
	Slp.ConvertDisplayScal();


	try
	{
		Douro.Create(path + "DK.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Tetudo.Create(path + "TK.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Kawa.Create(path + "KK.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		SuiikiKai.Create(path + "SK.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Eki.Create(path + "EK.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Gyousei.Create(path + "GK.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Chimei.Create(path + "CM.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Hashi.Create(path + "HA.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Tonneru.Create(path + "TO.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Koukyou.Create(path + "KO.sal" , ioNameDatabase);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		DouroNode.Create(path + "DS.sal");
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	try
	{
		Koudo.Create(path + "MH.slp" , path + "MH.sal" , &Slm);
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}
	try
	{
		GyoseiD.Create(path + "GD.sal" , atoi( inNumString.c_str() ) );
		printf(".");
	}
	catch(RIOCannotOpenException e)
	{
		TRACE(e.getMessage() );
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		throw;
	}

	printf(" write /");

	//書き込み
	path = inTo + "/temp/"  ;

	Douro.SmallPack(path, &Slp , ioNameDatabase , &Gyousei , &DouroNode ,  &Hashi , &Tonneru);
	printf(".");

	Tetudo.SmallPack(path, &Slp , ioNameDatabase , &Eki ,&FindHintEki, &Hashi , &Tonneru);
	printf(".");

	Kawa.SmallPack(path, &Slp , ioNameDatabase);
	printf(".");

	SuiikiKai.SmallPack(path, &Slp, ioNameDatabase);
	printf(".");

	Chimei.SmallPack(path, &Slp , ioNameDatabase);
	printf(".");

	Koukyou.SmallPack(path, &Slp , ioNameDatabase);
	printf(".");

	Koudo.SmallPack(path);
	printf(".");

	GyoseiD.SmallPack(path , &FindCityToNumberConvert );
	printf(".");

//	Gyousei.SmallPack(path , &Slp);

	FindHintEki.Write(path);
	printf(".");

	FindCityToNumberConvert.Write(path);
	printf(".");


	printf("\n");
}

//結合スクリプトの取得.
int RGIS25000::getJoinScript(JoinTempFormatScript * ioScripts , int inSize) const
{
	ASSERT(inSize >= 7);
	ioScripts[0] = Douro.getPackInfo();
	ioScripts[1] = Tetudo.getPackInfo();
	ioScripts[2] = Kawa.getPackInfo();
	ioScripts[3] = SuiikiKai.getPackInfo();
	ioScripts[4] = Koukyou.getPackInfo();
	ioScripts[5] = Chimei.getPackInfo();
	ioScripts[6] = Koudo.getPackInfo();
	return 7;
}

//結合スクリプトの取得.
int RGIS25000::getJoinHeadScript(JoinHeadFormatScript * ioHeadScripts , int inSize) const
{
	ASSERT(inSize >= 2);
	ioHeadScripts[0] = FindHintEki.getPackInfo();
	ioHeadScripts[1] = FindCityToNumberConvert.getPackInfo();
	return 2;
}

string RGIS25000::GetName(const char * inBuffer) throw(RException)
{

	const char * nmStart = strstr(inBuffer , "NM{");
	if (nmStart == NULL)
	{
		throw RException(EXCEPTIONTRACE + "NM{ が見つかりません");
	}

	const char * p = nmStart;

	const char * nmEnd;
	while( 1 )
	{
		nmEnd = strstr(p, "}");
		if (nmEnd == NULL)
		{
			throw RException(EXCEPTIONTRACE + "} が見つかりません");
		}
		unsigned char c = *(nmEnd + 1);
		if (c >= 'A' && c <= 'Z')
		{
			break;
		}

		p = nmEnd + 1;
	}

	return string(nmStart , 3 , nmEnd - nmStart - 3);
}


int RGIS25000::GetMaxID(const string &inFilename , const char *idParse) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "rb");

	string buffer;
	int id;
	int maxID = 0;
	for(int i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();

		int rsize = sscanf(buffer.c_str() , idParse,	&id );
		if (rsize == 1)
		{
			if (maxID < id) maxID = id;
		}
	}
	return maxID;
}



