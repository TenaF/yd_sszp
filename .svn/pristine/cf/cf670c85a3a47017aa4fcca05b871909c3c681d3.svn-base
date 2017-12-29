//
//  QLLog.cpp
//  hello2dx
//
//  Created by on 14-7-26.
//
//

#include "NYLog.h"
#include "sdk/sdk.h"

#define DEBUG (true) // DEBUG开关 by chen


static void _log(const char *format, va_list args)
{
	if (!DEBUG)
		return;

	char buf[MAX_LOG_LENGTH];

	vsnprintf(buf, MAX_LOG_LENGTH - 3, format, args);
	strcat(buf, "\n");

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info",  "%s", buf);

#elif CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WP8
	WCHAR wszBuf[MAX_LOG_LENGTH] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, (char*)(U2F(buf).c_str()), -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, buf, sizeof(buf), nullptr, FALSE);
	printf("%s",buf);
	fflush(stdout);
#else
    // Linux, Mac, iOS, etc
    fprintf(stdout, "cocos2d: %s", buf);
    fflush(stdout);
#endif
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    Director::getInstance()->getConsole()->log(buf);
#endif
    
}


void NYInfo(const char* tag, const char * format, ...)
{
#if CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WP8
	va_list args;
	va_start(args, format);
	_log(format, args);
	va_end(args);
#else
    va_list args;
    __String info;
    info.initWithFormat("[Info]%s %s", tag, format);

	const char * pChar = nullptr;
	pChar = info.getCString();
	va_start(args, pChar);
	_log(info.getCString(), args);
	va_end(args);
#endif
}

void NYErr(const char* tag, const char * format, ...)
{
#if CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WP8

#else
    va_list args;
    __String info;
    info.initWithFormat("[Error]%s %s", tag, format);
	const char * pChar = nullptr;
	pChar = info.getCString();
	va_start(args, pChar);
	_log(info.getCString(), args);
	va_end(args);
#endif
}
