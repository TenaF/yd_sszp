#include "sdk/sdk.h"
#include "sdk/message/SendMsg.h"
#include "WebView.h"

#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
#include <ole2.h>
#include <Windows.h>
#include <comutil.h>
#include <Mshtml.h>
#include <ExDispid.h>
#include <Exdisp.h>
#include <mshtmhst.h>

#pragma comment(lib,"iphlpapi.lib")

#if defined(DEBUG)||defined(_DEBUG)
#pragma comment(lib,"comsuppwd.lib")
#else
#pragma comment(lib,"comsuppw.lib")
#endif


template<typename T>
class CSafeObject
{
public:
	CSafeObject(T* pObject = NULL)
	:m_pObject(pObject)
	{
	}

	CSafeObject(CSafeObject<T>& rhs)
	{
		m_pObject = rhs->m_pObject;
		if(m_pObject)
		m_pObject->AddRef();
	}

	~CSafeObject()
	{
		//CC_SAFE_RELEASE_NULL(m_pObject);
		if (m_pObject)m_pObject->Release();

	}

	bool operator == (T* pObject)
	{
		return m_pObject == pObject;
	}

	bool operator == (CSafeObject& rhs)
	{
		return m_pObject == rhs.m_pObject;
	}

	CSafeObject& operator = (T* pObject)
	{
		Attach(pObject);
		return *this;
	}

	CSafeObject& operator = (CSafeObject& rhs)
	{
		if(&rhs!=this&&rhs.m_pObject !=m_pObject)
		{
			if(m_pObject)
			m_pObject->Release();
			m_pObject = rhs.m_pObject;
			if(m_pObject)
			m_pObject->AddRef();
		}

		return *this;
	}

	T* operator ->()
	{
		return m_pObject;
	}

	T& operator*()
	{
		return *m_pObject;
	}

	bool operator !()
	{
		return !m_pObject;
	}

	operator T* &()
	{
		return m_pObject;
	}

	void Attach(T* pObject)
	{
		if(pObject!=m_pObject)
		{
			if(m_pObject)
			m_pObject->Release();

			m_pObject = pObject;
		}

	}

	T* Dettach(void)
	{
		T* pObject = m_pObject;
		m_pObject = NULL;
		return pObject;
	}

	T* GetPoint(void)
	{
		return m_pObject;
	}

	T* GetPoint(void)const
	{
		return m_pObject;
	}

	bool IsNull(void)const
	{
		return m_pObject==NULL;
	}

	ULONG Release(void)
	{
		if(m_pObject)
		{
			ULONG lRet = m_pObject->Release();
			m_pObject = NULL;
			return lRet;
		}
		else
		return 0;
	}

	ULONG AddRef(void)
	{
		return m_pObject?m_pObject->AddRef():0;
	}
	private:
	T* m_pObject;
};



