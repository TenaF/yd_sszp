//
//  sendMsg.cpp
//  hello2dx
//
//
//

#include "SendGameMsg_fruit.h"
#include "base/ccUTF8.h"
#include "sdk/util/QFileUtil.h"
#include "sdk/message/CMD_GameServer.h"
#include "sdk/network/NetworkManager.h"
#include "game/fruit/data/PokerData_fruit.h"

namespace fruit
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

	void SendGameMsg::sendPlacePlaceJetton(int flag, SCORE jettonAllScore)
	{
		CMD_C_ChangeGold repeatPlaceJetton;;
		memset(&repeatPlaceJetton, 0, sizeof(CMD_C_ChangeGold));
		repeatPlaceJetton.lChangeGold = jettonAllScore;
		NetworkManager::getInstance()->sendData(flag, MDM_GF_GAME, SUB_C_CHANGE_GOLD, &repeatPlaceJetton, sizeof(CMD_C_ChangeGold));
	}
	void SendGameMsg::sendBigorSmallJetton(int flag, int isBig)
	{
		CMD_C_Guess repeatPlaceJetton;;
		memset(&repeatPlaceJetton, 0, sizeof(CMD_C_Guess));
		repeatPlaceJetton.bBig = isBig;
		NetworkManager::getInstance()->sendData(flag, MDM_GF_GAME, SUB_C_GUESS, &repeatPlaceJetton, sizeof(CMD_C_Guess));
	}

	void SendGameMsg::sendStartOrStop(int flag, bool isStart)
	{
		CMD_C_StartRoll repeatPlaceJetton;;
		memset(&repeatPlaceJetton, 0, sizeof(CMD_C_StartRoll));
		repeatPlaceJetton.bStartOrStop = isStart;
		NetworkManager::getInstance()->sendData(flag, MDM_GF_GAME, SUB_C_STARTSCROLL, &repeatPlaceJetton, sizeof(CMD_C_StartRoll));
	}

	//void SendGameMsg::sendRepeatBet(int flag, CMD_C_PlaceJettons &jettonScore)
	//{
	//	CMD_C_PlaceJettons RepeatBet;
	//	//memset(&RepeatBet, jettonScore, sizeof(CMD_C_PlaceJettons));

	//	NetworkManager::getInstance()->sendData(flag, MDM_GF_GAME, SUB_C_REPEAT_JETTON, &jettonScore, sizeof(CMD_C_PlaceJettons));
	//}

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
