// BAddressMatching.cpp: BAddressMatching ƒNƒ‰ƒX‚ÌƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BAddressMatching.h"
#include "RConv.h"
#include "RStdioFile.h"
#include "RFileUtil.h"
#include "BConf.h"
#include "BBanana.h"



	//“s“¹•{Œ§ƒ}ƒbƒ`ƒ“ƒOƒe[ƒuƒ‹‚È‚Ì‚Å‚·‚æ[.
	static const char* Todoufuken[] = {
		"–kŠC“¹",
		"ÂXŒ§",
		"ŠâèŒ§",
		"‹{éŒ§",
		"H“cŒ§",
		"RŒ`Œ§",
		"•Ÿ“‡Œ§",
		"ˆïéŒ§",
		"“È–ØŒ§",
		"ŒQ”nŒ§",
		"é‹ÊŒ§",
		"ç—tŒ§",
		"“Œ‹“s",
		"_“ŞìŒ§",
		"VŠƒŒ§",
		"•xRŒ§",
		"ÎìŒ§",
		"•ŸˆäŒ§",
		"R—œŒ§",
		"’·–ìŒ§",
		"Šò•ŒŒ§",
		"Ã‰ªŒ§",
		"ˆ¤’mŒ§",
		"OdŒ§",
		" ‰êŒ§",
		"‹“s•{",
		"‘åã•{",
		"•ºŒÉŒ§",
		"“Ş—ÇŒ§",
		"˜a‰ÌRŒ§",
		"’¹æŒ§",
		"“‡ªŒ§",
		"‰ªRŒ§",
		"L“‡Œ§",
		"RŒûŒ§",
		"“¿“‡Œ§",
		"ìŒ§",
		"ˆ¤•QŒ§",
		"‚’mŒ§",
		"•Ÿ‰ªŒ§",
		"²‰êŒ§",
		"’·èŒ§",
		"ŒF–{Œ§",
		"‘å•ªŒ§",
		"‹{èŒ§",
		"­™“‡Œ§",
		"‰«“êŒ§",
	};


//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
//////////////////////////////////////////////////////////////////////


//ƒAƒhƒŒƒX‚Éƒ}ƒbƒ`‚·‚éƒR[ƒh‚ğæ“¾‚·‚é.
const AddressToCodeMatchingTable* BAddressParse::ToCode(const string & inAddress , const  AddressToCodeMatchingTable* inMatchTable , int inTableSize) const
{
	ASSERT(inMatchTable != NULL);

	int addresSize = inAddress.size();

	for(int i = 0; i < inTableSize ; i++)
	{
		const AddressToCodeMatchingTable * table = &inMatchTable[i];

		int size = addresSize < (int) table->Address.size() ? addresSize : table->Address.size();
		if ( memcmp(inAddress.c_str() , table->Address.c_str() , size) == 0)
		{
			return table;
		}
	}

	//ƒ}ƒbƒ`‚µ‚Ü‚¹‚ñ‚Å‚µ‚½.
	return NULL;
}

bool BAddressParse::Parse(const string &inAddress ,  const  AddressToCodeMatchingTable* inMatchTable , int inTableSize)
{
	if (inAddress.empty() ) return false;

	int skip = SkipTodoufuKen(inAddress);
	string ad = inAddress.c_str() + skip;

	//“s“¹•{Œ§‚¾‚¯‚ÅŒŸõ‚µ‚½?
	if (ad.empty() )
	{
		return false;
	}

	//ƒR[ƒhƒ}ƒbƒ`ƒ“ƒO.
	const AddressToCodeMatchingTable* codeTable = ToCode( ad ,inMatchTable , inTableSize);
	if (codeTable == NULL)
	{
		return false;	//¸”s.
	}

	this->Code = codeTable->Code;

	//ƒ}ƒbƒ`‚µ‚½‚Ì‚ÅAŸ‚Í”Ô’n‚Ìæ“¾‚És‚«‚Ü‚·B
	//c‚Á‚½‚Æ‚±‚ë‚ª MachiCyoume ‚È‚í‚¯.

	//ƒR[ƒh‚Ì•”•ª‚Ì“Ç‚İ”ò‚Î‚µ.
	ad = ad.c_str() + codeTable->Address.size();
	if ( ad.empty() )
	{
		return false;	//¸”s.
	}

	skip = 0;
	const char * ban;
	do
	{
		//’š–Ú‚Æ‚¢‚¤w¦‚ª–¾Šm‚É‚ ‚éê‡...
		if ( ( ban = getChyoume(ad.c_str() , "’š–Ú" , &skip) ) != NULL) break;

		//ˆê-ˆê-ˆê ‚Æ‚¢‚¤•û®
		if ( ( ban = getChyoume(ad.c_str() , "-" , &skip) ) != NULL) break;

		//ˆê|ˆê|ˆê ‚Æ‚¢‚¤•û®
		if ( ( ban = getChyoume(ad.c_str() , "|" , &skip) ) != NULL) break;

		//ˆê‚Ìˆê‚Ìˆê ‚Æ‚¢‚¤•û®
		if ( ( ban = getChyoume(ad.c_str() , "‚Ì" , &skip) ) != NULL) break;

		//ˆêƒmˆêƒmˆê ‚Æ‚¢‚¤•û®
		if ( ( ban = getChyoume(ad.c_str() , "ƒm" , &skip) ) != NULL) break;

	}
	while(0);	//ƒ_ƒ~[ƒ‹[ƒv.


	string r;
	if (ban != NULL) 
	{
		//’š–Ú‚ª‚ ‚Á‚½‚Ì‚Å‚»‚±‚Ü‚Å.
		r = string(ad.c_str() , 0 , ban - ad.c_str()  );
		r += "’š–Ú";
	}
	else if (( ban = getChyoume(ad.c_str() , &skip) ) != NULL) 
	{
		//’š–Ú‚ª‚È‚¢ê‡ 2ƒoƒCƒgˆÈŠO‚Ì•¶š—ñ‚ªo‚Ä‚­‚é‚Ü‚Å.
		r = string(ad.c_str() , 0 , ban - ad.c_str()  );
	}
	else
	{
		//‚»‚ê‚·‚ç‚à‚È‚¢ê‡‚Í”F¯‚Å‚«‚½‚Æ‚±‚ë‚Ü‚Å
		this->MachiCyoume = ad ;
		this->Banchi = 0;

		return true;
	}

	this->MachiCyoume = RConv::atok(r);
	r = RConv::atos(ban + skip);
	this->Banchi = atoi( r.c_str() );

	return true;
}

