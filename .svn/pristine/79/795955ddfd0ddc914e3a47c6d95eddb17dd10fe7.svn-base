#ifndef __hello2dx__Socket__
#define __hello2dx__Socket__

#include "cocos2d.h"
#include "sdk/message/packet.h"
using namespace std;
USING_NS_CC;

//多线程库
#include <thread>


//socket库
#ifdef WIN32
#include <windows.h>
#include <WinSock2.h>//WIN32的
//#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll 在VS里配置等价
#else
#include <netinet/tcp.h>
#include <sys/socket.h>//socket
#include <fcntl.h>//要设置成非阻塞 需要用到
#include <errno.h>//打印错误信息
#include <netinet/in.h>//IP地址结构体
#include <arpa/inet.h>//IP转换函数等
#include <unistd.h>//通用的文件 目录操作
#include <sys/types.h>
#include <netdb.h>

typedef int SOCKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#endif



class Socket : public Ref
{
public:
    // stateCB状态回调   
	// msgCB消息回调
    Socket(Ref* parent, int flag, std::function<void(Ref*, int, bool)> stateCB, std::function<void(Ref*, int, const char*, int)> msgCB);
    ~Socket(void);
    
	void setAddr(const char* ip, unsigned int port); // 设置IP 端口
    
    bool start();
   
	void stop(); 
    
    int sendBuf(const char* buf, int len);// 发送

	
    
private:

	Ref* m_pParent;//父类节点
	int m_nFlag; //保存flag
    //pthread_mutex_t work_mutex;
	std::function<void(Ref*, int, bool)>      m_fOnState;//状态回调函数
	std::function<void(Ref*, int, const char*, int)>     m_fOnMsg;//消息回调函数

    string   m_pIP;//IP地址
    unsigned int  m_nPort;//端口

    bool    m_bRun;//是否socket运行
    SOCKET  m_Socket;//SOCKET
    
    bool createSocket();//创建socket
    bool connectSocket();//连接服务器

    bool receive(char* buf, int size);//接收
    bool closeSocket();//关闭socket
    int cleanSocket();//清理socket
    int getError();//错误


    void process();//线程执行函数


//多线程相关
#ifdef _WINDOWS_
    static DWORD WINAPI workFunc(LPVOID userdata);
#else
	static void* workFunc(void *userdata);
#endif
    
    void onState(bool state);//状态回调函数
    void onMsg(const char* buf, int len);//消息回调函数

    char m_pHead[8]; //消息头
    char m_pBuf[SOCKET_TCP_BUFFER];//真实数据 网络缓冲
    

};

#endif


