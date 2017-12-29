#include "Socket.h"
#include "sdk/message/Common.h"

Socket::Socket(Ref* parent ,int flag ,std::function<void(Ref* ,int ,bool)> stateCB ,std::function<void(Ref* ,int ,const char* ,int)> msgCB) {
	m_bRun = false;//初始化false

	m_pParent = parent;//保存父节点
	m_nFlag = flag;//保存连接标志
	m_fOnState = stateCB;//保存回调函数
	m_fOnMsg = msgCB;//保存回调函数

}

Socket::~Socket() {
	this->cleanSocket();
}


//设置 ID 端口
void Socket::setAddr(const char* ip ,unsigned int port) {
	m_pIP = ip;
	m_nPort = port;
}


// 启动线程 并初始化WIN32需要的 DLL
bool Socket::start() {

    //只初始化一次，如果在运行，则不再初始化
	if (!m_bRun) {

		m_bRun = true;//socket运行

//WIN32 上 需要先初始化DLL 
//初始化 DLL  Linux不需要，安卓IOS用的就是Linux的
#ifdef WIN32
		WSADATA wsaData;
		//告诉操作系统使用的socket版本
		//MAKEWORD(2, 2) 最新版本 2.2  也是推荐使用2.2
		//WSAStartup，是Windows Sockets Asynchronous的启动命令、Windows下的网络编程接口软件 Winsock1 或 Winsock2 里面的一个命令。
		//返回0 表示成功
		int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);        
		                                               
		if ( ret ) {//如果失败，打印错误信息
			log("WSAStartup failure...");
			return false;
		}
#endif


//以下多线程操作
#ifdef _WINDOWS_
		CreateThread(NULL,          //默认安全属性
				     0,             //使用默认堆栈大小
				     workFunc,      //线程执行函数
				     this,          //线程函数的参数( 谁来执行线程函数 )
				     0,             //使用默认创建标志
				     NULL);
#else
// 		pthread_t m_thread;
// 		pthread_create(&m_thread, NULL, workFunc, this);//第二个参数为NULL是 默认参数，可以满足大部分需求 
// 		pthread_detach(m_thread);//非阻塞，可立即返回
// 		//该子线程的状态设置为detached,则该线程运行结束后会自动释放所有资源

		std::thread  Mythread(&Socket::process, this);//开启线程
		Mythread.detach();

#endif
		return true;


	}else{

		return false;

	}

}



//创建套接字
bool Socket::createSocket() {
	
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);//创建socket   IPV4     AF_INET == PF_INET
	
	if (m_Socket == INVALID_SOCKET) {//如果是无效的
		log("socket failure...");
		return false;
	}

	//log("CREATE SOCKET----=============================%d", m_Socket);

	/*

#ifndef WIN32
	int chOpt = 1;
#else
	char chOpt = 1;
#endif

	//屏蔽掉 Nagle算法  减少网络延迟 (暂且不用)
	//设置socket选项
	int nErr = setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(chOpt));

	NYInfo(TAG, "setsockopt()   nErr=%d", nErr);
	if (nErr == SOCKET_ERROR) {
		NYInfo(TAG, "setsockopt()   error=%d", getError());
	}
	*/

	return true;
}


