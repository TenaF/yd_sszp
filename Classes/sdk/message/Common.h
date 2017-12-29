//
//  common.h
//  hello2dx
//
//  Created by on
//
//

#ifndef __hello2dx__common__
#define __hello2dx__common__

#include "sdk/message/Packet.h"
#include "sdk/message/Define.h"
#include "sdk/message/Struct.h"
#include "sdk/message/CMD_LogonServer.h"
#include "sdk/message/CMD_GameServer.h"
#include "sdk/message/MsgBase.h"
#include "sdk/network/Socket.h"//socket网络
#include "sdk/util/Util.h"
#include "sdk/network/NetworkManager.h"
#include "sdk/message/SendMsg.h"
#include "sdk/util/MD5.h"
#include "sdk/log/NYLog.h"


//加密数据
bool Common_MappedBuffer(void* data, int nDataSize);

//解密数据
bool Common_unMappedBuffer(void* data, int nDataSize);

//网络验证
bool Common_Compilation(TCHAR* data);

#endif
