// YSubject.cpp: YSubject クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "YSubject.h"
#include "YDATReader.h"
#include "YDATWritter.h"	//テストで使う.
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

YSubject::YSubject()
{
	this->No = 0;
}

YSubject::~YSubject()
{
	RAutoWriteSpinLock al(&Lock);

	//All Aborn!
	for(int i = 0; i < this->No ; i++)
	{
		delete DATs[i];
	}
}


void YSubject::Age(const YDAT* inDAT)
{
	RAutoWriteSpinLock al(&Lock);

	Update();

	DATNUMBER num = inDAT->getDatNumber();
	if (this->No  >= 1)
	{
		//実は既にトップ
		if (DATs[0]->getDatNumber() == num)
		{
			DATs[0]->SubjectChange( inDAT->getSubjectAndLastNo() );
			return ;
		}

		//どっかにある?
		for( int i = 1 ; i < this->No ; i++ )
		{
			if (DATs[i]->getDatNumber() == num)
			{
				YNiseDAT* p = DATs[i];
				memmove( &DATs[1] , DATs , sizeof(YNiseDAT*) * i);
				DATs[0] = p;
				p->SubjectChange( inDAT->getSubjectAndLastNo() );
				return ;
			}
		}
	}
	//なかったということですね。
	if (this->No < MAX_SUBJECT)
	{
		memmove( &DATs[1] , DATs , sizeof(YNiseDAT*) * this->No );
		DATs[0] = new YNiseDAT( num , inDAT->getSubjectAndLastNo() );
		this->No ++;
		return ;
	}

	//んぢゃ、最後のヤツをイケニエ.

	//DAT落ち決定.
	delete DATs[this->No - 1];
	memmove( &DATs[1] , DATs , sizeof(YNiseDAT*) * (this->No-1) );
	DATs[0] = new YNiseDAT( num , inDAT->getSubjectAndLastNo() );

	return ;
}

void YSubject::Sage(const YDAT* inDAT)
{
	RAutoWriteSpinLock al(&Lock);

	Update();

	DATNUMBER num = inDAT->getDatNumber();
	//どっかにある?
	for( int i = 0 ; i < this->No ; i++ )
	{
		if (DATs[i]->getDatNumber() == num)
		{
			DATs[i]->SubjectChange( inDAT->getSubjectAndLastNo() );
			return ;
		}
	}
	//ない? ぢゃ無視.
	return ;
}


void YSubject::Load(const string inBoardName)
{
	RAutoWriteSpinLock al(&Lock);
	//1036488047.dat<>スパイラル～推理の絆～ 実況スレその4 (33)

	string filename =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\subject.txt";

	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		return;
	}

	char buf[MAX_SUBJECT_LINE];

	int i = 0;
	while( !feof(fp) )
	{
		if (i >= MAX_SUBJECT) break;
		if ( fgets(buf , MAX_SUBJECT_LINE , fp) == NULL ) break;
		
		const char* datnumber = buf;
		const char* subject = YDATReader::next(buf);
		if (subject == NULL) break;
		const char* end = YDATReader::nextnr(subject , subject + strlen(subject));
		if (end == NULL) break;

		string datnumberstring = string(datnumber , subject - datnumber - 2);

		
		DATs[i] = new YNiseDAT( atol( datnumberstring.c_str() ) , string(subject, end - subject) );
		i++; 
	}
	this->No = i;

	fclose(fp);

	Update();
}

void YSubject::Save(const string inBoardName)
{
	RAutoWriteSpinLock al(&Lock);

	string filename =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\subject.txt";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		return;
	}

	string str;
	for(int i = 0; i < this->No ; i++)
	{
		str = DATs[i]->CompateLine();
		fprintf(fp, str.c_str() );
	}

	fclose(fp);
}


/**
 * Show:				表示			 YTextFileより継承
 *
 * @param outRPack		これに格納
 * @return void 
 */
void YSubject::Show(RPack * outRPack) const
{
	RAutoReadSpinLock al(&Lock);

	string ret;
	int no = this->No;

	ret.reserve(20 * no); //20は適当.
	for(int i = 0; i < no ; i++)
	{
		ret += DATs[i]->CompateLine().c_str() ;
	}
	outRPack->Write( ret.c_str() , ret.size() );
}


int YSubject::getNumbers(DATNUMBER * outNumbers , int inSize) const
{
	RAutoReadSpinLock al(&Lock);
	if ( inSize > this->No ) inSize = this->No;

	for(int i = 0; i < inSize ; i++)
	{
		outNumbers[i] = DATs[i]->getDatNumber();
	}
	return inSize;
}

int YSubject::WhatPosition(DATNUMBER inDATNumber) const
{
	RAutoReadSpinLock al(&Lock);

	const int n = this->No;
	for(int i = 0; i < n ; i++)
	{
		if (inDATNumber == DATs[i]->getDatNumber() ) return i;
	}
	
	ASSERT(0);
	return -1;
}

/**
 * DrawThreadIndex:			スレッド一覧の描画
 *
 * @param inTemplate		描画に使うテンプレート
 * @param inReadCGIPath		read.cgiのパス
 * @param inCount			何個描画するか?
 * @return string			描画した結果	
 */
