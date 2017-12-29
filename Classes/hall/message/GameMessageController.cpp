//
//  GameMessageController.cpp
//  NyGamePlaza
//
//  Created by zhangyang
//
//

#include "sdk/util/Util.h"
#include "GameMessageController.h"
#include "sdk/message/CMD_Commom.h"
#include "sdk/message/Encrypt.h"

#define TAG "GameMessageController"



// GameMessageController::GameMessageController() {
// }
// GameMessageController::~GameMessageController() {
// }

bool GameMessageController::handleMessage(int wMainCmdID, int wSubCmdID, void* buf, WORD wBufLen)
{
	NYInfo("MessageXXX", "handleMessage, main=%ld, sub=%ld", wMainCmdID, wSubCmdID);
	switch (wMainCmdID)
	{
		//登录命令
	case MDM_GR_LOGON:
		return handleLoginAck(wSubCmdID, buf, wBufLen);

		//房间配置
	case MDM_GR_CONFIG:
		return handleConfigAck(wSubCmdID, buf, wBufLen);

				//用户命令
			case MDM_GR_USER:
				return handleUserAck(wSubCmdID, buf, wBufLen);
		
				//游戏命令
			case MDM_GF_GAME:
				return handleGameAck(wSubCmdID, buf, wBufLen);
				//框架命令
			case MDM_GF_FRAME:
				return handleMainFrameAck(wSubCmdID, buf, wBufLen);
		
				//银行命令
			case MDM_GR_INSURE:
				break;
		
				//比赛命令
				//        case MDM_GM_GAMEMATCHSCORE:
				//       //     return handleGameMatchAck(wSubCmdID, buf, wBufLen);
				//
				//
				//            //淘汰赛
				//        case MDM_GM_GAMEMATCHWASHOUT:
				//        {
				//        //    return handleOutMatchAck(wSubCmdID, buf, wBufLen);
				//        }
				//            break;
		
				//管理命令
			case MDM_GR_MANAGE:
				NYInfo(TAG, "管理命令 MDM_GR_MANAGE");
				return true;
		
				//用户状态
			case MDM_GR_STATUS:
			{
								  return handleMainStatus(wSubCmdID, buf, wBufLen);
								  NYInfo(TAG, "获取用户状态 MDM_GR_STATUS");
								  //return handleStatus(wSubCmdID, buf, wBufLen);
								  //return true;
			}
		
		
				//管理命令
				//        case MDM_GF_PRIZE_POOL:
				//            NYInfo(TAG, "管理命令 MDM_GF_PRIZE_POOL");
				//            return true;

		// 内核命令
	case MDM_KN_COMMAND:
		NYInfo(TAG, "内核命令 MDM_KN_COMMAND");
		return true;


	default:
		NYInfo(TAG, "游戏消息处理 default 消息");
		if (wMainCmdID == 1000 && wSubCmdID == 1)//同账号不同设备登录房间，被踢出房间
		{
			NotificationCenter::getInstance()->postNotification(GAME_DISCONNECT, nullptr);
			return false;
		}
		return true;
	}
	return true;
}
/****************************
登录处理
***********************************/
bool GameMessageController::handleLoginAck(int nSubCmdId, void *buf, WORD wBufLen)
{
	//登录回应消息
	switch (nSubCmdId)
	{
	case SUB_GR_LOGON_SUCCESS:          //登录成功  100
		return handleLoginSuccess(buf, wBufLen);
	case SUB_GR_LOGON_FAILURE:          //登录失败  101
		return handleLoginFailure(buf, wBufLen);
	case SUB_GR_LOGON_FINISH:
		return hanleLoginFinish(buf, wBufLen);
	case SUB_GR_UPDATE_NOTIFY:
		return hanleUpdateNotify(buf, wBufLen);
	case SUB_GR_LOGONROOM_GETKEY_SUCCESS:
		return subEventDecryptLogin(buf, wBufLen);
	default:
		NYInfo(TAG, "登录异常处理 ");
		return false;
	}
	return true;
}


