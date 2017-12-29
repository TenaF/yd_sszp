//
//  NetworkMsg.h
//  hello2dx
//
//  Created by on 14-7-6.
//
//  网络消息

#ifndef __hello2dx__NetworkMsg__
#define __hello2dx__NetworkMsg__

#include "sdk/message/MsgBase.h"
#include "sdk/message/Define.h"

class NetworkMsg : public MsgBase
{
public:
    NetworkMsg(int flag);
    ~NetworkMsg();
    
private:
    CC_PROPERTY(WORD, m_nMainId, MainId);
    CC_PROPERTY(WORD, m_nSubId, SubId);
    CC_PROPERTY_READONLY(WORD, m_nLen, Len);
    CC_PROPERTY_READONLY(BYTE*, m_pBuffer, Buffer);
    void setBuffer(BYTE* buffer, WORD len);
};

#endif /* defined(__hello2dx__NetworkMsg__) */
