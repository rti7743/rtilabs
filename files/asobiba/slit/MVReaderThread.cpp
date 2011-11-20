// MVReaderThread.cpp: MVReaderThread クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVReaderThread.h"
#include "MVImage.h"
#include "MVStaticZLib.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MVReaderThread::MVReaderThread()
{
	this->PoolData = NULL;
	this->PoolPageData = NULL;
}

MVReaderThread::~MVReaderThread()
{
	delete [] PoolData;

	//成果物を誰も受け取ってくれなかったら、自分で処分する.
	delete [] PoolPageData;
}

/**
 * Setting:			作業に必要な材料をスレッドに渡す
 *					この時スレッドはまだ動作していない。
 *
 * @param poolData				圧縮されているデータ (スレッドが作業終了後、責任を持って開放します)
 * @param inSize				poolData のサイズ
 * @param inUncompressedSize	poolData を解凍した場合のサイズ
 * @param compressionMethod		圧縮に使用されているアルゴリズム
 * @param inImageConvert		変換に使用するパラメータ
 * @return void 
 */
void MVReaderThread::Setting(char* poolData , int inSize , int inUncompressedSize  , int compressionMethod ,
							 const MVImagePreConvert * inImageConvertParam)
{
	//動いてはダメよ.
	ASSERT( ! this->IsAlive() );
	ASSERT( this->PoolPageData == NULL);

	this->PoolData = poolData;
	this->Size = inSize;
	this->CompressionMethod = compressionMethod;
	this->UncompressedSize = inUncompressedSize;

	this->ImageConvertParam = *inImageConvertParam;


	ASSERT( (this->CompressionMethod == 0 && (this->UncompressedSize  == this->Size)) ||
			(this->CompressionMethod != 0)
		);
}




/**
 * TakeoverPageData:成果物をいただきます。
 *					この時スレッドは動作を終了していなければなりません.
 *
 *					一度呼ぶとスレッドは 成果物 が、呼んだ奴にわたったものだと思いデータの所有権を破棄するので注意
 *
 * @return MVPageData*	成果物	この成果物は、受け取った奴が責任を持って開放する必要があります.
 */
MVPageData* MVReaderThread::TakeoverPageData() throw(RException)
{
	//終わっていなきゃダメよ.
	ASSERT( ! this->IsAlive() );
	//結果ができていないということは、一度も動いていないよ
	ASSERT( this->PoolPageData != NULL);


	//エラーが起きているなら、エラーを例外として返す.
	if ( ! this->Error.empty() )
	{
		//成果物は壊れていると思うので破棄する.
		delete this->PoolPageData;
		this->PoolPageData = NULL;

		throw RException( this->Error );
	}

	MVPageData* ret = this->PoolPageData;

	//自分の管轄からはなれるから、もう知らねー
	this->PoolPageData = NULL;

	return ret;

}



//スレッド内部
unsigned int MVReaderThread::Run() 
{
	try
	{
		UnComp();
	}
	catch(RException & e)
	{
		this->Error = e.getMessage();
	}
	//圧縮されたデータはもう不要なので破棄します.
	delete [] PoolData;
	this->PoolData = NULL;

	return 0;
}


/**
 * UnComp:				スレッド内部で実際の展開処理を担当する.
 *
 * @return void 
 */
void MVReaderThread::UnComp()  throw(RException)
{
	//初期化していますよね
	ASSERT( this->PoolData != NULL);

	//スレッド内で生成するデータはまだ作られていませんよね
	ASSERT( this->PoolPageData == NULL);

	//成果物を収める領域.
	this->PoolPageData = new MVPageData;


	if (this->CompressionMethod == 8)
	{
		RBuffer bitmapMemory;
		//必要な分メモリを確保する.
		bitmapMemory.reserve(this->UncompressedSize);
		bitmapMemory.resize(this->UncompressedSize);

		//圧縮
		MVStaticZLib::getInstance()->UnCompressed((unsigned char*) &bitmapMemory[0] , this->UncompressedSize ,
												(const unsigned char*) this->PoolData , this->Size );

		//展開
		this->PoolPageData->Load(&bitmapMemory[0] , this->UncompressedSize);
	}
	else if (this->CompressionMethod == 0 && this->UncompressedSize == this->Size)
	{
		//無圧縮

		//展開
		this->PoolPageData->Load(this->PoolData , this->Size);
	}
	else
	{
		throw RException("未対応の zip " + num2str(this->CompressionMethod) + "形式が渡されました");
	}


	if ( this->ImageConvertParam.getJustFit() )
	{
		switch ( this->ImageConvertParam.getScrollType() )
		{
		case MVConfig::YOKO:
			{
				//縦幅にあわせる.
				double scale = (double)this->ImageConvertParam.getScreenHeight() / this->PoolPageData->getHeight();

				if (scale != 0)
				{
					this->PoolPageData->ChangeScale(this->PoolPageData->getWidth() * scale , 
													this->PoolPageData->getHeight() * scale );
				}
			}
			break;
		case MVConfig::TATE:
			{
				//横幅にあわせる.
				double scale = (double)this->ImageConvertParam.getScreenWidth() / this->PoolPageData->getWidth();

				if (scale != 0)
				{
					this->PoolPageData->ChangeScale(this->PoolPageData->getWidth() * scale , 
													this->PoolPageData->getHeight() * scale );
				}
			}
			break;
		}
	}

}

