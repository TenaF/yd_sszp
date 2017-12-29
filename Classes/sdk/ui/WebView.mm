

//#include "CommonData.h"
#include "WebView.h"
#include "WebViewImp.h"


CWebView::CWebView(void)
	:m_pWebObject(NULL)
    ,m_pfnBeforeOpenUrl(NULL)
    ,m_pfnCompleteOpenUrl(NULL)
    ,m_pfnOpenUrlError(NULL)
{
    
}
CWebViewImp* pWebObject;

CWebView::~CWebView(void)
{
    if(m_pWebObject)
	{
		[reinterpret_cast<CWebViewImp*>(m_pWebObject) release];
		m_pWebObject =NULL;
	}
}

bool CWebView::init()
{
    if(!CCLayer::init())
        return false;
    
    pWebObject =[[CWebViewImp alloc] init];
    m_pWebObject = pWebObject;
    [pWebObject setWebView:this];
    return true;
}

bool CWebView::closeWebView()
{
   
    
//    if (m_pWebObject)
//    {
//        return [reinterpret_cast<CWebViewImp*>(m_pWebObject) closeWebView];
//    }
    
    if (pWebObject)
    {
        [pWebObject closeWebView];
    }
//    if (m_pWebObject == NULL)
//    {
//        CCLog("没有初始化");
//        assert(false);
//        
//    }
    return false;
}

bool CWebView::openURL(const char *pUrl , int webNumber)
{
    if(m_pWebObject == NULL)
    {
        CCLog("没有初始化");
        assert(false);
        return false;
    }
    
    return [reinterpret_cast<CWebViewImp*>(m_pWebObject) openUrl:pUrl pWebNumber:webNumber];
}

void CWebView::cleanup()
{
	CCLayer::cleanup();
	if(m_pWebObject)
	{
		[reinterpret_cast<CWebViewImp*>(m_pWebObject) release];
		m_pWebObject =NULL;
	}
}