//
//  Util.cpp
//  hello2dx
//
//  Created by on 14-7-15.
//
//

#include <assert.h>
#include "Util.h"
#include "base/ccUTF8.h"
#include "MD5.h"
#include "sdk/log/NYLog.h"
#include "sdk/message/define.h"
#include <string.h>
#include "sdk/controller/MainController.h"
#include "sdk/network/NetworkManager.h"
#include "sdk/message/SendMsg.h"
#include "hall/data/UserInfo.h"

#include <openssl/md5.h>


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include <netdb.h>
#include <sys/socket.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Util_ios.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <regex>
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <regex.h>
#endif

#define TAG "Util"

using namespace cocos2d;

// 获取一个 UTF8 字符的字节数
uint32_t getUTF8CharLength(char ch) {
	if (0 == (ch >> 7 & 1))
		return 1;

	for (uint32_t i = 7; i > 0; --i) {
		if (0 == (ch >> i & 1))
			return 7 - i;
	}
	return 0;
}

uint32_t advancePosUTF8(const char* str, uint32_t start, uint32_t numAdvance) {
	const uint32_t len = strlen(str);
	uint32_t pos = start;

	for (uint32_t i = 0; i < numAdvance && pos < len; ++i)
		pos += getUTF8CharLength(str[pos]);

	return pos - start;
}

// 获取字符串宽度
float getUTF8StringWidth(const char* str, const char* fontName, float fontSize) {
	Texture2D txt;
	txt.initWithString(str, fontName, fontSize);
	return txt.getContentSize().width;
}

const char* getStrWithWidthLimit(const char* str, float maxWidth, const char* fontName, float fontSize, string& lastSafe) {
	if (getUTF8StringWidth(str, fontName, fontSize) <= maxWidth) {
		return str;
	}

	string orig(str);
	int pos = advancePosUTF8(str, 0, 3);        // 先获取三个字符
	int length = orig.length();

	for (; pos < length;) {
		string tmp(orig.begin(), orig.begin() + pos);
		tmp += "...";
		if (getUTF8StringWidth(tmp.c_str(), fontName, fontSize) <= maxWidth) {
			lastSafe = tmp.c_str();
			pos += advancePosUTF8(str, pos, 1);
		}
		else {
			lastSafe = tmp.c_str();
			break;
		}
	}

	NYInfo(TAG, "getStrWithWidthLimit, str=%s, ret=%s", str, lastSafe.c_str());
	return lastSafe.c_str();        //lastSafe.c_str();
}

const char* numToShortString(long long num) {
	if (num < 0) {
		return "0000";
	}
	CCString *str = new CCString();
	str->initWithFormat("%lld", num);
	const char* chars = str->getCString();

	int len = str->length();
	if (len > 4) {
		char numChars[4] = { 0 };
		for (int i = 0; i < 4; i++) {
			numChars[i] = chars[i];
		}
		int pointPos = len % 4;
		int afterPoint = 0;
		if (pointPos > 0) {
			for (int i = 3; i >= pointPos; i--) {
				if (numChars[i] == '0') {
					numChars[i] = NULL;
				}
				else {
					afterPoint = i - pointPos + 1;
					break;
				}
			}
		}
		else {
			pointPos = 4;
		}

		char ret[6] = { 0 };
		int index = 0;
		for (; index < pointPos; index++) {
			ret[index] = numChars[index];
		}
		if (afterPoint > 0) {
			ret[index++] = '.';
			for (int i = 0; i < afterPoint; i++) {
				ret[index] = numChars[index - 1];
				index++;
			}
		}

		chars = str->getCString();
	}

	//    NYInfo(TAG, "numToShortString, %lld, %s", num, chars);
	return chars;
}

