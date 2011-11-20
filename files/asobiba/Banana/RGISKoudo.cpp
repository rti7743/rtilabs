//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

#include "RGISKoudo.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RDiskUtil.h"
#include "RGISCVCut.h"
#include "RGISNameDatabaseDisk.h"
#include "RGISCvLight.h"
#include "RDebug.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////




RGISKoudo::RGISKoudo()
{
	Count = 0;
	Points= NULL;

}

RGISKoudo::~RGISKoudo()
{
	Delete();
}


void RGISKoudo::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}
typedef vector<int>	TestFiledToVectorData;

typedef vector<POINT>	TESTFILED_POINTES;
class RGISKoudoTestFiled
{
public:
	RGISKoudoTestFiled()
	{
		this->TestFiled = NULL;
		this->TestFiledRealNumber = NULL;
	}
	~RGISKoudoTestFiled()
	{
		delete [] this->TestFiled;
		delete [] this->TestFiledRealNumber;
	}
	//テスト領域の確保と初期化.
	void Create(const LRECT & inFiled ,int inMeshKankaku)
	{
		this->Filed = inFiled;
		this->MeshKankaku = inMeshKankaku;

		//テスト領域の確保と初期化.
		this->Width = (this->Filed.right - this->Filed.left) / inMeshKankaku + 1;
		this->Hegiht = (this->Filed.bottom - this->Filed.top) / inMeshKankaku + 1;
		this->TestFiled = new unsigned char[this->Width * this->Hegiht];
		memset(this->TestFiled ,0, this->Width * this->Hegiht * sizeof(unsigned char) );

		//slpでの位置しているか.
		this->TestFiledRealNumber = new long[this->Width * this->Hegiht];
		memset(this->TestFiledRealNumber , 0 , this->Width * this->Hegiht * sizeof(unsigned long) );
	}
	//地点の追加.
	void setPos(const UPOINT & xy,long inSlpNumer , unsigned char inLevel)
	{
		ASSERT( inLevel <= 20);
		ASSERT(xy.x >= this->Filed.left);
		ASSERT(xy.y >= this->Filed.top);
		ASSERT(xy.x <= this->Filed.right);
		ASSERT(xy.y <= this->Filed.bottom);

		int x = (xy.x - this->Filed.left) / this->MeshKankaku;
		int y = (xy.y - this->Filed.top) / this->MeshKankaku;

		int i = calePos(x,y);
		ASSERT(this->TestFiled[i] == 0);
		this->TestFiled[i] = inLevel ;
		this->TestFiledRealNumber[i] = inSlpNumer;
	}
	//横幅の取得
	int getWidth() const
	{
		return this->Width;
	}
	//縦幅の取得
	int getHegiht() const
	{
		return this->Hegiht;
	}
	//有効な点を探します.
	POINT EnablePoint() const
	{
		POINT xy;
		for(xy.y = 0; xy.y < this->Hegiht ; xy.y++)
		{
			for(xy.x = 0; xy.x < this->Width ; xy.x++)
			{
				if ( this->getPosEasy(xy.x , xy.y) >= 10) return xy;
			}
		}

		//もうないです。
		xy.x = -1;
		xy.y = -1;

		return xy;
	}


private:
	int calePos(int x,int y) const
	{
		ASSERT(x < Width && x >= 0);
		ASSERT(y < Hegiht && y >= 0);
		return y * Width + x;
	}
	POINT revCalePos(int xy) const
	{
		POINT ret;
		ret.y = xy / Width;
		ret.x = xy % Width;
		ASSERT(ret.x < Width && ret.x >= 0);
		ASSERT(ret.y < Hegiht && ret.y >= 0);
		return ret;
	}

	//地点の設定
	void setPosEasy(int x,int y,unsigned char inLevel) 
	{
		ASSERT(x < Width && x >= 0);
		ASSERT(y < Hegiht && y >= 0);
		this->TestFiled[calePos(x,y)] = inLevel;
	}
	//地点の取得
	unsigned char getPosEasy(int x,int y) const
	{
//		ASSERT(x < Width && x >= 0);
//		ASSERT(y < Hegiht && y >= 0);
		if ( !(x < Width && x >= 0) ) return 0;
		if ( !(y < Hegiht && y >= 0) ) return 0;

		ASSERT( this->TestFiled[calePos(x,y)] <= 20);

		return this->TestFiled[calePos(x,y)] ;
	}

