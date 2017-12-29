/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "base/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "jni/JniHelper.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "CCApplication.h"
#include "base/CCDirector.h"
#include "CCGLView.h"
#include <android/log.h>
#include <jni.h>
#include <cstring>

#define  LOG_TAG    "CCApplication_android Debug"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

NS_CC_BEGIN

// sharedApplication pointer
Application * Application::sm_pSharedApplication = 0;


//CCApplication.cpp
void Application::openURL(const char* pszUrl)
{
    JniMethodInfo minfo;
        
    if(JniHelper::getStaticMethodInfo(minfo, 
        "org/cocos2dx/lib/Cocos2dxActivity", 
        "openURL", 
        "(Ljava/lang/String;)V"))
    {
        jstring StringArg1 = minfo.env->NewStringUTF(pszUrl);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, StringArg1);
        minfo.env->DeleteLocalRef(StringArg1);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void Application::copyByPaltform(const char* copyText)
{
    //��̬����
	 JniMethodInfo minfo;

     jobject jobj;
     bool b = JniHelper::getStaticMethodInfo( minfo,
	                                          "org/cocos2dx/cpp/AppActivity",   //��·��
	                                          "getInstance",                    //��̬������
	                                          "()Ljava/lang/Object;");          //��������ǲ�����������Ƿ���ֵ��

     if (!b) {
	      log("��ȡgetInstance����ʧ��");
		  return;
     }else{
	      jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
      }

	 //���󷽷�
     JniMethodInfo info;

     bool isHave = JniHelper::getMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "handleCopyToClip", "(Ljava/lang/String;)V");
     if (!isHave)
     {
	    log("��ȡopenUrl����ʧ��");
     }

     jstring nickname_ = info.env->NewStringUTF(copyText);

     info.env->CallVoidMethod(jobj, info.methodID, nickname_);
}


void Application::saveUrlToCamera(const char* urlText)
{
    //��̬����
	 JniMethodInfo minfo;

     jobject jobj;
     bool b = JniHelper::getStaticMethodInfo( minfo,
	                                          "org/cocos2dx/cpp/AppActivity",   //��·��
	                                          "getInstance",                    //��̬������
	                                          "()Ljava/lang/Object;");          //��������ǲ�����������Ƿ���ֵ��

     if (!b) {
	      log("��ȡgetInstance����ʧ��");
		  return;
     }else{
	      jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
      }

	 //���󷽷�
     JniMethodInfo info;

     bool isHave = JniHelper::getMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "saveUrlToCamera", "(Ljava/lang/String;)V");
     if (!isHave)
     {
	    log("��ȡopenUrl����ʧ��");
     }

     jstring _urlText = info.env->NewStringUTF(urlText);

     info.env->CallVoidMethod(jobj, info.methodID, _urlText);
}


Application::Application()
{
    CCAssert(! sm_pSharedApplication, "");
    sm_pSharedApplication = this;
}

Application::~Application()
{
    CCAssert(this == sm_pSharedApplication, "");
    sm_pSharedApplication = NULL;
}

int Application::run()
{
    // Initialize instance and cocos2d.
    if (! applicationDidFinishLaunching())
    {
        return 0;
    }
    
    return -1;
}

void Application::setAnimationInterval(double interval)
{
    // NYI


		cocos2d::JniMethodInfo minfo;

		if (cocos2d::JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxRenderer", "setAnimationInterval", "(D)V"))
		{
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, interval);
		}

	
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
Application* Application::getInstance()
{
    CCAssert(sm_pSharedApplication, "");
    return sm_pSharedApplication;
}

// @deprecated Use getInstance() instead
Application* Application::sharedApplication()
{
    return Application::getInstance();
}

const char * Application::getCurrentLanguageCode()
{
    static char code[3]={0};
    strncpy(code,getCurrentLanguageJNI().c_str(),2);
    code[2]='\0';
    return code;
}

LanguageType Application::getCurrentLanguage()
{
    std::string languageName = getCurrentLanguageJNI();
    const char* pLanguageName = languageName.c_str();
    LanguageType ret = LanguageType::ENGLISH;

    if (0 == strcmp("zh", pLanguageName))
    {
        ret = LanguageType::CHINESE;
    }
    else if (0 == strcmp("en", pLanguageName))
    {
        ret = LanguageType::ENGLISH;
    }
    else if (0 == strcmp("fr", pLanguageName))
    {
        ret = LanguageType::FRENCH;
    }
    else if (0 == strcmp("it", pLanguageName))
    {
        ret = LanguageType::ITALIAN;
    }
    else if (0 == strcmp("de", pLanguageName))
    {
        ret = LanguageType::GERMAN;
    }
    else if (0 == strcmp("es", pLanguageName))
    {
        ret = LanguageType::SPANISH;
    }
    else if (0 == strcmp("ru", pLanguageName))
    {
        ret = LanguageType::RUSSIAN;
    }
    else if (0 == strcmp("nl", pLanguageName))
    {
        ret = LanguageType::DUTCH;
    }
    else if (0 == strcmp("ko", pLanguageName))
    {
        ret = LanguageType::KOREAN;
    }
    else if (0 == strcmp("ja", pLanguageName))
    {
        ret = LanguageType::JAPANESE;
    }
    else if (0 == strcmp("hu", pLanguageName))
    {
        ret = LanguageType::HUNGARIAN;
    }
    else if (0 == strcmp("pt", pLanguageName))
    {
        ret = LanguageType::PORTUGUESE;
    }
    else if (0 == strcmp("ar", pLanguageName))
    {
        ret = LanguageType::ARABIC;
    }
    else if (0 == strcmp("nb", pLanguageName))
    {
        ret = LanguageType::NORWEGIAN;
    }
    else if (0 == strcmp("pl", pLanguageName))
    {
        ret = LanguageType::POLISH;
    }
    return ret;
}

Application::Platform Application::getTargetPlatform()
{
    return Platform::OS_ANDROID;
}

void Application::applicationScreenSizeChanged(int newWidth, int newHeight) {

}

NS_CC_END

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
