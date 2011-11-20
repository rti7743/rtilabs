// RWebBrowser.h: RWebBrowser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEBBROWSER_H__624F33B3_0EFF_4348_AE82_F4A9C38F4257__INCLUDED_)
#define AFX_RWEBBROWSER_H__624F33B3_0EFF_4348_AE82_F4A9C38F4257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RWebBrowser;
class RWebBrowserSink;
class DisplayWebBrowserView;

#include "comm.h"
#include "RActiveXWindow.h"
#include <Exdisp.h>
#include <Exdispid.h>

//Webブラウザー
class RWebBrowser :
	public RActiveXWindow  
{
public:
	RWebBrowser();
	virtual ~RWebBrowser();

	//作成.
	//失敗	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//親のセット.
	void setParent(DisplayWebBrowserView* inParent)
	{
		Parent = inParent;
	}

	//表示.
	void Navigate2(const string inUrl);
	//タイトルを取得する
	string getTitle() const ;

public:
	//ブラウズが開始されたときに呼ばれる.
	HRESULT OnBeforNavigate2(IDispatch* pDisp, VARIANT* url, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel);
	//終了のとき呼ばれる
	HRESULT OnQuit();
	//新しいウィンドウが開かれるときに呼ばれる
	void OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel);

	//読み込みが終わったとき
	void OnNavigateComplete2(IDispatch* pDisp, VARIANT* URL);

	//エラーが出たとき
	void OnNavigateError(IDispatch* pDisp,VARIANT* URL, VARIANT* TargetFrameName,	VARIANT* StatusCode,VARIANT_BOOL* &Cancel);

	//タイトルが変更されました
	void OnTitleChange(BSTR Text);

	//ステータスバーのテキストが変更されました
	void OnStatusTextChange(BSTR Text);

	//プログレスバーが変化しました.
	void OnProgressChange(long Progress,	long ProgressMax);

	//描画まですべて終了したときに呼ばれる.
	//OnNavigateComplete2 -> OnDocumentComplete の順番
	//	OnNavigateComplete2	通信で内容をすべて取得できました。
	//	OnDocumentComplete	描画まで終わりました。
	void OnDocumentComplete(IDispatch* pDisp, VARIANT* URL);


private:
	CComPtr<IWebBrowser2>	WebBrowser;
	CComObject<RWebBrowserSink>* Sink;

	DisplayWebBrowserView*		Parent;
	//タイトル.
	string Title;
};



#define SINKID_COUNTEREVENTS 1

//ブラウザで発生したイベントを捕まえる.
//参考	http://www.nitoyon.com/vc/tips/ie_component.htm
//		http://www.s34.co.jp/cpptechdoc/misc/comevent/index.html
class RWebBrowserSink :
	public CComObjectRootEx<CComSingleThreadModel>	,
	public IDispEventImpl< SINKID_COUNTEREVENTS ,RWebBrowserSink , &DIID_DWebBrowserEvents2 , &LIBID_SHDocVw, 1, 0>
{
private:
	BEGIN_COM_MAP(RWebBrowserSink)
		COM_INTERFACE_ENTRY_IID(DIID_DWebBrowserEvents2, RWebBrowserSink)
	END_COM_MAP()
	BEGIN_SINK_MAP(RWebBrowserSink)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_BEFORENAVIGATE2 , OnBeforNavigate2)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_ONQUIT , OnQuit)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_NEWWINDOW2 , OnNewWindow2)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_NAVIGATECOMPLETE2 , OnNavigateComplete2)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_NAVIGATEERROR , OnNavigateError)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_TITLECHANGE , OnTitleChange)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_STATUSTEXTCHANGE , OnStatusTextChange)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_PROGRESSCHANGE , OnProgressChange)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_DOCUMENTCOMPLETE , OnDocumentComplete)
	END_SINK_MAP()
public:
	HRESULT AdviseTo(RWebBrowser* inCallBack , IUnknown* pUnkIE)
	{
		//コールバック登録.
		CallBack = inCallBack;

		// タイプ ライブラリへのポインタやバージョン情報の設定
		AtlGetObjectSourceInterface( pUnkIE, &m_libid, &m_iid, &m_wMajorVerNum, &m_wMinorVerNum) ;
		// イベントをDispatch
		HRESULT hr = this->DispEventAdvise( pUnkIE);
		return hr ;
	}
private:
    
	//ブラウズが開始されたときに呼ばれる.
	HRESULT _stdcall OnBeforNavigate2(IDispatch* pDisp, VARIANT* url, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel)
	{
		return CallBack->OnBeforNavigate2( pDisp, url, Flags, TargetFrameName, PostData, Headers, Cancel);
	}
	//終了のとき呼ばれる
	HRESULT _stdcall OnQuit()
	{
		return CallBack->OnQuit();
	}
	//新しいウィンドウが開かれるときに呼ばれる
	void _stdcall OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel)
	{
		CallBack->OnNewWindow2(ppDisp , Cancel );
	}

	//読み込みが終わったとき
	void _stdcall OnNavigateComplete2(IDispatch* pDisp, VARIANT* URL)
	{
		CallBack->OnNavigateComplete2(pDisp,URL);
	}

	//エラーが出たとき
	void _stdcall OnNavigateError(IDispatch* pDisp,VARIANT* URL, VARIANT* TargetFrameName,	VARIANT* StatusCode,VARIANT_BOOL* &Cancel)
	{
		CallBack->OnNavigateError(pDisp, URL, TargetFrameName,	StatusCode,Cancel);
	}

	//タイトルが変更されました
	void _stdcall OnTitleChange(BSTR Text)
	{
		CallBack->OnTitleChange(Text);
	}

	//ステータスバーのテキストが変更されました
	void _stdcall OnStatusTextChange(BSTR Text)
	{
		CallBack->OnStatusTextChange(Text);
	}
	
	//プログレスバーが変化しました.
	void _stdcall OnProgressChange(long Progress,	long ProgressMax)
	{
		CallBack->OnProgressChange(Progress , ProgressMax);
	}

	//描画まですべて終了したときに呼ばれる.
	//OnNavigateComplete2 -> OnDocumentComplete の順番
	//	OnNavigateComplete2	通信で内容をすべて取得できました。
	//	OnDocumentComplete	描画まで終わりました。
	void _stdcall OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
	{
		CallBack->OnDocumentComplete(pDisp , URL);
	}

	//コールバック先.
	RWebBrowser* CallBack;
};

#include "DisplayWebBrowserView.h"

#endif // !defined(AFX_RWEBBROWSER_H__624F33B3_0EFF_4348_AE82_F4A9C38F4257__INCLUDED_)
