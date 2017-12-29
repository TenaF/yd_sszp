//
//  PokerMessageController.cpp
//  hello2dx
//
//  Created by
//
//

#include "PokerMessageController_yxx.h"
#include "base/ccUTF8.h"
#include "sdk/util/SerialStream.h"
#include "hall/data/DataManager.h"
#include "sdk/ui/PromptLayer.h"
#include "sdk/message/CMD_GameServer.h"
#include "sdk/network/NetworkManager.h"
#include "sdk/message/PacketAide.h"
#include "GameClientDlg_yxx.h"
#include "hall/manager/SoundManager_hall.h"
#include "sdk/message/CMD_Commom.h"
#include "SimpleAudioEngine.h"
namespace yxx
{
	bool isFirstComeIn = true;
	static PokerMessageController *_instance = nullptr;
#define TAG "PokerMessageController"

	PokerMessageController* PokerMessageController::instance()
	{
		if (!_instance)
		{
			_instance = new PokerMessageController();
		}
		return _instance;
	}

	PokerMessageController::PokerMessageController()
	{
		NYInfo(TAG, " PokerMessageController() ");
		yxx_mainScene = nullptr;
		m_TableScene = nullptr;
		pokerData = PokerData::instance();
		m_reConnectFlag = false;
	}

	PokerMessageController::~PokerMessageController()
	{
		m_TableScene = nullptr;
		pokerData = nullptr;
	}