class CWebViewImp :private IOleClientSite
,private IOleInPlaceSite
,private IOleInPlaceFrame
,private DWebBrowserEvents2
,private IDocHostUIHandler2
{

public:

	CWebViewImp(CWebView* pWebView):m_pWebView(pWebView),m_hWebBrowser(NULL)
	{
		static LRESULT lInitCom = ::CoInitialize( NULL );
	}

	~CWebViewImp() {Destroy();}

	bool Create(int nLeft,int nTop,int nWidht,int nHeight)
	{
		for(;;)
		{
			///实例化WEBBROWSER
			IWebBrowser2* pWebBrowser;
			HRESULT hr = CoCreateInstance(CLSID_WebBrowser,NULL,CLSCTX_SERVER,IID_IWebBrowser2,reinterpret_cast<void**>(&pWebBrowser));
			if(FAILED(hr))
			break;

			m_pWebBrowser = pWebBrowser;

			IUnknown* pUnknown;

			hr = m_pWebBrowser->QueryInterface(IID_IOleObject,reinterpret_cast<void**>(&pUnknown));
			if(FAILED(hr))
			break;

			///关联Ole容器
			CSafeObject<IOleObject> pOleObject = reinterpret_cast<IOleObject*>(pUnknown);
			hr = pOleObject->SetClientSite(this);
			if(FAILED(hr))
			break;

			hr = ::OleSetContainedObject(m_pWebBrowser,TRUE);
			if(FAILED(hr))
			break;

			RECT rcClient;
			rcClient.left = nLeft;
			rcClient.top = nTop;
			rcClient.right = nLeft + nWidht;
			rcClient.bottom = nHeight;

			//RECT rcOlePos = rcClient;
			//rcOlePos.right = 0;
			//rcOlePos.left = -rcClient.right;

			HWND hParent = NULL;//CCEGLView::sharedOpenGLView()->getHWnd();
			//CCEGLView * pEGLView = Director::getInstance()->getOpenGLView();
			//HWND hParent = pEGLView->getHWnd();
			long lStyle = ::GetWindowLong(hParent,GWL_STYLE);
			::SetWindowLong(hParent,GWL_STYLE,lStyle|WS_CLIPCHILDREN);

			hr = pOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE,NULL,this,0,
					hParent,&rcClient);
			if(FAILED(hr))
			break;

			///设置对象位置并隐藏
			hr = pOleObject->QueryInterface(IID_IOleWindow,reinterpret_cast<void**>(&pUnknown));
			if(FAILED(hr))
			break;

			CSafeObject<IOleWindow> pOleWindow = reinterpret_cast<IOleWindow*>(pUnknown);
			hr = pOleWindow->GetWindow(&m_hWebBrowser);
			if(FAILED(hr))
			break;

			SetWindowPos(m_hWebBrowser,NULL,nLeft,nTop,nWidht,nHeight,SWP_NOZORDER);

			hr = m_pWebBrowser->QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void**>(&pUnknown));
			if(FAILED(hr))
			break;

			///获得连接点以获取WEBBROWSER事件
			CSafeObject<IConnectionPointContainer> pConnectPointContainer = reinterpret_cast<IConnectionPointContainer*>(pUnknown);

			hr = pConnectPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2,reinterpret_cast<IConnectionPoint**>(&pUnknown));
			if(FAILED(hr))
			break;

			CSafeObject<IConnectionPoint> pConnectPoint = reinterpret_cast<IConnectionPoint*>(pUnknown);

			DWORD dwCookie = 0;
			hr = pConnectPoint->Advise(dynamic_cast<DWebBrowserEvents2*>(this),&dwCookie);
			if(FAILED(hr))
			break;

			///隐藏WEBBROWSER的菜单栏、地址栏、状态栏、工具栏
			m_pWebBrowser->put_MenuBar(VARIANT_FALSE);
			m_pWebBrowser->put_AddressBar(VARIANT_FALSE);
			m_pWebBrowser->put_StatusBar(VARIANT_FALSE);
			m_pWebBrowser->put_ToolBar(VARIANT_FALSE);
			m_pWebBrowser->put_Silent(VARIANT_TRUE);

			return true;
		}

		Destroy();
		return false;
	}

	void Destroy(void)
	{
		if(!m_pWebBrowser.IsNull())
		{
			m_pWebBrowser->Quit();
			m_pWebBrowser.Release();
			DestroyWindow(m_hWebBrowser);
			m_hWebBrowser = NULL;
		}
	}

	bool openURL(const char * pUrl)
	{
		if(m_pWebBrowser.IsNull())
		return false;

		_variant_t vEmpty;
		_variant_t vUrl = NULL;			//CHelper::StringAToUtf16(pUrl).c_str();
		return SUCCEEDED(m_pWebBrowser->Navigate2(&vUrl,&vEmpty,&vEmpty,&vEmpty,&vEmpty));
	}

