//*******************************
// Json Parse
// Created by Simon on 10/29/2015
//*******************************

#ifndef _JSON_UTILS_H_
#define  _JSON_UTILS_H_

#include <iostream>
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "cocos2d.h"
#include "sdk/sdk.h"

using namespace std;
using namespace rapidjson;
USING_NS_CC;



// Basic Check;
#define json_check_is_bool(value, strKey) (value.HasMember(strKey) && value[strKey].IsBool())
#define json_check_is_string(value, strKey) (value.HasMember(strKey) && value[strKey].IsString())
#define json_check_is_int32(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt())
#define json_check_is_uint32(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint())
#define json_check_is_int64(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt64())
#define json_check_is_uint64(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint64())
#define json_check_is_double(value, strKey) (value.HasMember(strKey) && value[strKey].IsDouble())
#define json_check_is_number(value, strKey) (value.HasMember(strKey) && value[strKey].IsNumber())
#define json_check_is_array(value, strKey) (value.HasMember(strKey) && value[strKey].IsArray())
#define json_check_is_object(value, strKey) (value.HasMember(strKey) && value[strKey].IsObject())

// Utils Method;
#if (0)    // 1 Myself Debug Mode
inline void String_Check_Null(string _String_, string _Method_) { if (_String_ == "") MessageBox(StringUtils::format("Has null string In %s", _Method_).c_str(), "HasParseError"); }
inline void File_Not_Exist(const char* _File_, const char* _Method_) { MessageBox(StringUtils::format("CODE-ERROR %s Does Not Exist, then create this, Method = %s", _File_, _Method_).c_str(), "File does not exist"); }
#else
inline void String_Check_Null(string _String_, const char* _Method_) { if (_String_ == "") NYInfo("JsonUtils",StringUtils::format("Has null string In %s, HasParseError", _Method_).c_str()); }
inline void File_Not_Exist(const char* _File_, const char* _Method_) { NYInfo("JsonUtils", StringUtils::format("CODE-ERROR %s Does Not Exist, then create this, Method = %s ", _File_, _Method_).c_str()); }
inline void DefaultFile(string _File_) { NYInfo("JsonUtils", "Platform Default Path = %s", _File_.c_str()); }

#endif

// File Path;
const string _USER_ = "user.json";
const string _TEST_ = "test.json";
const string _LEVEL_DES_ = "leveldescribe.json";
const string REF_PATH = "data/";
const string _GAME_DOWNINFO_ = "downinfo.json";

const int ENCRYPTION = 0;

class JsonUtils : public Ref
{
private:
	JsonUtils();
	~JsonUtils();

	static JsonUtils* _self;

	map<std::string, rapidjson::Document*> _docBuffer;

public:
	static JsonUtils* getInstance();
	void deleteInstance();

	//-----------UserData-Method-Begin-----------;

	// Type 1 : writable path, Type : 0 resource path;
	void loadJson(string filename, bool isWritable = true);
	void parseJson(string filename, string jsonBuffer);
	void writeJson(string filename);
	void setValue(string filename, string key, rapidjson::Value& value, bool save = false);
	void saveJson(rapidjson::Document* doc, const std::string &fileName, bool prettyFlag = true, bool Asynchronous = false);
	void saveJson(const std::string& filename, bool prettyFlag = true, bool Asynchronous = false);
	void unloadJson(string filename);

	void setIntegerForKey(string key, int value, string filename, bool save = false);
	int getIntegerForKey(string key, string filename);
	void setBooleanForKey(string key, bool value, string filename, bool save = false);
	bool getBooleanForKey(string key, string filename);
	void setDoubleForKey(string key, float value, string filename, bool save = false);
	float getDoubleForKey(string key, string filename);
	void setStringForKey(string key, string value, string filename, bool save = false);
	string getStringForKey(string key, string filename);
	void setDataForKey(string key, const Data& data, string filename, bool save = false);
	Data getDataForKey(string key, string filename);
	rapidjson::Document* getDoc(string filename);
	rapidjson::Value& getValue(string filename, string key);
	void testCode();

	//--------------UserData-Method-End-------------

};

#endif  //_JSON_UTILS_H_
//_EOF_/.--- ... --- -. ..- - .. .-.. ... .-.-.- ..../