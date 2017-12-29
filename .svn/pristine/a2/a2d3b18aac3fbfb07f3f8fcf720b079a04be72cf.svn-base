#include "NetworkManager.h"
#include "sdk/sdk.h"
#include "sdk/message/Packet.h"
#include "sdk/message/Common.h"
#include "sdk/ui/PromptLayer.h"
#include "sdk/controller/MainController.h"
#include "sdk/util/Util.h"

static NetworkManager* s_Instance = nullptr;



NetworkManager* NetworkManager::getInstance() {
	if (s_Instance == nullptr) {
		s_Instance = new (std::nothrow) NetworkManager();//不抛异常的 方便用 NULL比较指针是否为空
	}
	return s_Instance;
}

NetworkManager::NetworkManager() {
	
	for (int i = 0; i < FLAG_MAX; i++) {
		m_bConnect[i] = false;
		m_bConnected[i] = false;
		m_bConnecting[i] = false;
		m_bCloseConnect[i] = false;

		m_Sockets[i] = nullptr;//初始化
	}
	m_tick = 0.0;
	m_fOnMsg = nullptr;

	m_pMsgQueue = new list<MsgBase*>();

	//pthread_mutex_init(&m_mutex, NULL);//初始化互斥锁

	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);//执行update() 

	m_bPause = true;//有毛线用啊

	m_pMsgRelease = new list<MsgBase*>();

	m_nReConnectCount = 0;

}



NetworkManager::~NetworkManager() {

	//pthread_mutex_destroy(&m_mutex);//销毁互斥锁

	if (m_pMsgQueue) {
		for_each(m_pMsgQueue->begin(), m_pMsgQueue->end(), obj_deleter());//删除
		CC_SAFE_DELETE(m_pMsgQueue);
	}
	if (m_pMsgRelease) {
		for_each(m_pMsgRelease->begin(), m_pMsgRelease->end(), obj_deleter());//删除
		CC_SAFE_DELETE(m_pMsgRelease);
	}
	

}

//重置 4个标致
void NetworkManager::reset(int flag) {
	m_bConnect[flag] = false;
	m_bConnecting[flag] = false;
	m_bConnected[flag] = false;
	m_bCloseConnect[flag] = false;
}


//开始连网
bool NetworkManager::startConnect(Ref* object ,std::function<void(Ref* ,MsgBase*)> cb ,int flag ,const char* ip ,unsigned int port) {


	NYInfo("NetworkManager", "startConnect ip :%s  port:%lu", ip, port);
	if (flag < FLAG_HALL || flag >= FLAG_MAX) {
		return false;
	}

	if (m_Sockets[flag]) {//如果已经存在 
		m_Sockets[flag]->stop();//停止socket
	}

	if (!m_bConnect[flag] && !m_bConnecting[flag]) {

		m_Object = object;//保存父类
		m_fOnMsg = cb;//保存回调函数

		m_bConnecting[flag] = true;//设置正在连接ing

		//最最开始连接网络
		m_Sockets[flag] = new (std::nothrow) Socket(this, flag, socketStateChanged, socketMsg);

		if (m_Sockets[flag]) {

			m_Sockets[flag]->setAddr(ip, port);//设置IP 跟 端口

			this->pushMsg(new NetworkStateMsg(flag, NET_STATE_TYPE::NET_STATE_START_CONNECT));//发送(开始连接消息)

			if (m_Sockets[flag]->start()) {// 启动线程 并初始化WIN32需要的 DLL
				return true;
			} else {
				goto deal_fail;//操作失败
			}

		} else {

			goto deal_fail;//操作失败

		}

	}

	return false;

	deal_fail://操作失败
    this->reset(flag);
	
    return false;

}


//断开连接
void NetworkManager::disconnect(int flag) {

	this->reset(flag);
	m_bCloseConnect[flag] = true;//设置主动关闭联网
	if (m_Sockets[flag]) {
		m_Sockets[flag]->stop();
		m_Sockets[flag] = nullptr;
	}

}



