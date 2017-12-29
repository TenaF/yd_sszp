//
//  HallMessageController.cpp
//  hello2dx
//
//  Created by on
//
//

#include "sdk/util/Util.h"
#include "HallMessageController.h"
#include "base/ccUTF8.h"
#include "sdk/util/QFileUtil.h"
#include "hall/data/DataManager.h"
#include "hall/ui/MainLayer.h"
#include "hall/ui/LoginLayer.h"
#include "sdk/ui/PromptLayer.h"
#include "sdk/message/CMD_GameServer.h"
#include "sdk/message/Encrypt.h"


static HallMessageController *_instance = 0;

#define TAG "HallMessageController"

HallMessageController* HallMessageController::instance() {
	if (!_instance) {
		_instance = new HallMessageController();
	}
	return _instance;
}

HallMessageController::HallMessageController() {
}


bool HallMessageController::handleMessage(int wMainCmdID, int wSubCmdID,void* buf, WORD wBufLen) {

	//CCLog(U2F("大厅处理消息 main=%d   sub=%d buflength=%d").c_str(), wMainCmdID, wSubCmdID, wBufLen);
	switch (wMainCmdID) //主命令
	{
		//帐号登录
		case MDM_MB_LOGON: 						//广场登录	100.
		{
			return handleLoginAck(wSubCmdID, buf, wBufLen);
		}
		//比赛列表消息处理
		case MDM_MB_SERVER_LIST: 				//列表信息	101
		{
			return handleListAck(wSubCmdID, buf, wBufLen);
		}

		case MDM_GP_SERVER_LIST: 				//列表信息	2
		{
			return handleGPServerList(wSubCmdID, buf, wBufLen);
		}

		case MDM_GP_USER_SERVICE: 				//用户服务	3
		{
            return handleUserService(wSubCmdID, buf, wBufLen);
		}
		case MDM_KN_COMMAND:				//内核命令  0
            break;
            
	//	case SUB_MB_UPDATE_NOTIFY:          //升级提示  200
	//		break;
		case MDM_GP_WECHAT_CODE:			//微信验证 200
		{
												return false;//handleWechatCode(wSubCmdID,buf, wBufLen);
		}
		case MDM_GP_GET_KEY:                //5
		{
			
			return subEventDecrypt(buf, wBufLen, wSubCmdID);
		
		}

		default:
            break;
	}

	return true;
}



//解密登录RC4 KEY
bool HallMessageController::subEventDecrypt(void *buf, WORD wBufLen, int subCmd)
{
	char szOutBuff[512] = { 0 };
	char szRc4Key[32] = { 0 };

	//效验数据
	assert(wBufLen >= sizeof(CMD_GP_GetKeySuccess));
	if (wBufLen < sizeof(CMD_GP_GetKeySuccess))
		return false;

	//消息处理
	CMD_GP_GetKeySuccess * pGetKeySuccess = (CMD_GP_GetKeySuccess *)buf;
    
	PubKey_Decrypt((unsigned char *)pGetKeySuccess->szEncrypt,
		(unsigned char *)szRc4Key,
		(unsigned char *)pGetKeySuccess->szPubKey,
		pGetKeySuccess->wPubKeyLen);

	Vt_SetPlazaRc4Key(szRc4Key);

	//原来的登录流程
	switch (subCmd)
	{ 
// 	case SUB_GP_GETKEY_WECHATSHARE_SUCCESS:
// 
// 		SendMsg::sendShareWechat();
// 		break;
	case SUB_GP_GETKEY_QUICKLOGON_SUCCESS:

		SendMsg::sendQuickLoginReq();
		break;
	case SUB_GP_LOGIN_GETKEY_SUCCESS:
		
		SendMsg::sendLoginReq(F2U(UserInfo::instance()->getAccount()).c_str(), Util::strToMd5Str(UserInfo::instance()->getTempPassword()).c_str(), "");
		break;
	case SUB_GP_GETKEY_QUERYSCORE_SUCCESS:

		SendMsg::refreshUserScore();
		break;
	case SUB_GP_GETKEY_SAVESCORE_SUCCESS:

		SendMsg::sendSaveMoney(UserInfo::instance()->getAmountTemp());
		break;
	case SUB_GP_GETKEY_TAKESCORE_SUCCESS:

		SendMsg::sendTakeMoney(UserInfo::instance()->getAmountTemp(), UserInfo::instance()->pswTemp.c_str());
		break;
	case SUB_GP_GETKEY_TRANSFERSCORE_SUCCESS:

		SendMsg::sendTransferMoney(F2U(UserInfo::instance()->nickTemp).c_str(), UserInfo::instance()->getAmountTemp(), UserInfo::instance()->pswTemp.c_str());
		break;
	case SUB_GP_REGISTER_GETKEY_SUCCESS:

		SendMsg::registerAccounts(UserInfo::instance()->getAccount().c_str(), Util::strToMd5Str(UserInfo::instance()->getTempPassword()).c_str(), UserInfo::instance()->nickTemp.c_str(), UserInfo::instance()->checkValue, UserInfo::instance()->wFaceID, Util::strToMd5Str(UserInfo::instance()->strBankPass).c_str(), UserInfo::instance()->strCard.c_str());
		break;
	case SUB_GP_GETKEY_CHANGFACE_SUCCESS:

		SendMsg::sendUpdateHeadInfo(UserInfo::instance()->selectFace);
		break;
// 	case SUB_GP_GETKEY_SUCCESS://获得验证码的RC4成功
// 
// 		SendMsg::sendVerifactionCode();
// 		break;
	default:
		break;
	}

	return true;
}

