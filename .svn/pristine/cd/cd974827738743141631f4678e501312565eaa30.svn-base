#ifndef __QFILEUTIL_H__
#define __QFILEUTIL_H__

#include "sdk/sdk.h"
#include "tinyxml2/tinyxml2.h"
#include <vector>

using namespace tinyxml2;

class QFileUtil {
	public:
		
		static std::string readXML(std::string fileName ,std::string id) {
			Dictionary * dic = Dictionary::createWithContentsOfFile(fileName.c_str());
			String * title = (String *)dic->objectForKey(id);
			if(title)
				return title->getCString();
			else
			{
				return NULL;
			}
		};

		//
		static int getLevelAndTitle(int exp ,std::string * title) {
			if (exp > 0) {
				Dictionary * dic = Dictionary::createWithContentsOfFile("game_level.xml");

				CCArray * pArr = new CCArray();
				pArr->retain();

				cocos2d::CCArray * pKeys = dic->allKeys();
				Ref * pObj;
				CCARRAY_FOREACH(pKeys,pObj) {
					CCString * pStr = (CCString *) pObj;
					if (exp >= pStr->intValue()) {
						pArr = (CCArray*) dic->objectForKey(pStr->getCString());
						CCString * pTitle = (CCString *) (pArr->objectAtIndex(1));
						*title = pTitle->getCString();
						CCString * pLevel = (CCString *) (pArr->objectAtIndex(0));
						return pLevel->intValue();
					}
				}
			}

			return 0;
		};

		//保存登录过的用户列表
		static void saveLoginFile(const char * key ,const char * val) {
			string path = FileUtils::getInstance()->getWritablePath();
			string fullPath = FileUtils::getInstance()->fullPathFromRelativeFile("loginLog.xml", path);

			cocos2d::ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath.c_str());
			//dict.erase(key);
			//int cunt = dict.size();
			cocos2d::Value vl = cocos2d::Value(val);
			//dict.insert(make_pair(key,vl));
			dict[key] = vl;
			FileUtils::getInstance()->writeToFile(dict, fullPath.c_str());
		};

		//获得登录过的用户列表
		static cocos2d::ValueMap getLoginFile() {

			string path = FileUtils::getInstance()->getWritablePath();
			string fullPath = FileUtils::getInstance()->fullPathFromRelativeFile("loginLog.xml", path);

			return FileUtils::getInstance()->getValueMapFromFile(fullPath.c_str());

		};

		//更新保存登录过的用户列表
		static void updateLoginFile(cocos2d::ValueMap& dict) {
			string path = FileUtils::getInstance()->getWritablePath();
			string fullPath = FileUtils::getInstance()->fullPathFromRelativeFile("loginLog.xml", path);
// 
// 			cocos2d::ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath.c_str());
// 
// 			string search = key;
// 			for (auto &x : dict) {
// 				if ((string)(x.first) == search) {
// 					//dict.erase(search);
// 					Value vl = Value(val);
// 					//dict.insert(make_pair(key,vl));
// 					dict[key] = vl;

			FileUtils::getInstance()->writeToFile(dict, fullPath.c_str());
// 				}
// 			}

		}
		;

		//记录上次登录的用户信息
		static void saveLastLogin(const char * key ,const char * val) {
			string filename = "lastLoginAccount.txt";

			FileUtils::getInstance()->purgeCachedEntries();
			string path = FileUtils::getInstance()->getWritablePath();
			string fullPath = FileUtils::getInstance()->fullPathFromRelativeFile(filename, path);

			string accounts = key;
			string password = val;
			string writeStr = accounts + "|" + password;

			FILE * fp = fopen(fullPath.c_str(), "wb");
			if (fp) {
				size_t ret0 = fwrite(key, 1, strlen(key), fp);
				CCLog("记录登录用户 %s  %d", key, ret0);
				fclose(fp);
			}
		}
		;

		//获得上次登录用户信息
		static string getLoginAccounts() {

			string filename = "lastLoginAccount.txt";
			string path = FileUtils::getInstance()->getWritablePath();
			string fullPath = FileUtils::getInstance()->fullPathFromRelativeFile(filename, path);

			string st = FileUtils::getInstance()->getStringFromFile(fullPath.c_str());

			return st;
		};

		//记录上次登录的用户信息
		static void saveBankPass(const char * val) {
			string filename = "LastBankPass.txt";

			FileUtils::getInstance()->purgeCachedEntries();
			string path = FileUtils::getInstance()->getWritablePath();
			string fullPath = FileUtils::getInstance()->fullPathFromRelativeFile(filename, path);

			FILE * fp = fopen(fullPath.c_str(), "wb");
			if (fp) {
				size_t ret0 = fwrite(val, 1, strlen(val), fp);
				fclose(fp);
			}
		}
		;

		//获得上次登录用户信息
		static string getBankPass() {

			string filename = "LastBankPass.txt";
			string path = FileUtils::getInstance()->getWritablePath();
			string fullPath = FileUtils::getInstance()->fullPathFromRelativeFile(filename, path);

			string st = FileUtils::getInstance()->getStringFromFile(fullPath.c_str());

			return st;
		};

};

#endif