/**************
该消息系统命令消息
****************/
bool GameMessageController::handleSystemAck(int nSubCmdId, void *buf, WORD wBufLen)
{

	switch (nSubCmdId)
	{
	case SUB_CM_SYSTEM_MESSAGE:              //100
	{
		CMD_CM_SystemMessage pSystemMessage;

		memset(&pSystemMessage, 0, sizeof(CMD_CM_SystemMessage));
		memcpy(&pSystemMessage, buf, sizeof(CMD_CM_SystemMessage));

		WORD wType = pSystemMessage.wType;


		//关闭处理
		if ((wType&(SMT_CLOSE_ROOM | SMT_CLOSE_LINK)) != 0)
		{

		}

		//显示消息
		if (wType&SMT_CHAT)
		{

			NotificationCenter::getInstance()->postNotification(GAME_REQUEST_FAILURE, (Ref *)buf);

			//string strErr = Util::TCHARToChar(pSystemMessage.szString);
			string strErr = Util::getStringWithTChar(pSystemMessage.szString);
			NYInfo(TAG, "请求失败 = %s", strErr.c_str());

			PromptLayer::createObject(strErr.c_str());

		}

		//关闭游戏
		if (wType&SMT_CLOSE_GAME)
		{

		}

		//弹出消息
		if (wType&SMT_EJECT)
		{

		}

		//关闭房间
		if (wType&(SMT_CLOSE_ROOM | SMT_CLOSE_LINK))
		{
			NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
		}

		return true;
	}
	default:
		break;
	}
	return true;
}


/*****
*********************/
bool GameMessageController::handleConfigAck(int nSubCmdId, void *buf, WORD wBufLen)
{
	NYInfo("MessageXXX", "handleConfigAck 房间配置消息 %d", nSubCmdId);
	switch (nSubCmdId)
	{
	case SUB_GR_CONFIG_COLUMN:          //列表配置
	{
											return handleRoomList(buf, wBufLen);
	}
	case SUB_GR_CONFIG_SERVER:          //房间配置
	{
											return handleRoomConfig(buf, wBufLen);
	}
	case SUB_GR_CONFIG_PROPERTY:      //道具配置
	{
										  return handlePropertyConfig(buf, wBufLen);
	}
	case SUB_GR_CONFIG_FINISH:      //配置完成
	{
										return handleConfigFinish(buf, wBufLen);
	}
	case SUB_GR_CONFIG_MOBILE_ROOM_BASEDATA:
	{
											   //效验大小
											   if (wBufLen != sizeof(MSG_GR_ConfigMobileBaseData))
											   {
												   return false;
											   }
											   MSG_GR_ConfigMobileBaseData *dataBuf = (MSG_GR_ConfigMobileBaseData*)buf;
											   return true;
	}
	default:
		NYInfo(TAG, "房间配置 异常处理 ");
		return true;
	}
	return true;
}

/********************************
关键处理：用户信息处理
**********************************/
bool GameMessageController::handleUserAck(int nSubCmdId, void *buf, WORD wBufLen)
{
	NYInfo("MessageXXX", "handleUserAck 用户消息 %d", nSubCmdId);
	switch (nSubCmdId)
	{
	case SUB_GR_USER_ENTER:
	{
							  NYInfo(TAG, " 用户进入 ");
							  return handleUserEnter(buf, wBufLen);
	}

	case SUB_GR_USER_SCORE:
	{
							  NYInfo(TAG, " 用户分数 ");
							  return handleUserScore(buf, wBufLen);
	}

	case SUB_GR_USER_STATUS:
	{
							   NYInfo(TAG, " 用户状态 ");
							   return handleUserStatus(buf, wBufLen);
	}

	case SUB_GR_MOBILE_USER_STATUS:
	{
									  return handleMobileUserStatus(buf, wBufLen);
	}

	case SUB_GR_REQUEST_FAILURE:
	{
								   NYInfo(TAG, " 请求失败 ");
								   return handleUserReqFailure(buf, wBufLen);
	}

	case SUB_GR_USER_RULE:
		NYInfo(TAG, " 用户规则 ");
		return true;

	case SUB_GR_USER_LOOKON:
		NYInfo(TAG, " 用户旁观 ");
		return true;

	case SUB_GR_USER_SITDOWN:
		NYInfo(TAG, " 用户坐下 ");
		return true;

	case SUB_GR_USER_STANDUP:
		NYInfo(TAG, " 用户起立 ");
		return true;

	case SUB_GR_USER_INVITE:
		NYInfo(TAG, " 用户邀请 ");
		return true;

	case SUB_GR_USER_INVITE_REQ:
		NYInfo(TAG, " 邀请请求 ");
		return true;

	case SUB_GR_USER_REPULSE_SIT:
		NYInfo(TAG, " 拒绝玩家坐下 ");
		return true;

	case SUB_GR_USER_WAIT_DISTRIBUTE:
		NYInfo(TAG, " 等待分配 ");
		NotificationCenter::getInstance()->postNotification(GAME_WAIT_DISTRIBUTE, nullptr);
		return true;

	case SUB_GR_USER_CHAT:
		NYInfo(TAG, " 用户聊天信息 ");
		return true;

	case SUB_GR_USER_EXPRESSION:
		NYInfo(TAG, " 表情消息 ");
		return true;

	case SUB_GR_WISPER_CHAT:
		NYInfo(TAG, " 私聊消息 ");
		return true;

	case SUB_GR_WISPER_EXPRESSION:
		NYInfo(TAG, " 私聊表情 ");
		return true;

	case SUB_GR_COLLOQUY_CHAT:
		NYInfo(TAG, " 会话消息 ");
		return true;

	case SUB_GR_COLLOQUY_EXPRESSION:
		NYInfo(TAG, " 会话表情 ");
		return true;

	case SUB_GR_PROPERTY_BUY:
		NYInfo(TAG, " 购买道具 ");
		return true;

	case SUB_GR_PROPERTY_SUCCESS:
		NYInfo(TAG, " 道具成功 ");
		return true;

	case SUB_GR_PROPERTY_FAILURE:
		NYInfo(TAG, " 道具失败 ");
		return true;

	default:
		NYInfo(TAG, "房间配置 异常处理 ");
		return true;
	}
	return true;
}


