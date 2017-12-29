//
//  sendMsg.h
//  hello2dx
//
//
//

#ifndef __hello2dx__sendGameMsg_sszp__
#define __hello2dx__sendGameMsg_sszp__

//#include "Packet.h"
//#include "Define.h"
#include "sdk/message/Common.h"
#include <string>
#include "stdio.h"
#include "memory.h"
#include "sdk/message/Define.h"

using namespace std;
USING_NS_CC;

namespace sszp
{
	class SendGameMsg
	{
	public:
		//wangbo
		static void sendPlace_Jetton(int flag, BYTE jettonArea, SCORE jettonScore);//下注
		static void sendPlacePlaceJetton(int flag, SCORE jettonArea[]);
		static void sendApplyBanker(bool isBanker);//切换庄家
		//static void sendStopBanker();
	};
}

#endif /* defined(__hello2dx__sendMsg__) */
