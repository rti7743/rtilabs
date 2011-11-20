// RWebBrowser.cpp: RWebBrowser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RWebBrowser.h"

#include "Filer.h"
#include "RfilesPreviewOne.h"
#include "DisplayFactory.h"
#include "RProgressBar.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RWebBrowser::RWebBrowser()
{
	Parent = NULL;
}

RWebBrowser::~RWebBrowser()
{
}

//作成.
//失敗	RNotSupportException 
//		RDoNotInitaliseException
void RWebBrowser::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{

	RActiveXWindow::Create(inINstance,"Shell.Explorer.2" , IID_IWebBrowser2, (void**)&WebBrowser,isParentWindow,0, 0 , 0);

	// シンクオブジェクトの作成.
	CComObject<RWebBrowserSink>::CreateInstance( &Sink);	//この中で new しているらしい.

	// イベントをDispatchするよう、IEコントロールにお願い.
	HRESULT hr = Sink->AdviseTo( this , WebBrowser);

}

void RWebBrowser::Navigate2(const string inUrl)
{
	ASSERT(WebBrowser != NULL);

    CComVariant ve;
    CComVariant vurl( inUrl.c_str() );
    WebBrowser->put_Visible(VARIANT_TRUE);
	WebBrowser->put_MenuBar(VARIANT_TRUE);
	WebBrowser->put_AddressBar(VARIANT_TRUE);
	WebBrowser->put_StatusBar(VARIANT_TRUE);
	WebBrowser->put_ToolBar(VARIANT_TRUE);
    WebBrowser->Navigate2(&vurl, &ve, &ve, &ve, &ve);
}

//ブラウズが開始されたときに呼ばれる.
HRESULT RWebBrowser::OnBeforNavigate2(IDispatch* pDisp, VARIANT* url, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel)
{
	USES_CONVERSION;

	const char* p = OLE2CT(url->bstrVal);
	Parent->OnQuickChange( string(p) );

	return S_OK;
}

//終了のとき呼ばれる
HRESULT RWebBrowser::OnQuit()
{
	Sink->DispEventUnadvise( WebBrowser );
	Sink = NULL;

	return S_OK;
}


//新しいウィンドウが開かれるときに呼ばれる
void RWebBrowser::OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel)
{
	//もともと、パス -> ウィンドウ作成　という考え方でいましたが、
	//OnNewWindow は ウィンドウ作成 -> パスという逆の手順で作らなくてはならないので、
	//とても汚くなるのですが、強引にウィンドウを作ります。

	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );

	Rfiles * rfiles = NULL;
	DisplayWebBrowserView* dis = NULL;
	Window* win = NULL;

	try
	{
		win = sub->NewWindow("");

		rfiles = new RfilesPreviewOne("");
		rfiles->MakeTable( __FILECOLUMN_TYPE_NAME , true);
		rfiles->ls();

		dis = (DisplayWebBrowserView*) DisplayFactory::New(__DISPLAYTYPE_PREVIEW_WEBBROWSER , win ,__RFILETYPE_PREVIEWONE);

		win->UpdateWindow( rfiles , dis );
	}
	catch(RException e)
	{
		if (rfiles) rfiles->Dec();
		if (dis) dis->Dec();

		if (win) sub->CloseWindow(win);

		*Cancel = VARIANT_TRUE;
		return ;
	}

	const RWebBrowser*	browser = dis->getBrowser();
	browser->WebBrowser->QueryInterface(IID_IDispatch, (void**)ppDisp);

	*Cancel = VARIANT_FALSE;
}


//読み込みが終わったとき
void RWebBrowser::OnNavigateComplete2(IDispatch* pDisp, VARIANT* URL)
{
}

//エラーが出たとき
void RWebBrowser::OnNavigateError(IDispatch* pDisp,VARIANT* URL, VARIANT* TargetFrameName,	VARIANT* StatusCode,VARIANT_BOOL* &Cancel)
{
}

//タイトルが変更されました
void RWebBrowser::OnTitleChange(BSTR Text)
{
	USES_CONVERSION;
	Title = OLE2CT(Text);

	//親に通知
	Parent->OnTitleChange();
}

//ステータスバーのテキストが変更されました
void RWebBrowser::OnStatusTextChange(BSTR Text)
{
	if ( ocslen(Text) == 0 ) return ;

	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );

	USES_CONVERSION;
	const char*  t = OLE2CT(Text);
	if (t != NULL)	sub->getStatue()->setStatue( string(t) );
	else			sub->getStatue()->setStatue( "" );
}

//プログレスバーが変化しました.
void RWebBrowser::OnProgressChange(long Progress,	long ProgressMax)
{
	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );

	if (ProgressMax == 0)	//読み込みが完了したりすると 0 がくるらしい.
		sub->getStatue()->setProgressBar( 0 );
	else
		sub->getStatue()->setProgressBar( CHANGE_SCALE( Progress , ProgressMax , 100) );
}

//描画まですべて終了したときに呼ばれる.
//OnNavigateComplete2 -> OnDocumentComplete の順番
//	OnNavigateComplete2	通信で内容をすべて取得できました。
//	OnDocumentComplete	描画まで終わりました。
void RWebBrowser::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
{
	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );
	sub->getStatue()->setProgressBar( 100 );
}


//タイトルを取得する
string RWebBrowser::getTitle() const 
{
	return Title;
}