const char* numToShortStringWithUnit(long long num) {
	//    NYInfo(TAG, "numToShortStringWithUnit, num=%lld", num);
	if (num < 0) {
		return "0000";
	}

	CCString *str = new CCString();
	str->initWithFormat("%lld", num);
	const char* chars = str->getCString();

	int len = str->length();
	if (len > 4) {
		char numChars[4] = { 0 };
		for (int i = 0; i < 4; i++) {
			numChars[i] = chars[i];
		}
		int pointPos = len % 4;
		int afterPoint = 0;
		if (pointPos > 0) {
			for (int i = 3; i >= pointPos; i--) {
				if (numChars[i] == '0') {
					numChars[i] = NULL;
				}
				else {
					afterPoint = i - pointPos + 1;
					break;
				}
			}
		}
		else {
			pointPos = 4;
		}

		char ret[6] = { 0 };
		int index = 0;
		for (; index < pointPos; index++) {
			ret[index] = numChars[index];
		}
		if (afterPoint > 0) {
			ret[index++] = '.';
			for (int i = 0; i < afterPoint; i++) {
				ret[index] = numChars[index - 1];
				index++;
			}
		}

		if (len > 8) {
			str->initWithFormat("%s亿", ret);
		}
		else {
			str->initWithFormat("%s万", ret);
		}

		chars = str->getCString();
	}

	//    NYInfo(TAG, "numToShortStringWithUnit, %lld, %s", num, chars);
	return chars;
}



//字符串分割函数
std::vector<std::string> Util::splitString(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}


BYTE Util::getDeviceType() {
	BYTE deviceType = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	deviceType = 2;
#endif

	// #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// 	deviceType = 3;
	// #endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	deviceType = 1;
#endif
	return deviceType;
}


bool Util::getDeviceMD5(TCHAR deviceMd5[LEN_MD5]) {
	return true;
}


bool Util::createTCHARWithString(std::string str, TCHAR* dest) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	char * pTemp = (char *)(str.c_str());
	MultiByteToWideChar(CP_ACP, 0, pTemp, -1, dest, strlen(pTemp));
#else
	std::u16string utf16String;
	StringUtils::UTF8ToUTF16(str, utf16String);
	for (int i = 0; i < utf16String.length(); i++)
		dest[i] = utf16String[i];
#endif
	return true;
}

//string -> MD5 NSString
std::string Util::strToMd5Str(std::string str) {
	std::string resStr;
	CMd5 md5;
	const char* cStr = str.c_str();
	unsigned char* result;

	result = (unsigned char*)md5.MakeMD5(reinterpret_cast<const BYTE*>(cStr), str.length());

	std::string desStr = StringUtils::format("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", result[0], result[1], result[2], result[3],
		result[4], result[5], result[6], result[7], result[8], result[9], result[10], result[11], result[12], result[13], result[14], result[15]);
	return desStr;
}

std::string Util::getFileMd5Str(FILE * &pFile)
{
	std::string MD5Str = "";
	// 	FILE *pFile = fopen(path.c_str(), "rb");
	if (pFile)
	{
		MD5_CTX ctx;
		int len = 0;
		unsigned char buffer[1024] = { 0 };
		unsigned char digest[16] = { 0 };
		MD5_Init(&ctx);

		while ((len = fread(buffer, 1, 1024, pFile)) > 0)
		{
			MD5_Update(&ctx, buffer, len);
		}

		MD5_Final(digest, &ctx);

		fclose(pFile);


		int i = 0;
		char buf[33] = { 0 };
		char tmp[3] = { 0 };
		for (i = 0; i < 16; i++)
		{
			sprintf(tmp, "%02X", digest[i]); // sprintf并不安全  
			strcat(buf, tmp); // strcat亦不是什么好东西  
		}
		MD5Str = (char*)buf;
	}

	//NYInfo(TAG,"%s", MD5Str.c_str());  // 文件的md5值 

	return MD5Str;
}