//用户服务信息
bool HallMessageController::handleUserService(int nSubCmdId, void *buf,WORD wBufLen) {

	switch (nSubCmdId) {

		case SUB_GP_USER_INSURE_INFO: 				//银行信息
		{
			CMD_GP_UserInsureInfo * pUserInsureInfo = (CMD_GP_UserInsureInfo *) buf;
			UserInfo::instance()->setScore(pUserInsureInfo->lUserScore);
			UserInfo::instance()->setInsureScore(pUserInsureInfo->lUserInsure);
			//CCLog(U2F("接收到用户的金币信息 ");

			NotificationCenter::getInstance()->postNotification(LOGON_USER_INSURE_SUCCESS, (Ref *) buf);
			//联网
//			MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName(LOGIN_SERVER_IP), LOGIN_SERVER_PORT);
		}
		break;

		/*
		case SUB_GP_USER_GAME_INSURE_INFO: 					//用户金币
		{

			CMD_GP_UserGameInsureInfo * pInfo = (CMD_GP_UserGameInsureInfo *) buf;
			UserInfo::instance()->setScore(pInfo->lUserScore);
			UserInfo::instance()->setInsureScore(pInfo->lUserInsure);

			CCLog("接受到用户金币信息 数量：%d 银行币:%d" , pInfo->lUserScore , pInfo->lUserInsure);
			NotificationCenter::getInstance()->postNotification(LOGON_REFRESH_USER_INFO,(Ref *)buf);

			MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName(LOGIN_SERVER_IP), LOGIN_SERVER_PORT);
		}
		break;
		*/

		/*
		case SUB_GP_USER_TRANSFER_SUCCESS: 					//转账完成
		{
			//兑换后更新消息
			CMD_GP_UserTransferResult * result = (CMD_GP_UserTransferResult *) buf;
			UserInfo::instance()->setScore(result->lUserScore);
			UserInfo::instance()->setInsureScore(result->lUserInsure);

			NotificationCenter::getInstance()->postNotification(LOGON_TRANSFER_SUCCESS,(Ref *)buf);
			//CCLog(U2F(" 转账完成").c_str());
		}
		break;
		*/


		//case SUB_GP_PACKAGE_EMPTY:
		//{
		//	DataManager::getInstance()->emptyPackage();
		//}
		//break;

		//case SUB_GP_SC_COMPOSE_ITEM_LIST: 			//合成道具
		//{
		//	tagComposeItemInfo * pItemInfo = (tagComposeItemInfo *) buf;
		//	int iLenth = wBufLen / sizeof(tagComposeItemInfo);
		//	for (int i = 0; i < iLenth; i++) {
		//		DataManager::getInstance()->insertComposeItem(pItemInfo);
		//		pItemInfo++;
		//	}
		//	//CCLog(U2F("接受到合成道具列表信息 数量：%d").c_str() , iLenth);
		//}
		//break;

// 		case SUB_GP_S_QUERY_BILLINFO : 					//账单
// 		{
// 			//CCLog(U2F("  接受到账单信息").c_str());
// 			NotificationCenter::getInstance()->postNotification(LOGON_BILL_INFO,(Ref *)buf);
// 		}
		//break;
		/*
		case SUB_GP_TRANS_RECORD:
		{
			NotificationCenter::getInstance()->postNotification(LOGON_BILL_INFO,(Ref *)buf);
		}
		break;
		*/

		case SUB_GP_USER_FACE_INFO :
		{
			//CCLog(U2F(" 接受修改头像信息").c_str());
			CMD_GP_UserFaceInfo * pFace = (CMD_GP_UserFaceInfo *)buf;
			UserInfo::instance()->setFaceId(pFace->wFaceID);

			NotificationCenter::getInstance()->postNotification(LOGON_FACE_INFO,(Ref *)buf);
		}
		break;

		case SUB_GP_OPERATE_SUCCESS:					//操作成功
		{
			//CCLog(U2F(" 接受到操作成功信息").c_str());
			NotificationCenter::getInstance()->postNotification(LOGON_OPERATE_SUCCESS,(Ref *)buf);

			//服务器断开连接，需要重新连接
//			MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName(LOGIN_SERVER_IP), LOGIN_SERVER_PORT);
			
		}
		break;

		case SUB_GP_OPERATE_FAILURE:					//操作失败
		{
			CMD_GP_OperateFailure * pFail = (CMD_GP_OperateFailure*)buf;
			string strError = Util::getStringWithTChar(pFail->szDescribeString);
			NYInfo(TAG , " 接受到操作失败信息 option fail %s", strError.c_str());

			NotificationCenter::getInstance()->postNotification(LOGON_OPERATE_FAILURE,(Ref *)buf);
			if(!UserInfo::instance()->getTempPassword().empty())
			{
				UserInfo::instance()->setTempPassword("");
			}
			PromptLayer::createObject(strError.c_str());
		
			NotificationCenter::getInstance()->postNotification(SHAREWECHAT_FAILURE, nullptr);

		}
		break;


		case SUB_GP_USER_INSURE_SUCCESS :        //转账成功
			{
				PromptLayer::createObject("操作成功！");
				CMD_GP_UserInsureSuccess * pSuccess = (CMD_GP_UserInsureSuccess *) buf;

				UserInfo::instance()->setScore(pSuccess->lUserScore);
				UserInfo::instance()->setInsureScore(pSuccess->lUserInsure);

				NotificationCenter::getInstance()->postNotification(LOGON_TRANSFER_SUCCESS, (Ref *) buf);
			}
			break;

		case SUB_GP_USER_INSURE_FAILURE :        //转账失败
			{
				//CCLog(U2F("%s  接受到转账失败信息").c_str(), TAG);
				CMD_GP_UserInsureFailure * pFail = (CMD_GP_UserInsureFailure *) buf;

				string strError = Util::getStringWithTChar(pFail->szDescribeString);
				NotificationCenter::getInstance()->postNotification(LOGON_USER_INSURE_FAILURE, (Ref *) strError.c_str());

				//重新连接
//				MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName(LOGIN_SERVER_IP), LOGIN_SERVER_PORT);

			}
			break;
	}
	return true;
}