	//地点の取得
	bool isNextRoute(int x,int y,unsigned char inLevel) const
	{
		if ( getPosEasy(x,y) != inLevel) return false;

		unsigned char l;
		l = getPosEasy(x + 1 ,y) ;
		if ( l != 1 && l != inLevel) return true;
		l = getPosEasy(x - 1 ,y);
		if ( l != 1 && l != inLevel) return true;
		l = getPosEasy(x ,y + 1);
		if ( l != 1 && l != inLevel) return true;
		l = getPosEasy(x ,y - 1);
		if ( l != 1 && l != inLevel) return true;

		return false;
	}
	void PaintRev(int inX, int inY , unsigned char inLevel , int inFlg)
	{
	//	if (inX < 0 || inY < 0 || inX >= Width || inY >= Height) return ;

		int l ,r , x;
		//左にいけるところまで行きます。
		for(l = inX ; l >= 0 ; l-- )
		{
			if ( getPosEasy( l ,  inY) != inLevel) break;
		}
		l++;

		//右にいけるところまで行きます。
		for(r = inX ; r < Width ; r++ )
		{
			if ( getPosEasy( r ,  inY) != inLevel) break;
		}
//		r--;

		//OK. その間を塗りつぶします。
		for(int n = l ; n < r ; n++)
		{
			setPosEasy(n , inY , 0);
		}
//		::MoveToEx(PoolDC , l , inY , NULL);
//		::LineTo(PoolDC , r , inY );

		//で、その間にあったやつのひとつ上のピクセルで塗りつぶせるやつがあるんだったら塗りつぶす.
//		if (inFlg & 1)
		{
			if (inY > 0) 
			{
				int y = inY - 1;
				for(x = l ; x < r ; x++ )
				{
					if ( getPosEasy( x ,  y) == inLevel ) 
						PaintRev(x,y , inLevel ,1 );
				}
			}
		}
		//また、その間にあったやつのひとつ下のピクセルで塗りつぶせるやつがあるんだったら塗りつぶす.
//		if (inFlg & 2)
		{
			if (inY < Hegiht) 
			{
				int y = inY + 1;
				for(x = l ; x < r ; x++ )
				{
					if ( getPosEasy( x ,  y) == inLevel ) 
						PaintRev(x,y , inLevel ,2 );
				}
			}
		}
	}
	void ComplementVec(TestFiledToVectorData * ioFailedPointes)
	{
		if (ioFailedPointes->size() < 2) return ;

		int start = ioFailedPointes->at(0);
		int end = ioFailedPointes->at(ioFailedPointes->size() - 1);

		//結局はベクトルを閉じる.
		ioFailedPointes->push_back(start);

		//start地点のxよりも大きいところで終焉を迎えてしまうと、
		//逆時計回りになるので、修正する.
		POINT s = revCalePos(start);
		POINT e = revCalePos(end);
		if ( s.x < e.x )
		{
			//逆順にする.
			reverse( ioFailedPointes->begin() , ioFailedPointes->end() );
		}
	}

public:
	void Dump()
	{
		static no = 0;
		no ++;
		string out;
		out.reserve(6553500);
		out += "=======MH=============-\n";
		for (int y = 0 ; y < Hegiht ; y++)
		{
			for (int x = 0 ; x < Width ; x++)
			{
				char buf[10];
				
				::sprintf(buf, "%2d" , (int)this->getPosEasy(x , y) );
				out += (string) buf+ " ";
			}
			out += "\n";
		}
		out += "\n";
		RDebug::MemoryToFile("test/mh_log." + num2str(no) , out);
	}