std::string Util::getFilePathMd5Str(const std::string& path)
{
	std::string MD5Str = "";
	FILE *pFile = fopen(path.c_str(), "rb");
	if (pFile)
	{
		MD5_CTX ctx;
		int len = 0;
		unsigned char buffer[1024] = { 0 };
		unsigned char digest[16] = { 0 };
		MD5_Init(&ctx);

		while ((len = fread(buffer, 1, 1024, pFile)) > 0)
		{
			MD5_Update(&ctx, buffer, len);
		}

		MD5_Final(digest, &ctx);

		fclose(pFile);


		int i = 0;
		char buf[33] = { 0 };
		char tmp[3] = { 0 };
		for (i = 0; i < 16; i++)
		{
			sprintf(tmp, "%02X", digest[i]); // sprintf并不安全  
			strcat(buf, tmp); // strcat亦不是什么好东西  
		}
		MD5Str = (char*)buf;
	}

	//NYInfo(TAG,"%s", MD5Str.c_str());  // 文件的md5值 

	return MD5Str;
}


std::string Util::getStringWithTChar(TCHAR * pChar) {

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	string str = "";
	int iLen = WideCharToMultiByte(CP_ACP, 0, pChar, -1, NULL, 0, NULL, NULL);
	char * chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, pChar, -1, chRtn, iLen, NULL, NULL);
	str = chRtn;
	//str = UnicodeToUTF8(str);
	return str;
#else
	std::string strUtf8;
	std::basic_string<unsigned short> strUtf16 = pChar;
	if (!strUtf16.empty()) {
		StringUtils::UTF16ToUTF8((std::u16string&) (strUtf16), strUtf8);
		return strUtf8;
	}
	return strUtf8;
#endif
}


int Util::gameRoomType(unsigned long serverRule) {
	if ((serverRule & SR_GAMEMATCH_SCORE) != 0)
		return 1;        //
	if ((serverRule & SR_GAMEMATCH_WASHOUT) != 0)
		return 2;        //

	return 0;			//
}

//正则表达式检查
bool Util::checkRegex(string str, string reg) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	//regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
	regex pattern(reg);
	if (!regex_match(str, pattern))
	{
		return false;
	}

#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	char ss[100] = {};
	const char * pStr = str.c_str();
	//int len = strlen(pStr);
	strcpy(ss, pStr);

	regmatch_t pmatch[4];
	regex_t match_regex;
	regcomp(&match_regex, reg.c_str(), REG_EXTENDED);
	if (regexec(&match_regex, ss, 4, pmatch, 0) != 0)
	{
		return false;
	}
	regfree(&match_regex);

#endif
	return true;
}

////账号检查
bool Util::checkRegAccount(string str) {
	// #if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// 
	// 	//regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
	// 	regex pattern(reg);
	// 	if(!regex_match(str,pattern))
	// 	{
	// 		return false;
	// 	}
	// 
	// #endif
	// #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	char ss[100] = {};
	const char * pStr = str.c_str();
	int len = strlen(pStr);
	strcpy(ss, pStr);

	for (int i = 0; i < len; i++)
	{
		NYInfo(TAG, "%c", ss[i]);

		if (!isalpha(ss[i]) && !isalnum(ss[i]))
			return false;
	}
	/*#endif*/
	return true;
}

//限制昵称字符数
//int Util::checkNameUnicode(string name)
//{
//	int len = 0;
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	const char * pStr = name.c_str();
//	int len = strlen(pStr);
//	char ss[100] = {};
//	strcpy(ss, pStr);
//
//	for (int i = 0; i < len; i++)
//	{
//		if (!isalpha(ss[i]) && !isalnum(ss[i]))
//		{
//			len += 2;
//		}
//		else
//		{
//			len++;
//		}
//	}
//	return len;
//#else
//	return len = strlen(UTF8ToUnicode(UserInfo::instance()->getNickName()).c_str());
//#endif
//}

//去除前后空格
string& Util::trim(string& str) {
	if (str.empty()) {
		return str;
	}

	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);
	return str;
}