//登录处理
bool HallMessageController::handleLoginAck(int nSubCmdId, void *buf,WORD wBufLen) {
	switch (nSubCmdId) {
		case SUB_MB_QUICKLOGON_SUCCESS: //登录成功	102
		{
			return subQuickLoginSuccess(buf, wBufLen);
		}
		case SUB_MB_LOGON_SUCCESS: //登录成功	100
		{
			return subLoginSuccess(buf, wBufLen);
		}
		case SUB_MB_QUICKLOGON_FAILURE://快速登录失败 103 
		case SUB_MB_LOGON_FAILURE: //登录失败	101
		{
			return subLoginFailure(buf, wBufLen);
		}

		default:
			return true;
	}
	return true;
}

bool HallMessageController::handleListAck(int subCmdId, void *buf,WORD wBufLen) {

	switch (subCmdId) {
		case SUB_MB_LIST_FINISH: //列表完成	200
		{
			return subLoginFinishList(buf, wBufLen);
		}
	}
	return true;
}

bool HallMessageController::handleGPServerList(int subCmdId, void *buf,WORD wBufLen) {

	switch (subCmdId)
	{
		//case SUB_GR_SERVER_TIME: //同步服务器系统时间
		//{
		//	MSG_GP_GetServerTime * ptime = (MSG_GP_GetServerTime *) buf;
		//	string sysTime = Util::getStringWithTChar(ptime->szServerTime);
		//	DataManager::getInstance()->setSysTime(sysTime);

		//	//CCLog(U2F("  接受到登录服务器发送的系统时间 ").c_str());
		//	//通知接口 系统时间
		//	// NotificationCenter::getInstance()->postNotification(LOGIN_SERVER_TIME,(Ref *)buf);

		//}
		//break;

		//case SUB_GP_ANS_NOTICE_LIST :        //系统消息
		//	{
		//		CCLog(U2F("%s 接受到系统消息 ").c_str(), TAG);
		//		if (wBufLen == 0) {
		//			DataManager::getInstance()->clearMessage();
		//		} else {

		//			DataManager::getInstance()->clearMessage();

		//			int messageSize = sizeof(MSG_GP_SendMessage);
		//			int messageCount = wBufLen / messageSize;

		//			CCLog(U2F("%s 收到消息数 messageCount = %d").c_str(), TAG, messageCount);

		//			MSG_GP_SendMessage * pData = (MSG_GP_SendMessage *) buf;
		//			for (int i = 0; i < messageCount; i++) {
		//				DataManager::getInstance()->insertMessage(pData);
		//				pData++;
		//			}
		//		}
		//	}
		//	break;

		//case SUB_GP_LIST_CONFIGURE :        //系统配置信息
		//	{
		//		MSG_GP_SystemBaseConfigure * pConfigure = (MSG_GP_SystemBaseConfigure *) buf;
		//		string strKey = Util::getStringWithTChar(pConfigure->szKey);
		//		//CCLog(" receve : %s  %d",strKey.c_str(),pConfigure->wValue);
		//		//if (strKey == "RevenueRateTransfer") {
		//		//	DataManager::getInstance()->setTransferRatio(pConfigure->wValue);
		//		//}

		//	}
		//	break;
		case SUB_GP_GET_CONFIG_Success:
		{
			SendMsg::sendServerList();//发送获取房间列表
		}
		break;
		case SUB_GP_LIST_SERVER: //房间列表  104
		{
			int roomListSize = sizeof(tagGameServer);
			int roomListCount = wBufLen / roomListSize;

			//CCLog(U2F(" 房间列表数 roomListCount = %d wBufLen = %d sizeof = %d").c_str(), roomListCount, wBufLen ,sizeof(tagGameServer));
			//生成游戏房间列表

			tagGameServer * pData = (tagGameServer *) buf;
			for (int i = 0; i < roomListCount; i++) {
				if (pData->bShowMobile)//显示在手机的房间才加入
					DataManager::getInstance()->insertGameServer(pData);
				//NYInfo(TAG,"pData->wSortID:%d, pData->dwGameSubTypeID:%d, pData->dwKindID:%d,pData->szServerName:%s", pData->wSortID, pData->dwGameSubTypeID, pData->wKindID, U2F(Util::getStringWithTChar(pData->szServerName)).c_str());
				pData++;
			}
			//通知接口 服务器列表
			//请求上传头像更改，RC4后续添加请求，请按顺序来，不然会出现问题
			SendMsg::sendFaceId();
		}
		break;
		case SUB_GP_GAME_SUBTYPE:// 游戏房间子类型列表 110
		{
			int roomListSize = sizeof(tagGameSubType);
			int roomListCount = wBufLen / roomListSize;

			//生成游戏房间列表
			tagGameSubType * pData = (tagGameSubType *)buf;
			for (int i = 0; i < roomListCount; i++) {
				//NYInfo(TAG,"pData->wSortID:%d, pData->dwTypeID:%d, pData->dwKindID:%d,pData->szTypeName:%s", pData->wSortID, pData->dwTypeID, pData->dwKindID, U2F(Util::getStringWithTChar(pData->szTypeName)).c_str());
				DataManager::getInstance()->insertGameSubType(pData);
				pData++;
			}

			//通知接口 FANG列表
			NotificationCenter::getInstance()->postNotification(LOGON_SERVER_LIST, nullptr);
		}
		break;
		case SUB_GP_LIST_TYPE:// 大厅游戏类型列表 100
		{
			int roomListSize = sizeof(tagGameType);
			int roomListCount = wBufLen / roomListSize;

			//大厅游戏类型列表
			tagGameType * pData = (tagGameType *)buf;
			for (int i = 0; i < roomListCount; i++) {
				DataManager::getInstance()->insertGameType(pData);
				//NYInfo(TAG,"pData->wSortID:%d, pData->szTypeName:%s, pData->wTypeID:%d", pData->wSortID,U2F(Util::getStringWithTChar( pData->szTypeName)).c_str(),pData->wTypeID);
				pData++;
			}
			tagGameType *all = new tagGameType();//如果不止一个游戏类型  就将全部类加入进表里面
			all->wSortID = 0;
			all->wTypeID = 0;
			if (roomListCount>1)
			{
				DataManager::getInstance()->insertGameType(all);
			}
			NotificationCenter::getInstance()->postNotification(MAINLAYER_GAME_TYPE_NOTIFY, nullptr);
			SendMsg::sendGameSubType();//获取房间类型列表
		}
		break;
	}
	return true;
}


