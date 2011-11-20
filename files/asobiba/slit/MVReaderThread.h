// MVReaderThread.h: MVReaderThread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVREADERTHREAD_H__4966B572_8A84_4521_A660_0BA5AA3A7113__INCLUDED_)
#define AFX_MVREADERTHREAD_H__4966B572_8A84_4521_A660_0BA5AA3A7113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RThread.h"
#include "MVPageData.h"
#include "MVImagePreConvert.h"

/**
 * class MVReaderThread:		先読みスレッド
 *
 * @author 
 */
class MVReaderThread : public RThread  
{
public:
	MVReaderThread();
	virtual ~MVReaderThread();

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
	void Setting(char* poolData , int inSize , int inUncompressedSize  , int compressionMethod ,
				 const MVImagePreConvert * inImageConvertParam);

	/**
	 * TakeoverPageData:成果物をいただきます。
	 *					この時スレッドは動作を終了していなければなりません.
	 *
	 *					一度呼ぶとスレッドは 成果物 が、呼んだ奴にわたったものだと思いデータの所有権を破棄するので注意
	 *
	 * @return MVPageData*	成果物	この成果物は、受け取った奴が責任を持って開放する必要があります.
	 */
	MVPageData* TakeoverPageData() throw(RException);

private:
	//スレッド内部
	virtual unsigned int Run() ;
	/**
	 * UnComp:				スレッド内部で実際の展開処理を担当する.
	 *
	 * @return void 
	 */
	void MVReaderThread::UnComp()  throw(RException);


	char* PoolData ;
	int Size ;
	int UncompressedSize;
	int CompressionMethod ;

	MVImagePreConvert ImageConvertParam;
	string	Error;

	MVPageData* PoolPageData;
};

#endif // !defined(AFX_MVREADERTHREAD_H__4966B572_8A84_4521_A660_0BA5AA3A7113__INCLUDED_)