	/******************************************
	桌子的状态消息处理
	******************************************/
	bool PokerMessageController::handleMainStatus(int nSubCmdId, void *buf, WORD wBufLen)
	{
		switch (nSubCmdId)
		{
		case SUB_GR_TABLE_INFO:		//桌子信息
		{
										//变量定义
										CMD_GR_TableInfo * pTableInfo = (CMD_GR_TableInfo *)buf;
										WORD wHeadSize = sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatusArray);

										//效验数据
										if ((wHeadSize + pTableInfo->wTableCount*sizeof(pTableInfo->TableStatusArray[0])) != wBufLen) return false;

										if (nullptr != m_TableScene)
										{
											//消息处理
											for (WORD i = 0; i < pTableInfo->wTableCount; i++)
											{
												BYTE cbTableLock = pTableInfo->TableStatusArray[i].cbTableLock;
												BYTE cbPlayStatus = pTableInfo->TableStatusArray[i].cbPlayStatus;
												m_TableScene->setTableStatus(i, (cbPlayStatus == 1), (cbTableLock == 1));
											}
										}

										return true;
		}
		case SUB_GR_TABLE_STATUS:	//桌子状态
		{
										//效验参数
										if (wBufLen != sizeof(CMD_GR_TableStatus)) return false;

										//变量定义
										CMD_GR_TableStatus * pTableStatus = (CMD_GR_TableStatus *)buf;

										//设置桌子
										BYTE cbTableLock = pTableStatus->TableStatus.cbTableLock;
										BYTE cbPlayStatus = pTableStatus->TableStatus.cbPlayStatus;
										if (nullptr != m_TableScene)
										{
											m_TableScene->setTableStatus(pTableStatus->wTableID, (cbPlayStatus == 1), (cbTableLock == 1));
										}

										return true;
		}
		}
		return true;
	}

	bool PokerMessageController::hanleUpdateNotify(void *buf, WORD wBufLen)
	{
		return true;
	}

	bool PokerMessageController::handleLoginSuccess(void *buf, WORD bufLen)
	{
		NYInfo(TAG, "    ****房间登录成功");

		return true;

	}

	bool PokerMessageController::handleLoginFailure(void *buf, WORD bufLen)
	{
		NYInfo(TAG, "房间登录失败");
		CMD_GR_LogonFailure dataBuf;
		memset(&dataBuf, 0, sizeof(CMD_GR_LogonFailure));
		memcpy(&dataBuf, buf, sizeof(CMD_GR_LogonFailure));
		//    std::basic_string<unsigned short> str = dataBuf.szDescribeString;
		//    string strErr = Util::getStringWithTChar(str);
		string strErr = Util::TCHARToChar(dataBuf.szDescribeString);
		auto tips = __String::create(U2F(strErr.c_str()).c_str());
		tips->retain();
		NotificationCenter::getInstance()->postNotification(GAME_LOGON_FAILURE, tips);


		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);//
		return true;
	}

	bool PokerMessageController::hanleLoginFinish(void *buf, WORD wBufLen)
	{
		this->cacheGameSceneData();
		if (((pokerData->getStatus() == US_FREE) || pokerData->getStatus() == US_NULL))
		{

			m_reConnectFlag = false;
			if (pokerData->getMatchRule() & 0x00000040)                //防作弊
			{
				if (UserInfo::instance()->getSeatId() == INVALID_CHAIR)
				{
					SendMsg::sendSitDownReq(NetworkManager::FLAG_GAME, INVALID_TABLE, INVALID_CHAIR);
				}
			}
			else
			{
				//自己空闲状态启动房间
				NotificationCenter::getInstance()->postNotification(GAME_LOGIN_FINISH, (Ref *)buf);

				SendMsg::sendSitDownReq(NetworkManager::FLAG_GAME, INVALID_TABLE, INVALID_CHAIR);
				SendMsg::sendGetSeatUserInfoReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId());
				//SendMsg::sendGameRuleReq(NetworkManager::FLAG_GAME, false, INVALID_DWORD, INVALID_DWORD);
				//m_TableScene = new TableScene();
				//m_TableScene->bindData(pokerData);

				//Director::getInstance()->pushScene(SceneBase::createScene(m_TableScene));
			}
			return true;
		}

		if (pokerData->getStatus() == US_PLAYING || pokerData->getStatus() == US_READY || pokerData->getStatus() == US_SIT)
		{
			NotificationCenter::getInstance()->postNotification(GAME_LOGIN_FINISH, (Ref *)buf);
			//断线重连
			m_reConnectFlag = true;

			if (!yxx_mainScene)
			{
				yxx_mainScene = MainScene_yxx::createScene();
				CGameClientDlg::getInstance()->yxx_mainScene = yxx_mainScene;
				Director::getInstance()->pushScene(SceneBase::createSceneWithPhysics(yxx_mainScene));
			}

			for (int i = 0; i < GAME_PLAYER; i++)
			{
				auto playerInfo = PokerData::instance()->getPlayerInfoBySeatId(i);
				if (playerInfo)
				{
					if (playerInfo->getInfo().wTableID == UserInfo::instance()->getTableId())
					{
						if (yxx_mainScene)
							yxx_mainScene->setOtherPlayer(playerInfo);
					}
				}
			}
			SendMsg::sendGetSeatUserInfoReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId());
			SendMsg::sendGameRuleReq(NetworkManager::FLAG_GAME, false, INVALID_DWORD, INVALID_DWORD);
		}

		return true;
	}


	bool PokerMessageController::handleRoomList(void *buf, WORD wBufLen)
	{
		NYInfo(TAG, "列表配置");
		return true;
	}

	bool PokerMessageController::handleRoomConfig(void *buf, WORD wBufLen)
	{
		NYInfo(TAG, "房间配置 桌子列表");
		//效验大小
		if (wBufLen != sizeof(CMD_GR_ConfigServer))
		{
			NYInfo(TAG, "ERROR !!! handleRoomConfig ");
			return false;
		}

		CMD_GR_ConfigServer *dataBuf = (CMD_GR_ConfigServer*)buf;
		NYInfo(TAG, "  setMatchRule  ------- ");
		pokerData->setMatchRule(dataBuf->dwServerRule);
		//pokerData->setMinTableScore(dataBuf->lMinTableScore);
		pokerData->setTableCount(dataBuf->wTableCount);

		//NYInfo(TAG, "tableCount = %d, seatCount = %d, type = %d level = %d",dataBuf->wTableCount,dataBuf->wChairCount,dataBuf->wServerType,dataBuf->wServerLevel);
		//NYInfo(TAG, "rule = %ld limitScore = %lld minScore = %lld",dataBuf->dwServerRule,dataBuf->lRestrictScore,dataBuf->lMinTableScore);

		return true;
	}

	bool PokerMessageController::handlePropertyConfig(void *buf, WORD wBufLen)
	{
		NYInfo(TAG, "道具配置");
		return true;
	}

	bool PokerMessageController::handleConfigFinish(void *buf, WORD wBufLen)
	{
		NYInfo(TAG, "房间桌子配置完成");
		return true;
	}

	/**************
	该消息用于建立用户信息
	****************/
	bool PokerMessageController::handleUserEnter(void *buf, WORD wBufLen)
	{
		if (wBufLen < sizeof(tagMobileUserInfoHead))
		{
			return false;
		}

		//所有信息的初始化地方，基本数据都在这个里面接收
		tagMobileUserInfoHead dataBuf;
		memset(&dataBuf, 0, sizeof(tagMobileUserInfoHead));
		memcpy(&dataBuf, buf, sizeof(tagMobileUserInfoHead));

		//保存自己的游戏服务器真实临时分数
		if (UserInfo::instance()->getUserId() == dataBuf.dwUserID && dataBuf.cbUserStatus != US_NULL)
		{
			UserInfo::instance()->setScore(dataBuf.lScore);
		}

		//旁观不处理
		if (dataBuf.cbUserStatus == US_LOOKON)
		{
			return true;
		}
		NYInfo(TAG, "dataBuf.dwUserID = %lu", dataBuf.dwUserID);
		//昵称
		WORD wDataSize = 0;
		WORD wDataDescribe = 0;
		TCHAR szNickName[LEN_NICKNAME];
		CSerialStream stream((tagMobileUserInfoHead*)buf + 1);
		stream >> wDataSize;
		stream >> wDataDescribe;
		string nickName = "";
		if (wDataDescribe == DTP_GR_NICK_NAME)
		{
			memset(&szNickName, 0, sizeof(szNickName));
			memcpy(&szNickName, (const void *)((char*)buf + sizeof(tagMobileUserInfoHead)+sizeof(tagDataDescribe)), wDataSize);
			nickName = Util::getStringWithTChar(szNickName);
		}

		//如果存在
		std::map<DWORD, tagTableUserSitDownInfo>::iterator it = pokerData->m_mapUserSitDownInfo.find(dataBuf.dwUserID);
		if (it == pokerData->m_mapUserSitDownInfo.end())
		{
			//如果不存在 插入
			tagTableUserSitDownInfo SitDownbuf;
			memset(&SitDownbuf, 0, sizeof(tagTableUserSitDownInfo));
			SitDownbuf.wTableID = dataBuf.wTableID;
			SitDownbuf.wChairID = dataBuf.wChairID;
			SitDownbuf.strName = nickName;
			pokerData->m_mapUserSitDownInfo.insert(pair<DWORD, tagTableUserSitDownInfo>(dataBuf.dwUserID, SitDownbuf));//插入坐下信息的地方

		}
		else
		{
			//如果存在则更新位置和名字信息
			it->second.strName = nickName;
			it->second.wChairID = dataBuf.wChairID;
			it->second.wTableID = dataBuf.wTableID;
		}

		if (m_TableScene != nullptr)
		{
			m_TableScene->onMoblieUserStatusRefresh(pokerData->m_mapUserSitDownInfo);
		}

		// 根据USERID从玩家数据管理,取这个USERID的玩家数据.
		PokerPlayerInfo* playerInfo = pokerData->getPlayerInfo(dataBuf.dwUserID);
		if (!playerInfo)
			playerInfo = new PokerPlayerInfo();

		playerInfo->setInfo(dataBuf);
		playerInfo->setNickName(nickName);
		playerInfo->setScore(dataBuf.lScore);
		playerInfo->setFaceId(dataBuf.wFaceID);//
		playerInfo->setSeatId(dataBuf.wChairID);
		playerInfo->setTableId(dataBuf.wTableID);
		playerInfo->setUserStatus(dataBuf.cbUserStatus);
		pokerData->setPlayerInfo(dataBuf.dwUserID, playerInfo);
		NYInfo(TAG, "4444444444");
		if (yxx_mainScene)
		{
			NYInfo(TAG, "555555555555");
			updatePlayerInfo(playerInfo);
		}
		NYInfo(TAG, "66666666666666");
		// 自己的消息 或者 坐下以后，才接受本桌玩家消息，其他不管
		if (!((dataBuf.dwUserID == UserInfo::instance()->getUserId()) // 自己的消息
			|| (dataBuf.wTableID == UserInfo::instance()->getTableId() && dataBuf.wTableID != INVALID_TABLE)))
		{
			NYInfo(TAG, "handleUserEnter, not self or not sit or not same table");
			return true;
		}

		if (dataBuf.cbUserStatus != US_NULL || dataBuf.cbUserStatus != US_LOOKON)//旁观和没有状态不处理
		{
			// 自己进入
			if (dataBuf.dwUserID == UserInfo::instance()->getUserId())
			{

				// 记录用户自己信息
				if (m_TableScene != nullptr)
				{
					m_TableScene->autoClose(0.0f);
				}

				pokerData->setStatus(dataBuf.cbUserStatus);
				UserInfo::instance()->setTableId(dataBuf.wTableID);
				UserInfo::instance()->setSeatId(dataBuf.wChairID);
			}
		}

		return true;

	}
	bool PokerMessageController::handleUserScore(void *buf, WORD wBufLen)
	{
		//效验大小
		if (wBufLen != sizeof(CMD_GR_MobileUserScore))
		{
			NYInfo(TAG, "ERROR !!! handleUserScore ");
			return false;
		}
		CMD_GR_MobileUserScore dataBuf;
		NYInfo(TAG, "ERROR——CMD_GR_MobileUserScore");
		memset(&dataBuf, 0, sizeof(dataBuf));
		NYInfo(TAG, "ERROR——dataBuf");
		memcpy(&dataBuf, buf, sizeof(dataBuf));
		NYInfo(TAG, "ERROR——----__dataBuf");
		//获取用户
		PokerPlayerInfo* playerInfo = pokerData->getPlayerInfo(dataBuf.dwUserID);
		NYInfo(TAG, "ERROR——----playerInfo");
		if (playerInfo)
		{
			playerInfo->setUserScoreInfo(dataBuf.UserScore);
			playerInfo->setScore(dataBuf.UserScore.lScore);
			if (yxx_mainScene)
				yxx_mainScene->updateScore(dataBuf);
		}

		return true;
	}


	bool PokerMessageController::handleMobileUserStatus(void *buf, WORD wBufLen)
	{
		if (wBufLen != sizeof(CMD_GR_UserStatus))
		{
			return false;
		}

		CMD_GR_UserStatus dataBuf;
		memset(&dataBuf, 0, sizeof(dataBuf));
		memcpy(&dataBuf, buf, sizeof(dataBuf));

		if (m_TableScene == nullptr)
		{
			return true;
		}

		//如果用户已经坐下成功
		/*if (true == m_TableScene->getPlayIsSitDown(pokerData->m_mapUserSitDownInfo, dataBuf.dwUserID))
		{
		m_TableScene->onMoblieUserStatusChange(PokerData::instance()->m_mapUserSitDownInfo, &dataBuf, 0, "");
		if (dataBuf.UserStatus.cbUserStatus == US_NULL || dataBuf.UserStatus.cbUserStatus == US_LOOKON || dataBuf.UserStatus.cbUserStatus == US_FREE)
		{
		if (dataBuf.dwUserID == UserInfo::instance()->getUserId())
		{

		}
		}
		else if (dataBuf.UserStatus.cbUserStatus == US_READY)
		{
		auto palyer = pokerData->getPlayerInfo(dataBuf.dwUserID);
		palyer->setUserStatus(dataBuf.UserStatus.cbUserStatus);
		palyer->setSeatId(dataBuf.UserStatus.wChairID);
		palyer->setTableId(dataBuf.UserStatus.wTableID);
		}
		}
		else
		{
		if (dataBuf.UserStatus.cbUserStatus == US_NULL)
		{
		pokerData->removePlayerInfo(dataBuf.dwUserID);
		}
		SendMsg::sendGetUserInfoReq(NetworkManager::FLAG_GAME, dataBuf.dwUserID, dataBuf.UserStatus.wTableID);
		}*/

		return true;
	}

	bool PokerMessageController::handleGameAck(int nSubCmdId, void *buf, WORD wBufLen)
	{
		CGameClientDlg::getInstance()->OnEventGameMessage(nSubCmdId, buf, wBufLen);
		return true;
	}

	bool PokerMessageController::handleUserStatus(void *buf, WORD wBufLen)
	{
		if (wBufLen != sizeof(CMD_GR_UserStatus))
		{
			return false;
		}
		CMD_GR_UserStatus dataBuf;
		memset(&dataBuf, 0, sizeof(dataBuf));
		memcpy(&dataBuf, buf, sizeof(dataBuf));


		std::map<DWORD, tagTableUserSitDownInfo>::iterator it = pokerData->m_mapUserSitDownInfo.find(dataBuf.dwUserID);
		if (it != pokerData->m_mapUserSitDownInfo.end())
		{
			it->second.wTableID = dataBuf.UserStatus.wTableID;
			it->second.wChairID = dataBuf.UserStatus.wChairID;
		}

		if (m_TableScene != nullptr)
		{
			m_TableScene->onMoblieUserStatusRefresh(pokerData->m_mapUserSitDownInfo);
		}

		PokerPlayerInfo* playerInfo = pokerData->getPlayerInfo(dataBuf.dwUserID);
		if (playerInfo == NULL)//如果没有在数据管理中
		{
			if (dataBuf.UserStatus.wTableID != INVALID_TABLE&&dataBuf.UserStatus.wTableID == UserInfo::instance()->getTableId())
			{
				SendMsg::sendGetSeatUserInfoReq(NetworkManager::FLAG_GAME, dataBuf.UserStatus.wTableID, INVALID_CHAIR);

			}
			return true;
		}
		//自己数据
		if (dataBuf.dwUserID == UserInfo::instance()->getUserId())
		{
			if (UserInfo::instance()->getIsChangeTable())
			{
				if (m_TableScene)
				{
					m_TableScene->addTableFlag();
				}
			}
			UserInfo::instance()->setTableId(dataBuf.UserStatus.wTableID);//设置桌子ID
			UserInfo::instance()->setSeatId(dataBuf.UserStatus.wChairID);//设置椅子ID
			UserInfo::instance()->setStatus(dataBuf.UserStatus.cbUserStatus);//设置状态
			pokerData->setStatus(dataBuf.UserStatus.cbUserStatus);
			switch (dataBuf.UserStatus.cbUserStatus)
			{
			case US_NULL:
				break;
			case US_FREE:
				if (yxx_mainScene && isGetOut.empty())
				{
					clearMainScene();
				}
				break;
			case US_READY:
			{

			}
				break;
			case US_LOOKON:
			{
							  SendMsg::sendUserRuleReq(NetworkManager::FLAG_GAME);//用户规则
							  SendMsg::sendSitDownReq(NetworkManager::FLAG_GAME, dataBuf.UserStatus.wTableID, dataBuf.UserStatus.wChairID);
			}
				break;
			case US_SIT:
			case US_PLAYING:
			{
							   if (DataManager::getInstance()->getCurrentKindID() == KIND_ID)
							   {
								   if (!yxx_mainScene)
								   {
									   yxx_mainScene = MainScene_yxx::createScene();
									   CGameClientDlg::getInstance()->yxx_mainScene = yxx_mainScene;
									   Director::getInstance()->pushScene(SceneBase::createSceneWithPhysics(yxx_mainScene));
									   SendMsg::sendUserRuleReq(NetworkManager::FLAG_GAME);
									   SendMsg::sendGameRuleReq(NetworkManager::FLAG_GAME, false, INVALID_DWORD, INVALID_DWORD);
								   }
							   }
							   if (UserInfo::instance()->getIsChangeTable())
							   {
								   UserInfo::instance()->setIsChangeTable(false);
								   if (m_TableScene)
								   {
									   m_TableScene->tableFlagClear();
								   }
							   }
			}
				break;
			case US_OFFLINE:
				break;
			default:
				break;
			}
		}
		else  //其他人
		{
			switch (dataBuf.UserStatus.cbUserStatus)
			{
			case US_NULL:
				break;
			case US_OFFLINE:
			case US_FREE:
			{
							if (playerInfo)
							{
								if (playerInfo->getTableId() == UserInfo::instance()->getTableId())
								{
									if (yxx_mainScene)
										yxx_mainScene->clearPlayer(playerInfo);
									pokerData->removePlayerInfo(dataBuf.dwUserID);
								}
								playerInfo->setUserStatus(dataBuf.UserStatus.cbUserStatus);
								playerInfo->setTableId(dataBuf.UserStatus.wTableID);
								playerInfo->setSeatId(dataBuf.UserStatus.wChairID);

							}
			}
				break;
			case US_READY:
			{

			}
				break;
			case US_LOOKON:
				break;
			case US_PLAYING:
			case US_SIT:
			{
						   if (playerInfo)
						   {
							   playerInfo->setUserStatus(dataBuf.UserStatus.cbUserStatus);
							   playerInfo->setTableId(dataBuf.UserStatus.wTableID);
							   playerInfo->setSeatId(dataBuf.UserStatus.wChairID);
							   if (playerInfo->getInfo().wTableID == UserInfo::instance()->getTableId())
							   {


								   if (yxx_mainScene)
									   yxx_mainScene->setOtherPlayer(playerInfo);
							   }
						   }
			}
				break;
			default:
				break;
			}
		}
		return true;
	}

	bool PokerMessageController::handleUserReqFailure(void *buf, WORD wBufLen)
	{
		NYInfo(TAG, "handleUserReqFailure 用户请求失败处理 ");
		CMD_GR_RequestFailure dataBuf;
		memset(&dataBuf, 0, sizeof(CMD_GR_RequestFailure));
		memcpy(&dataBuf, buf, wBufLen);
		string strErr = Util::TCHARToChar(dataBuf.szDescribeString);
		NYInfo(TAG, "请求失败 = %s", strErr.c_str());
		auto tips = __String::create(U2F(strErr.c_str()).c_str());
		tips->retain();
		NotificationCenter::getInstance()->postNotification(GAME_REQUEST_FAILURE, tips);

		return true;
	}

	bool PokerMessageController::handleTableInfo(void* buf, WORD wBufLen)
	{
		NYInfo(TAG, "handleStatus 桌子信息处理 ");
		CMD_GR_TableInfo dataBuf;
		memset(&dataBuf, 0, sizeof(CMD_GR_TableInfo));
		memcpy(&dataBuf, buf, wBufLen);

		NYInfo(TAG, "handleStatus 桌子数目:%d", dataBuf.wTableCount);
		for (int i = 0; i < dataBuf.wTableCount; i++)
		{
			NYInfo(TAG, "handleStatus 桌子是否锁定？:%d", dataBuf.TableStatusArray[i].cbTableLock);
			NYInfo(TAG, "handleStatus 桌子状态，是否已经开始游戏？:%d", dataBuf.TableStatusArray[i].cbPlayStatus);
		}
		return true;
	}
	bool PokerMessageController::handleTableStatus(void* buf, WORD wBufLen)
	{
		NYInfo(TAG, "handleStatus 桌子信息处理 ");
		CMD_GR_TableStatus dataBuf;
		memset(&dataBuf, 0, sizeof(CMD_GR_TableStatus));
		memcpy(&dataBuf, buf, wBufLen);

		//NYInfo(TAG, "handleStatus 桌子数目:%d",dataBuf.wTableCount);
		NYInfo(TAG, "handleStatus 桌子号码？:%d", dataBuf.wTableID);
		NYInfo(TAG, "handleStatus 桌子状态，是否已经开始游戏？:%d", dataBuf.TableStatus);
		return true;
	}
	/*************************************************
	游戏消息处理
	*****************************************************/

	void PokerMessageController::updateTest(float dt)
	{
		NYInfo(TAG, "updateTest dt=%f", dt);
		// 发送准备请求
		if (PokerData::instance()->getMatchRule() & 0x04000000)			////系统分配机器人
		{
			SendMsg::sendUserReadyReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getUserId(), UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), US_READY);
		}
		else{
			SendMsg::sendGetUserReadyReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getUserId(), UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), US_READY);
		}
	}


	bool PokerMessageController::handleMainFrameGameOption(void *buf, WORD wBufLen)
	{
		return true;
	}

	bool PokerMessageController::handleMainFrameReady(void *buf, WORD wBufLen)
	{
		return true;
	}

	bool PokerMessageController::handleMainFrameChat(void *buf, WORD wBufLen)
	{
		return true;
	}

	bool PokerMessageController::handleMainFrameExpression(void *buf, WORD wBufLen)
	{
		return true;
	}

	bool PokerMessageController::handleMainFrameLookOnConfig(void *buf, WORD wBufLen)
	{
		return true;
	}

	bool PokerMessageController::handleMainFrameGameStatus(void *buf, WORD wBufLen)
	{
		NYInfo(TAG, "handleMainFrameGameStatus 游戏状态 ");
		if (sizeof(CMD_GF_GameStatus) != wBufLen)
		{
			NYInfo(TAG, "handleMainFrameGameStatus  ERROR !!!");
		}
		CMD_GF_GameStatus dataBuf;
		memset(&dataBuf, 0, sizeof(CMD_GF_GameStatus));
		memcpy(&dataBuf, buf, wBufLen);
		NYInfo(TAG, "游戏状态 ＝ %d 旁观标志 = %d", dataBuf.cbGameStatus, dataBuf.cbAllowLookon);
		CCLog("游戏状态 ＝ %d 旁观标志 = %d", dataBuf.cbGameStatus, dataBuf.cbAllowLookon);
		pokerData->setGameStatus(dataBuf.cbGameStatus);
		return true;
	}

	bool PokerMessageController::handleMainFrameGameScene(void *buf, WORD wBufLen)
	{
		NYInfo(TAG, "handleMainFrameGameScene 游戏场景 gameStatus = %d", pokerData->getGameStatus());
		BYTE gameStatus = pokerData->getGameStatus();
		if (UserInfo::instance()->getTableId() != INVALID_TABLE)
		{
			SendMsg::sendGetSeatUserInfoReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), INVALID_CHAIR);
		}
		switch (gameStatus)
		{
		case GAME_STATUS_FREE:
			NYInfo(TAG, "空闲等待开始 ");
			return handleGameSceneFree(buf, wBufLen);

		case 102:
		case 101://下注时间101
			NYInfo(TAG, "游戏进行中");
			return handleGameScenePlay(buf, wBufLen);

		case GAME_STATUS_WAIT: //200
			NYInfo(TAG, "等待状态");

		default:
			break;
		}
		return true;
	}

	bool PokerMessageController::handleMainFrameSystemMessage(void *buf, WORD wBufLen)
	{
		//数据包不固定大小
		CMD_CM_SystemMessage pSystemMessage;

		memset(&pSystemMessage, 0, sizeof(CMD_CM_SystemMessage));
		memcpy(&pSystemMessage, buf, wBufLen);


		WORD wType = pSystemMessage.wType;


		//关闭处理
		if ((wType&(SMT_CLOSE_ROOM | SMT_CLOSE_LINK)) != 0)
		{

		}

		//显示消息
		if (wType&SMT_CHAT)
		{

		}

		//关闭游戏
		if (wType&SMT_CLOSE_GAME)
		{
			string strErr = Util::getStringWithTChar(pSystemMessage.szString);
			NYInfo(TAG, "分数不够,5秒后自动退出房间 = %s", strErr.c_str());
			auto tips = __String::create(U2F(strErr.c_str()).c_str());
			isGetOut = tips->getCString();
			PromptLayer::promptCommon(PokerMessageController::instance()->getIsGetOut().c_str(), 34, CC_CALLBACK_0(MainScene_yxx::autoExit, yxx_mainScene), PromptLayer::PromptType::TYPE_PROMT_FORCE_CONFIRM);

			return false;
		}
		//关闭房间
		if (wType&(SMT_CLOSE_ROOM | SMT_CLOSE_LINK))
		{
			NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
		}
		return true;
	}

	bool PokerMessageController::handleGameSceneFree(void *buf, WORD wBufLen)
	{
		int siz = sizeof(CMD_S_StatusFree);
		if (wBufLen != sizeof(CMD_S_StatusFree)) return false;
		CMD_S_StatusFree pStatusFree;
		memset(&pStatusFree, 0, sizeof(CMD_S_StatusFree));
		memcpy(&pStatusFree, buf, wBufLen);

		if (yxx_mainScene)
		{
			yxx_mainScene->initGameScene(&pStatusFree);
			yxx_mainScene->setLabelType(pStatusFree.cbTimeLeave, TimeType_yxx_Leisure);
		}
		return true;
	}

	bool PokerMessageController::handleGameScenePlay(void *buf, WORD wBufLen)
	{
		//效验数据
		int siz = sizeof(CMD_S_StatusPlay);
		if (wBufLen != sizeof(CMD_S_StatusPlay)) return false;
		CMD_S_StatusPlay pStatusPlay;
		memset(&pStatusPlay, 0, sizeof(CMD_S_StatusPlay));
		memcpy(&pStatusPlay, buf, wBufLen);
		TimeType_yxx type = TimeType_yxx_none;
		if (pStatusPlay.cbGameStatus == 101)
		{
			type = TimeType_yxx_PlaceJetton;
		}
		else
		{
			type = TimeType_yxx_Start;
		}
		
		if (yxx_mainScene &&isFirstComeIn)
		{
			yxx_mainScene->initGameScene(&pStatusPlay);
			isFirstComeIn = false;
		}
		if (yxx_mainScene)
		{
			yxx_mainScene->setLabelType(pStatusPlay.cbTimeLeave, (TimeType_yxx)type);
		}
		return true;
	}


	//退出
	void PokerMessageController::onQuitGame()
	{
		UserInfo::instance()->setTableId(INVALID_TABLE);
		UserInfo::instance()->setSeatId(INVALID_CHAIR);
		UserInfo::instance()->setStatus(US_NULL);

		Director::getInstance()->popScene();

		if (yxx_mainScene)
		{
			yxx_mainScene->unscheduleAllSelectors();
			yxx_mainScene->stopAllActions();
			yxx_mainScene->removeAllChildren();
			yxx_mainScene->removeFromParentAndCleanup(true);
			yxx_mainScene = nullptr;
			MainScene_yxx::instance = nullptr;
			isFirstComeIn = true;
			CGameClientDlg::getInstance()->yxx_mainScene = nullptr;
		}

		if (m_TableScene != nullptr)
		{
			m_TableScene->release();
			m_TableScene = nullptr;
			Director::getInstance()->popScene();
		}

		CC_SAFE_DELETE(pokerData);
		CC_SAFE_DELETE(_instance);

		UserInfo::instance()->httpRequest();
		UserInfo::instance()->SendType = 0;
		UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
		UserInfo::instance()->subCmd = SUB_GP_GETKEY_QUERYSCORE;

		this->removeGameSceneData();
		UserInfo::instance()->setCurrentServerID(0);
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
		MainController::getInstance()->setGameState(false);

		HallSoundManager::instance()->stopBgMusic();
	}

	void PokerMessageController::onTableSceneQuit()
	{
		if (m_TableScene)
		{
			m_TableScene->release();
			m_TableScene = nullptr;
		}
		HallSoundManager::instance()->stopBgMusic();
	}

	void PokerMessageController::clearMainScene()
	{

		if (yxx_mainScene)
		{
			yxx_mainScene->unscheduleAllSelectors();
			yxx_mainScene->stopAllActions();
			yxx_mainScene->removeAllChildren();
			yxx_mainScene->removeFromParentAndCleanup(true);
			yxx_mainScene = nullptr;
			isFirstComeIn = true;
			MainScene_yxx::instance = nullptr;
			CGameClientDlg::getInstance()->yxx_mainScene = nullptr;
		}
		Director::getInstance()->popScene();

		CC_SAFE_DELETE(pokerData);
		CC_SAFE_DELETE(_instance);

		this->removeGameSceneData();

		UserInfo::instance()->httpRequest();
		UserInfo::instance()->SendType = 0;
		UserInfo::instance()->mainCmd = MDM_GP_GET_KEY;
		UserInfo::instance()->subCmd = SUB_GP_GETKEY_QUERYSCORE;

		UserInfo::instance()->setCurrentServerID(0);
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
		MainController::getInstance()->setGameState(false);
		HallSoundManager::instance()->stopBgMusic();
	}


	void PokerMessageController::updatePlayerInfo(PokerPlayerInfo *playerInfo)
	{
		NYInfo(TAG, "7777777771111");
		switch (playerInfo->getUserStatus())
		{
		case US_NULL:
		{

		}
			break;
		case US_FREE:
		{
						if (playerInfo)
						{
							if (yxx_mainScene)
								yxx_mainScene->clearPlayer(playerInfo);
						}
		}
			break;
		case US_READY:
		{
		}
			break;
		case US_LOOKON:
			break;
		case US_PLAYING:
		case US_SIT:
		case US_OFFLINE:
		{
						   NYInfo(TAG, "777777777722222");
						   if (playerInfo)
						   {
							   NYInfo(TAG, "777777773333333333");
							   if (yxx_mainScene)
								   yxx_mainScene->setOtherPlayer(playerInfo);
						   }
		}
			break;
		default:
			break;
		}

	}

	//清理和加载资源
	void PokerMessageController::cacheGameSceneData()
	{
		/*CCSpriteFrame* texture = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("b_card_bg.png");
		if (!texture)
		{
		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(SETPATH("hhsw/b_cards/b_card.plist").c_str());
		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(SETPATH("hhsw/s_cards/s_card.plist").c_str());
		for (int i = 1; i < 10; i++)
		{
		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(__String::createWithFormat(SETPATH("hhsw/animation/Bomb/effect_boom_%d.plist").c_str(), i)->getCString());
		}
		}*/
	}

	void PokerMessageController::removeGameSceneData()
	{
		ArmatureDataManager::getInstance()->removeArmatureFileInfo(SETPATH("yxx/animation/Yxx.ExportJson"));
		SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(SETPATH("yxx/animation/Yxx0.plist").c_str());
		SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(SETPATH("yxx/animation/Yxx1.plist").c_str());
		//TextureCache::getInstance()->removeTextureForKey(SETPATH("yxx/sicbao0.png").c_str());
// 		Director::getInstance()->getTextureCache()->removeAllTextures();
// 		CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
// 		CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
	}

	const string PokerMessageController::changePath(std::string str)
	{
		string strPath = "";
		int x = DataManager::getInstance()->getCurrentKindID();
		if (DataManager::getInstance()->getCurrentKindID() == KIND_ID)
		{
			strPath = "hhsw/" + str;
		}
		else if (DataManager::getInstance()->getCurrentKindID() == KIND_ID)
		{
			strPath = "yxx/" + str;
		}
		return strPath;
	}

	void PokerMessageController::didEnterBackground()
	{
		if (false == NetworkManager::getInstance()->isConnect(NetworkManager::FLAG_GAME))
			return;

		//SendMsg::sendStandUpReq(NetworkManager::FLAG_GAME, UserInfo::instance()->getTableId(), UserInfo::instance()->getSeatId(), 1, 0);
		NetworkManager::getInstance()->disconnect(NetworkManager::FLAG_GAME);
		if (pokerData->getMatchRule() & 0x00000040)
			clearMainScene();
	}
}