bool HallMessageController::handleOutMatchService(int subCmdId, void *buf,WORD wBufLen)
{
	//CCLog(U2F("  淘汰赛数据 ").c_str());
	//switch (subCmdId)
	//{
	//	case SUB_S_QUERY_GAMEMATCH_MYAPPLYDATA:
	//	{
	//		//CCLog(U2F("淘汰赛-报名数据").c_str());

	//		DataManager::getInstance()->clearOutMatchInfo();	//清理淘汰赛报名数据

	//		time_t today = time(NULL);
	//		struct tm pSystemTime = *localtime(&today);

	//		MSG_GM_QUERY_GameMatchMyApplyData_Data * pDt = (MSG_GM_QUERY_GameMatchMyApplyData_Data *)buf;
	//		
	//		for(int i = 0 ; i< 60;i++)
	//		{
	//			struct tm pMatchStart = *localtime(&today);
	//			if(pDt->dwServerID[i] > 0)
	//			{
	//				list<tagGameServer *> serverList = DataManager::getInstance()->getServerList();
	//				std::list<tagGameServer *>::iterator it = serverList.begin();
	//				for(;it != serverList.end();++it)
	//				{
	//					tagGameServer * pServer = (tagGameServer * )(*it);
	//					for(int j = 0 ; j < 20; ++j)
	//					{
	//						if(pServer->dwMatchID[j] == pDt->dwMatchID[i] && pServer->wServerID == pDt->dwServerID[i])
	//						{

	//							string strStart = Util::getStringWithTChar(pServer->szMatchStartTime[j]);
	//							if(strStart.empty())
	//								break;

	//							string szHour = strStart.substr(0,2);
	//							Value vl = Value(szHour);
	//							pMatchStart.tm_hour = vl.asInt();

	//							string szMin = strStart.substr(3,2);
	//							vl = Value(szMin);
	//							pMatchStart.tm_min = vl.asInt();


	//							string szSec = strStart.substr(6,2);
	//							vl = Value(szSec);
	//							pMatchStart.tm_sec = vl.asInt();

	//							time_t tBegin = mktime(&pMatchStart);
	//							time_t tSys = mktime(&pSystemTime);

	//							double diff = difftime(tBegin ,tSys)-DataManager::getInstance()->getSysDiffTime();
	//							if(diff>0){
	//								DataManager::getInstance()->setOutMatchInfo(pDt->dwServerID[i] , pDt->dwMatchID[i]);
	//								GlobalSchedule::start(30,0);
	//								//CCLog(U2F(" 用户报名数据 i = %d matchID = %d serverID = %d").c_str(),i , pDt->dwMatchID[i],pServer->wServerID );
	//							}
	//						}
	//					}
	//				}
	//			}
	//			//CCLog(U2F(" 比赛数据 i = %d  serverID=%d   matchID = %ld ").c_str(),i ,pDt->dwServerID[i] , pDt->dwMatchID[i] );
	//		}

	//		NotificationCenter::getInstance()->postNotification(LOGON_OUT_MATCH_DATA,NULL);

	//	}
	//	break;
	//}
	return true;
}