//真实的发送消息 给数据加密 就直接发给服务端 by chen
void NetworkManager::sendMsg(int flag ,const char* buf ,int len) {
	//给buf数据加密
	if (!Common_MappedBuffer((void*) buf, len)) {
		return;
	}
	if (m_bConnect[flag]) {//如果连接上网络
		m_Sockets[flag]->sendBuf(buf, len);//底层发送消息
	}
}

//发送数据(把数据 跟 数据头 组装一起发送)
void NetworkManager::sendData(int flag ,int mainCmdId ,int subCmdId ,void*buf ,int bufLen) {

	//log("flag %d, mainCmdId %d, subCmdId %d, bufLen %d", flag,mainCmdId, subCmdId, bufLen);
    if (m_bConnected[flag]) {//已经联网1
		//添加数据头
		TCP_Buffer tcp_buffer;//网络缓冲
		memset(&tcp_buffer, 0, sizeof(TCP_Buffer));
		tcp_buffer.Head.CommandInfo.wMainCmdID = mainCmdId;//主命令
		tcp_buffer.Head.CommandInfo.wSubCmdID = subCmdId;//子命令
		memcpy(&tcp_buffer.cbBuffer, buf, bufLen);

		this->sendMsg(flag, (const char*) &tcp_buffer, bufLen + sizeof(TCP_Head));//发送消息

	}else{                     

 		if (flag == NetworkManager::FLAG_HALL)//如果是大厅
		{
			m_nReConnectCount++;//计数器递增
			if (m_nReConnectCount>10)
			{
				PromptLayer::createObject("登录服务器连接已断开,请检查网络设置");
			}
			else
			{
 				//PromptLayer::createObject(Util::gbk_2_utf8("登录服务器连接断开,重连中...").c_str());
				MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName((char*)(UserInfo::instance()->getServerIp().c_str())), UserInfo::instance()->getServerPort());
			}
		}
		else
		{
			this->onStateChanged(flag, false);	
		}
	}

}

//上发验证数据
void NetworkManager::sendVerifyData(int flag ,int mainCmdId ,int subCmdId ,void*buf ,int bufLen) {

	TCP_Buffer tcp_buffer;
	memset(&tcp_buffer, 0, sizeof(TCP_Buffer));
	tcp_buffer.Head.CommandInfo.wMainCmdID = mainCmdId;
	tcp_buffer.Head.CommandInfo.wSubCmdID = subCmdId;
	memcpy(&tcp_buffer.cbBuffer, buf, bufLen);

	this->sendMsg(flag, (const char*) &tcp_buffer, bufLen + sizeof(TCP_Head));
	

}

//派发消息 派发到游戏逻辑层去 by chen
void NetworkManager::dispatch(MsgBase* msg) {
	if (m_Object && m_fOnMsg && msg) {
		m_fOnMsg(m_Object, msg);
	}
}

//socket状态回调
void NetworkManager::socketStateChanged(Ref* object ,int flag ,bool state) {
	static_cast<NetworkManager*>(object)->onStateChanged(flag, state);
}

//socket消息回调
void NetworkManager::socketMsg(Ref* object ,int flag ,const char* buf ,int len) {
	static_cast<NetworkManager*>(object)->onMsg(flag, buf, len);
}


//状态回调
void NetworkManager::onStateChanged(int flag ,bool state) {
	
	if (state)      // 连上了
	{

		m_nReConnectCount = 0;//置0

		if (m_bConnecting[flag]) {//正在连接中
			m_bConnect[flag] = true;//是否连接上
			this->pushMsg(new NetworkStateMsg(flag, NET_STATE_TYPE::NET_STATE_CONNECTED));//连接成功
		} else {
			log("ERROR, connected but m_bConnecting false");
		}
	}else        // 断开了
	{
		m_bConnect[flag] = false;//是否连接上
		m_bConnected[flag] = false;//已连接

		if (m_bConnecting[flag]) {//正在连接中
			this->pushMsg(new NetworkStateMsg(flag, NET_STATE_TYPE::NET_STATE_FAILED));//连接失败
		} else {
			if (m_bCloseConnect[flag]) {//主动关闭
				this->pushMsg(new NetworkStateMsg(flag, NET_STATE_TYPE::NET_STATE_CLOSE));//主动断开
				m_bCloseConnect[flag] = false;
			} else {
				//被动断开连接
				this->pushMsg(new NetworkStateMsg(flag, NET_STATE_TYPE::NET_STATE_DISCONNECT));// 断开连接，包括主动和被动
				
			}
		}

	}

	m_bConnecting[flag] = false;

}