#ifdef WIN32
BOOL IsTextUTF8(char* str, ULONGLONG length)
{
	DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节  
	UCHAR chr;
	BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8  
	for (int i = 0; i < length; ++i)
	{
		chr = *(str + i);
		if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx  
			bAllAscii = FALSE;
		if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数  
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
					return FALSE;

				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx  
		{
			if ((chr & 0xC0) != 0x80)
				return FALSE;

			nBytes--;
		}
	}
	if (nBytes > 0) //违返规则  
		return FALSE;
	if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8  
		return FALSE;

	return TRUE;
}
#endif

std::string Util::UnicodeToUTF8(const std::string str) {
#ifdef WIN32
	if (IsTextUTF8((char*)str.c_str(), str.length()))
		return str;
	int nwLen;

	nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string reStr(pBuf);

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;
	return reStr;
#else
	return str;
#endif

}

std::string Util::UTF8ToUnicode(const std::string str) {

#ifdef WIN32
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string reStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;
	return reStr;
#else
	return str;
#endif

}


int Util::GetToken(const std::string str)
{
	int numbLen = 0;
	int len = str.size();
	short high, low;
	unsigned int code;
	string s;
	for (int i = 0; i < len; i++)
	{
		if (str[i] >= 0 || i == len - 1)
		{
			//NYInfo("GetToken", "ASCii字符----------------------");   //ASCii字符
		}
		else
		{
			//计算编码
			high = (short)(str[i] + 256);
			low = (short)(str[i + 1] + 256);
			code = high * 256 + low;

			//获取字符
			s = "";
			s += str[i];
			s += str[i + 1];
			i++;

			if ((code >= 0xB0A1 && code <= 0xF7FE) || (code >= 0x8140 && code <= 0xA0FE) || (code >= 0xAA40 && code <= 0xFEA0))
			{
				numbLen++;
			}
			else
			{
				//NYInfo("GetToken", "numbLen----------------------");
			}
		}
	}
	return numbLen;
}


std::string Util::TCHARToChar(TCHAR* src) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::basic_string<unsigned short> str = src;
	return Util::getStringWithTCharShort(str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	std::basic_string<unsigned short> str = src;
	return Util::getStringWithTCharShort(str);
#else
	char * chText;
	DWORD dwNum = WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, NULL, NULL, NULL);
	chText = new char[dwNum + 1];
	memset(chText, 0, dwNum + 1);
	WideCharToMultiByte(CP_UTF8, 0, src, -1, chText, dwNum, NULL, NULL);
	return chText;
#endif
	return "";
}



std::string Util::getStringWithTCharShort(StringW &strUtf16) {
	std::string strUtf8;
	if (!strUtf16.empty()) {
		StringUtils::UTF16ToUTF8((std::u16string&) (strUtf16), strUtf8);
		return strUtf8;
	}
	return strUtf8;
}

//获取真实IP
const char* Util::getIpByHostName(char* strHostName) {
	hostent* host_entry = gethostbyname(strHostName);
	char IPStr[64] = { 0 };
	if (host_entry != 0) {
		sprintf(IPStr, "%d.%d.%d.%d", (host_entry->h_addr_list[0][0] & 0x00ff), (host_entry->h_addr_list[0][1] & 0x00ff),
			(host_entry->h_addr_list[0][2] & 0x00ff), (host_entry->h_addr_list[0][3] & 0x00ff));
		__String *str = new __String();
		str->initWithFormat("%s", IPStr);
		const char* chars = str->getCString();
		return chars;
	}
	else {
		return strHostName;
	}
}




//截取
void Util::split(const std::string& s, const std::string& delim, std::vector<std::string>* ret) {
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos) {
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0) {
		ret->push_back(s.substr(last, index - last));

	}
}


////////////////////////////////调用jni接口//////////////////////////


void Util::vibrate() {
	NYInfo(TAG, "vibrate");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "vibrate", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	vibrateIOS();
#endif
}



// 折算为 0~100
int Util::getSignal() {
	NYInfo(TAG, "getSignal");
	int signal = 100;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getSignal", "()I")) {
		signal = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	signal = getSignalIOS();
#endif

	NYInfo(TAG, "getSignal, signal=%d", signal);
	return signal;
}