//快速登录成功 二级处理
bool HallMessageController::subQuickLoginSuccess(void *buf, WORD bufLen) {

	//效验大小
	//效验数据
 	NYInfo(TAG,"%d-%d", sizeof(CMD_MB_QuickLogon_Success), bufLen);
 	assert(bufLen >= sizeof(CMD_MB_QuickLogon_Success));
 
 	if (bufLen < sizeof(CMD_MB_QuickLogon_Success)) return false;

	CMD_MB_QuickLogon_Success * pLogonSuccess = (CMD_MB_QuickLogon_Success *)buf;
// 	memset(&pLogonSuccess, 0, sizeof(CMD_MB_QuickLogon_Success));
// 	memcpy(&pLogonSuccess, buf, sizeof(CMD_MB_QuickLogon_Success));

	//快速登录比普通账号登陆多一个账号和MD5密码传回客户端[2016/05/11 zhangyang]
	//////////////////////////////////////////////////////////////////////////
	string strAccount = F2U(Util::getStringWithTChar(pLogonSuccess->szAccounts)).c_str();
    //NYInfo(TAG,"!!!!!!!!strAccount:%s",strAccount.c_str());
    
	UserInfo::instance()->setAccount(strAccount);
 
	string strPassword = F2U(Util::getStringWithTChar(pLogonSuccess->szPassword)).c_str();
   // NYInfo(TAG,"!!!!!!strPassword:%s",strPassword.c_str());
	//UserInfo::instance()->setPassword(strPassword);
	UserInfo::instance()->setMD5Password(strPassword);

	//////////////////////////////////////////////////////////////////////////
	UserInfo::instance()->setUserId(pLogonSuccess->dwUserID);
	UserInfo::instance()->setGameId(pLogonSuccess->dwGameID);
	UserInfo::instance()->setExprience(pLogonSuccess->dwExperience);
	UserInfo::instance()->setSex(pLogonSuccess->cbGender);
	UserInfo::instance()->setScore(pLogonSuccess->lUserScore);
	UserInfo::instance()->setInsureScore(pLogonSuccess->lBankScore);
	UserInfo::instance()->setbWeChatBind(pLogonSuccess->bWeChatBind);
	UserInfo::instance()->setLoginType(pLogonSuccess->dwLogonType);
	UserInfo::instance()->setFaceId(pLogonSuccess->wFaceID);
	UserInfo::instance()->setMedal(pLogonSuccess->lMedal);

	string strNick = F2U(Util::getStringWithTChar(pLogonSuccess->szNickName)).c_str();
	UserInfo::instance()->setNickName(strNick);

	string strMessage = F2U(Util::getStringWithTChar(pLogonSuccess->szMessage)).c_str();
	UserInfo::instance()->setStartMessage(strMessage);

	//设置用户密码
	if (!UserInfo::instance()->getTempPassword().empty())
	{
		UserInfo::instance()->setPassword(UserInfo::instance()->getTempPassword());
		UserInfo::instance()->setTempPassword("");
	}

	NotificationCenter::getInstance()->postNotification(LOGON_LOGON_SUCCESS, nullptr);

	//处理会员信息
	//扩展信息
	void * pDataBuffer = NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonSuccess+1, bufLen - sizeof(CMD_MB_QuickLogon_Success));
	while (true)
	{
		//提取数据
		pDataBuffer = RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe == DTP_NULL) break;

		//提取数据
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_MEMBER_INFO:	//会员信息
		{
			assert(DataDescribe.wDataSize == sizeof(DTP_GP_MemberInfo));
			if (DataDescribe.wDataSize == sizeof(DTP_GP_MemberInfo))
			{
				DTP_GP_MemberInfo * pMemberInfo = (DTP_GP_MemberInfo *)pDataBuffer;
				UserInfo::instance()->setCbMemberOrder(pMemberInfo->cbMemberOrder);
				UserInfo::instance()->setMemberOverDate(pMemberInfo->MemberOverDate);
			}

			break;
		}
		}
	}
    //MainController::getInstance()->restartLoginSer();
	//保存登录用户 
	//快速登录不保存帐号

	UserInfo::instance()->SendType = 2;
	UserInfo::instance()->httpRequest();

	return true;

}


