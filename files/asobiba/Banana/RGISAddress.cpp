// RGISAddress.cpp: RGISAddress クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISAddress.h"
#include "RStdioFile.h"
#include "Sec2Utm.h"
#include "RFileUtil.h"
#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RGISNameDatabaseDisk.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISAddress::RGISAddress()
{
	Count = 0;
	Points= NULL;
}

RGISAddress::~RGISAddress()
{
	Delete();
}

void RGISAddress::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISAddress::Create(const string & inFilename,int inCode , RGISNameDatabase * ioNameDatabase , int inScale) throw(RException)
{
	ASSERT(Points == NULL);

	if (! RDiskUtil::Exist( inFilename ) )
	{
		TRACE(inFilename + "はないので省略します");
		return ;
	}

	RStdioFile file;
	file.Open(inFilename , "rb");

	string buffer;
	buffer = file.ReadLine();

	this->Count = RFileUtil::LineCount(inFilename) - 2;
	if (this->Count <= 0) return ;

	//要素の総数の取得.
	this->Points = new AddressData[this->Count];

	char TodoufukenBuffer[1024];
	char CityBuffer[1024];
	char MachiBuffer[1024];
	double banshi;	//番地が 小数点で入っているやつがある.. 12402JGD---> 千葉県の山武郡大網白里町の清名幸谷の (地図データのコピペはイカンので削除)
	int dummy;
	double dummyF;
	double x,y;

	unsigned int i;
	//未使用フラグを立てる.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (buffer == "")
		{
			break;
		}

		//読み込み.
		AddressData* p = &Points[i];
		int rsize = sscanf(buffer.c_str() , 
			"%1024[^,],%1024[^,],%1024[^,],%lf,%d,%lf,%lf,%lf,%lf,%d,%d" , 
			TodoufukenBuffer,CityBuffer,MachiBuffer,&banshi,&dummy,&dummyF,&dummyF,&y,&x,&dummy,&dummy);
		p->Banchi = (int) banshi;

		if (rsize != 11)
		{
			if (rsize >= 1)
			{
				if ( noQoute(TodoufukenBuffer) == "都道府県名" )
				{
					continue;
				}
			}
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str() ) );
		}

//		p->Todoufuken = noQoute(TodoufukenBuffer);
//		p->City = noQoute(CityBuffer);
		p->Machi = noQoute(MachiBuffer);
		
		try
		{
			p->NameIndex = ioNameDatabase->setName(p->Machi.c_str() );
		}
		catch(RException e)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("setNameが失敗しました %s %d行:\n" ,inFilename.c_str() , i) + e.getMessage() );
		}
		p->Code = inCode;

		RGISKeiidoData xDeg,yDeg;
		xDeg.setDouble(x);
		yDeg.setDouble(y);


		RGISMathZantei zantei;
		zantei.Create(inScale);

		UPOINT xy = zantei.RevKukan25000ToKeiido( &xDeg , &yDeg );
		p->xy =     zantei.ConvertScale(xy.x , xy.y);

		p->Use = true;
//		TRACE("%d Address XY , %I64u %I64u \n" , i, p->xy.x , p->xy.y);

	}
}

// " " でくくられている単語のククリをはずします。
const string RGISAddress::noQoute(const string inStr)
{
	int size = inStr.size() - 2;
	if (size <= 0) return "";

	return inStr.substr(1 , size );
}

//スケールを変更し、ディスプレイ座標にします。
void RGISAddress::ConvertDisplayScal(int inShift) throw(RException)
{
	int count = this->Count;
	for(int i = 0 ; i < count ; i++)
	{
		this->Points[i].xy.x = RGISMathZantei::ConvertDisplayScalX( (unsigned long) (this->Points[i].xy.x) );
		this->Points[i].xy.y = RGISMathZantei::ConvertDisplayScalY( (unsigned long) (this->Points[i].xy.y) );
	}
}


void RGISAddress::Draw(const RGISDraw * inDraw ,const RGISNameDatabase * ioNameDatabase) const
{
	int count = this->Count;
	for(int i = 0 ; i < count ; i++)
	{
		if (this->Points[i].Use)
			inDraw->TexOut( &this->Points[i].xy , ioNameDatabase->getName( this->Points[i].NameIndex ) , 1, RGB(255,0,0) );
	}
}

void RGISAddress::SmallPack(const string & inDir, const RGISNameDatabase* inNameData, RGISFindHintAddress * ioFindHintAddress)  const throw(RException)
{
	string dir = inDir + "AD/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NA/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Writeの中から呼びだれる頂点データのヘッダーを書き込むルーチン.
	class RGISCVCutAddressData  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//名前データベース
		string Dir;								//名前ファイルを作成する場所.

	public:

		unsigned short Code;			//01010 のような町コード.				2
		unsigned short NameIndex;		//このマップの中手の名前番号			2
		unsigned short Banchi;			//番地									1

		RGISCVCutAddressData(const RGISNameDatabase* inNameData , const string inDir)
		{
			this->NameData = inNameData;
			this->Dir = inDir;
		}

		//これが呼び出される.
		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			//所定の場所に名前をスライド.
			RGISNameDatabaseDisk nameDataBase;
			unsigned short newNameIndex = 
				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->NameIndex) );

			//main
			ioFile->fwrite(&Code , sizeof(Code));
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));
			//sub
			ioFile->fwrite(&Banchi , sizeof(Banchi));
		}
	};

	RGISCVCutAddressData	data(inNameData , namedir);

	RGISCVCut cut;
	for(unsigned int n = 0 ; n < Count ; n++)
	{
		const AddressData* p = &Points[n];
		if (p->Use)
		{
			data.Code =  p->Code;
			data.NameIndex = (unsigned short)p->NameIndex;
			data.Banchi = p->Banchi;
			cut.Write(dir , &p->xy , &data );

			//住所の検索ヒントを作成
			{
				unsigned long block = cut.selectBlock( &p->xy ) ;
				ioFindHintAddress->Create(block , p->Code);
			}

		}
	}
}

JoinTempFormatScript RGISAddress::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "AD";
	ret.BlockStartSimbol = 'A';
	ret.DataSize =  sizeof(unsigned short) + sizeof(unsigned short);	//Code +  newNameIndex
	ret.DataSizeSub =  sizeof(unsigned short);	// Banchi
	ret.CVType = CVTypeOnePoint;
	ret.WithNode = false;

	return ret;
}

