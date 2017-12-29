//*******************************
// Json Parse
// Created on 10/29/2015
//*******************************

#include "JsonUtils.h"

#define TAG "JsonUtils"

const char* StringRef(const char* c)
{
	return c;
}

JsonUtils* JsonUtils::_self = nullptr;

JsonUtils::JsonUtils()
{
	_docBuffer.clear();
}

JsonUtils::~JsonUtils()
{
}

JsonUtils* JsonUtils::getInstance()
{
	if (!_self)
	{
		_self = new JsonUtils();
	}
	return _self;
}

void JsonUtils::deleteInstance()
{
	if (_self)
	{
		delete(_self);
	}
	_self = nullptr;
}

void JsonUtils::loadJson(string filename, bool isWritable)
{
	string path = "";
	if (isWritable)
	{
		path = FileUtils::getInstance()->getWritablePath().append(filename);
		assert(path != "");
	}
	else
	{
		path = REF_PATH + filename;
	}

	if (!FileUtils::getInstance()->isFileExist(path))
	{
		File_Not_Exist(filename.c_str(), "JsonUtils::loadJson");
		writeJson(filename);
	}

	string jsonBuffer = FileUtils::getInstance()->getStringFromFile(path);
	assert(jsonBuffer != "");
	parseJson(filename, jsonBuffer);
}

void JsonUtils::parseJson(string filename, string jsonBuffer)
{
	rapidjson::Document* doc = new rapidjson::Document();
	doc->Parse<rapidjson::kParseDefaultFlags>(jsonBuffer.c_str());
	assert(doc->IsObject());
	_docBuffer[filename] = doc;
}

void JsonUtils::writeJson(string filename)
{
	string value = filename + " data";
	rapidjson::Document* doc = new rapidjson::Document();
	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	doc->SetObject();
	doc->AddMember("description", StringRef(value.c_str()), allocator);
	saveJson(doc, filename);
}

void JsonUtils::setValue(string filename, string key, rapidjson::Value& value, bool save)
{
	rapidjson::Document* doc = _docBuffer[filename];
	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	assert(doc->IsObject());
	rapidjson::Value& var = *doc;
	if (doc->HasMember(key.c_str()))
	{
		var[key.c_str()] = value;
		if (save)
			saveJson(doc, filename);
	}
	else
	{
		doc->AddMember(StringRef(key.c_str()), value, allocator);

		saveJson(doc, filename);
		string name = filename;
		if (string::npos == name.find(".json"))
			name += ".json";
		string jsonBuffer = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath().append(name));
		assert(jsonBuffer != "");
		parseJson(filename, jsonBuffer);
	}
}

void JsonUtils::saveJson(rapidjson::Document* doc, const std::string &fileName, bool prettyFlag, bool Asynchronous)
{
	StringBuffer buffer;
	buffer.Clear();
	if (prettyFlag)
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		//writer.SetIndent(' ', 2);
		doc->Accept(writer);
	}
	else
	{
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc->Accept(writer);
	}

	std::function<void(std::string, std::string)> saveHandler = [](std::string str, std::string fName)
	{
#if ENCRYPTION
		std::string tag = fName;
		for (int i = 0; i < str.size(); i++)
			str[i] ^= tag[i % tag.size()];
#endif
		std::string filepath = FileUtils::getInstance()->getWritablePath().append(fName);
		FILE *fp = fopen(filepath.c_str(), "wb");
		//fputs(str.c_str(), fp);
		fwrite(str.c_str(), 1, str.size(), fp);

#if ENCRYPTION
		string tagStr = tag;
		for (int i = 0; i < tag.size(); i++)
			tagStr[i] ^= tag[tag.size() - 1 - i];
		fwrite(tagStr.c_str(), 1, tagStr.size(), fp);
#endif
		fclose(fp);
	};

	if (Asynchronous)
	{
		std::thread save(saveHandler, std::string(buffer.GetString(), buffer.Size()), fileName);
		save.detach();
	}
	else
	{
		saveHandler(std::string(buffer.GetString(), buffer.Size()), fileName);
	}
}

void JsonUtils::saveJson(const std::string& filename, bool prettyFlag /*= true*/, bool Asynchronous /*= false*/)
{
	saveJson(_docBuffer[filename], filename, prettyFlag, Asynchronous);
}

