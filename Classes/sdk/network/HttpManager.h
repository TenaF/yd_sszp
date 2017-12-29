/*
 * HttpManager.h
 *
 *  Created on: 2014-10-13
 *      Author: win7
 */

#ifndef HTTPMANAGER_H_
#define HTTPMANAGER_H_

#include "sdk/sdk.h"

#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"



class HttpManager : public Ref
{
public:

	HttpManager(std::string& pUrl,const cocos2d::network::ccHttpRequestCallback& callback, std::string& data, network::HttpRequest::Type _type);
	~HttpManager();

	static HttpManager* createHttpRequest(std::string pUrl, const cocos2d::network::ccHttpRequestCallback& func, std::string data = "", network::HttpRequest::Type _type = network::HttpRequest::Type::GET);

	void Send(int sendType = 1);

	network::HttpRequest * pRequest;

};

#endif
