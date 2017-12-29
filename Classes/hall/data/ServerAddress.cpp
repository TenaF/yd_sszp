#include "ServerAddress.h"
#include "sdk/message/Define.h"
#include "sdk/message/Encrypt.h"
#include "sdk/message/Rijndael.h"


//JSON库
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace  rapidjson;




#include "UserInfo.h"
#include "sdk/network/CheckSocket.h"

USING_NS_CC;
using namespace network;


#define TAG "ServerAddress"

struct tagNetServerItem
{
	WORD								wServerPort;					//服务端口
	DWORD								dwServerAddr;					//服务地址
	bool								bDistributePort;				//是否分发端口
	WORD								wCheckPort;					//检测服务器速度
};

struct tagNetServerArray
{
	tagNetServerItem						ServerItem[255];				//服务器数组
	DWORD								ServerConutNum;					//服务器数组数目
};


std::string dwIP2csIP(DWORD dwIP)
{
	WORD add1, add2, add3, add4;

	add1 = (WORD)(dwIP & 255);
	add2 = (WORD)((dwIP >> 8) & 255);
	add3 = (WORD)((dwIP >> 16) & 255);
	add4 = (WORD)((dwIP >> 24) & 255);

	return StringUtils::format("%d.%d.%d.%d", add4, add3, add2, add1);

}

////判断是否IPv6网络
//bool isIPV6Net(const std::string domainStr = "www.baidu.com")
//{
//    bool isIPV6Net = false;
//    
//    struct addrinfo *result = nullptr,*curr;
//    
//    struct sockaddr_in6 dest;
//    bzero(&dest, sizeof(dest));
//    
//    dest.sin6_family  = AF_INET6;
//    
//    int ret = getaddrinfo(domainStr.c_str(),nullptr,nullptr,&result);
//    if (ret == 0)
//    {
//        for (curr = result; curr != nullptr; curr = curr->ai_next)
//        {
//            switch (curr->ai_family)
//            {
//                case AF_INET6:
//                {
//                    isIPV6Net = true;
//                    break;
//                }
//                case AF_INET:
//                    
//                    break;
//                    
//                default:
//                    break;
//            }
//        }
//    }
//    
//    freeaddrinfo(result);
//    
//    return isIPV6Net;
//}

ServerAddress::ServerAddress()
{
//    m_bIsIPV6Net = isIPV6Net();
//    NYInfo(TAG,m_bIsIPV6Net?"!!!!!!!!!!!ipv6 net!!!!!!!!!":"!!!!!!!!!!!ipv4 net!!!!!!!!!");
}


ServerAddress::~ServerAddress()
{
}


void ServerAddress::httpRequest(){

	HttpRequest* request = new HttpRequest();
	request->setUrl(UserInfo::instance()->getDomainUrlByName("serverList").c_str());
	request->setRequestType(HttpRequest::Type::POST);//获取资源图片用GET  提交用POST
    auto dataStr = StringUtils::format("clientVersion=%s", Util::getClientVersion().c_str());
    //NYInfo(TAG,"===ServerAddress:%s,%s===", request->getUrl(), dataStr.c_str());
	std::string encrytStr = Util::UrlUTF8(aes_encryt(dataStr, "1234567890123456").c_str());
	std::string data = __String::createWithFormat("data=%s", encrytStr.c_str())->getCString();
	if (!data.empty())
	{
		//NYInfo(TAG,"===httpRequest data:%s===",data.c_str());
		request->setRequestData(data.c_str(), data.length());//设置数据
	}


	request->setResponseCallback(CC_CALLBACK_2(ServerAddress::onHttpRequestCompleted, this));//设置回调

	HttpClient::getInstance()->send(request);

	HttpClient::getInstance()->setTimeoutForRead(5);//发送超时
	HttpClient::getInstance()->setTimeoutForConnect(3);//连接超时(单位：秒)
	
	request->release();//释放

}

void ServerAddress::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
  
	if (nullptr == response || false == response->isSucceed())
	{
		NYInfo(TAG,"ServerAddress:nullptr == response || false == response->isSucceed() \n :%s", response->getHttpRequest()->getUrl());
		//访问服务器列表地址失败
		if (false == UserInfo::instance()->switchDomain())
		{
			PromptLayer::promptCommon(U2F("当前网络环境差，建议您连接WIFI，点击确定重试！如需更多帮助请联系客服！").c_str());
			return;
		}

		PromptLayer::createObject("请求服务器地址失败，请联系客服！ (ERROR CODE:10006)");
		return;
	}
    
	//读取ASPX网页地址是否成功，如果成功进入BASE64解码并XOR解密
 	std::vector<char> *responseData = response->getResponseData();//返回的数据
	std::string responseStr = std::string(responseData->begin(), responseData->end());//组装成字符串
   
	int dwMaxDataLength = responseStr.length();
	char* pBuf = const_cast<char*>(responseStr.c_str());
	char chrencweb[1024 * 10] = { 0 };

	DecDataXor(pBuf, chrencweb, dwMaxDataLength);
	memset(pBuf, 0, dwMaxDataLength*sizeof(char));
	memcpy(pBuf, chrencweb, dwMaxDataLength);

	// reader将Json字符串解析到root，root将包含Json里所有子元素  

	rapidjson::Document d;
	d.Parse<rapidjson::kParseDefaultFlags>(pBuf);
	if (d.HasParseError()) {
		NYInfo(TAG,"READ JSON  ERROR %s\n", d.GetParseError());
	}else{
		NYInfo(TAG,"responseStr:%s", responseStr.c_str());
		if (d.IsArray()) {

			rapidjson::Value& data_array = d;
			unsigned int len = data_array.Size();
			MainController::getInstance()->setSerMaxNum(len);
			for (unsigned int i = 0; i < len; i++) {
				if (data_array[i].IsObject()){

// 					[{"IP":"3322682743", "Port" : 8300, "DistributePort" : 1, "CheckPort" : 60000 }, 
// 					{ "IP":"3581168760", "Port" : 8300, "DistributePort" : 1, "CheckPort" : 60000 }, 
// 					{ "IP":"1484475512", "Port" : 8300, "DistributePort" : 1, "CheckPort" : 60000 }
					// 获取服务端Ip(网络应用Ip)
					std::string stNetIp = data_array[i]["IP"].GetString();
                    
                    //string host = stNethostName,"1234567890123456"
                    //NYInfo(TAG,"hostName:%s",stNetIp.c_str());
                    //检测端口
					int checkPort = data_array[i]["CheckPort"].GetInt();
                    //真实登陆端口
					int realPort = data_array[i]["Port"].GetInt();

					int DistributePort = data_array[i]["DistributePort"].GetInt();

					CheckSocket* tempC = new CheckSocket();
					tempC->connect(stNetIp, checkPort, realPort, DistributePort);
			

				}
			}

		}
	}

	delete this;

}



