//
//  sendMsg.cpp
//  hello2dx
//
//
//

#include "SendGameMsg_yxx.h"
#include "base/ccUTF8.h"
#include "sdk/util/QFileUtil.h"
#include "sdk/message/CMD_GameServer.h"
#include "sdk/network/NetworkManager.h"
#include "game/yxx/data/PokerData_yxx.h"

namespace yxx
{
#define TAG "SendGameMsg"

#define QL_GAME_ID KIND_ID

	void SendGameMsg::sendPlace_Jetton(int flag, BYTE jettonArea, SCORE jettonScore)
	{
		CMD_C_PlaceJetton PlaceJetton;
		memset(&PlaceJetton, 0, sizeof(CMD_C_PlaceJetton));
		PlaceJetton.cbJettonArea = jettonArea;
		PlaceJetton.lJettonScore = jettonScore;

		NetworkManager::getInstance()->sendData(flag, MDM_GF_GAME, SUB_C_PLACE_JETTON, &PlaceJetton, sizeof(CMD_C_PlaceJetton));
	}

	/*void SendGameMsg::sendPlacePlaceJetton(int flag, SCORE jettonArea[])
	{
	CMD_C_PlaceJettons repeatPlaceJetton;;
	memset(&repeatPlaceJetton, 0, sizeof(CMD_C_PlaceJettons));
	for (int i = 1; i < AREA_COUNT + 1; i++)
	{
	repeatPlaceJetton.lJettonScore[i] = jettonArea[i - 1];
	}
	NetworkManager::getInstance()->sendData(flag, MDM_GF_GAME, SUB_C_REPEAT_JETTON, &repeatPlaceJetton, sizeof(CMD_C_PlaceJettons));
	}
	*/
	void SendGameMsg::sendApplyBanker(bool isBanker)
	{
		int sub = 0;
		if (isBanker)
		{
			sub = SUB_C_APPLY_BANKER;
		}
		else
		{
			sub = SUB_C_CANCEL_BANKER;
		}
		NetworkManager::getInstance()->sendData(NetworkManager::FLAG_GAME, MDM_GF_GAME, sub, NULL, 0);
	}

}
