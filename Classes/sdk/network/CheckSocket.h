
#ifndef __hello2dx__CheckSocket__
#define __hello2dx__CheckSocket__

#include "cocos2d.h"
#include <string>
//socket库
#ifdef WIN32
#include <windows.h>
#include <WinSock2.h>//WIN32的
//#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll 在VS里配置等价
#else

#include <sys/socket.h>//socket
#include <fcntl.h>//要设置成非阻塞 需要用到
#include <errno.h>//打印错误信息
#include <netinet/in.h>//IP地址结构体
#include <arpa/inet.h>//IP转换函数等
#include <unistd.h>//通用的文件 目录操作
#include <sys/types.h>
#include <netdb.h>
typedef int SOCKET;

#endif


using namespace std;

static std::mutex		s_SleepMutex;//互斥锁


class CheckSocket
{
public:
	CheckSocket();
	~CheckSocket();
    
    //连接任务方法，供线程执行
	void connect(std::string hostname, unsigned short checkPort, unsigned int realPort, int DistributePortInt);
private:

    SOCKET  m_uSocket;//SOCKET
    bool Connectsocket(const char* domain, unsigned short port);
	void threadConnectTask(std::string ip, unsigned short checkPort, unsigned int realPort, int DistributePortInt);
    //接收任务方法，供线程执行
    bool closeConnect();

	bool createSocket();
};


#endif