//登录成功 二级处理 by chen
bool HallMessageController::subLoginSuccess(void *buf, WORD bufLen) {

	//效验大小
 	NYInfo(TAG,"%d-%d", sizeof(CMD_MB_LogonSuccess), bufLen);
 	assert(bufLen >= sizeof(CMD_MB_LogonSuccess));
 
 	if (bufLen < sizeof(CMD_MB_LogonSuccess)) return false;

	CMD_MB_LogonSuccess * pLogonSuccess = (CMD_MB_LogonSuccess *)buf;
	// 	memset(&pLogonSuccess, 0, sizeof(CMD_MB_LogonSuccess));
	// 	memcpy(&pLogonSuccess, buf, sizeof(CMD_MB_LogonSuccess));

	UserInfo::instance()->setUserId(pLogonSuccess->dwUserID);//唯一保存 UserID的地方by chen
	UserInfo::instance()->setGameId(pLogonSuccess->dwGameID);
	UserInfo::instance()->setExprience(pLogonSuccess->dwExperience);
	UserInfo::instance()->setSex(pLogonSuccess->cbGender);
	UserInfo::instance()->setScore(pLogonSuccess->lUserScore);
	UserInfo::instance()->setInsureScore(pLogonSuccess->lBankScore);
	UserInfo::instance()->setbWeChatBind(pLogonSuccess->bWeChatBind);
	UserInfo::instance()->setLoginType(pLogonSuccess->dwLogonType);
	UserInfo::instance()->setMedal(pLogonSuccess->lMedal);
	UserInfo::instance()->setFaceId(pLogonSuccess->wFaceID);


	string strNick = U2F(Util::getStringWithTChar(pLogonSuccess->szNickName)).c_str();
	UserInfo::instance()->setNickName(strNick);

	string strMessage = U2F(Util::getStringWithTChar(pLogonSuccess->szMessage)).c_str();
	UserInfo::instance()->setStartMessage(strMessage);

	//设置用户密码
	if(!UserInfo::instance()->getTempPassword().empty())
	{
		UserInfo::instance()->setPassword(UserInfo::instance()->getTempPassword());
		UserInfo::instance()->setMD5Password(Util::strToMd5Str(UserInfo::instance()->getPassword()));
		UserInfo::instance()->setTempPassword("");
	}

	NotificationCenter::getInstance()->postNotification(LOGON_LOGON_SUCCESS,nullptr);

	//处理会员信息
	//扩展信息
	void * pDataBuffer = NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonSuccess + 1, bufLen - sizeof(CMD_MB_LogonSuccess));
	while (true)
	{
		//提取数据
		pDataBuffer = RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe == DTP_NULL) break;

		NYInfo(TAG,"DTP_GP_MemberInfo %d-%d", DataDescribe.wDataSize,sizeof(DTP_GP_MemberInfo));
		//提取数据
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_MEMBER_INFO:	//会员信息
		{
			assert(DataDescribe.wDataSize == sizeof(DTP_GP_MemberInfo));
			if (DataDescribe.wDataSize == sizeof(DTP_GP_MemberInfo))
			{
				DTP_GP_MemberInfo * pMemberInfo = (DTP_GP_MemberInfo *)pDataBuffer;
				UserInfo::instance()->setCbMemberOrder(pMemberInfo->cbMemberOrder);
				UserInfo::instance()->setMemberOverDate(pMemberInfo->MemberOverDate);
			}

			break;
		}
		}
	}
    
   // MainController::getInstance()->restartLoginSer();
    
	UserInfo::instance()->SendType = 2;
	UserInfo::instance()->httpRequest();
	
	return true;

}


