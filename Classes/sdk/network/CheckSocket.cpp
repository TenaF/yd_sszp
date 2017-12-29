#include "CheckSocket.h"
#include "sdk/controller/MainController.h"
#include "sdk/network/NetworkManager.h"
//===============================================================

#define TAG "CheckSocket"

CheckSocket::CheckSocket(){
	m_uSocket = 0;
}

CheckSocket::~CheckSocket(){
	
}

void CheckSocket::connect(std::string hostname, unsigned short checkPort, unsigned int realPort, int DistributePortInt)
{
    if(hostname.empty())
    {
        delete this;
        return;
    }
	std::thread  t1(&CheckSocket::threadConnectTask, this, hostname, checkPort, realPort, DistributePortInt);//开启线程
		t1.detach();
}
//创建套接字
bool CheckSocket::createSocket() {

	m_uSocket = socket(AF_INET, SOCK_STREAM, 0);//创建socket   IPV4     AF_INET == PF_INET

	if (m_uSocket == INVALID_SOCKET) {//如果是无效的
		NYInfo(TAG,"socket failure...");
		return false;
	}
}

bool CheckSocket::Connectsocket(const char* domain, unsigned short port)
{

	int nret = -1;
#ifdef WIN32
	std::string str_ip = Util::getIpByHostName((char*)domain);
	//NYInfo(TAG,"!!!!!!!!!!!!!!HostName:%s:%s", domain,str_ip.c_str());

	if (!this->createSocket()) {//首先创建套接字
		//创建失败
		NYInfo(TAG,"Socket::connectSocket, ERROR: sockt create err");
		return false;
	}

	if (m_uSocket == INVALID_SOCKET) {//如果是无效的
		//NYInfo(TAG,"Socket::connectSocket, ERROR: sockt invalid line=%d", __LINE__);
		return false;
	}

	//IPV4地址 结构体
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(str_ip.c_str());
	svraddr.sin_port = htons(port);//将主机的无符号短整形数转换成网络字节顺序 简单说就是 高低位置互换


	nret = ::connect(m_uSocket, (struct sockaddr*) &svraddr, sizeof(svraddr));//连接服务器函数
#else
    //连接ip
    char ip[128];
    memset(ip, 0, sizeof(ip));
    strcpy(ip, domain);
    
    void* svraddr = nullptr;
    int error=-1, svraddr_len;
    bool ret = true;
    struct sockaddr_in svraddr_4;
    struct sockaddr_in6 svraddr_6;
    
    //获取网络协议
    struct addrinfo *result;
    error = getaddrinfo(ip, NULL, NULL, &result);
    if(nullptr == result)
        return false;
    const struct sockaddr *sa = result->ai_addr;

    socklen_t maxlen = 128;
    switch(sa->sa_family) {
        case AF_INET://ipv4
            if ((m_uSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                NYInfo(TAG,"socket create failed");
                ret = false;
                break;
            }
            if(inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                         ip, maxlen) < 0){
                perror(ip);
                ret = false;
                break;
            }
            svraddr_4.sin_family = AF_INET;
            svraddr_4.sin_addr.s_addr = inet_addr(ip);
            svraddr_4.sin_port = htons(port);
            svraddr_len = sizeof(svraddr_4);
            svraddr = &svraddr_4;
            break;
        case AF_INET6://ipv6
            if ((m_uSocket = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
                NYInfo(TAG,"socket create failed");
                ret = false;
                break;
            }
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                      ip, maxlen);
            
            NYInfo(TAG,"socket created ipv6/n");
            
            bzero(&svraddr_6, sizeof(svraddr_6));
            svraddr_6.sin6_family = AF_INET6;
            svraddr_6.sin6_port = htons(port);
            if ( inet_pton(AF_INET6, ip, &svraddr_6.sin6_addr) < 0 ) {
                perror(ip);
                ret = false;
                break;
            }
            svraddr_len = sizeof(svraddr_6);
            svraddr = &svraddr_6;
            break;
            
        default:
            NYInfo(TAG,"Unknown AF\ns");
            ret = false;
    }
    freeaddrinfo(result);
    if(!ret)
    {
        NYInfo(TAG,"Cannot Connect the server!n");
        return false;
    }
    nret = ::connect(m_uSocket, (struct sockaddr*)svraddr, svraddr_len);
#endif
    if(nret==SOCKET_ERROR )
    {
        return false;
    }
    
    return true;
}

void CheckSocket::threadConnectTask(std::string ip, unsigned short checkPort, unsigned int realPort, int DistributePortInt)
{
    if(Connectsocket(ip.c_str(),checkPort))
    {
        s_SleepMutex.lock();
		NYInfo(TAG,"Connectsocket:%s : %d",ip.c_str(),realPort);
        //if (!MainController::getInstance()->IsCheckSocket())
        {
            //登陆采用真实端口
			SerInfo serinfo;
			strcpy(serinfo.IP, ip.c_str());
			serinfo.nPort = realPort;
			serinfo.bDistributePort = (bool)DistributePortInt;
			MainController::getInstance()->pushSerInfo(serinfo);
        }
        s_SleepMutex.unlock();
    }

	this->closeConnect();

	delete this;//删除自己

}



bool  CheckSocket::closeConnect()
{
	
	int res = -1;
#ifdef WIN32
	res = (closesocket(m_uSocket));
#else
	shutdown(m_uSocket, SHUT_RDWR);//很重要 安卓必须要
	res = (close(m_uSocket));
#endif

	return res;
	
}


//===========================================================