private:
	virtual ULONG STDMETHODCALLTYPE AddRef(void) {return 1;}

	virtual ULONG STDMETHODCALLTYPE Release(void) {return 1;}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id,void** ppUnkown)
	{
		if(id== IID_IOleClientSite)
		*ppUnkown = dynamic_cast<IOleObject*>(this);
		else if(id == IID_IUnknown)
		*ppUnkown = this;
		else if(id == IID_IDispatch)
		*ppUnkown = dynamic_cast<IDispatch*>(this);
		else if(id == IID_IOleInPlaceSite)
		*ppUnkown = dynamic_cast<IOleInPlaceSite*>(this);
		else if(id == IID_IOleInPlaceFrame)
		*ppUnkown = dynamic_cast<IOleInPlaceFrame*>(this);
		else if(id == IID_IOleInPlaceUIWindow)
		*ppUnkown = dynamic_cast<IOleInPlaceUIWindow*>(this);
		else if(id == DIID_DWebBrowserEvents2)
		*ppUnkown = dynamic_cast<DWebBrowserEvents2*>(this);
		else if(id==DIID_HTMLElementEvents2)
		*ppUnkown = dynamic_cast<HTMLElementEvents2*>(this);
		else if(id == DIID_DWebBrowserEvents)
		*ppUnkown = dynamic_cast<DWebBrowserEvents*>(this);
		else if(id == IID_IDocHostUIHandler||id == IID_IDocHostUIHandler2)
		*ppUnkown = dynamic_cast<IDocHostUIHandler2*>(this);
		else
		{
			*ppUnkown = 0;
			return E_NOINTERFACE;
		}

		return S_OK;
	}
	///IOleClientSite
	virtual HRESULT STDMETHODCALLTYPE SaveObject( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE GetMoniker( DWORD , DWORD , __RPC__deref_out_opt IMoniker ** ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE GetContainer( __RPC__deref_out_opt IOleContainer **ppContainer ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE ShowObject( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnShowWindow( BOOL fShow ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout( void ) {return S_OK;}
	/////////////////////////////////////////////////////////////////
	//IOleWindow
	virtual HRESULT STDMETHODCALLTYPE GetWindow( __RPC__deref_out_opt HWND *phwnd )
	{
		//*phwnd = CCEGLView::sharedOpenGLView()->getHWnd();
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( BOOL fEnterMode ) {return E_NOTIMPL;}
	//IOleInPlaceSite
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnUIActivate( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE GetWindowContext( __RPC__deref_out_opt IOleInPlaceFrame **ppFrame, __RPC__deref_out_opt IOleInPlaceUIWindow **ppDoc, __RPC__out LPRECT lprcPosRect, __RPC__out LPRECT lprcClipRect, __RPC__inout LPOLEINPLACEFRAMEINFO lpFrameInfo )
	{
		*ppFrame = dynamic_cast<IOleInPlaceFrame*>(this);
		lpFrameInfo->cb = sizeof(OLEINPLACEFRAMEINFO);
		GetWindow(&lpFrameInfo->hwndFrame);
		lpFrameInfo->cAccelEntries = 0;
		lpFrameInfo->fMDIApp = FALSE;
		lpFrameInfo->haccel = NULL;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Scroll( SIZE scrollExtant ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate( BOOL fUndoable ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange( __RPC__in LPCRECT lprcPosRect ) {return S_OK;}

	///IOleInPlaceUIWindow
	virtual HRESULT STDMETHODCALLTYPE GetBorder( __RPC__out LPRECT lprectBorder ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE RequestBorderSpace( __RPC__in_opt LPCBORDERWIDTHS pborderwidths ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE SetBorderSpace( __RPC__in_opt LPCBORDERWIDTHS pborderwidths ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE SetActiveObject( __RPC__in_opt IOleInPlaceActiveObject *pActiveObject, __RPC__in_opt LPCOLESTR pszObjName ) {return S_OK;}

	///IOleInPlaceFrame
	virtual HRESULT STDMETHODCALLTYPE InsertMenus( __RPC__in HMENU hmenuShared, __RPC__inout LPOLEMENUGROUPWIDTHS lpMenuWidths ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE SetMenu( __RPC__in HMENU hmenuShared, __RPC__in HOLEMENU holemenu, __RPC__in HWND hwndActiveObject ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE RemoveMenus( __RPC__in HMENU hmenuShared ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE SetStatusText( __RPC__in_opt LPCOLESTR pszStatusText ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE EnableModeless( BOOL fEnable ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( __RPC__in LPMSG lpmsg, WORD wID ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID ) {return E_NOTIMPL;}

	///IDispatch
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( __RPC__out UINT *pctinfo ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo **ppTInfo ) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( __RPC__in REFIID riid, __RPC__in_ecount_full(cNames ) LPOLESTR *rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr )
	{
		switch(dispIdMember)
		{
			case DISPID_NEWWINDOW2:        //弹出新窗口
			*pDispParams->rgvarg[0].pboolVal = VARIANT_TRUE;
			openURL(m_strNewUrl.c_str());
			break;
			case DISPID_BEFORENAVIGATE2://打开/跳转网址
			{
				///参数1：是否取消；参数2：附带消息头；参数3：POSTDATA；参数4：框架名；参数5：保留参数；参数6:URL地址;参数7：接口
				// 			if(pDispParams->rgvarg[6].pdispVal == m_pWebBrowser)
				// 			{
				//*pDispParams->rgvarg[0].pboolVal = m_pWebView->onNotifyOpenUrlStart(CHelper::Utf16ToStringA(pDispParams->rgvarg[5].pvarVal->bstrVal).c_str())?VARIANT_FALSE:VARIANT_TRUE;
			}
			break;
			case DISPID_DOCUMENTCOMPLETE:				//网页加载完成
			if(pDispParams->rgvarg[1].pdispVal == m_pWebBrowser)
			m_pWebView->onNotifyOpenUrlComplete();
			break;
// 		case DISPID_NAVIGATECOMPLETE2://打开完成
// 			if(pDispParams->rgvarg[1].pdispVal == m_pWebBrowser)
// 				HookWebWindow();
// 			break;
			case DISPID_NAVIGATEERROR://加载失败
			{				///参数1：是否取消；参数2，错误码；参数3：框架名；参数4：URL；参数5：接口
							//if(pDispParams->rgvarg[4].pdispVal == m_pWebBrowser)
							//*pDispParams->rgvarg[0].pboolVal =  m_pWebView->onNotifyOpenUrlError(CHelper::Utf16ToStringA(pDispParams->rgvarg[3].pvarVal->bstrVal).c_str())?VARIANT_FALSE:VARIANT_TRUE;
			}
			break;
		}
		return S_OK;
	}

	///IDocHostUIHandler
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( DOCHOSTUIINFO *pInfo ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE ShowUI( DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE HideUI( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( BOOL fActivate ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( BOOL fActivate ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( __out LPOLESTR *pchKey, DWORD dw ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( IDropTarget *pDropTarget, IDropTarget **ppDropTarget ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE GetExternal( IDispatch **ppDispatch ) {return S_OK;}

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( DWORD dwTranslate, __in __nullterminated OLECHAR *pchURLIn, __out OLECHAR **ppchURLOut )
	{
		//m_strNewUrl = CHelper::Utf16ToStringA(pchURLIn);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject( IDataObject *pDO, IDataObject **ppDORet ) {return E_NOTIMPL;}

	////IDocHostUIHandler2
	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( LPOLESTR *pchKey, DWORD dw ) {return E_NOTIMPL;}
	private:
	CWebView* m_pWebView;
	CSafeObject<IWebBrowser2> m_pWebBrowser;					//浏览器接口
	StringA m_strNewUrl;
	HWND m_hWebBrowser;

};


//如果是安卓平台================<<
#elif CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM
#include "platform/android/jni/JniHelper.h"
extern "C"
{
	bool Java_org_cocos2dx_cpp_AppActivity_onNotifyOpenUrlStart(JNIEnv* ,jobject jthiz,jint nPointToWebView,jstring strUrl)
	{
		return reinterpret_cast<CWebView*>(nPointToWebView)->onNotifyOpenUrlStart(JniHelper::jstring2string(strUrl).c_str());
	}

	void Java_org_cocos2dx_cpp_AppActivity_onNotifyOpenUrlComplete(JNIEnv* ,jobject jthiz,jint nPointToWebView)
	{
		reinterpret_cast<CWebView*>(nPointToWebView)->onNotifyOpenUrlComplete();
	}

	bool Java_org_cocos2dx_cpp_AppActivity_onNotifyOpenUrlError(JNIEnv* ,jobject jthiz,jint nPointToWebView,jstring strError)
	{
		reinterpret_cast<CWebView*>(nPointToWebView)->onNotifyOpenUrlError(JniHelper::jstring2string(strError).c_str());
		return false;
	}

	//刷新用户的金币
	void Java_org_cocos2dx_cpp_AppActivity_onNotifyRefreshScore()
	{
		//SendMsg::getUserInfo();
	}
};
#endif
//如果是安卓平台================>>



CWebView::CWebView(void) :
		m_pWebObject(NULL), m_pfnBeforeOpenUrl(NULL), m_pfnCompleteOpenUrl(NULL), m_pfnOpenUrlError(NULL) {
}

CWebView::~CWebView(void) {
#if CC_TARGET_PLATFORM	== CC_PLATFORM_WIN32
	if(m_pWebObject)
	{
		delete reinterpret_cast<CWebViewImp*>(m_pWebObject);
		m_pWebObject = NULL;
	}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID //安卓释放
	if(m_pWebObject)
	{
		JniMethodInfo info;
		bool bFind = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","destroyWeb","()V");
		if(!bFind)
		{
			CCLog("获取destroyWeb函数失败");
		}
		info.env->CallStaticVoidMethod(info.classID,info.methodID);
		m_pWebObject = NULL;
	}
#endif

}


void CWebView::showWeb() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
	if (m_pWebObject)
	{
		JniMethodInfo info;
		bool bFind = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "showWeb", "()V");
		if (!bFind)
		{
			CCLog("获取destroyWeb函数失败");
		}
		info.env->CallStaticVoidMethod(info.classID, info.methodID);
	}
#endif

}

void CWebView::hideWeb() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
	if (m_pWebObject)
	{
		JniMethodInfo info;
		bool bFind = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "hideWeb", "()V");
		if (!bFind)
		{
			CCLog("获取destroyWeb函数失败");
		}
		info.env->CallStaticVoidMethod(info.classID, info.methodID);
	}
#endif

}

bool CWebView::init() {
	if (!CCLayer::init())
		return false;

#if CC_TARGET_PLATFORM	== CC_PLATFORM_WIN32
	CWebViewImp* pWebView = new CWebViewImp(this);
	if(pWebView == NULL)
	return false;

	m_pWebObject = pWebView;
#endif

	return true;
}
bool CWebView::closeWebView() {
	return true;
}

bool CWebView::openURL(const char* pUrl ,int webNumber) {

	CCSize size = this->getContentSize();//ContentSize
	Vec2 pt = this->getPosition();//左上角位置
	
	
#if CC_TARGET_PLATFORM	== CC_PLATFORM_WIN32
	//MessageBox("Windows暂不支持webview","提示");
	return false;

	if(!m_pWebObject)
	return false;

	CWebViewImp* pWebView = reinterpret_cast<CWebViewImp*>(m_pWebObject);
	pWebView->Destroy();	
	if (!pWebView->Create(static_cast<int>(pt.x), static_cast<int>(pt.y), static_cast<int>(size.width), static_cast<int>(size.height))
			||!pWebView->openURL(pUrl))
	return true;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID//安卓平台调用
	//传参数 left  top  width height


	JniMethodInfo info;
	bool bFind = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","openUrl","(ILjava/lang/String;IIII)V");
	if(!bFind)
	{
		CCLog("获取openUrl函数失败");
		return false;
	}

	jstring strUrl = info.env->NewStringUTF(pUrl);

	/*info.env->CallStaticVoidMethod(info.classID,info.methodID,reinterpret_cast<jint>(this)
		,strUrl,static_cast<int>(webNumber),static_cast<int>(pt.y),
		static_cast<int>(size.width),static_cast<int>(size.height));*/

	info.env->CallStaticVoidMethod(info.classID,info.methodID,reinterpret_cast<jint>(this)
		,strUrl,static_cast<int>(pt.x),static_cast<int>(pt.y),
			static_cast<int>(size.width),static_cast<int>(size.height));

	//info.env.getSettings().setUseWideViewPort(true);
	//info.env.getSettings().setLoadWithOverviewMode(true);

	m_pWebObject = this;
#endif
	return true;
}

void CWebView::cleanup() {

#if CC_TARGET_PLATFORM	== CC_PLATFORM_WIN32
	if(m_pWebObject)
	{
		delete reinterpret_cast<CWebViewImp*>(m_pWebObject);
		m_pWebObject = NULL;
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if(m_pWebObject)
	{
		JniMethodInfo info;
		bool bFind = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/AppActivity","destroyWeb","()V");
		if(!bFind)
		{
			CCLog("获取destroyWeb函数失败");
		}
		info.env->CallStaticVoidMethod(info.classID,info.methodID);
		m_pWebObject = NULL;
	}
#endif

}
