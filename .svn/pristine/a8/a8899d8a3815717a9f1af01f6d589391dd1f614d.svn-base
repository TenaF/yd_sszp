//
//  HallMessageController.h
//  hello2dx
//
//  Created by on
//
//

#ifndef __hello2dx__HallMessageController__
#define __hello2dx__HallMessageController__

#include "sdk/sdk.h"
#include "hall/data/UserInfo.h"


class HallMessageController:public Ref
{

public:
	//UserInfo userInfo;

public:
    static HallMessageController* instance();
    // 大厅回应消息分发
    bool handleMessage(int nMainCmdId,int nSubCmdId,void* buf, WORD wBufLen);
    // 登录回应消息处理
    bool handleLoginAck(int nSubCmdId, void *buf, WORD wBufLen);
	//用户服务
	bool handleUserService(int nSubCmdId, void *buf, WORD wBufLen);
	 // 列表回应消息处理
    bool handleListAck(int nSubCmdId, void *buf, WORD wBufLen);
	bool handleGPServerList(int subCmdId, void *buf, WORD wBufLen);
	//淘汰赛数据
	bool handleOutMatchService(int subCmdId, void *buf,WORD wBufLen);
	
public:
	bool subQuickLoginSuccess(void *buf, WORD bufLen);
    bool subLoginSuccess(void *buf, WORD wBufLen);
    bool subLoginFailure(void *buf, WORD wBufLen);

    bool subLoginFinishList(void *buf, WORD wBufLen);
	bool subEventDecrypt(void *buf, WORD wBufLen,int subCmd);
	bool subEventDecryptLogin(void *buf, WORD wBufLen, int subCmd);
	
private:
    HallMessageController();
    
};

#endif /* defined(__hello2dx__HallMessageController__) */