void JsonUtils::unloadJson(string filename)
{
	map<string, rapidjson::Document*>::iterator iter;
	iter = _docBuffer.find(filename);
	if (iter != _docBuffer.end())
	{
		_docBuffer.erase(iter);
	}
}

void JsonUtils::setIntegerForKey(string key, int value, string filename, bool save)
{
	rapidjson::Document* doc = _docBuffer[filename];
	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	if (json_check_is_int32((*doc), key.c_str()))
	{
		(*doc)[key.c_str()].SetInt(value);
		if (save)
			saveJson(doc, filename);
	}
	else
	{
		doc->AddMember(StringRef(key.c_str()), value, allocator);

		saveJson(doc, filename);
		string name = filename;
		if (string::npos == name.find(".json"))
			name += ".json";
		string jsonBuffer = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath().append(name));
		assert(jsonBuffer != "");
		parseJson(filename, jsonBuffer);
	}
}

int JsonUtils::getIntegerForKey(string key, string filename)
{
	rapidjson::Document* doc = _docBuffer[filename];
	assert(doc);
	if (json_check_is_int32((*doc), key.c_str()))
	{
		return (*doc)[key.c_str()].GetInt();
	}
	return 0;
}

void JsonUtils::setBooleanForKey(string key, bool value, string filename, bool save)
{
	rapidjson::Document* doc = _docBuffer[filename];
	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	rapidjson::Value& var = *doc;
	if (json_check_is_bool(var, key.c_str()))
	{
		var[key.c_str()].SetBool(value);
		if (save)
			saveJson(doc, filename);
	}
	else
	{
		doc->AddMember(StringRef(key.c_str()), value, allocator);

		saveJson(doc, filename);
		string name = filename;
		if (string::npos == name.find(".json"))
			name += ".json";
		string jsonBuffer = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath().append(name));
		assert(jsonBuffer != "");
		parseJson(filename, jsonBuffer);
	}
}

bool JsonUtils::getBooleanForKey(string key, string filename)
{
	if (_docBuffer[filename] && _docBuffer[filename]->HasMember(key.c_str()))
	{
		return (*_docBuffer[filename])[key.c_str()].GetBool();
	}
	return false;
}

void JsonUtils::setDoubleForKey(string key, float value, string filename, bool save)
{
	rapidjson::Document* doc = _docBuffer[filename];
	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	rapidjson::Value& var = *doc;
	string floatStr = __String::createWithFormat("%.2f", value)->getCString();
	if (json_check_is_string(var, key.c_str()))
	{
		var[key.c_str()].SetString(StringRef(floatStr.c_str()), allocator);
		if (save)
			saveJson(doc, filename);
	}
	else
	{
		doc->AddMember(StringRef(key.c_str()), StringRef(floatStr.c_str()), allocator);
		saveJson(doc, filename);
		string name = filename;
		if (string::npos == name.find(".json"))
			name += ".json";
		string jsonBuffer = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath().append(name));
		assert(jsonBuffer != "");
		parseJson(filename, jsonBuffer);
	}
}

float JsonUtils::getDoubleForKey(string key, string filename)
{
	if (_docBuffer[filename] && _docBuffer[filename]->HasMember(key.c_str()))
	{
		return atof(((*_docBuffer[filename])[key.c_str()].GetString()));
	}
	return 0.0f;
}

void JsonUtils::setStringForKey(string key, string value, string filename, bool save)
{
	rapidjson::Document* doc = _docBuffer[filename];
	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	rapidjson::Value& var = *doc;
	if (json_check_is_string(var, key.c_str()))
	{
		var[key.c_str()].SetString(StringRef(value.c_str()), allocator);
		if (save)
			saveJson(doc, filename);
	}
	else
	{
		doc->AddMember(StringRef(key.c_str()), StringRef(value.c_str()), allocator);

		saveJson(doc, filename);
		string name = filename;
		if (string::npos == name.find(".json"))
			name += ".json";
		string jsonBuffer = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath().append(name));
		assert(jsonBuffer != "");
		parseJson(filename, jsonBuffer);
	}
}

std::string JsonUtils::getStringForKey(string key, string filename)
{
	if (_docBuffer[filename] && _docBuffer[filename]->HasMember(key.c_str()))
	{
		return (*_docBuffer[filename])[key.c_str()].GetString();
	}
	return "";
}

