// RGISNode.cpp: RGISNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISNode.h"
#include "RGIS25000.h"
#include "RDiskUtil.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


RGISNodeIntArray::RGISNodeIntArray()
{
	this->Count = 0;
	this->Points = NULL;
}

RGISNodeIntArray::~RGISNodeIntArray()
{
	Delete();
}

void RGISNodeIntArray::Create(const char* inStr) throw (RException)
{
	ASSERT(this->Points == NULL);

	string findStr = "BD(IR{ND";

	//いくつあるのか数えます。
	int i;
	int count;
	const char * p = inStr;

	for(count = 0 ; p != NULL; count++)
	{
		p = strstr(p , findStr.c_str() );
		if (p == NULL) break;

		p += findStr.size();
	}
	if (count <= 0) return;

	//数わかったのでメモリ確保.
	this->Count = count;
	this->Points = new unsigned long[count];


	//変換しながら追加.
	int dummy;
	int nd;
	p = inStr;
	for(i = 0 ; i < count; i++)
	{
		p = strstr(p , findStr.c_str() );
		ASSERT(p != NULL);

		int rsize = sscanf(p , "BD(IR{ND%5d%d})", &dummy,	&nd );
		if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %d ソース[%s]" ,rsize , inStr  ) );

		this->Points[i] = nd;

		p += findStr.size();
	}

}

void RGISNodeIntArray::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISNodeIntArray::test()
{
	//国土地理院の了承がないとテストデータでも侵害になるかもしれないので、
	//テストルーチンは動かしません
	/*
	unsigned long r;
	/// @test BD(IR{ND を認識して正しくパースできるか?
	try
	{
		string src = "国土地理院の了承がないのでデータは削除しました";

		RGISNodeIntArray ndArray;
		ndArray.Create( src.c_str() );

		ASSERT( (r = ndArray.getCount() ) == 2);
		ASSERT( (r = ndArray.getPoint(0) ) == 2407);
		ASSERT( (r = ndArray.getPoint(1) ) == 2461);
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
	/// @test BD(IR{ND がまったくないやつを渡しても正しいか?
	try
	{
		string src = "国土地理院の了承がないのでデータは削除しました";

		RGISNodeIntArray ndArray;
		ndArray.Create( src.c_str() );

		ASSERT( (r = ndArray.getCount() ) == 0);
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
	*/
}


RGISNode::RGISNode()
{
	this->Count = 0;
	this->Points = NULL;
}

RGISNode::~RGISNode()
{
	Delete();
}

void RGISNode::Create(const string & inFilename) throw (RException)
{
	ASSERT(Points == NULL);

//	TRACE("douro %s\n" , inFilename.c_str() );

	this->Count = RGIS25000::GetMaxID(inFilename , "DS(ID{DS%d})");
	if (this->Count <= 0) return ;

	this->Points = new NodeData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string buffer;
	int i;
	int pt;
	//未使用フラグを立てる.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		int id;
		int rsize = sscanf(buffer.c_str() , "DS(ID{DS%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1から始まるため.
		if (id >= Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "の " + num2str(i) + "行目のIDが変です!");
		}
		NodeData* p = &Points[id];

		const char * toOption = strstr(buffer.c_str() , "PT(");
		rsize = sscanf(toOption , "PT(ID{PT%d}",
				&pt );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		p->PT = pt;

		p->Use = true;
	}
}

void RGISNode::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

