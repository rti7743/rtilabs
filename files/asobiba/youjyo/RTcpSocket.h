// RTcpSocket.h: RTcpSocket クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTCPSOCKET_H__5D7FD354_C38E_4942_AE2F_E627747B7D39__INCLUDED_)
#define AFX_RTCPSOCKET_H__5D7FD354_C38E_4942_AE2F_E627747B7D39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RSocket.h"

class RTcpSocket  : public RSocket
{
public:
	RTcpSocket();
	virtual ~RTcpSocket();

	/**
	 * Open:			TCP/IPのソケットを作成する.
	 *
	 * @throws			RIOCannotOpenException		開けなかったとき
	 */
	void Open( ) throw(RIOCannotOpenException);
	

};

#endif // !defined(AFX_RTCPSOCKET_H__5D7FD354_C38E_4942_AE2F_E627747B7D39__INCLUDED_)
