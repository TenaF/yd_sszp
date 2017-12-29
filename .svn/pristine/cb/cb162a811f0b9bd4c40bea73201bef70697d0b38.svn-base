//
//  GameMessageController.h
//  NyGamePlaza
//
//  Created by zhangyang
//
//

#ifndef __NyGamePlaza__GameMessageController__
#define __NyGamePlaza__GameMessageController__

#include "sdk/sdk.h"
#include "hall/data/UserInfo.h"


class GameMessageController :public Ref
{
public:
	bool handleMessage(int wMainCmdID, int wSubCmdID, void* buf, WORD wBufLen);
protected:
	/////////////////////////登录回应消息处理/////////////////////////////////////////////////
	virtual bool handleLoginSuccess(void *buf, WORD wBufLen) = 0;
	virtual bool handleLoginFailure(void *buf, WORD wBufLen) = 0;
	virtual bool hanleLoginFinish(void *buf, WORD wBufLen) = 0;
	virtual bool hanleUpdateNotify(void *buf, WORD wBufLen) = 0;
	/////////////////////////房间配置消息处理/////////////////////////////////////////////////
	virtual bool handleRoomList(void *buf, WORD wBufLen) = 0;
	virtual bool handleRoomConfig(void *buf, WORD wBufLen) = 0;
	virtual bool handlePropertyConfig(void *buf, WORD wBufLen) = 0;
	virtual bool handleConfigFinish(void *buf, WORD wBufLen) = 0;
	/////////////////////////用户消息处理////////////////////////////////////////////////
	virtual bool handleUserEnter(void *buf, WORD wBufLen) = 0;
	virtual bool handleUserScore(void *buf, WORD wBufLen) = 0;
	virtual bool handleUserStatus(void *buf, WORD wBufLen) = 0;
	virtual bool handleUserReqFailure(void *buf, WORD wBufLen) = 0;
	virtual bool handleMobileUserStatus(void *buf, WORD wBufLen) = 0;
	/////////////////////////用户状态////////////////////////////////////////////////
	virtual bool handleTableInfo(void* buf, WORD wBufLen) = 0;
	virtual bool handleTableStatus(void* buf, WORD wBufLen) = 0;
	/////////////////////////框架消息处理////////////////////////////////////////////////
	virtual bool handleMainFrameGameOption(void *buf, WORD wBufLen) = 0;
	virtual bool handleMainFrameReady(void *buf, WORD wBufLen) = 0;
	virtual bool handleMainFrameChat(void *buf, WORD wBufLen) = 0;
	virtual bool handleMainFrameExpression(void *buf, WORD wBufLen) = 0;
	virtual bool handleMainFrameLookOnConfig(void *buf, WORD wBufLen) = 0;
	virtual bool handleMainFrameGameStatus(void *buf, WORD wBufLen) = 0;
	virtual bool handleMainFrameGameScene(void *buf, WORD wBufLen) = 0;
	virtual bool handleMainFrameSystemMessage(void *buf, WORD wBufLen) = 0;
	/////////////////////////游戏消息处理/////////////////////////////////////////////////
	virtual bool handleGameAck(int nSubCmdId, void *buf, WORD wBufLen) = 0;
	/////////////////////////主桌面消息处理/////////////////////////////////////////////////
	virtual bool handleMainStatus(int nSubCmdId, void *buf, WORD wBufLen) = 0;

private:
	bool handleLoginAck(int nSubCmdId, void *buf, WORD wBufLen);
	bool handleConfigAck(int nSubCmdId, void *buf, WORD wBufLen);
	bool handleUserAck(int nSubCmdId, void *buf, WORD wBufLen);
	bool handleStatus(int nSubCmdId, void* buf, WORD wBufLen);
	bool handleMainFrameAck(int nSubCmdId, void *buf, WORD wBufLen);
	bool subEventDecryptLogin(void* data, WORD data_size);
	/////////////////////////系统消息///////////////////////////////////////////////
	bool handleSystemAck(int nSubCmdId, void *buf, WORD wBufLen);
};

#endif
