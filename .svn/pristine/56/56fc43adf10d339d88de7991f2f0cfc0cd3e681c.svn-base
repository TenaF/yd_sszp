//
//  MsgBase.h
//  hello2dx
//
//  Created by on 14-7-6.
//
//  消息基类

#ifndef __hello2dx__MsgBase__
#define __hello2dx__MsgBase__

#include "cocos2d.h"

enum class MSG_TYPE
{
    MSG_TYPE_NONE,
    MSG_TYPE_NETWORK_STATE,
    MSG_TYPE_NETWORK,
    MSG_TYPE_CUSTOM,
};

class MsgBase : public cocos2d::Ref
{
public:
    MsgBase();
    MsgBase(int flag);
    ~MsgBase();
    int getFlag() { return m_nFlag; }
    MSG_TYPE getType() { return m_eType; }

protected:
    int m_nFlag; // 来自哪条连接
    MSG_TYPE m_eType;
};

#endif /* defined(__hello2dx__MsgBase__) */