// 折算为 0~100
int Util::getBattery() {
	NYInfo(TAG, "getBattery");
	int battery = 100;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getBattery", "()I")) {
		battery = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	battery = getBatteryLevelIOS();
#endif

	NYInfo(TAG, "getBattery, battery=%d", battery);
	return battery;
}

// 得到设备id号
const char* Util::getUniqueCode() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	const char* str;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getIMEI", "()Ljava/lang/String;")) {
		jstring strJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		str = t.env->GetStringUTFChars(strJava, 0);
		//        t.env->ReleaseStringUTFChars(strJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	NYInfo(TAG, "getUniqueCode, str=%s", str);
	return str;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NYInfo(TAG, "getUniqueCode, uuid=%s", getUniqueCodeIOS());
	return getUniqueCodeIOS();
#endif
	return "";
}


void Util::keepScreenOn(bool flag) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "keepScreenOn", "(Z)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#endif
}

//升级
void Util::upgrade(string filename) {
	NYInfo(TAG, "upgrade");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	//const char* str;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "updateVersion", "(Ljava/lang/String;)V")) {
		NYInfo(TAG, "初始化参数");
		jstring param = t.env->NewStringUTF(filename.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, param);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NYInfo(TAG, "方法为实现");
#endif
}



// 获得所有应用列表
char* Util::getAppList() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	const char* str;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppList", "read", "()Ljava/lang/String;")) {
		jstring strJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		str = t.env->GetStringUTFChars(strJava, 0);
		//        t.env->ReleaseStringUTFChars(strJava, str);
		t.env->DeleteLocalRef(t.classID);
	}
	NYInfo(TAG, "applist json, str=%s", str);
	return const_cast<char*>(str);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NYInfo(TAG, "applist json, uuid=%s", getUniqueCodeIOS());
	return NULL;
#endif
	return NULL;
}


bool Util::openApp(string filename) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	jboolean str;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppList", "openApp", "(Ljava/lang/String;)Z")) {
		NYInfo(TAG, "初始化参数");
		jstring fn = t.env->NewStringUTF(filename.c_str());
		str = (jboolean)t.env->CallStaticBooleanMethod(t.classID, t.methodID, fn);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NYInfo(TAG, "方法为实现");
	//vibrateIOS();
	//return (bool)str;
	return true;
#endif
	return false;
}


char* Util::getPhoneNumber()
{
	NYInfo(TAG, "getPhoneNumber");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* str;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getPhoneNumber", "()Ljava/lang/String;")) {
		jstring strJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		str = t.env->GetStringUTFChars(strJava, 0);
		NYInfo(TAG, str);
		t.env->DeleteLocalRef(t.classID);
	}
	return const_cast<char*>(str);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return "";
#endif
	return "";
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
extern "C"
{
	void Java_org_cocos2dx_cpp_AppActivity_onNotifyReconnectServer(JNIEnv * env, jobject jthiz)
	{
		NYInfo(TAG, "onNotifyReconnectServer");
		SendMsg::getUserScore();
	}
};
#endif


//爱贝支付
void Util::iPay(string score, string acc) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "ipay", "(Ljava/lang/String;Ljava/lang/String;)V")) {
		NYInfo(TAG, "初始化参数");
		jstring num = t.env->NewStringUTF(score.c_str());
		jstring account = t.env->NewStringUTF(acc.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, num, account);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NYInfo(TAG, "爱贝不支持IOS支付");
#endif
}

//by chen
string Util::getCurrentTime()
{
	time_t t;
	time(&t);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d%X", localtime((&t)));
	string timeStr = tmp;
	return timeStr;
}


