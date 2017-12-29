//
//  Util.h
//  hello2dx
//
//  Created by on 14-7-15.
//
//

#ifndef __hello2dx__Util__
#define __hello2dx__Util__

#include "cocos2d.h"
#include "sdk/message/Define.h"
#include <string>
#include <vector>

USING_NS_CC;
using namespace std;

typedef std::basic_string<char> StringA;        //GBk,utf8编码字符串
typedef std::basic_string<unsigned short> StringW;        //UTF16编码

//typedef basic_string<char16_t> u16string;

// 截短昵称，三个汉字+...
const char* getStrWithWidthLimit(const char* str, float maxWidth, const char* fontName, float fontSize, string& outStr);
// 将大数字改为短数字，不带单位
const char* numToShortString(long long num);
// 将大数字改为带单位的短数字
const char* numToShortStringWithUnit(long long num);


struct obj_deleter {
	template<typename T>
	void operator()(const T* pX) const {
		delete pX;
	}
};

struct second_deleter {
	template<typename T>
	void operator()(const T& pX) const {
		delete pX.second;
	}
};

class Util {
public:

	static std::vector<std::string> splitString(std::string str, std::string pattern);
	//获取当前时间 (年 月 日)   by chen
	static string getCurrentTime();

	static DWORD getTimeStamp();
	//GBK to UTF8 (因为WIN32默认是使用GBK) by chen
	static void UTF_8ToGB2312(string &pOut, char *pText, int pLen);
	static string gbk_2_utf8(const string text);
	//        char StrToBin(char *str);
	//        string UrlGB2312Decode(string str);
	static string UrlDecode(string str);
	static string UrlUTF8(const char* str);                             //urlutf8 编码
	static BYTE getDeviceType();
	static bool getDeviceMD5(TCHAR deviceMd5[LEN_MD5]);

	static bool createTCHARWithString(std::string str, TCHAR* dest);
	static std::string strToMd5Str(std::string str);
	static std::string getFileMd5Str(FILE * &pFile);
	static std::string getFilePathMd5Str(const std::string& path);

	//    static StringA Utf16ToStringA(const StringW& strUtf16);
	static std::string getStringWithTChar(TCHAR * pChar);

	static std::string getStringWithTCharShort(StringW &strUtf16);
	static std::string  TCHARToChar(TCHAR* src);
	static void keepScreenOn(bool flag);

	//static std::string getStringWithTChar(std::u16string &strUtf16);
	static std::string Utf16ToUtf8(std::u16string strUtf16);
	/**
	* @brief utf16转utf8
	*/
	static StringA Utf16ToUtf8(const StringW& strUtf16);

	/**
	* @brief 多字节字符串(移动平台为utf8,Win平台为GBK) 转换为utf16字符串
	*/
	//	static StringW StringAToUtf16(const StringA& strUtf8);
	/**
	* @brief utf8转utf16
	*/
	//    static std::u16string Util::Utf8ToUtf16( const StringA& strUtf8 )
	//	static StringW Utf8ToUtf16(const StringA& strUtf8);
	// 震动
	static void vibrate();
	// 获取网络
	static int getSignal();
	// 获取电量
	static int getBattery();

	static const char* getUniqueCode();

	static int gameRoomType(unsigned long serverRule);
	//正则表达式检查
	static bool checkRegex(string email, string reg);
	//账号检查
	static bool checkRegAccount(string email);
	//横竖屏切换
	static void setRequestedOrientation(bool isLandscape);
	//限制昵称字符数
	//static int checkNameUnicode(string name);
	//去除前后空格
	static string& trim(string& str);

	static std::string UTF8ToUnicode(const std::string str);
	static std::string UnicodeToUTF8(const std::string str);

	static int GetToken(const std::string str);

	static void upgrade(string filename);

	static const char* getIpByHostName(char* strHostName);

	// 获得所有应用列表
	static char* getAppList();
	static bool openApp(string filename);
	static char* getPhoneNumber();

	//截取
	static void split(const std::string& s, const std::string& delim, std::vector<std::string>* ret);

	//爱贝支付
	static void iPay(string score, string acc);

	//手机系统版本
	static string getSystemVersion();
	//ios是否越狱--android是否root
	static BYTE isJailBroken();
	//手机型号
	static string getPhoneModel();
	//唯一标识
	static string getIDFA();
	//获取客户端版本
	static string getClientVersion();
};

#endif /* defined(__hello2dx__Util__) */