//消息回调函数
void NetworkManager::onMsg(int flag, const char* buf, int len) {

	TCP_Buffer tcpBuf;
	memset(&tcpBuf, 0, sizeof(TCP_Buffer));
	memcpy(&tcpBuf, buf, len);

	//命令码
	WORD mainCmdId = tcpBuf.Head.CommandInfo.wMainCmdID;
	WORD subCmdId = tcpBuf.Head.CommandInfo.wSubCmdID;
	WORD cbbufLen = tcpBuf.Head.TCPInfo.wPacketSize - sizeof(TCP_Head);
	BYTE *cbBuf = tcpBuf.cbBuffer;

	//0 1 
 	if (mainCmdId == 0 && subCmdId == 1) {
 		return;
 	}

	NetworkMsg* msg = new (std::nothrow) NetworkMsg(flag);
	if (msg) {
		msg->setMainId(mainCmdId);
		msg->setSubId(subCmdId);
		msg->setBuffer(cbBuf, cbbufLen);
		this->pushMsg(msg);
	}

}


//推送消息 标志 比如 现在是NET_STATE_START_CONNECT (先进先出) 
void NetworkManager::pushMsg(MsgBase* msg) {
	m_mutex.lock();//开始锁住

	if (m_pMsgQueue) {
		m_pMsgQueue->push_back(msg);//添加到消息列表最后
	} else {
		
		CC_SAFE_DELETE(msg);
	}
	m_mutex.unlock();//解除锁住
}

//获取消息
MsgBase* NetworkManager::getMsg() {
	MsgBase *msg = nullptr;
	m_mutex.lock();//开始锁住
	if (m_pMsgQueue && m_pMsgQueue->size()) {
		msg = m_pMsgQueue->front();
		m_pMsgQueue->pop_front();
	}
	m_mutex.unlock();//解除锁住
	return msg;
}


void NetworkManager::pushReleaseMsg(MsgBase* msg) {
	
	if (m_pMsgRelease) {
		m_pMsgRelease->push_back(msg);
	} else {
		CC_SAFE_DELETE(msg);
	}
}

MsgBase* NetworkManager::getReleaseMsg() {
	MsgBase *msg = nullptr;
	if (m_pMsgRelease && m_pMsgRelease->size()) {
		msg = m_pMsgRelease->front();
		m_pMsgRelease->pop_front();
	}
	return msg;
}


//一直处理
void NetworkManager::update(float dt) {

	if (m_bConnected[FLAG_GAME])
	{
		m_tick += dt;
		if (m_tick >= 9.0f)
		{
			m_tick = 0;
			SendMsg::sendHeartbeat(NetworkManager::FLAG_GAME);
		}
	}


	MsgBase* pReleaseMsg = this->getReleaseMsg();

	if (pReleaseMsg != nullptr) {
		pReleaseMsg->release();
	}

	while (m_bPause) {//如果没有暂停
		MsgBase *msg = this->getMsg();
		if ((msg&&msg->getType() != MSG_TYPE::MSG_TYPE_NETWORK) || (msg&&m_bConnect[msg->getFlag()] == true) || (msg&&msg->getFlag() == FLAG_GAME&&msg->getType() == MSG_TYPE::MSG_TYPE_NETWORK && ((NetworkMsg*)(msg))->getMainId() == MDM_GR_LOGON) || (msg&&msg->getFlag() == FLAG_HALL)) {
			this->dispatch(msg);//派发消息 (客户端进行处理)
			this->pushReleaseMsg(msg);//把这个消息放进 待删除列表中
		}else {
			break;//跳出
		}
	}

}