//在Win32平台下，将GBK编码转换为UTF-8  by chen
string Util::gbk_2_utf8(const string text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (IsTextUTF8((char*)text.c_str(), text.length()))
		return text;

	//采用Lambda表达式,将string转换成wstring
	wstring tes = [=]() {
		setlocale(LC_ALL, "chs");
		const char* _Source = text.c_str();
		size_t _Dsize = text.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest, _Source, _Dsize);
		std::wstring result = _Dest;
		delete[]_Dest;
		setlocale(LC_ALL, "C");
		return result;
	}();

	int asciSize = WideCharToMultiByte(CP_UTF8, 0, tes.c_str(), tes.size(), NULL, 0, NULL, NULL);
	if (asciSize == ERROR_NO_UNICODE_TRANSLATION || asciSize == 0)
	{
		return string();
	}

	char *resultString = new char[asciSize];
	int conveResult = WideCharToMultiByte(CP_UTF8, 0, tes.c_str(), tes.size(), resultString, asciSize, NULL, NULL);
	if (conveResult != asciSize)
	{
		return string();
	}
	string buffer = "";
	buffer.append(resultString, asciSize);

	delete[] resultString;
	return buffer;

#else
	return text;
#endif

}

char Char2Int(char ch){
	if (ch >= '0' && ch <= '9')return (char)(ch - '0');
	if (ch >= 'a' && ch <= 'f')return (char)(ch - 'a' + 10);
	if (ch >= 'A' && ch <= 'F')return (char)(ch - 'A' + 10);
	return -1;
}

char Str2Bin(char *str){
	char tempWord[2];
	char chn;

	tempWord[0] = Char2Int(str[0]);                                //make the B to 11 -- 00001011
	tempWord[1] = Char2Int(str[1]);                                //make the 0 to 0  -- 00000000

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

	return chn;
}

string Util::UrlDecode(string str){
	string output = "";
	char tmp[2];
	int i = 0, len = str.length();

	while (i < len){
		if (str[i] == '%'){
			tmp[0] = str[i + 1];
			tmp[1] = str[i + 2];
			output += Str2Bin(tmp);
			i = i + 3;
		}
		else if (str[i] == '+'){
			output += ' ';
			i++;
		}
		else{
			output += str[i];
			i++;
		}
	}

	return output;
}

//把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节 如%3D%AE%88

string Util::UrlUTF8(const char* str)
{
	string tt;
	string dd;
	tt = gbk_2_utf8(str);

	size_t len = tt.length();
	for (size_t i = 0; i < len; i++)
	{
		if (isalnum((BYTE)tt.at(i)))
		{
			char tempbuff[2] = { 0 };
			sprintf(tempbuff, "%c", (BYTE)tt.at(i));
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)tt.at(i)))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff, "%%%X%X", ((BYTE)tt.at(i)) >> 4, ((BYTE)tt.at(i)) % 16);
			dd.append(tempbuff);
		}

	}
	return dd;
}

// 获取时间戳
DWORD Util::getTimeStamp()
{
#ifdef WIN32

	return GetTickCount();//返回操作系统启动后的毫秒数  (unsigned long) 

#else
	//获得精准时间
	struct timeval nowTime;
	gettimeofday(&nowTime, NULL);
	//转化为毫秒数
	DWORD tempTime = nowTime.tv_sec * 1000 + nowTime.tv_usec / 1000;
	return  tempTime;

#endif
}
//

//手机系统版本
string Util::getSystemVersion()
{
	string versionstr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* str;
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getSystemVersion", "()Ljava/lang/String;")) {
		jstring strJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		str = t.env->GetStringUTFChars(strJava, 0);
		NYInfo(TAG, str);
		t.env->DeleteLocalRef(t.classID);
	}
	versionstr = str;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	versionstr = getSysVersionIOS();
#endif
	return versionstr;
}

//ios是否越狱--android是否root
BYTE Util::isJailBroken()
{
	BYTE isjailbroken = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	jboolean str;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "isRoot", "()Z")) {
		str = (jboolean)t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		//NYInfo(TAG, str);
		t.env->DeleteLocalRef(t.classID);
	}
	isjailbroken = (BYTE)str;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	isjailbroken = (BYTE)isJailBrokenIOS();
#endif
	return isjailbroken;
}