const string YSubject::DrawThreadIndex(const RHtmlTemplate* inTemplate , const string inReadCGIPath ,  int inCount) const
{
	RAutoReadSpinLock al(&Lock);

	RStringMap	smap;
	const int n = min( this->No , inCount);
	if (n <= 0) return "";

	string ret;
	ret.reserve(n * 512);	//512は適当.

	for(int i = 0; i < n ; i++)
	{
		smap["YThreadIndexNo"] = num2str(i+1);
		smap["YThreadIndexSubjectAndNo"] = DATs[i]->getSubject();
		smap["YThreadL50"] = inReadCGIPath + num2str( DATs[i]->getDatNumber() ) + "/l50";

		ret += inTemplate->Replace(&smap);
	}
	
	return ret;
}



void YSubject::test()
{
	string str;
	//とりあえず、書き込んでみる!
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("七資産","age","クララがたった!" , "クソスレ");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("クソスレ");

		ASSERT(sub.No == 0);
		sub.Age(dat);
		ASSERT(sub.No == 1);
		ASSERT(sub.DATs[0]->getDatNumber() == 12345);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "12345.dat<>クソスレ (1)\n");

		delete dat;
	}
	//2個入れてみる
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("七資産","age","クララがたった!" , "クソスレ");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("クソスレ");
		sub.Age(dat);		//一回目
		ASSERT(sub.No == 1);

		dat->Append(&pack);
		sub.Age(dat);		//1回目 同じすれだから

		ASSERT(sub.No == 1);

		ASSERT(sub.DATs[0]->getDatNumber() == 12345);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "12345.dat<>クソスレ (2)\n");

		delete dat;
	}
	//別2個入れてみる
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("七資産","age","クララがたった!" , "クソスレ");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("クソスレ");
		sub.Age(dat);		//一回目
		ASSERT(sub.No == 1);
		delete dat;

		dat = new YDAT(54321);
		dat->setSubject("クソスレ2");
		dat->Append(&pack);
		sub.Age(dat);		//2回目
		ASSERT(sub.No == 2);
		delete dat;

		ASSERT(sub.DATs[0]->getDatNumber() == 54321);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "54321.dat<>クソスレ2 (1)\n");

	}

	//A B A と入れてみる.
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("七資産","age","クララがたった!" , "クソスレ");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("クソスレ");
		sub.Age(dat);		//一回目
		ASSERT(sub.No == 1);

		YDAT* dat2 = new YDAT(54321);
		dat2->setSubject("クソスレ2");
		dat2->Append(&pack);
		sub.Age(dat2);		//2回目
		ASSERT(sub.No == 2);

		dat->Append(&pack);
		sub.Age(dat);		//2回目
		ASSERT(sub.No == 2);

		ASSERT(sub.DATs[0]->getDatNumber() == 12345);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "12345.dat<>クソスレ (2)\n");

		delete dat2;
		delete dat;
	}

	//A B A B .... と MAX_SUBJECT 入れてみます.
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		dat->setSubject("クソスレ");
		RPack pack;
		str = YDATWritter::make("七資産","age","クララがたった!" , "クソスレ");
		pack.Write(str.c_str() , str.size() );
		RPack pack2;
		str = YDATWritter::make("ななしさん","age","ずざー" , "伝説のﾊｯｶｰ");
		pack2.Write(str.c_str() , str.size() );

		YDAT* dat2 = new YDAT(54321);
		dat2->setSubject("伝説のﾊｯｶｰ");

		for(int i = 0; i < MAX_SUBJECT ; i ++)
		{
			dat->Append(&pack);
			sub.Age(dat);		// i*2 回目

			dat2->Append(&pack2);
			sub.Age(dat2);		// i*2+1 回目

			ASSERT(sub.No == 2);
		}

		ASSERT(sub.DATs[0]->getDatNumber() == 54321);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "54321.dat<>伝説のﾊｯｶｰ (300)\n");

		delete dat2;
		delete dat;
	}
	//A[1] A[2] .... と MAX_SUBJECT 入れてみます.
	{
		YSubject	sub;
		for(int i = 0; i < MAX_SUBJECT ; i ++)
		{
			YDAT* dat = new YDAT(i);
			dat->setSubject("クソスレ");
			RPack pack;
			str = YDATWritter::make("七資産","age","クララがたった!" , "クソスレ");
			pack.Write(str.c_str() , str.size() );
			dat->Append(&pack);
			sub.Age(dat);		// i回目

			ASSERT(sub.No == i+1);
			delete dat;
		}

		ASSERT(sub.DATs[0]->getDatNumber() == 299);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "299.dat<>クソスレ (1)\n");

		//さらに C を投下.
		RPack pack2;
		str = YDATWritter::make("ななしさん","age","ずざー" , "伝説のﾊｯｶｰ");
		pack2.Write(str.c_str() , str.size() );

		YDAT* dat2 = new YDAT(54321);
		dat2->setSubject("伝説のﾊｯｶｰ");
		dat2->Append(&pack2);
		sub.Age(dat2);		// MAX_SUBJECT回目

		ASSERT(sub.DATs[0]->getDatNumber() == 54321);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "54321.dat<>伝説のﾊｯｶｰ (1)\n");

		delete dat2;
	}
	puts("YSubject::test() OK");
}

