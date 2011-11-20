// RGISJoin.h: RGISJoin クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISJOIN_H__FBD8AEF8_0F4D_4CED_A9FC_C42A5C7EB0DE__INCLUDED_)
#define AFX_RGISJOIN_H__FBD8AEF8_0F4D_4CED_A9FC_C42A5C7EB0DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStdioFile.h"
#include "RZlib.h"

const int SEGMENT_MAX	=	26;


enum ENUMCVType
{
	CVTypeNone	= 0,
	CVTypeOnePoint	= 1,
	CVTypeMultiPoint	= 2,
	CVTypeString	= 3,
	CVTypeByteData	= 4,
};

struct JoinTempFormatScript
{
	string TempFileDirectoryName;		//結合するファイルがおいてあるディレクトリ名 DK とか
	unsigned char BlockStartSimbol;		//ブロックがスタートするときのシンボル	道路だったら D とか
	int	DataSize;						//CVデータを除く固定長データサイズ	不変データ
	int	DataSizeSub;					//CVデータを除く固定長データサイズ	変動データ
	bool WithNode;						//CVの前にノード情報を記載する.
	ENUMCVType CVType;					//0 -> CVなし  1-> 1Point 2-> 複数Point 3->文字列
};




enum ENUMHeadType
{
	HeadTypeFixed = 1,					//固定長 DataSize のデータの繰り返し
	HeadTypeFixedAndString = 2,			//固定長 DataSize + 文字列\0 の繰り返し
};

struct JoinHeadFormatScript
{
	string TempFilename;				//結合するファイルがおいてあるファイル名 DK とか
	unsigned char BlockStartSimbol;		//ブロックがスタートするときのシンボル	目次だったら I とか
	int DataSize;						//固定データサイズ 
	ENUMHeadType	HeadType;
};

struct OptimizeTemp
{
	char Share[16];
	vector<DWORD>	Pos;
};


class RGISJoin  
{
public:
	RGISJoin();
	virtual ~RGISJoin();

	//結合処理
	void Join(const string & inDir , const string &inMapArcFilename ,
		const JoinTempFormatScript* inScript, int inSize,
		const JoinHeadFormatScript* inHead , int inHeadSize);
	//整合性のチェック
	void Check(const string & inMasterFilename) throw (RException);
	//地図の詳しい情報を得る.
	void Info(const string & inMasterFilename) throw (RException);

private:
	void GetAllFilenameList(RStringVector * inList ,const string & inDir ,const JoinTempFormatScript* inScript, int inSize);
	//masterに自身を丸ごとコピー
	void FileAppend( RStdioFile * ioMaster , RStdioFile * ioFile , unsigned long inSize);
	//件数を数えます
	unsigned long GetDataCount(RStdioFile * ioFile , const JoinTempFormatScript* inScript , unsigned long inSize) ;
	//件数を数えます
	unsigned long RGISJoin::GetHeadDataCount(RStdioFile * ioFile , const JoinHeadFormatScript* inScript , unsigned long inSize) ;

	//ヘッダーを書き込みます。
	void RGISJoin::WriteHeader(RStdioFile * ioMaster , const string & inDir ,
						const JoinHeadFormatScript* inHead , int inHeadSize);
	//masterに圧縮した自分自身をコピー
	void RGISJoin::ZFileAppend( RStdioFile * ioMaster , RStdioFile * ioZFile);
	//最適化
	DWORD RGISJoin::Optimize(RStdioFile *  ioReadFile , RStdioFile * ioWriteFile , const JoinTempFormatScript* inScript , int inCount) ;
	DWORD RGISJoin::DataSkip(RStdioFile * ioFile , const JoinTempFormatScript* inScript , char * outMainData = NULL);
	//文字列フォーマzットで記録されているファイルの
	//件数を数えます
	unsigned long RGISJoin::StringCount(RStdioFile * ioFile, unsigned long inSize);
	//文字列フォーマzットで記録されているファイルの
	//件数を数えます
	unsigned long RGISJoin::FixedAndStringCount(RStdioFile * ioFile , unsigned long inSize , int inFixedSize);

private:
	RZlib	Zlib;
};

#endif // !defined(AFX_RGISJOIN_H__FBD8AEF8_0F4D_4CED_A9FC_C42A5C7EB0DE__INCLUDED_)
