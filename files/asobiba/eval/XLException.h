// XLException.h: XLException クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLEXCEPTION_H__D2331AFF_2532_48BB_AEF0_A3AD924595E1__INCLUDED_)
#define AFX_XLEXCEPTION_H__D2331AFF_2532_48BB_AEF0_A3AD924595E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * class XLException:	例外クラスの大元
 *
 * @note				なぜ、 std::exception を使わないのか?
 *						std::exception は、機能なさ杉.
 *						string ぐらい渡せないようでは困る.
 *						また、仮にstd::exceptionを継承して作ったとしても、
 *						その恩恵はたいしたことがない。
 */
class XLException  
{
public:
	/**
	 * XLException:		コンストラクタ
	 *
	 */
	XLException()
	{
	}

	/**
	 * ~XLException:	デストラクタ
	 *
	 */
	virtual ~XLException()
	{
	}

	/**
	 * XLException:				コンストラクタ
	 *
	 * @param inErrorMessage	セットするエラー
	 *
	 */
	XLException(const string & inErrorMessage)
	{
		setError(inErrorMessage);
	}

	/**
	 * getMessage:				エラーメッセージの取得
	 *
	 * @return const char*		エラーメッセージ
	 */
	const char * getMessage() const
	{
		return this->ErrorMessage.c_str();
	}
protected:
	/**
	 * setError:				エラーメッセージの設定
	 *
	 * @param inErrorMessage	設定するエラーメッセージ
	 */
	void setError(string inErrorMessage)
	{
		this->ErrorMessage = inErrorMessage;
	}
private:
	string	ErrorMessage;
};

#endif // !defined(AFX_XLEXCEPTION_H__D2331AFF_2532_48BB_AEF0_A3AD924595E1__INCLUDED_)
