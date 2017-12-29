//
//  NetworkStateMsg.h
//  hello2dx
//
//  Created by on 14-7-6.
//
//  网络状态消息

#ifndef __hello2dx__NetworkStateMsg__
#define __hello2dx__NetworkStateMsg__

#include "sdk/message/MsgBase.h"

enum class NET_STATE_TYPE
{
    NET_STATE_NONE,
    NET_STATE_START_CONNECT, 	// 开始连接
    NET_STATE_CONNECTED, 		// 连接成功
    NET_STATE_FAILED, 			// 连接失败
    NET_STATE_DISCONNECT, 		// 断开连接，包括主动和被动
    NET_STATE_CLOSE ,			//主动断开
};

class NetworkStateMsg : public MsgBase
{
public:
    NetworkStateMsg(int flag, NET_STATE_TYPE type);
    NET_STATE_TYPE getState() { return m_eState; }

private:
    NET_STATE_TYPE m_eState;
};

#endif /* defined(__hello2dx__NetworkStateMsg__) */