//向服务器（特定的IP和端口）发起请求
bool Socket::connectSocket() {

	int nret = -1;
#ifdef WIN32
	//std::string str_ip = Util::getIpByHostName((char*)m_pIP.c_str());
	if (!this->createSocket()) {//首先创建套接字
		//创建失败
		log("Socket::connectSocket, ERROR: sockt create err");
		return false;
	}

	if (m_Socket == INVALID_SOCKET) {//如果是无效的
		//log("Socket::connectSocket, ERROR: sockt invalid line=%d", __LINE__);
		return false;
	}

	//IPV4地址 结构体
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(m_pIP.c_str());
	svraddr.sin_port = htons(m_nPort);//将主机的无符号短整形数转换成网络字节顺序 简单说就是 高低位置互换


	nret = ::connect(m_Socket, (struct sockaddr*) &svraddr, sizeof(svraddr));//连接服务器函数
#else
    //连接ip
    char ip[128];
    memset(ip, 0, sizeof(ip));
    strcpy(ip, m_pIP.c_str());

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
            if ((m_Socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                log("socket create failed");
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
            svraddr_4.sin_port = htons(m_nPort);
            svraddr_len = sizeof(svraddr_4);
            svraddr = &svraddr_4;
            break;
        case AF_INET6://ipv6
            if ((m_Socket = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
                log("socket create failed");
                ret = false;
                break;
            }
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                      ip, maxlen);
            
            log("socket created ipv6/n");
            
            bzero(&svraddr_6, sizeof(svraddr_6));
            svraddr_6.sin6_family = AF_INET6;
            svraddr_6.sin6_port = htons(m_nPort);
            if ( inet_pton(AF_INET6, ip, &svraddr_6.sin6_addr) < 0 ) {
                perror(ip);
                ret = false;
                break;
            }
            svraddr_len = sizeof(svraddr_6);
            svraddr = &svraddr_6;
            break;
            
        default:
            log("Unknown AF\ns");
            ret = false;
    }
    freeaddrinfo(result);

	if (!ret)
	{
		log("Cannot Connect the server!n");
		return false;
	}

	nret = ::connect(m_Socket, (struct sockaddr*)svraddr, svraddr_len);
#endif


    if(nret==SOCKET_ERROR )
    {
		int err = this->getError();
        return false;
    }

    return true;
    
}


void Socket::stop() {
	m_bRun = false;
    this->closeSocket();
	m_Socket = INVALID_SOCKET;
	m_pIP = "";
	m_nPort = 0;
}


//向服务器发送消息
int Socket::sendBuf(const char* buf ,int len) {
	int bytes = 0;
	int count = 0;

	if (!buf) {
		//NYInfo(TAG, "Socket::send, ERROR: Send buf is null");
		return -1;
	}

	if (m_Socket == INVALID_SOCKET) {//如果是无效的

		this->onState(false);
		return -1;

	}

	while (count < len) {
		bytes = send(m_Socket, (buf + count), (len - count), 0);//send只是负责COPY数据到 socket的缓冲区，并不发送数据，数据由socket协议发送，此函数返回 实际copy的字节数

		if (bytes == SOCKET_ERROR || bytes == 0) {

			this->onState(false);
			return -1;

		}

		count += bytes;
	}

	return count;// 实际copy的字节数

}

//接收服务器消息
bool Socket::receive(char* buf ,int size) {
	
	int recvLen = 0;
	int recvTotalLen = 0;

	while (recvTotalLen < size) {
		recvLen = recv(m_Socket, (buf + recvTotalLen), (size - recvTotalLen), 0);//接收服务器消息
		if (recvLen == SOCKET_ERROR || recvLen == 0) {
			//log("Socket::receive, ERROR RecV recvLen=%d, line=%d ", recvLen, __LINE__);
			return false;
		}
		recvTotalLen += recvLen;
	}
	return true;
}


//子线程  执行过程 跟游戏主线程一起执行，所以不会阻塞游戏，让游戏假死卡住
void Socket::process() {

	if (!this->connectSocket()) {//如果连接服务器失败
		this->onState(false);
		goto ERR;
	}

	this->onState(true);//连接成功------------//

	//当socket正在运行时才 执行网络操作
	while (m_bRun) {//运行就一直执行
		
		if (this->receive(m_pHead, sizeof(TCP_Head))) {//如果收到数据
			//取出前8字节数据
			char headChar[8];
			memset(headChar, 0, sizeof(headChar));//置0
			memcpy(headChar, m_pHead, 8);

			
			TCP_Head* head = (TCP_Head*)headChar;
			int curLen = head->TCPInfo.wPacketSize;//数据大小
			int sizeofTCP = sizeof(TCP_Head);
			//长度效验，小于包头 或者 大于当前数据总长度,说明只接收了一半数据，跳出处理
			//NYInfo(TAG, "收到包头长度 curLen = %d ", curLen );
			if (curLen < sizeofTCP) {
				this->onState(false);
				break;
			}

			//取的当前长度 开始处理
			char curBuffer[20000];
			memset(curBuffer, 0, sizeof(curBuffer));//置0

			//NYInfo(TAG, "应该收到数据内容长度 bufLen = %d", curLen);

			if (this->receive(curBuffer, curLen - 8)) {
				memset(m_pBuf, 0, sizeof(m_pBuf));//置0
				memcpy(m_pBuf, m_pHead, 8);//首先复制 上面接收的8个位字节
				memcpy(m_pBuf + 8, curBuffer, curLen - 8);//接着复制剩下的所有数据

				//解密数据
				if (!Common_unMappedBuffer(m_pBuf, curLen)) {
					continue;
				}
				//完整正确接收到 解密后的数据 可以进行处理了
				
				this->onMsg(m_pBuf, curLen);////消息回调处理

			}
			else {
				log("-------------m_nFlag:%d-----------------NO DATA2", m_nFlag);
				this->onState(false);//原来是这里断开网络的
				goto ERR;
			}
		}
		else {
			//如果接收数据出错 或者收到的数据长度为0
			log("-------------m_nFlag:%d-----------------NO DATA1", m_nFlag);
			
 			this->onState(false);//原来是这里断开网络的
 			goto ERR;
		}

	}

	this->onState(false);

    ERR: this->stop();

}


//关闭socket
bool Socket::closeSocket() {
	

	int res = -1;
#ifdef WIN32
	res = (closesocket(m_Socket));
#else
	shutdown(m_Socket, SHUT_RDWR);//很重要 安卓必须要
	res = (close(m_Socket));
#endif

	return res;
}


//清理socket WIN32需要
int Socket::cleanSocket() {
#ifdef WIN32
	return (WSACleanup());//清理socket
#endif
	return 0;
}



//获得错误信息
int Socket::getError() {
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}



#ifdef _WINDOWS_
DWORD WINAPI Socket::workFunc(LPVOID userdata)
{
	//log("-------------------------------Socket::workFunc");
	static_cast<Socket*>(userdata)->process();//执行线程操作
	return 0;
}
#else
//void Socket::workFunc() {
//	static_cast<Socket*>(this)->process();//执行线程操作
//}
#endif


//间接执行回调函数
void Socket::onState(bool state) {
	if (m_pParent && m_fOnState) {
		this->m_fOnState(m_pParent, m_nFlag, state);
	}
}
//间接执行回调函数
void Socket::onMsg(const char* buf ,int len) {
	if (m_pParent && m_fOnMsg) {
		this->m_fOnMsg(m_pParent, m_nFlag, buf, len);
	}
}




//非阻塞情况下connect产生EINPROGRESS错误 (暂时没有用到)
int CheckConnect(int iSocket)
{
#ifndef WIN32

	fd_set rset;
	FD_ZERO(&rset);//将rset清零使集合中不含任何fd
	FD_SET(iSocket, &rset);//将fd加入rset集合

	timeval tval;
	tval.tv_sec = 0;
	tval.tv_usec = 0;

	//select测试指定的rset可读？可写？有异常条件待处理？
	/*
	返回值：

	负值：select错误

	正值：某些文件可读写或出错

	0：等待超时，没有可读写或错误的文件
	*/

	if (select(iSocket + 1, NULL, &rset, NULL, &tval) <= 0)
	{
		close(iSocket);
		return -1;
	}

	if (FD_ISSET(iSocket, &rset))
	{
		int err = -1;
		socklen_t len = sizeof(int);
		if (getsockopt(iSocket, SOL_SOCKET, SO_ERROR, &err, &len) < 0)
		{
			close(iSocket);
			printf("errno:%d %s\n", errno, strerror(errno));
			return -2;
		}

		if (err)
		{
			errno = err;
			close(iSocket);

			return -3;
		}
	}

	return 0;
#endif
	return 0;

}