//手机型号
string Util::getPhoneModel()
{
	string str;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getPhoneModel", "()Ljava/lang/String;")) {
		jstring strJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		str = t.env->GetStringUTFChars(strJava, 0);
		NYInfo(TAG, str.c_str());
		t.env->DeleteLocalRef(t.classID);
	}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	str = getPhoneModelIOS();
#endif
	return str;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
bool GetMacByGetAdaptersAddresses(std::string& macOUT)
{
	bool ret = false;

	ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
	if (pAddresses == NULL)
		return false;
	// Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variable
	if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAddresses);
		pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
		if (pAddresses == NULL)
			return false;
	}

	if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == NO_ERROR)
	{
		// If successful, output some information from the data we received
		for (PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
		{
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if (pCurrAddresses->PhysicalAddressLength != 6)
				continue;
			char acMAC[32];
			sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pCurrAddresses->PhysicalAddress[0]),
				int(pCurrAddresses->PhysicalAddress[1]),
				int(pCurrAddresses->PhysicalAddress[2]),
				int(pCurrAddresses->PhysicalAddress[3]),
				int(pCurrAddresses->PhysicalAddress[4]),
				int(pCurrAddresses->PhysicalAddress[5]));
			macOUT = acMAC;
			ret = true;
			break;
		}
	}

	free(pAddresses);
	return ret;
}

#endif
//唯一标识
string Util::getIDFA()
{
	string uuid;
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

	if (GetMacByGetAdaptersAddresses(uuid))
	{
		NYInfo(TAG, "getIDFA:%s", uuid.c_str());
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// Android
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getUniqueId", "()Ljava/lang/String;")) {
		jstring strJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		uuid = t.env->GetStringUTFChars(strJava, 0);
		NYInfo(TAG, uuid.c_str());
		t.env->DeleteLocalRef(t.classID);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// IOS
	uuid = getDeviceIdIos();
#endif
	return uuid;
}

//横竖屏切换
void Util::setRequestedOrientation(bool isLandscape)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//这个数值表示屏幕方向
	//JNI函数并没最优化,应用到项目的时候请自行优化
	int value = isLandscape ? 1 : 2;
	std::string msg = "abc";
	JniMethodInfo jniMethodInfo;
	bool isHave = JniHelper::getStaticMethodInfo(
		jniMethodInfo,
		"org/cocos2dx/cpp/AppActivity",
		"requestAction",
		"(ILjava/lang/String;)V");
	if (isHave)
	{
		//需要传递到Java层的字符串
		jstring strArg = jniMethodInfo.env->NewStringUTF(msg.c_str());
		jniMethodInfo.env->CallStaticVoidMethod(jniMethodInfo.classID, jniMethodInfo.methodID, value, strArg);
		jniMethodInfo.env->DeleteLocalRef(jniMethodInfo.classID);
		jniMethodInfo.env->DeleteLocalRef(strArg);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	setRequestedOrientationIOS(isLandscape);
#endif
	CCEGLView *pEGLView = CCDirector::sharedDirector()->getOpenGLView();
	CCSize frameSize = pEGLView->getFrameSize();

	auto width = VisibleRect::getWidth();
	auto height = VisibleRect::getHeight();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (!isLandscape){
		frameSize = frameSize*0.65f;
	}
	else{
		frameSize = CCSize(height, width);
	}
#endif
	pEGLView->setFrameSize(frameSize.height, frameSize.width);
	pEGLView->setDesignResolutionSize(isLandscape ? width : height, isLandscape ? height : width, ResolutionPolicy::SHOW_ALL);
	Director::getInstance()->setProjection(Director::Projection::_2D);
}

std::string Util::getClientVersion()
{
	std::string ver;
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	ver = "2.2.0.1";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// Android
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/QLUtil", "getVersionName", "()Ljava/lang/String;")) {
		jstring strJava = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		ver = t.env->GetStringUTFChars(strJava, 0);
		NYInfo(TAG, ver.c_str());
		t.env->DeleteLocalRef(t.classID);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// IOS
	ver = getClientVersionIOS();
#endif
	return ver;
}
