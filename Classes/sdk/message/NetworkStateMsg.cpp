//
//  NetworkStateMsg.cpp
//  hello2dx
//
//  Created by on 14-7-6.
//
//

#include "NetworkStateMsg.h"

NetworkStateMsg::NetworkStateMsg(int flag, NET_STATE_TYPE type)
:MsgBase(flag)
{
    m_eType = MSG_TYPE::MSG_TYPE_NETWORK_STATE;
    m_eState = type;
}