/*************************************************
用户状态消息处理
*****************************************************/
bool GameMessageController::handleStatus(int nSubCmdId, void* buf, WORD wBufLen)
{
	NYInfo(TAG, "handleStatus 用户状态处理 ");
	switch (nSubCmdId)
	{
	case SUB_GR_TABLE_INFO:
	{
							  return handleTableInfo(buf, wBufLen);
	}
		break;
	case SUB_GR_TABLE_STATUS:
	{
								return handleTableStatus(buf, wBufLen);
	}
		break;
	default:
	{
			   NYInfo(TAG, "用户状态处理 异常处理 ");
			   return true;
	}
	}

	return true;
}

bool GameMessageController::handleMainFrameAck(int nSubCmdId, void *buf, WORD wBufLen)
{
	NYInfo("MessageXXX", "handleMainFrameAck 框架命令 %d", nSubCmdId);
	switch (nSubCmdId)
	{
	case SUB_GF_GAME_OPTION:
		NYInfo(TAG, " 游戏配置 ");
		return handleMainFrameGameOption(buf,wBufLen);
	case SUB_GF_USER_READY:
		NYInfo(TAG, " 用户准备 ");
		return handleMainFrameReady(buf, wBufLen);

	case SUB_GF_USER_CHAT:
		NYInfo(TAG, " 用户聊天 ");
		return handleMainFrameChat(buf, wBufLen);
	case SUB_GF_USER_EXPRESSION:
		NYInfo(TAG, " 用户表情 ");
		return handleMainFrameExpression(buf, wBufLen);
	case SUB_GF_LOOKON_CONFIG:
		NYInfo(TAG, " 旁观配置 ");
		return handleMainFrameLookOnConfig(buf,wBufLen);

	case SUB_GF_GAME_STATUS:
		NYInfo(TAG, " 游戏状态 ");
		return handleMainFrameGameStatus(buf, wBufLen);

	case SUB_GF_GAME_SCENE:
		NYInfo(TAG, " 游戏场景 ");
		return handleMainFrameGameScene(buf, wBufLen);

	case SUB_GF_SYSTEM_MESSAGE:
		NYInfo(TAG, " 系统消息 ");
		return handleMainFrameSystemMessage(buf, wBufLen);
	default:
		NYInfo(TAG, "框架命令 异常处理 ");
		return true;
	}
	return true;
}


//解密登录RC4 KEY
bool GameMessageController::subEventDecryptLogin(void* data, WORD data_size)
{
	char szOutBuff[512] = { 0 };
	char szRc4Key[32] = { 0 };

	//效验数据
	CC_ASSERT(data_size >= sizeof(CMD_GR_GetKeySuccess));
	if (data_size < sizeof(CMD_GR_GetKeySuccess)) return false;

	//消息处理
	CMD_GR_GetKeySuccess pGetKeySuccess;
	memset(&pGetKeySuccess, 0, sizeof(CMD_GR_GetKeySuccess));
	memcpy(&pGetKeySuccess, data, data_size);

	PubKey_Decrypt((unsigned char *)pGetKeySuccess.szEncrypt,
		(unsigned char *)szRc4Key,
		(unsigned char *)pGetKeySuccess.szPubKey,
		pGetKeySuccess.wPubKeyLen);

	Vt_SetPlazaRc4Key(szRc4Key);

	SendMsg::sendRoomLoginReq(UserInfo::instance()->getUserId());

	return true;

}