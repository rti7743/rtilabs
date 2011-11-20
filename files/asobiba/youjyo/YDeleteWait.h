// YDeleteWait.h: YDeleteWait クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDELETEWAIT_H__39039959_ED97_4302_ACC5_F4AEDBEFA557__INCLUDED_)
#define AFX_YDELETEWAIT_H__39039959_ED97_4302_ACC5_F4AEDBEFA557__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


//一度に削除依頼が出せる数.
const int DELETE_REQUEST_MAX	=	12;
const int DELETE_TXT_LINE		=	1024;

//マスクできる単語の最大値
const int MASK_TEXT_MAX	=	5;

class YDeleteWaitData
{
public:
	/**
	 * YDeleteWaitData:				削除に使用するデータを設定します。
	 *
	 * @param inDeleteRequest		削除依頼すれ
	 * @param inDeleteTarget		削除対処すれ
	 * @param inDeleteTime			削除が執行される時刻
	 * @param inSize				↓の数.
	 * @param inDelete				削除対象書き込み.
	 * @return  
	 */
	YDeleteWaitData(DATNUMBER inDeleteRequest , DATNUMBER inDeleteTarget , 
					time_t inDeleteTime , int inSize ,const int*	inDelete)
	{
		this->DeleteRequest = inDeleteRequest;
		this->DeleteTarget = inDeleteTarget;
		this->DeleteTime = inDeleteTime;
		this->DeleteSize = inSize;

		memcpy( this->Delete , inDelete , sizeof(int) * inSize );
	}

	/**
	 * getCompateLine:				削除に使用するデータを一行の文字列として返します。
	 *
	 * @return const string 
	 */
	const string getCompateLine() const
	{
		string ret =	num2str(this->DeleteRequest) + "<>" + 
						num2str(this->DeleteTarget) + "<>" +
						num2str(this->DeleteTime) + "<>" ;
		for(int i = 0; i < this->DeleteSize ; i ++)
		{
			ret += num2str( this->Delete[i] ) + "<>";
		}
		return ret;
	}

	bool isTimeLimit(time_t inTime) const
	{
		return inTime >= this->DeleteTime;
	}
	//あぼーん実行.
	void DoAborn(char* outShareBuffer,const string inRequestIta , const string inTargetIta);

	//あなたが管理しているリクエストですか?
	bool IsYourRequest(DATNUMBER inDeleteRequest) const
	{
		return inDeleteRequest == this->DeleteRequest;
	}

	DATNUMBER getDeleteTarget() const
	{
		return this->DeleteTarget;
	}


	//DeleteTime でソートします.
	static bool Compre(class YDeleteWaitData *const _X, class YDeleteWaitData *const _Y)
	{
		return _X->DeleteTime < _Y->DeleteTime;
	}


private:
	DATNUMBER	DeleteRequest;		//削除依頼すれ
	DATNUMBER	DeleteTarget;		//削除対処すれ

	time_t		DeleteTime;			//削除が執行される時刻

	int			DeleteSize;			//↓の数.
	int			Delete[DELETE_REQUEST_MAX];	//削除対象書き込み.
};

class YDeleteWait  
{
public:
	YDeleteWait();
	virtual ~YDeleteWait();

	void Load(const string inBoard) ;
	void Save(const string inBoard) ;

	/**
	 * Check:				時間がすぎていたらあぼーんする.
	 *
	 * @param outShareBuffer
	 * @param inRequestIta 
	 * @param inTargetIta 
	 * @return void 
	 */
	void Check(char* outShareBuffer,const string inRequestIta , const string inTargetIta);
	//削除依頼の提出.
	void Presentation(DATNUMBER inDeleteRequest , DATNUMBER inDeleteTarget , time_t inDeleteTime , int inSize ,const int*	inDelete);
	//削除依頼のキャンセル.
	bool Cancel(DATNUMBER inDeleteRequest);
	//あなたが保持している削除依頼ですか?
	DATNUMBER IsYourRequest(DATNUMBER inDeleteRequest);

	static void YDeleteWait::test();

private:
	//すべて削除
	void YDeleteWait::Clear();

		//削除するデータ.
	//常に、削除する時刻をキーに昇順にソートされています.
	typedef deque<YDeleteWaitData*>	YDELETE_WAIT_DATA;
	YDELETE_WAIT_DATA	Pool;
};

#endif // !defined(AFX_YDELETEWAIT_H__39039959_ED97_4302_ACC5_F4AEDBEFA557__INCLUDED_)
