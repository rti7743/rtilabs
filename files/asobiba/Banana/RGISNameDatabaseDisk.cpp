// RGISNameDatabaseDisk.cpp: RGISNameDatabaseDisk ƒNƒ‰ƒX‚ÌƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“
//
//////////////////////////////////////////////////////////////////////

#include "RGISNameDatabaseDisk.h"
#include "RStdioFile.h"
#include "RDiskUtil.h"


//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
//////////////////////////////////////////////////////////////////////

RGISNameDatabaseDisk::RGISNameDatabaseDisk()
{

}

RGISNameDatabaseDisk::~RGISNameDatabaseDisk()
{

}

unsigned short RGISNameDatabaseDisk::Write(const string & inFilename , const string & inName) throw (RException)
{
	RStdioFile file;

	//•¶Žš—ñ‚ÍA \0 ‹æØ‚è‚Å“o˜^‚³‚ê‚Ä‚¢‚Ü‚·B
	file.Open(inFilename , "a+b");

	long size = file.getSize();
	char* buffer = new char[size + 1];

	int line = 0;
	if (size > 0)
	{
		file.fseek(0);	//æ“ª‚É.
		file.fread(buffer , size );	//‘S•”“Ç‚Ý...

		//‚ÅA inName ‚Á‚Ä“o˜^‚³‚ê‚Ä‚¢‚Ü‚Á‚·‚é?
		int addpoint = 0;
		for(line = 0 ;  ; line++)
		{
			const char * name = buffer + addpoint ;

			if ( strcmp(inName.c_str() , name) == 0)
			{
				delete [] buffer ;
				//“¯‚¶•¶Žš—ñ‚ª‚ ‚Á‚½‚Ì‚Å‚»‚¢‚Â‚ð‹¤—L‚·‚é‚Ì‚Å‚·B
				return line;
			}
			//ŽŸ‚Ì•¶Žš‚Ö.
			addpoint += strlen(name) + 1;	//+1‚Í \0‚Ì•ª.
			if (addpoint >= size)
			{
				line ++;
				break;	//‚±‚êˆÈã‚È‚¢‚Ì‚Å‚·B
			}
		}
	}
	//‚¤[‚ñA“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚æ‚¤‚Å‚·‚ËB
	//‚Å‚ÍAV‹K“o˜^‚Æ‚¢‚¤‚±‚Æ‚Å...

	file.fseek(0,SEEK_END);	//ÅŒã•”‚É.
	file.fwrite(inName.c_str() , inName.size() + 1);

	delete [] buffer ;

	if (line > 0xffff)
	{
		throw RException(EXCEPTIONTRACE + "•¶Žš—ñ‚ª 65535 ‚ð’´‚¦‚Ü‚µ‚½");
	}
	return line ;
}

void RGISNameDatabaseDisk::test()
{
	string file = "test/namedatabasedisk.test";
	
	try
	{
		RDiskUtil::Delete(file);
	}
	catch(RException e)
	{
	}

	RGISNameDatabaseDisk disk;
	int r;
	r = disk.Write(file , "‚ ‚É‚¿‚á‚Ü");
	ASSERT(r == 0);
	r = disk.Write(file , "‚ ‚É‚¿‚á‚Ü");
	ASSERT(r == 0);
	r = disk.Write(file , "‚ ‚É‚¬‚Ý‚³‚Ü");
	ASSERT(r == 1);
	r = disk.Write(file , "‚ ‚É‚¬‚Ý‚³‚Ü");
	ASSERT(r == 1);
	r = disk.Write(file , "‚¨‚É‚¡‚¿‚á‚Ü");
	ASSERT(r == 2);
	r = disk.Write(file , "‚ ‚É‚¿‚á‚Ü");
	ASSERT(r == 0);
}
