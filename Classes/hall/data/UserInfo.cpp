//
//  UserInfo.cpp
//  hello2dx
//
//
//

#include "UserInfo.h"
//#include "game/dntg/def/timer_manager.h"

USING_NS_CC;
using namespace cocos2d::network;

#define TAG "UserInfo"

const std::string UserInfo::setPath(string path)
{
	const std::string writablePath = FileUtils::getInstance()->getWritablePath();
	string pathString = writablePath + path.c_str();

	if (UserDefault::getInstance()->getBoolForKey("uploaded"))
	{
		return pathString;
	}

	return "GameRes/" + path;
}

//3.X版本以上 HttpClient默认是多线程的
void UserInfo::httpRequest()
{
	HttpRequest* request = new HttpRequest();

	request->setUrl(UserInfo::instance()->getDomainUrlByName("currentTime").c_str());
	
	request->setRequestType(HttpRequest::Type::GET);//获取资源图片用GET  提交用POST

	request->setResponseCallback(CC_CALLBACK_2(UserInfo::onHttpRequestCompleted,this));


	HttpClient::getInstance()->send(request);//添加 请求到发送队列

	HttpClient::getInstance()->setTimeoutForRead(5);	//发送超时
	HttpClient::getInstance()->setTimeoutForConnect(3);	//连接超时(单位：秒)
	
	request->release();//释放
}

void UserInfo::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	//恢复点击
	if (!response || !response->isSucceed())
	{

		switch (SendType)
		{
		case 0:
			//通用请求RC4key可以传主副命令
			SendMsg::SendGetRc4KeyWithSubCmdID(mainCmd, subCmd, 0);//在这里向服务器发送
			break;
		case 1:
			SendMsg::SendGetRc4KeyLogin(0);
			break;
		case 2:
			SendMsg::SendGetRc4KeyServerList(0);
			break;
		default:
			break;
		}
		
		NYInfo(TAG,"FAILURE");
		NYInfo(TAG,"FAILURE MESSAGE: %s", response->getErrorBuffer());
		return;

	}


	std::vector<char> *responseData = response->getResponseData();//返回的数据
	std::string responseStr = std::string(responseData->begin(), responseData->end());//组装成字符串
	if (!responseStr.empty())
	{
		switch (SendType)
		{
		case 0:
			//通用请求RC4key可以传主副命令
			SendMsg::SendGetRc4KeyWithSubCmdID(mainCmd, subCmd, std::atol(responseStr.c_str()));//在这里向服务器发送
			break;
		case 1:
			SendMsg::SendGetRc4KeyLogin(std::atol(responseStr.c_str()));
			break;
		case 2:
			SendMsg::SendGetRc4KeyServerList(std::atol(responseStr.c_str()));
			break;
		default:
			break;
		}
	}
	
}


UserInfo* UserInfo::_instance = 0;

UserInfo::UserInfo()
{
    init();
}

void UserInfo::init()
{
    m_dwUserId = 0;
    m_dwGameId = 0;
    m_dwKindId = 0;
    m_wFaceId = 0;
    m_bySix = 0;
    m_strUserName = "";
    m_strPassWord = "";
    m_strBankWord = "";
    m_strNickName = "";
    m_strPhoneNum = "";
    m_strMachineID = "";
    m_strTempPassWord = "";

    m_byDeviceType = 0;
    m_wTableId = INVALID_TABLE;
    m_wSeatId = INVALID_CHAIR;
    m_byStatus = 0; 
	m_dwExprience = 0;
	m_lScore = 0;
	m_dwMedal = 0;
	m_lInsure = 0;
	m_lCoin = 0;
	m_outMatchID = -1;
	m_currentServerID = 0;
	m_isChangeTable = false;
	gamePort = 0;
	m_strDomainName = "";
	SendType = 0;//默认类型
	mainCmd = 0;
	subCmd = 0;
	m_bWeChatBind = false;
	m_nStoreSwitch = SWITCH_STATE_NONE;
	m_nBillSwitch = SWITCH_STATE_NONE;
	m_nGiveSwitch = SWITCH_STATE_NONE;
	m_nExcShareSwitch = SWITCH_STATE_NONE;
	m_strServerTips = "";
	m_gameflagID = "";
	m_PlazaNetVer = "";
	m_cbMemberOrder = 0;
	m_nDomainIndex = 0;

}

UserInfo::~UserInfo()
{
}

UserInfo* UserInfo::instance()
{
    if(!_instance)
    {
        _instance = new UserInfo();
    }
    return _instance;
}


#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && CODE_LLVM_ENCRYPT
#pragma obfuscate on
#endif

bool UserInfo::switchDomain()
{
	++m_nDomainIndex;
	if (m_nDomainIndex >= m_DomainVector.size()){
		m_nDomainIndex = 0;
	}
	m_strDomainName = m_DomainVector.at(m_nDomainIndex);
	
	return m_nDomainIndex;
}

std::string UserInfo::getDomainUrlByName(const char* nameUrl, bool isKeyName/* = true*/)
{
	std::string urlPath = "";
	if (!m_strDomainName.empty())
	{
		if (isKeyName)
		{
			auto temp_pDict = __Dictionary::createWithContentsOfFile("configure.xml");//读取XML
			urlPath = m_strDomainName + "/" + temp_pDict->valueForKey(nameUrl)->getCString();
		}
		else
		{
			urlPath = m_strDomainName + "/" + nameUrl;
		}
	}
	return urlPath;
}



#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && CODE_LLVM_ENCRYPT
#pragma obfuscate off
#endif