bool HallMessageController::subLoginFailure(void *buf, WORD bufLen) {
	//变量定义
	NotificationCenter::getInstance()->postNotification(LOGON_LOGON_FAILURE,(Ref *)buf);

	MainController::getInstance()->setGameState(false);

	if(!UserInfo::instance()->getTempPassword().empty())
	{
		UserInfo::instance()->setPassword(UserInfo::instance()->getTempPassword());
		UserInfo::instance()->setTempPassword("");
	}
	//联网
//	MainController::getInstance()->startConnect(NetworkManager::FLAG_HALL, Util::getIpByHostName(LOGIN_SERVER_IP), LOGIN_SERVER_PORT);

	return true;

}

// bool HallMessageController::subRoomList(void *buf, WORD wBufLen) {
// 
// 	int roomListSize = sizeof(tagGameServer);
// 	int roomListCount = wBufLen / roomListSize;
// 
// 	CCLog(" 房间列表数 roomListCount = %d wBufLen = %d sizeof = %d", roomListCount, wBufLen ,sizeof(tagGameServer));
// 
// 
// 	tagGameServer * pData = (tagGameServer *) buf;
// 	for (int i = 0; i < roomListCount; i++) {
// 		DataManager::getInstance()->insertServer(pData);
// 		CCLog(" 房间kindid : %d ",pData->wKindID);
// 		pData++;
// 	}
// 	return true;
// }

bool HallMessageController::subLoginFinishList(void *buf, WORD wBufLen) {
	SendMsg::getUserScore();			//用户金币
	return true;
}

