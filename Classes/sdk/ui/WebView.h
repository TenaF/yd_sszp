/**
 * @file 	WebView.h
 * @brief	网页显示容器
 *********************************************************************/
#pragma once

#include "cocos2d.h"
using namespace cocos2d;

typedef bool (CCObject::*FNONBEFOREOPENURL)(Ref* ,const char*);
typedef void (CCObject::*FNONCOMPLETEOPENURL)(Ref*);
typedef bool (CCObject::*FNONOPENURLERROR)(Ref* ,const char*);

#define Select_WebError(fn) ((FNONOPENURLERROR)fn)
#define Select_WebComplete(fn) ((FNONCOMPLETEOPENURL)fn)
#define Select_WebBeforeOpen(fn) ((FNONBEFOREOPENURL)fn)

class CWebView: public cocos2d::CCLayer {
	public:
		CWebView(void);
		~CWebView(void);

		bool init();

		bool openURL(const char* pUrl ,int webNumber);

		void  showWeb();//显示网页
		void  hideWeb();//隐藏网页

		bool onNotifyOpenUrlStart(const char* strUrl) {
			if (m_pfnBeforeOpenUrl) {
				return (m_pObjCallBeforeOpenUrl->*m_pfnBeforeOpenUrl)(this, strUrl);
			}
			return true;
		}

		void onNotifyOpenUrlComplete() {
			if (m_pfnBeforeOpenUrl)
				(m_pObjCallCompleteOpenUrl->*m_pfnCompleteOpenUrl)(this);
		}

		bool onNotifyOpenUrlError(const char* strError) {
			if (m_pfnOpenUrlError) {
				return (m_pObjCallOpenUrlError->*m_pfnOpenUrlError)(this, strError);
			}
			return false;
		}

		void addEventOpenUrl(Ref* pObject ,FNONBEFOREOPENURL fn) {
			m_pObjCallBeforeOpenUrl = pObject;
			m_pfnBeforeOpenUrl = fn;
		}

		void addEventCompleteOpenUrl(Ref* pObject ,FNONCOMPLETEOPENURL fn) {
			m_pObjCallCompleteOpenUrl = pObject;
			m_pfnCompleteOpenUrl = fn;
		}

		void addEventOpenUrlError(Ref* pObject ,FNONOPENURLERROR fn) {
			m_pObjCallOpenUrlError = pObject;
			m_pfnOpenUrlError = fn;
		}

		bool closeWebView();
		void cleanup();

		CREATE_FUNC (CWebView);
	private:
		void * m_pWebObject;
	private:
		Ref* m_pObjCallBeforeOpenUrl;
		Ref* m_pObjCallCompleteOpenUrl;
		Ref* m_pObjCallOpenUrlError;
		FNONBEFOREOPENURL m_pfnBeforeOpenUrl;
		FNONCOMPLETEOPENURL m_pfnCompleteOpenUrl;
		FNONOPENURLERROR m_pfnOpenUrlError;

};