void JsonUtils::setDataForKey(string key, const Data& data, string filename, bool save/* = false*/)
{
	if (key.empty())
	{
		return;
	}

	char *encodedData = 0;

	base64Encode(data.getBytes(), static_cast<unsigned int>(data.getSize()), &encodedData);

	setStringForKey(key, encodedData, filename, save);

	if (encodedData)
		free(encodedData);

}

Data JsonUtils::getDataForKey(string key, string filename)
{
	Data ret;
	std::string encodedData = getStringForKey(key,filename);
	if (!encodedData.empty())
	{
		//const char* encodedData = ;
		unsigned char * decodedData = nullptr;
		int decodedDataLen = base64Decode((unsigned char*)encodedData.c_str(),encodedData.length(), &decodedData);

		if (decodedData) {
			ret.fastSet(decodedData, decodedDataLen);
		}
	}

	return ret;
}

rapidjson::Document* JsonUtils::getDoc(string filename)
{
	if (_docBuffer[filename])
	{
		return _docBuffer[filename];
	}
	return nullptr;
}

rapidjson::Value& JsonUtils::getValue(string filename, string key)
{
	if (_docBuffer[filename] && _docBuffer[filename]->HasMember(key.c_str()))
	{
		return (*_docBuffer[filename])[key.c_str()];
	}
}

void JsonUtils::testCode()
{
	JsonUtils::getInstance()->loadJson(_USER_);
	JsonUtils::getInstance()->loadJson(_TEST_);

	int num = 100;
	string key1 = "aaa";
	string key2 = "bb";
	string key3 = "c";
	string key4 = "d";
	JsonUtils::getInstance()->setIntegerForKey(key1, num, _USER_);
	JsonUtils::getInstance()->setIntegerForKey(key1, num, _TEST_);
	int temp = JsonUtils::getInstance()->getIntegerForKey(key1, _USER_);
	int temp2 = JsonUtils::getInstance()->getIntegerForKey(key1, _TEST_);
	NYInfo(TAG,"%d  %d ", temp, temp2);

	JsonUtils::getInstance()->setStringForKey(key4, "dddddddd", _USER_);
	string temp9 = JsonUtils::getInstance()->getStringForKey(key4, _USER_);
	NYInfo(TAG,"%s", temp9.c_str());

	JsonUtils::getInstance()->setStringForKey(key4, "ffffffff", _USER_);
	string temo0 = JsonUtils::getInstance()->getStringForKey(key4, _USER_);
	NYInfo(TAG,"%s", temo0.c_str());

	JsonUtils::getInstance()->setBooleanForKey(key2, true, _USER_);
	JsonUtils::getInstance()->setBooleanForKey(key2, true, _TEST_);
	bool temp32 = JsonUtils::getInstance()->getBooleanForKey(key2, _USER_);
	bool temp3 = JsonUtils::getInstance()->getBooleanForKey(key2, _TEST_);

	JsonUtils::getInstance()->setDoubleForKey(key3, 1.56f, _USER_);
	double temp6 = JsonUtils::getInstance()->getDoubleForKey(key3, _USER_);
	NYInfo(TAG,"%f     ", temp6);

	JsonUtils::getInstance()->saveJson(_USER_);

	// Value Methods;
	rapidjson::Value cache;
	cache.SetInt(12220);
	JsonUtils::getInstance()->setValue(_USER_, "qwer", cache);

	cache.SetBool(false);
	JsonUtils::getInstance()->setValue(_USER_, "bool", cache);

	cache.SetDouble(1.9966f);
	JsonUtils::getInstance()->setValue(_USER_, "float", cache);

	cache.SetString("gggg");
	JsonUtils::getInstance()->setValue(_USER_, "ConstString", cache);

	string hello = "dd";
	cache.SetString(StringRef(hello.c_str()));
	JsonUtils::getInstance()->setValue(_USER_, "CopyString", cache);

	rapidjson::Document* please = JsonUtils::getInstance()->getDoc(_USER_);
	NYInfo(TAG,"%s", (*please)["d"].GetString());

	// Other Path Files;
	JsonUtils::getInstance()->loadJson("data.json", false);
	string data = JsonUtils::getInstance()->getStringForKey(key4, "data");
	NYInfo(TAG,"data from ref/data %s", data.c_str());
}

//_EOF_