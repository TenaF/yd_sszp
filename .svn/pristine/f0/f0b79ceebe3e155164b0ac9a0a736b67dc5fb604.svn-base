//
//  NetworkManager.h
//  hello2dx
//
//  Created by on 14-7-6.
//
//

#ifndef __hello2dx__NetworkManager__
#define __hello2dx__NetworkManager__

#include "cocos2d.h"
#include "sdk/message/MsgBase.h"
#include "sdk/network/Socket.h"//socket网络
#include "hall/message/HallMessageController.h"

using namespace std;

class NetworkManager: public Ref 
{
	public:

		enum // Socket 的连接标志flag
		{
			FLAG_HALL, FLAG_GAME, FLAG_MAX,
		};

		
		static NetworkManager* getInstance();
		~NetworkManager();

		// 将处理切换回主线程
		void update(float dt);
		// 是否连接
		bool isConnect(int flag) {
			return m_bConnect[flag];
		}
		// 是否创建完成
		bool isConnected(int flag) {
			return m_bConnected[flag];
		}

		void setConnected(int flag ,bool bFlag) {
			m_bConnected[flag] = bFlag;
		}

		void setCloseConnect(int flag ,bool bFlag) {
			m_bCloseConnect[flag] = bFlag;
		}

		//最最开始联网  开始连接
		bool startConnect(Ref* object ,std::function<void(Ref* ,MsgBase*)> cb ,int flag ,const char* ip ,unsigned int port);
		// 断开连接
		void disconnect(int flag);
		// 发送消息
		void sendMsg(int flag ,const char* buf ,int len);
		// 发送用户命令
		void sendData(int flag ,int mainCmdId ,int subCmdId ,void*buf ,int bufLen);
		// 发送验证用户命令
		void sendVerifyData(int flag ,int mainCmdId ,int subCmdId ,void*buf ,int bufLen);
		//设置暂停
		void pause(bool flag) {
			m_bPause = flag;
		}

	private:

		NetworkManager();

		void reset(int flag);//重置 4个标致

		std::function<void(Ref* ,MsgBase*)> m_fOnMsg;
		
		void dispatch(MsgBase* msg);//派发消息 派发到游戏逻辑层去 by chen

		static void socketStateChanged(Ref* object ,int flag ,bool state);

		static void socketMsg(Ref* object ,int flag ,const char* buf ,int len);

		void onStateChanged(int flag ,bool state);

		void onMsg(int flag ,const char* buf ,int len);

		void pushMsg(MsgBase* msg);

		MsgBase* getMsg();

		void pushReleaseMsg(MsgBase* msg);

		MsgBase* getReleaseMsg();

	private:

		bool m_bConnect[FLAG_MAX]; 			// 是否连接上
		bool m_bConnecting[FLAG_MAX];        // 正在连接中
		bool m_bConnected[FLAG_MAX]; 		// 验证通过的连接
		bool m_bCloseConnect[FLAG_MAX];        //主动关闭连接
		Socket* m_Sockets[FLAG_MAX]; //socket网络

		std::mutex m_mutex;        // 消息队列 ( 互斥锁 )

		Ref* m_Object;//父类

		std::list<MsgBase*>* m_pMsgQueue;//消息列表
		std::list<MsgBase*>* m_pMsgRelease;

		int m_nReConnectCount;//
		float m_tick;
		bool m_bPause;  //暂停处理消息：为了播放动画的需求

};

#endif 