//’š–Ú‚Æ‚¢‚¤w¦‚ª–¾Šm‚É‚ ‚éê‡...
const char * BAddressParse::getChyoume(const char * inStr,const string inPrefix , int *outSkip) const
{
	const char * p;
	p = strstr(inStr , inPrefix.c_str() );

	*outSkip = inPrefix.size();

	return p;
}
const char * BAddressParse::getChyoume(const char * inStr, int *outSkip) const
{
	const char * p;
	p = FindNot2Byte(inStr );

	*outSkip = 0;

	return p;
}

const char * BAddressParse::FindNot2Byte(const char *inStr) const
{
	int count = strlen(inStr) ;
	if (count <= 1) return NULL;

	for (int i = 0 ; i < count ; i++)
	{
		if ( RConv::isKanji( (unsigned char)inStr[i] , (unsigned char)inStr[i + 1] ) )
		{
			i++;
		}
		else
		{
			return inStr + i;
		}
	}
	return NULL;
}

//“s“¹•{Œ§‚ª‚ ‚Á‚½‚ç“Ç‚İ”ò‚Î‚µ.
int BAddressParse::SkipTodoufuKen(const string & inAddress) const
{

	const char * mas = inAddress.c_str();


	for(int i = 0 ; i < TABLE_COUNT_OF(Todoufuken) ; i++)
	{
		//æ‚É—ë‚É‚È‚Á‚½‚Ù‚¤‚ª•‰‚¯.
		const char * p = mas;
		const char * n = Todoufuken[i];
		for( ; *n && *p && *n==*p; n ++ , p++)	
			;
		if (*n == '\0')	return p - mas;
	}
	//“s“¹•{Œ§‚Í‚Â‚¢‚Ä‚È‚¢‚ç‚µ‚¢.
	return 0;
} 


void BAddressParse::test()
{
	AddressToCodeMatchingTable table[4];
	table[0].Address = "ìèsìè‹æ";
	table[0].Code = 14131;
	table[1].Address = "D–ys’†‰›‹æ";
	table[1].Code = 1101;
	table[2].Address = "D–ys–k‹æ";
	table[2].Code = 1102;
	table[3].Address = "D–ys“Œ‹æ";
	table[3].Code = 1103;

	{
		BAddressParse parse;

		parse.Parse("_“ŞìŒ§ìèsìè‹æ–ì–]‰¤‘1192", table , 4);
		ASSERT( parse.getCode() == 14131 );
		ASSERT( parse.getMachiCyoume() == "–ì–]‰¤‘" );
		ASSERT( parse.getBanchi() == 1192 );
	}
	{
		BAddressParse parse;

		parse.Parse("–kŠC“¹D–ys’†‰›‹æˆ®ƒP‹u123", table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "ˆ®ƒP‹u" );
		ASSERT( parse.getBanchi() == 123 );
	}
	{
		BAddressParse parse;

		parse.Parse("–kŠC“¹D–ys’†‰›‹æˆ®ƒP‹uˆê’š–Ú1”Ô", table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "ˆ®ƒP‹uˆê’š–Ú" );
		ASSERT( parse.getBanchi() == 1 );
	}
	{
		BAddressParse parse;

		parse.Parse("–kŠC“¹D–ys’†‰›‹æˆ®ƒP‹uˆê’š–Ú" , table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "ˆ®ƒP‹uˆê’š–Ú" );
		ASSERT( parse.getBanchi() == 0 );
	}
}

