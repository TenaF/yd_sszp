/*
 * HttpManager.cpp
 *
 *  Created on: 2014-10-13
 *      Author: win7
 */

#include "HttpManager.h"


HttpManager::HttpManager(std::string&  pUrl, const network::ccHttpRequestCallback& func , std::string&  data, network::HttpRequest::Type _type) {


	pRequest = new network::HttpRequest();
	pRequest->setUrl(pUrl.c_str());//设置URL
	pRequest->setRequestType(_type);//POST

	if (!data.empty())
		pRequest->setRequestData(data.c_str(), data.length());//设置数据

	pRequest->setResponseCallback(func);//设置回调

	pRequest->setTag("send message");//TAG

	network::HttpClient::getInstance()->setTimeoutForRead(5);//发送超时
	network::HttpClient::getInstance()->setTimeoutForConnect(3);//连接超时(单位：秒)
	//pRequest->release();

}

void HttpManager::Send(int sendType/* = 1*/)
{
	if (sendType)
		network::HttpClient::getInstance()->send(pRequest);//请求发送
	else
		network::HttpClient::getInstance()->sendImmediate(pRequest);//请求发送

	CC_SAFE_RELEASE_NULL(pRequest);
}

// void HttpManager::Send()
// {
// 	network::HttpClient::getInstance()->send(pRequest);//请求发送
// 
// 	CC_SAFE_DELETE(pRequest);
// }

HttpManager::~HttpManager() {
}

HttpManager* HttpManager::createHttpRequest(std::string  pUrl, const network::ccHttpRequestCallback& func, std::string  data, network::HttpRequest::Type _type/* = network::HttpRequest::Type::GET*/) {
	if (pUrl.empty())
		return nullptr;
	return new HttpManager(pUrl, func, data, _type);
}