//ファイルにノード情報を書き込む.
void RGISNode::WriteToFile(RStdioFile * ioFile ,const RGISNode * inNode , const RGISNodeIntArray* inNodeArray ,
						   const UPOINT * writePointArray ,int inPointCount , const RGISSlp* inSlp,
						   const RGISGyousei* inGyousei) throw(RException)
{
	//ノードがどの CV に当てはまるのか入れる配列.
	//256 の理由は、 CVは 256以上行くと分割するようにしているため.
	unsigned char NodeCV[256];


	int count = 0;
	for(int i = 0; i < inPointCount ; i++)
	{
		const UPOINT * srcPoint = &writePointArray[i];

		//ノード情報に srcPoint が記載されていれば ノードとして処理します。
		int nodeArrayCount = inNodeArray->getCount();
		for(int n = 0 ; n < nodeArrayCount ; n++)
		{
			int nodeAS = inNodeArray->getPoint(n) ;
			int pt = inNode->getPoint( nodeAS - 1)->PT;
			UPOINT destPoint = inSlp->getPoint(pt  - 1 );

			//ノードに記載されていた地点?
			if (srcPoint->x == destPoint.x && srcPoint->y == destPoint.y)
			{
				ASSERT(count < 256);
				ASSERT(i < inPointCount);

				//つまり、このポイントに属している cvの要素は ノードに当たるわけです。
				NodeCV[count] = i;

				count++;
			}
		}
	}
	/*
	if (inPointCount > 1)
	{
		//行政単位でマップは作られているため、他の町につながっている道路などは処理できません。
		//そこで、行政の幅ぎりぎりにつくられている道路はノードとしてしるしを着けます。
		//つまり、ノードとしてしるしはついているけど、実はノードではない道路がでる可能性もあります。
		//そこはデコーダーががんばってください(w
		int gc = inGyousei->getCount();
		for(i = 0 ; i < gc ; i ++)
		{
			const GISINTArray * gcv = &((inGyousei->getPoint(i))->CV);
			int gcvc = gcv->getCount();
			for(int n = 0 ; n < gcvc ; n++)
			{
				int pt = gcv->getPoint(n);
				UPOINT destPoint = inSlp->getPoint(pt  - 1 );

				const UPOINT * startPoint = &writePointArray[0];
				const UPOINT * endPoint = &writePointArray[ inPointCount - 1 ];

				//ノードに記載されていた地点?
				if (startPoint->x == destPoint.x && startPoint->y == destPoint.y)
				{
					ASSERT(count < 256);
					NodeCV[count] = 0;	//最初の点
					count++;
				}
				else if (endPoint->x == destPoint.x && endPoint->y == destPoint.y)
				{
					ASSERT(count < 256);
					NodeCV[count] = inPointCount - 1;	//最後の点
					count++;
				}
			}
		}
	}
	*/

	ASSERT(count < 256);

	//結果をファイルに書き込み.

	//まず、数を書き込みます。
	unsigned char size = count;
	ioFile->fwrite( &size , sizeof(size) );

	//結果が 0だったら、データを書き込まずに終わります。
	if (size == 0)	return ;

	//結果の書き込み.
	ioFile->fwrite( NodeCV , sizeof(unsigned char) , size);


}



void RGISNode::test()
{
		//国土地理院の了承がないとテストデータでも侵害になるかもしれないので、
		//テストルーチンは動かしません
/*
	unsigned long r;
	/// @test DS を認識して正しくパースできるか?
	try
	{
		RGISNode node;
		node.Create( "test/test.ds" );

		ASSERT( (r = node.getCount() ) == 2);
		ASSERT( (r = node.getPoint(0)->PT ) == 350);
		ASSERT( (r = node.getPoint(1)->PT ) == 357);
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
	/// @testノードを正しく保存できるか?
	try
	{
		RGISSlm		slm;
		RGISSlp		slp;
		RGISDouro	douro;
		RGISNode	node;
		RGISHashi	hashi;
		RGISTonneru	tonneru;
		RGISNameDatabase ndb;
		RGISGyousei gyousei;

		slp.Create("test/node/test.slp" , &slm);

		douro.Create("test/node/test.dk" , &ndb);
		node.Create("test/node/test.ds");

		RDiskUtil::Delete("test/node_pack");
		RDiskUtil::MakeDirectory("test/node_pack");
		douro.SmallPack("test/node_pack/", &slp , &ndb , &gyousei , &node , &hashi , &tonneru );

		RStdioFile file;
		file.Open("test/node_pack/DK/0","rb");

		{
			//データ部分を読み飛ばし.
			file.fseek( douro.getPackInfo().DataSize , SEEK_CUR);
			file.fseek( douro.getPackInfo().DataSizeSub , SEEK_CUR);

			//最初の node の部分.
			unsigned char ndCound;
			unsigned char nnn[256];
			file.fread(&ndCound , sizeof(ndCound) );
			ASSERT(ndCound == 1);
			file.fread(nnn , sizeof(unsigned char) , ndCound);
			ASSERT(nnn[0] == 1);

			//CV部分を読み飛ばし.
			unsigned char cvCound;
			file.fread(&cvCound , sizeof(cvCound) );
			file.fseek( sizeof(unsigned short) * 2, SEEK_CUR);
			file.fseek( (cvCound - 1)* sizeof(unsigned char) * 2, SEEK_CUR);
		}
		{
			//データ部分を読み飛ばし.
			file.fseek( douro.getPackInfo().DataSize , SEEK_CUR);
			file.fseek( douro.getPackInfo().DataSizeSub , SEEK_CUR);

			//次の node の部分.
			unsigned char ndCound;
			unsigned char nnn[256];
			file.fread(&ndCound , sizeof(ndCound) );
			ASSERT(ndCound == 1);
			file.fread(nnn , sizeof(unsigned char) , ndCound);
			ASSERT(nnn[0] == 1);

			//CV部分を読み飛ばし.
			unsigned char cvCound;
			file.fread(&cvCound , sizeof(cvCound) );
			file.fseek( cvCound  * sizeof(unsigned short) * 2, SEEK_CUR);
		}
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
*/
}