	void Vec2CV(GISINTArray * outArray ,const TestFiledToVectorData * inFailedPointes )
	{
		TestFiledToVectorData::const_iterator i = inFailedPointes->begin();
		TestFiledToVectorData::const_iterator e = inFailedPointes->end();

		int size = inFailedPointes->size();
		unsigned long * ulongs = new unsigned long[ size ] ;
		for(int n = 0 ; i != e; i++, n++)
		{
			ulongs[n] = this->TestFiledRealNumber[*i];
		}
		outArray->Create(ulongs , size);

		delete [] ulongs;
	}
	//そこからベクトルデータを抽出します.
	unsigned char toVector(const POINT & inStart ,  TestFiledToVectorData * outFailedPointes)
	{
		struct MovePoint
		{
			int x,y,houkou;
		};

		static MovePoint MovePoint0[8] = 
		{
			{ -1 , -1 ,	0} ,	//左上
			{ 0  , -1 ,	0} ,	//上
			{ 1  , -1 ,	0} ,	//右上
			{ 1  , 0  ,	1} ,	//右
			{ 1  , 1  ,	2} ,	//右下
			{ 0  , 1  ,	2} ,	//下
			{ -1 , 1  ,	2} ,	//左下
			{ -1 , 0  ,	3} ,	//左
		};
		static MovePoint MovePoint1[8] = 
		{
			{ 1  , 0  ,	1} ,	//右
			{ 1  , 1  ,	2} ,	//右下
			{ 0  , 1  ,	2} ,	//下
			{ -1 , 1  ,	2} ,	//左下
			{ -1 , 0  ,	3} ,	//左
			{ -1 , -1 ,	0} ,	//左上
			{ 0  , -1 ,	0} ,	//上
			{ 1  , -1 ,	0} ,	//右上
		};
		static MovePoint MovePoint2[8] = 
		{
			{ 1  , 1  ,	2} ,	//右下
			{ 0  , 1  ,	2} ,	//下
			{ -1 , 1  ,	2} ,	//左下
			{ -1 , 0  ,	3} ,	//左
			{ -1 , -1 ,	0} ,	//左上
			{ 0  , -1 ,	0} ,	//上
			{ 1  , -1 ,	0} ,	//右上
			{ 1  , 0  ,	1} ,	//右
		};
		static MovePoint MovePoint3[8] = 
		{
			{ -1 , 1  ,	2} ,	//左下
			{ -1 , 0  ,	3} ,	//左
			{ -1 , -1 ,	0} ,	//左上
			{ 0  , -1 ,	0} ,	//上
			{ 1  , -1 ,	0} ,	//右上
			{ 1  , 0  ,	1} ,	//右
			{ 1  , 1  ,	2} ,	//右下
			{ 0  , 1  ,	2} ,	//下
		};
		static MovePoint* MovePointAll[4];
		MovePointAll[0] = MovePoint0;
		MovePointAll[1] = MovePoint1;
		MovePointAll[2] = MovePoint2;
		MovePointAll[3] = MovePoint3;

		outFailedPointes->clear();
		//時計回りに8方位を調べます.
		POINT p = inStart;
		unsigned char mySelf = this->getPosEasy(p.x  , p.y);
		outFailedPointes->push_back(calePos(p.x , p.y ) );

		//袋小路から抜け出し中?
		bool blindAlleyBack = false;
		int backCount ;


		int LastHoukou = 0;

		Dump();
		while(1)
		{
			//スタート地点に戻ってきたらおしまい.
			if (outFailedPointes->size() > 1 && p.x == inStart.x && p.y == inStart.y)
			{
				outFailedPointes->push_back(calePos(p.x , p.y ) );
				break;
			}

			const MovePoint* useSearchTable = MovePointAll[LastHoukou];

			for(int i = 0; i < TABLE_COUNT_OF(MovePoint0) ;i ++)
			{
				const MovePoint * sp = &useSearchTable[i];
				if ( this->isNextRoute(p.x + sp->x , p.y + sp->y,mySelf) )
				{
					LastHoukou = sp->houkou;
					p.x = p.x + sp->x;	p.y = p.y + sp->y;
					this->setPosEasy(p.x  , p.y ,1) ;
					outFailedPointes->push_back(calePos(p.x , p.y ) );
					blindAlleyBack = false;
					break;
					
				}
			}
			if (i != TABLE_COUNT_OF(MovePoint0)) continue;
			//細いひげのようなところに迷い込んだ?
			if (!blindAlleyBack)
			{
				backCount = outFailedPointes->size();
				blindAlleyBack = true;
			}
			//スタート地点まで戻ってもダメな場合
			if (backCount <= 1)
			{
				ComplementVec(outFailedPointes);
				break;
			}
			backCount --;
			p = revCalePos( outFailedPointes->at(backCount) );
		}
		Dump();
		//チェックに入れていた値を元の値に戻します.
		{
			TestFiledToVectorData::iterator i = outFailedPointes->begin();
			TestFiledToVectorData::iterator e = outFailedPointes->end();
			for( ; i != e; i++)
			{
				this->TestFiled[*i] = mySelf;
			}
		}
		Dump();
		//同じ色のところを 0 で塗りつぶします.
		PaintRev(inStart.x , inStart.y ,mySelf ,1 + 2 );
		Dump();

		return mySelf;
	}


private:
	LRECT Filed;
	int MeshKankaku;

	int Width;
	int Hegiht;

	unsigned char * TestFiled;
	long * TestFiledRealNumber;
};

