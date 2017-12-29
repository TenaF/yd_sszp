//
//  sendMsg.h
//  hello2dx
//
//
//

#ifndef __hello2dx__sendMsg__
#define __hello2dx__sendMsg__

//#include "Packet.h"
//#include "Define.h"
#include "Common.h"
#include <string>
#include "stdio.h"
#include "memory.h"


using namespace std;

class SendMsg
{
public:
	// 发送心跳包
	static void sendHeartbeat(int flag);
    // 网络校验消息
    static void sendVerifyReq(int flag);
    // 登录消息
	static void sendQuickLoginReq();
    static void sendLoginReq(const char *  strUserName, const char *  strPassWord, const char *  strPhoneNum);
    static void registerAccounts(const char *  accounts , const char *  password ,const char *  nickname  , int sex ,int face, const char *  bankpassword, const char *  card );
    static void sendUpdateHeadInfo(int faceID);
	//static void sendVerifactionCode();//发送请求验证码
    static void sendUpdatePassword(const char * oldPass, const char * newPass);
    static void sendServerList();
	static void sendGameSubType();
	static void getUserScore() ;										//查询银行
	static void sendTransferMoney(const char * nick, SCORE amount, const char *  password, bool isWechatBind = false);		//发送转账操作请求
	static void sendSaveMoney(SCORE amount);
	static void sendTakeMoney(SCORE amount, const char *  password, bool isWechatBind = false);
	static void sendQueryBill();
	static int PackCrc32(const char* p, int l);

	//游戏登陆
	static void sendRoomLoginReq(DWORD dwUserId);
	static void sendUserRuleReq(int flag);
    static void sendGameRuleReq(int flag, BYTE byAllowLookOn, DWORD dwFrameVersion, DWORD dwClientVersion);
    static void sendSitDownReq(int flag, WORD wTableId, WORD wSeatId);
    static void sendStandUpReq(int flag, WORD wTableId, WORD wSeatId, BYTE byForceLeave, BYTE byExitTable);
    static void sendQuickSitDown(int flag);
    static void sendGetUserReadyReq(int flag, DWORD dwUserId, WORD wTableId, WORD wSeatId, BYTE byUserStatus);
    static void sendUserReadyReq(int flag, DWORD dwUserId, WORD wTableId, WORD wSeatId, BYTE byUserStatus);
    static void sendGetUserInfoReq(int flag, DWORD dwUserId, WORD wTableId);
    static void sendGetSeatUserInfoReq(int flag, WORD wTableId, WORD wSeatId);
	static void sendSitDownReq(int flag, WORD wTableId, WORD wSeatId, const char * password);
	static void refreshUserScore();
	//static void sendShareWechat();
	static bool SendGetRc4KeyWithSubCmdID(int mainCmd, int subCmd, DWORD currentTime );//请求登录RC4包
	static bool SendGetRc4KeyLogin(DWORD currentTime);//请求登录RC4包
	static bool SendGetRc4KeyServerList(DWORD currentTime);
	static void sendFaceId();//请求替换faceId

private:


	

};

#endif /* defined(__hello2dx__sendMsg__) */
