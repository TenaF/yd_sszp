
#ifndef __SERVER__ADDRESS_HServerAddress__
#define __SERVER__ADDRESS_HServerAddress__

#include "cocos2d.h"
#include "network/HttpClient.h"//网络


class ServerAddress
{
public:

	ServerAddress();
	~ServerAddress();

	void httpRequest();//HTTP请求
private:
    //bool m_bIsIPV6Net;
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};


#endif