void RGISKoudo::Create(const string & inSlpFilename,const string & inMHFilename,const RGISSlm * inMeshSlm) throw(RException)
{
	ASSERT(Points == NULL);

	//メッシュの slpの読み込み.
	MeshSlp.Create(inSlpFilename , inMeshSlm);
	MeshSlp.ConvertDisplayScal();

	//四隅の取得.
	LRECT filed = MeshSlp.getRect();

	//テストフィールドの作成.
	RGISKoudoTestFiled testFiled;
	testFiled.Create(filed , 200);


//	this->Count = RGIS25000::GetMaxID(inMHFilename , "MH(ID{MH%d})");
//	if (this->Count <= 0) return ;


	RStdioFile file;
	file.Open(inMHFilename , "rb");

	FILE * fp = file.getHandle();

	string buffer;
	int i;


	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		int id;
		int rsize = sscanf(buffer.c_str() , "MH(ID{MH%6d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1から始まるため.
		if (id < 0)
		{
			throw RException(EXCEPTIONTRACE + inMHFilename + "の " + num2str(i) + "行目のIDが変です!");
		}
		int dummy;
		unsigned int hk;
		unsigned int pm;
		const char * hkOption = strstr(buffer.c_str() , "HK{");
		rsize = sscanf(hkOption  , "HK{%d}",	 &hk  );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanfが失敗しました %s %d ソース[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		const char * ptStart = strstr(buffer.c_str() , "PT(");
		if (ptStart == NULL)
		{
			throw RException(inMHFilename + "の " + num2str(i) + "行目のデータのCV形式が辺です!");
		}
		rsize = sscanf(ptStart , "PT(ID{PM%d}){%d}",	&dummy , &pm);
		if (rsize != 2) continue;	//PTがないデータは無視

		//テストフィールドに追加.
		testFiled.setPos( MeshSlp.getPoint(pm - 1) , pm , selectKoudoLevel(hk) );
	}

	//テストフィールドからベクトルの作成.
	typedef vector<KoudoData> TestFiledToVectorDataManager;
	TestFiledToVectorDataManager VecManager;
	while(1)
	{
		POINT point = testFiled.EnablePoint();
		if (point.x == -1 && point.y == -1) break;

		TRACE("fill start %d:%d\n" , point.x , point.y);

		KoudoData kd;
		TestFiledToVectorData  vec;
		kd.Level = testFiled.toVector(point , &vec);

		//最低3ピースからならないような、とこのは排除.
		if (vec.size() > 2)
		{
			testFiled.Vec2CV(&kd.CV , &vec);
			ASSERT( MeshSlp.TestCV(&kd.CV) == true );
			VecManager.push_back(kd);

		}
	}

	this->Count = VecManager.size();
	this->Points = new KoudoData[this->Count];
	//ベクトルデータの格納.
	for( i = 0  ;i < this->Count ; i++)
	{
		this->Points[i].CV = VecManager[i].CV;
		this->Points[i].Level = VecManager[i].Level;
		ASSERT( MeshSlp.TestCV(&this->Points[i].CV) == true )
	}


}

//標高をレベルに変換.
unsigned char RGISKoudo::selectKoudoLevel(int inKM)
{
//	return 10;
	if (inKM <  200) return 10;
	if (inKM <  400) return 11;
	if (inKM <  600) return 12;
	if (inKM <  800) return 13;
	if (inKM < 1000) return 14;
	if (inKM < 1500) return 15;
	if (inKM < 2000) return 16;
	if (inKM < 2500) return 17;
	if (inKM < 3000) return 18;
	if (inKM < 4000) return 19;
	if (inKM < 5000) return 20;
	return 21;
}

void RGISKoudo::Draw(const RGISDraw * inDraw) const
{
}

COLORREF RGISKoudo::selectColor(GISType* inShubetsu) const
{
	return 0;
}
COLORREF RGISKoudo::selectSize(GISType* inShubetsu) const
{
	return 0;
}

void RGISKoudo::SmallPack(const string & inDir )  const throw(RException)
{
	string dir = inDir + "MH/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NX/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Writeの中から呼びだれる頂点データのヘッダーを書き込むルーチン.
	class RGISCVCutKoudoData  : public RGISCVCutDataIml
	{
	private:

	public:

		unsigned char Level;				//高度レベル

		RGISCVCutKoudoData()
		{
		}

		//これが呼び出される.
		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			//
			ioFile->fwrite(&Level , sizeof(Level));
		}
	};

	RGISCVCutKoudoData	data;

	RGISCvLight light;
	RGISCVCut cut;
	for(int n = 0 ; n < Count ; n++)
	{
		const KoudoData* p = &Points[n];
		data.Level =  p->Level;

		GISINTArray lightCV;
		if ( light.DoLight( &MeshSlp , &p->CV , &lightCV) )
		{
			cut.Write(dir , &MeshSlp , &lightCV , &data );
		}
		else
		{
			cut.Write(dir , &MeshSlp , &p->CV , &data );
		}
	}
}

JoinTempFormatScript RGISKoudo::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "MH";
	ret.BlockStartSimbol = 'H';
	ret.DataSize = sizeof(unsigned char);	//1	=	1
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeMultiPoint;
	ret.WithNode = false;

	return ret;
}


void RGISKoudo::test()
{
/*
	RGISSlm slm;
	RGISKoudo mh;
	slm.Create("test/01101.slm" , 1);
	mh.Create("test/test.mh.slp","test/test.mh",&slm);
*/
}
