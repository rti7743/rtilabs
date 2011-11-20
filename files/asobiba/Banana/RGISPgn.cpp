// RGISPgn.cpp: RGISPgn クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISPgn.h"
#include "RStdioFile.h"
#include "RDiskUtil.h"
#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RGISNameDatabaseDisk.h"
#include "RBigArray2.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISPgn::RGISPgn()
{
	Count = 0;
	Points= NULL;
}

RGISPgn::~RGISPgn()
{
	Delete();
}

void RGISPgn::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISPgn::Create(const string & inFilename, int inScale) throw(RException)
{
	ASSERT(Points == NULL);

	this->Filename = inFilename;

	RStdioFile file;
	file.Open(inFilename , "rb");
	FILE * fp = file.getHandle();

	string buffer;
	buffer = file.ReadLine();

	//ヘッダーの取得.
	RGIS2500Head	head;
	head.Create( buffer.c_str() ,inScale);

	//要素の総数の取得.
	this->Count = head.getCount();
	this->Points = new PgnData[this->Count];

//	char LineCountArray[MAX_PRG_BUFFER][MAX_PRG_STRING_BUFFER];
	RBigArray2<char> bigarray(MAX_PRG_BUFFER , MAX_PRG_STRING_BUFFER);
	char **LineCountArray = bigarray.get();

	int line = 0;
	for(int i = 0  ; !file.IsEof(); i++ , line ++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		//読み込み.		A1105,261,12,750.0,999.9
		int lineCount;
		double x,y;
		PgnData* p = &Points[i];
		int rsize = sscanf(buffer.c_str() , "A%d,%d,%d,%lf,%lf" , &p->Type , &p->No,&lineCount , &x , &y);
		if (rsize != 5)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );
		}

		if (lineCount >= MAX_PRG_BUFFER)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(line) + "行目のデータが ポリゴンの最大値" + num2str(MAX_PRG_BUFFER) + "を超えて" + num2str(lineCount) + "になりました");
		}

		//ポリゴンの各頂点データの読み込み.
		for(int n = 0; n < lineCount ; n++ , line ++)
		{
			buffer = file.ReadLine();

			if (buffer.size() >= MAX_PRG_STRING_BUFFER)
			{
				throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(line) + "行目のデータが ポリゴンの一要素の長さの最大値" + num2str(MAX_PRG_STRING_BUFFER) + "を超えて" + num2str(buffer.size()) + "になりました");
			}

			strcpy(LineCountArray[n] , buffer.c_str() );
		}
		p->Array.Create(lineCount , (const char **)LineCountArray );
		p->Representative = head.convertXY(x,y);
	}
}

void RGISPgn::Draw(const RGISDraw * inDraw , const RGISArc * inArc) const
{
	for(int n = 0 ; n < this->Count ; n++)
	{
		PgnData* p = &Points[n];

		inArc->Draw(inDraw , & p->Array , p->Type);
	}
}

void RGISPgn::SmallPack(const RGISArc * inArc , const RGISAtr * inAtr , 
						const string & inPath  ,const string & inMark,RGISNameDatabase* ioNameDatabase,
						RGIS2500InfoVector* gis2500infoVector)
{
	string dir = inPath + inMark + "/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inPath + "NX/";
	RDiskUtil::MakeDirectory(namedir);


	//RGISCVCut.Writeの中から呼びだれる頂点データのヘッダーを書き込むルーチン.
	class RGISCVCut2500Data  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//名前データベース
		string Dir;								//名前ファイルを作成する場所.

	public:
		string filename;

		unsigned short NameIndex;		//このマップの中手の名前番号						2

		RGISCVCut2500Data(const RGISNameDatabase* inNameData , const string inDir )
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
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));

		}
	};

	RGISCVCut2500Data	data(ioNameDatabase , namedir);

	data.filename = this->Filename;

	RGISCVCut cut;
	for(int n = 0 ; n < this->Count ; n++)
	{
		const PgnData* p = &Points[n];

		const AtrData* atr = NULL;
		///このポリゴンに該当する atr を探す
		{
			int atrCountMax = inAtr->getCount();
			for(int m = 0 ; m < atrCountMax ; m++ )
			{
				const AtrData* temp = inAtr->getPoint(m);
				if ( temp->No == p->No )
				{
					atr = temp;
					break;
				}
			}
		}

		string name ;
		if (atr != NULL && !atr->attrib[1].empty() )
		{
			try
			{
				data.NameIndex = ioNameDatabase->setName( atr->attrib[1].c_str() );
			}
			catch(RException e)
			{
				throw RException(EXCEPTIONTRACE + PrintFormat("setNameが失敗しました %s [%s]:\n" , this->Filename.c_str() , atr->attrib[1].c_str() ) + e.getMessage() );
			}
			name = atr->attrib[1];
		}
		else
		{
			data.NameIndex = 0;
			name = "";
		}

		const RGIS2500IntArray*	pgnArray = &p->Array;
		int						pgnType = p->Type;

		//ポリゴン情報がない場合は没.
		if (pgnArray->getCount() <= 0)
		{
			continue;
		}

		//現在処理したアーク数
		int arcCountMax = inArc->getCount();
		int arcCount = 0;
		int	targetArc = abs( pgnArray->getPoint(arcCount ));
		for( int i = 0; i < arcCountMax ; )
		{
			const ArcData* arc = inArc->getPoint(i);
			if ( targetArc == arc->No )
			{
				cut.Write(dir , &arc->Array , &data );

				gis2500infoVector->push_back( new RGIS2500Info(&arc->Array , name) );

				//このアークで最後?
				arcCount ++;
				if (arcCount >= pgnArray->getCount()) break ;

				targetArc = abs(pgnArray->getPoint(arcCount));
				i = 0;
			}
			else	i++;
		}
		
	}
}


void RGISPgn::SmallPackNoWrite(const RGISArc * inArc , const RGISAtr * inAtr , RGIS2500InfoVector* gis2500infoVector)
{
	for(int n = 0 ; n < this->Count ; n++)
	{
		const PgnData* p = &Points[n];

		const AtrData* atr = NULL;
		///このポリゴンに該当する atr を探す
		{
			int atrCountMax = inAtr->getCount();
			for(int m = 0 ; m < atrCountMax ; m++ )
			{
				const AtrData* temp = inAtr->getPoint(m);
				if ( temp->No == p->No )
				{
					atr = temp;
					break;
				}
			}
		}

		string name ;
		if (atr != NULL && !atr->attrib[1].empty() )
		{
			name = atr->attrib[1];
		}
		else
		{
			name = "";
		}

		const RGIS2500IntArray*	pgnArray = &p->Array;
		int						pgnType = p->Type;

		//ポリゴン情報がない場合は没.
		if (pgnArray->getCount() <= 0)
		{
			continue;
		}

		//現在処理したアーク数
		int arcCountMax = inArc->getCount();
		int arcCount = 0;
		int	targetArc = abs( pgnArray->getPoint(arcCount ));
		for( int i = 0; i < arcCountMax ; )
		{
			const ArcData* arc = inArc->getPoint(i);
			if ( targetArc == arc->No )
			{
				gis2500infoVector->push_back( new RGIS2500Info(&arc->Array , name) );

				//このアークで最後?
				arcCount ++;
				if (arcCount >= pgnArray->getCount()) break ;

				targetArc = abs(pgnArray->getPoint(arcCount));
				i = 0;
			}
			else	i++;
		}
		
	}
}
